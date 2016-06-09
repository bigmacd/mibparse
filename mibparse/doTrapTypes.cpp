#pragma warning(disable: 4786)

#ifndef __MIBPARSER_H__
#include "MibParser.h"
#endif

BOOL       isVariableClause(char* lineBuf);
BOOL       isAssignment(char* lineBuf);
BOOL       isEnterprise(char* lineBuf);
void       markInUse(char* objectId);

#define GETLINE     1000
#define OPENCURLY   2000
#define FINDVARBIND 3000 
#define DONE        4000

void
MibParser::doTrapTypes(ifstream* ifile)
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

      if (isTrapType(lineBuf))
      {
	node* n = new node;
	n->trap = true;
	while (*lineBuf == ' ' || *lineBuf == '\t') 
	  lineBuf++;
	char* spec = strstr(lineBuf, "TRAP-TYPE");
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
	  
	  if (isEnterprise(lineBuf))
	  {
	    char* ent = strstr(lineBuf, "ENTERPRISE");
	    ent += 10;
	    //	    while (*ent == ' ' || *ent == '\t')
	    while (*ent == ' ' || *ent == '\t' || *ent == '{')
	      ent++;
	    //	    int len = strlen(lineBuf);
	    int len = strlen(ent);
	    char* entEnd = ent + (len - 1);
	    //	    while (*entEnd == ' ' || *entEnd == '\r' || *entEnd == '\t')
	    while (*entEnd == ' ' || *entEnd == '\r' || *entEnd == '\t' || *entEnd == '}')
	      entEnd--;
	    *++entEnd = 0;
	    strncpy(n->oid, ent, 256);

	  }

	  if (isAssignment(lineBuf))
	  {
	    char* assign = strstr(lineBuf, "::=");
	    assign += 3;

	    while (*assign == ' ' || *assign == '\t')
		assign++;

	    //////////////////////
	    char* commentSpec = strstr(assign, "--");
	    if (commentSpec != NULL)
	      *commentSpec = 0;
	    //////////////////////
	    
	    char* assignEnd = assign + (strlen(assign) - 1);
	    while (*assignEnd == ' ' || *assignEnd == '\r' || *assignEnd == '\t')
	      assignEnd--;
	    *++assignEnd = 0;

	    strcat(n->oid, ".");	    
	    strcat(n->oid, assign);

	    n->insert(n);
	    break;
	  }

	  if (isVariableClause(lineBuf))
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
      } // if (isTrapType(lineBuf))
    }
    catch(...)
    {
      
    }
  }
  ifile->clear();
  ifile->seekg(0);
}

BOOL       
MibParser::isTrapType(char* lineBuf)
{
  char* pos = strstr(lineBuf, "TRAP-TYPE");
  if (pos != NULL && ((pos - lineBuf) > 2))
    return TRUE;
  return FALSE;
}

BOOL       
isVariableClause(char* lineBuf)
{
  if (strstr(lineBuf, "VARIABLES"))
    return TRUE;
  return FALSE;  
}

BOOL       
isAssignment(char* lineBuf)
{
  if (strstr(lineBuf, "::="))
    return TRUE;
  return FALSE;  
}

BOOL       
isEnterprise(char* lineBuf)
{
  if (strstr(lineBuf, "ENTERPRISE"))
    return TRUE;
  return FALSE;  
}

void
markInUse(char* objectId)
{
  node Node;
  node* n = Node.NodeList();
  while (n != NULL)
  {
    if (!strcmp(n->name, objectId))
    {
      n->used = TRUE;
      break;
    }
    n = n->next;
  }
}
