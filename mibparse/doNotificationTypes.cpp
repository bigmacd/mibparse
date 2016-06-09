#pragma warning(disable: 4786)

#ifndef __MIBPARSER_H__
#include "MibParser.h"
#endif

#include "Node.h"

extern BOOL           isAssignment(char* lineBuf);
extern void           markInUse(char* objectId);

BOOL                  isNotificationType(char* lineBuf);
BOOL                  isObjectsClause(char* lineBuf);

#define GETLINE     1000
#define OPENCURLY   2000
#define FINDVARBIND 3000 
#define DONE        4000

void
MibParser::doNotificationTypes(ifstream* ifile)
{
  skipImports(ifile);
  // now lets do trap types
  while (!ifile->eof())
  {
    try 
    {

      // read each line of file
      char lineBuffer[4096];
      char* lineBuf = lineBuffer;
      memset(lineBuffer, 0, 4096);

      ifile->getline(lineBuffer, 4096);
      int count = ifile->gcount();
      
      if (isComment(lineBuf))
	continue;
      
      if (strstr(lineBuf, "DESCRIPTION"))
	skipDescription(lineBuf, ifile);

      if (isNotificationType(lineBuf))
      {
	node* n = new node;
	n->trap = true;
	while (*lineBuf == ' ' || *lineBuf == '\t') 
	  lineBuf++;
	char* spec = strstr(lineBuf, "NOTIFICATION-TYPE");
	if (spec != NULL)
	{
	  spec--;
	  while (*spec == ' ' || *spec == '\t')
	    spec--;
	}
	*++spec = 0;
	strncpy(n->name, lineBuf, 256);
	  
	while (1)
	{
	  memset(lineBuffer, 0, 4096);
	  lineBuf = lineBuffer;	
	  ifile->getline(lineBuffer, 4096);
	  count = ifile->gcount();
	  
	  if (isAssignment(lineBuf))
	  {
	    char* tbuf = strstr(lineBuf, "::=");
	    if (tbuf == NULL)
	      break; // can't happen, really

	    tbuf += 3;
	    
	    // strip all white space and the curly bracket
	    char* curly = strchr(tbuf, '{');
	    if (curly)
	    {
	      curly++;
	      // skip white space
	      while (*curly == ' ' || *curly == '\t')
		curly++;
	      char* t2buf = strchr(tbuf, '}');
	      if (t2buf)
	      {
		t2buf--;
		// skip white space
		while (*t2buf == ' ' || *t2buf == '\t')
		  t2buf--;
		*++t2buf = 0;
		strncpy(n->oid, curly, 256);
		n->insert(n);
	      }
	    }
	    break;
	    /////////////////////////////////
	    //	    char* assign = strstr(lineBuf, "::=");
	    //	    assign += 3;

	    //	    while (*assign == ' ' || *assign == '\t')
	    //		assign++;

	    //	    strcat(n->oid, ".");
	    //	    strcat(n->oid, assign);
	    //	    insert(n);
	    //	    break;
	  }

	  if (isObjectsClause(lineBuf))
	  {
	    BOOL done = FALSE;
	    char* firstPosition;
	    char* lastPosition;
	    int currentCount;
	    int state = OPENCURLY;
	    int nextState;

	    while (!done)
	    {
	      switch (state)
	      {
	       case GETLINE:
		 {
		   memset(lineBuffer, 0, 4096);
		   lineBuf = lineBuffer;	
		   ifile->getline(lineBuffer, 4096);
		   count = ifile->gcount();
		   currentCount = 1;
		   state = nextState;
		   firstPosition = lineBuf;
		   break;
		 }
	     
	       case OPENCURLY:
		 {
		   firstPosition = lineBuf;
		   // find the opening curly brace
		   for (currentCount = 1; currentCount < count; currentCount++)
		   {
		     if (*firstPosition != '{')
		       firstPosition++;
		     else
		       break;
		   }
		   if (currentCount == count)
		   {
		     // look on next line
		     nextState = state;
		     state = GETLINE;
		   }
		   else
		   {
		     firstPosition++; 
		     currentCount++;
		     state = FINDVARBIND;
		   }
		   break;
		 }
	     
	       case FINDVARBIND:
		 {
		   // now skip white space to first varbind
		   for (; currentCount < count; currentCount++)
		   {
		     if (*firstPosition == ' ' || *firstPosition == '\t')
		       firstPosition++;
		     else
		       break;
		   }
		   if (currentCount == count)
		   {
		     nextState = state;
		     state = GETLINE;
		   }
		   else
		   {
		     // now find end of first varbind
		     lastPosition = firstPosition;
		     for (; currentCount < count; currentCount++)
		     {
		       if (*lastPosition != ' ' && *lastPosition != '\t' 
			   && *lastPosition != ',' && *lastPosition != '}')
			 lastPosition++;
		       else
			 break;
		     }
		     if (currentCount == count) // error
		     {
		       state = DONE;
		       break;
		     }
		   
		     char saveChar = *lastPosition;
		     *lastPosition = 0;
		     markInUse(firstPosition);
		     *lastPosition = saveChar;
		     if (saveChar == ',')
		     {
		       lastPosition++; 
		       currentCount++;
		     }
		     else
		       if (strchr(lastPosition, '}'))
			 state = DONE;
		     firstPosition = lastPosition;
		   }
		   break;
		 }
	     
	       case DONE:
		 done = TRUE;
		 break;
	      } // switch (state)
	    } // while (!done)
	  } // if (isVariableClause(lineBuf))	  
	} // while (1)
      } // if (isNotificationType(lineBuf))
    }
    catch(...)
    {
      
    }
  }
  ifile->clear();
  ifile->seekg(0);
}

BOOL       
isNotificationType(char* lineBuf)
{
  char* pos = strstr(lineBuf, "NOTIFICATION-TYPE");
  if (pos != NULL && ((pos - lineBuf) > 2))
    return TRUE;
  return FALSE;
}

BOOL       
isObjectsClause(char* lineBuf)
{
  if (strstr(lineBuf, "OBJECTS"))
    return TRUE;
  return FALSE;  
}

