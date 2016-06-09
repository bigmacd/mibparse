
#ifndef __MIBPARSER_H__
#include "MibParser.h"
#endif

#include "stdlib.h"

// types of SYNTAXes
char* syntaxes [] = { "IpAddress",
		      "INTEGER",
   	              "SEQUENCE OF",
		      "Index",
		      "OCTET STRING",
		      "Counter",
		      "DisplayString",
		      "Gauge",
		      "TimeTicks",
                      "NetworkAddress",
                      "Counter32",
		      "Counter64",
                      "Unsigned32",  // SMIv2 follow
		      "DisplayString",     // index 13
		      "PhysAddress",       // index 14
		      "MacAddress",        // index 15
		      "TruthValue",        // index 16
		      "TestAndIncr",       // index 17
		      "AutonomousType",    // index 18
		      "VariablePointer",   // index 19
		      "RowPointer",        // index 20
		      "RowStatus",         // index 21
		      "TimeStamp",         // index 22
		      "TimeInterval",      // index 23
		      "DateAndTime",       // index 24
		      "StorageType",       // index 25
		      "TDomain",           // index 26
		      "TAddress",          // index 27
		      0L
};

void
MibParser::doObjectTypes(ifstream* ifile)
{
  // now lets do object types
  bool skippedImports = FALSE;

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

      if (!skippedImports)
      {
	skipImports(ifile);
	skippedImports = TRUE;
      }

      // ignore comments
      if (isComment(lineBuf))
	continue;

      // see if it is an OBJECT-TYPE
      // if it is, store the name

      if (isObjectType(lineBuf))
      {
	// skip white space
	int x = 0;
	for (x = 0; x < count; x++)
	{
	  if  (*lineBuf == ' ' || *lineBuf == '\t')
	    lineBuf++;
	  else
	    break;
	}
	if (x == count)
	  continue;

	// find first blank after non-white space
	char* tbuf = lineBuf;
	for (; x < count; x++)
	{
	  if (*tbuf != ' ' && *tbuf != '\t')
	    tbuf++;
	  else
	    break;
	}
	if (x == count)
	  continue;

	// save the char off
	char tc = *tbuf;

	// null terminate the string
	*tbuf = 0;
	node* tNode = new node;

	// save the string off
	strncpy(tNode->name, lineBuf, 256);

	// restore the char
	*tbuf = tc;

	// now get the syntax
	while ((tbuf = strstr(lineBuf, "SYNTAX")) == NULL)
	{
	  lineBuf = lineBuffer;	
	  memset(lineBuffer, 0, 4096);
	  ifile->getline(lineBuffer, 4096);
	  
	  // ignore comments
	  if (isComment(lineBuf))
	    continue;
	}	

	// point just after "SYNTAX"
	tbuf += 7;
	while (*tbuf == ' ' || *tbuf == '\t')
	  tbuf++;

	// here, tbuf points to the located syntax
	char* syn;
	int index = 0;
	while ((syn = syntaxes[index]) != NULL)
	  //	  if (!strcmp(syn, tbuf))
	  if (strstr(tbuf, syn)) // found one of the syntaxes
	    break;
	  else
	    index++;

	if (syn != NULL)
	{
	  tNode->syntax = index;

	  if (!strcmp(syn, "INTEGER"))
	  {
	    // see if there is a qualifier like '(' or '{'
	    char* leftBrace = NULL;
	    ///////////////////////////////////////
	    if ((leftBrace = strchr(tbuf, '{')) != NULL)  	    // found an enumerated integre
	    {
	      // lets check the first line completely
	      // skip the whitespace
	      while (*leftBrace == ' ' || *leftBrace == '\t')
		leftBrace++;
	      
	      // see if there is an enum on the same line
	      char* paren = NULL;
	      EnumNode* lastNode = NULL;
	      if ((paren = strchr(leftBrace, '(')) != NULL)
	      {
		EnumNode* n = new EnumNode();
		*paren = 0; // terminate the name
		strncpy(n->mName, leftBrace, 255);
		paren++; // point to the value
		char* paren2 = strchr(paren, ')');
		if (paren2 == NULL) // major bad
		  delete n;
		else
		{
		  *paren2 = 0;
		  n->mValue = atoi(paren);
		  tNode->mEnumNodeList = n;
		  lastNode = n;
		}
	      }
	      
	      // now, pick up all enumerations until the '}'
	      BOOL done = FALSE;
	      while (!done)
	      {
		lineBuf = lineBuffer;	
		memset(lineBuffer, 0, 4096);
		ifile->getline(lineBuffer, 4096);
		lineBuf = lineBuffer;
		// ignore comments
		if (isComment(lineBuf))
		  continue;		
		
		// lets check the first line completely
		// skip the whitespace
		while (*lineBuf == ' ' || *lineBuf == '\t')
		  lineBuf++;
		
		// see if there is an enum on the same line
		char* paren = NULL;
		if ((paren = strchr(lineBuf, '(')) != NULL)
		{
		  EnumNode* n = new EnumNode();
		  *paren = 0; // terminate the name
		  strncpy(n->mName, lineBuf, 255);
		  paren++; // point to the value
		  char* paren2 = strchr(paren, ')');
		  if (paren2 == NULL) // major bad
		    delete n;
		  else
		  {
		    *paren2 = 0;
		    n->mValue = atoi(paren);
		    if (lastNode == NULL)
		    {
		      tNode->mEnumNodeList = n;
		      lastNode = n;
		    }
		    else
		    {
		      lastNode->mNext = n;
		      lastNode = n;
		    }
		  }
		}
		
		if ((tbuf = strchr(lineBuf, '}')) != NULL)
		  // this is the last one
		  done = TRUE;
	      } // while (!done)
	    } // if ((leftBrace = strchr(tbuf, '{')) != NULL)
	    /////////////////////////////////////////////
	    else
	    {
	      /////////////////////////////////////////////
	      if ((leftBrace = strchr(tbuf, '(')) != NULL)
	      {
		// found a range specification
		leftBrace++;
		// skip illegal whitespace
		while (*leftBrace == ' ' || *leftBrace == '\t')
		  leftBrace++;
		
		// null terminate the min value
		char* dot = strchr(tbuf, '.');
		if (dot != NULL)
		  *dot = 0;
		// and save it
		tNode->mMinValue = atoi(leftBrace);
		
		// now find the max value
		*dot = '.';
		leftBrace = dot;
		
		// skip the illegal whitespace and the ".."
		while (*leftBrace == ' ' || *leftBrace == '\t' || *leftBrace == '.')
		  leftBrace++;
		
		// now pointing to first digit of max value
		// find the end of the range
		dot = strchr(tbuf, ')');
		if (dot != NULL)
		  *dot = 0;
		tNode->mMaxValue = atoi(leftBrace);
		/////////////////////////////////////////////
	      } // if ((leftBrace = strchr(tbuf, '(')) != NULL)
	    } // else
	  } // if (!strcmp(syn, "INTEGER"))
	  ///////////////////////////////////////////////////////////////
	  // let's check other syntaxes like here:
	  else
	  {
	    switch (tNode->syntax)
	    {
	     case 13: // DisplayString
	       tNode->mMinValue = 0;
	       tNode->mMaxValue = 255;
	       break;
	     case 14: // nothing special here for PhysAddress
	       break;
	     case 15: // MacAddress
	       tNode->mMinValue = 6;
	       tNode->mMaxValue = 6;
	       break;
	     case 16: // TruthValue
	     {
	       EnumNode* n = new EnumNode();
	       n->mValue = 1; // true
	       strcpy(n->mName, "true");
	       tNode->mEnumNodeList = n;
	       n = new EnumNode();
	       n->mValue = 2; // false
	       strcpy(n->mName, "false");
	       tNode->mEnumNodeList->mNext = n;
	     }
	       break;
	     case 17: // TestAndIncr
	       tNode->mMinValue = 0;
	       tNode->mMaxValue = 2147473647;
	       break;
	     case 18: // nothing special here for AutonomousType
	       tNode->mMinValue = 0;
	       tNode->mMaxValue = 2147473647;
	       break;
	     case 19: // nothing special here for VariablePointer
	       break;
	     case 20: // nothing special here for RowPointer
	       break;
	     case 21: // RowStatus
	     {
	       EnumNode* lastNode = NULL;
	       EnumNode* n = new EnumNode();
	       n->mValue = 1; // active
	       strcpy(n->mName, "active");
	       tNode->mEnumNodeList = n;
	       lastNode = n;
	       n = new EnumNode();
	       n->mValue = 2; // notInService
	       strcpy(n->mName, "notInService");
	       lastNode->mNext = n;
	       lastNode = n;
	       n = new EnumNode();
	       n->mValue = 3; // notReady
	       strcpy(n->mName, "notReady");
	       lastNode->mNext = n;
	       lastNode = n;
	       n = new EnumNode();
	       n->mValue = 4; // createAndGo
	       strcpy(n->mName, "createAndGo");
	       lastNode->mNext = n;
	       lastNode = n;
	       n = new EnumNode();
	       n->mValue = 5; // createAndWait
	       strcpy(n->mName, "createAndWait");
	       lastNode->mNext = n;
	       lastNode = n;
	       n = new EnumNode();
	       n->mValue = 6; // destroy
	       strcpy(n->mName, "destroy");
	       lastNode->mNext = n;
	     }
	       break;
	     case 22: // TimeStamp
	       break;
	     case 23: // TimeInterval
	       tNode->mMinValue = 0;
	       tNode->mMaxValue = 2147473647;
	       break;
	     case 24: // DateAndTime
	       // not really a range but lets pretend (asshole RFC writers)
	       tNode->mMinValue = 8;
	       tNode->mMaxValue = 11; 
	       break;
	     case 25: // StorageType
	     {
	       EnumNode* lastNode = NULL;
	       EnumNode* n = new EnumNode();
	       n->mValue = 1; // other
	       strcpy(n->mName, "other");
	       tNode->mEnumNodeList = n;
	       lastNode = n;
	       n = new EnumNode();
	       n->mValue = 2; // volatile
	       strcpy(n->mName, "volatile");
	       lastNode->mNext = n;
	       lastNode = n;
	       n = new EnumNode();
	       n->mValue = 3; // nonVolatile
	       strcpy(n->mName, "nonVolatile");
	       lastNode->mNext = n;
	       lastNode = n;
	       n = new EnumNode();
	       n->mValue = 4; // permanent
	       strcpy(n->mName, "permanent");
	       lastNode->mNext = n;
	       lastNode = n;
	       n = new EnumNode();
	       n->mValue = 5; // readOnly
	       strcpy(n->mName, "readOnly");
	       lastNode->mNext = n;
	     }
	       break;
	     case 26: // nothing special here for TDomain
	       break;
	     case 27: // TAddress
	       tNode->mMinValue = 1;
	       tNode->mMaxValue = 255; 
	       break;
	    }

	  }
	  ///////////////////////////////////////////////////////////////
	} // if (syn != NULL)

	lineBuf = lineBuffer;	

	// now look for the "::=" marker, skipping all lines in between
	BOOL specFound = FALSE;
	//	while ((tbuf = strstr(lineBuf, "::=")) == NULL)
	while (specFound == FALSE || !ifile->eof())
	{

	  if ((tbuf = strstr(lineBuf, "::=")) != NULL)
	  {
	    char* dashes = strstr(lineBuf, "--");
	    if (dashes != NULL)
	    {
	      if (dashes > tbuf)
	      {
		specFound = TRUE;
		*dashes = 0;
		break; // don't do the getline below
	      }
	    }
	    else
	    {
	      specFound = TRUE;
	      break; // don't do the getline below
	    }
	  }
	  //	  if (strstr(lineBuf, "DESCRIPTION"))
	  //	    skipDescription(lineBuf, ifile);

	  lineBuf = lineBuffer;	
	  memset(lineBuffer, 0, 4096);
	  ifile->getline(lineBuffer, 4096);
	  
	  // ignore comments
	  //	  if (isComment(lineBuf))
	  //	    continue;
	}	
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
	    strncpy(tNode->oid, curly, 256);
	    tNode->insert(tNode);
	  }
	}
      }
    }
    catch(...)
    {
      
    }
  }
  ifile->clear();
  ifile->seekg(0);
}

BOOL
MibParser::isObjectType(char* lineBuf)
{
  if (strstr(lineBuf, "OBJECT-TYPE"))
    return TRUE;
  return FALSE;
}

