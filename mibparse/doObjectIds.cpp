
#ifndef __MIBPARSER_H__
#include "MibParser.h"
#endif


void
MibParser::doObjectIds(ifstream* ifile)
{
  // first let's do object identifiers
  while (!ifile->eof())
  {
    try 
    {
      // read each line of file
      char lineBuffer[4096];
      char* lineBuf = lineBuffer;
      memset(lineBuffer, 0, 4096);

      int lastPos = ifile->tellg();
      int cnt = ifile->gcount();

      lastPos -= (cnt + 1);

      ifile->getline(lineBuffer, 4096, '\n');

      // ignore comments
      if (isComment(lineBuf))
	continue;

      if (strstr(lineBuf, "DESCRIPTION"))
	skipDescription(lineBuf, ifile);

      if (isObjectId(lineBuf))
      {
	mNumberOfObjectIds++;
	// skip white space
	while (*lineBuf == ' ' || *lineBuf == '\t')
	  lineBuf++;

	node* tNode = new node;
	char* tbuf = NULL;

	if (!strncmp("OBJECT IDENTIFIER", lineBuf, 17)) // missing the name, probably on previous line
	{
	  int newPos = ifile->tellg();
	  int newCnt = ifile->gcount();

	  newPos -= (newCnt);

	  ifile->seekg(lastPos + 1);

	  char lastPosBuffer[2048];
	  memset(lastPosBuffer, 0, 2048);
	  ifile->getline(lastPosBuffer, 2048, '\n');
	  ifile->seekg(newPos);

	  memset(lineBuffer, 0, 4096);
	  //	  ifile.getline(lineBuffer, 4096);
	  ifile->getline(lineBuffer, 4096, '\n');

	  lineBuf = lineBuffer;
	  while (*lineBuf == ' ' || *lineBuf == '\t')
	    lineBuf++;

	  int len = strlen(lastPosBuffer);
	  if (lastPosBuffer[len - 1] == '\r')
	  {
	    lastPosBuffer[len - 1] = 0;
	    len -= 1;
	  }
	  char* lastPosBuf = lastPosBuffer;
	  while (*lastPosBuf == ' ' || *lastPosBuf == '\t')
	    lastPosBuf++;

	  strncpy(tNode->name, lastPosBuf, min(255, len));
	}
	else
	{
	  // find first blank after non-white space
	  tbuf = lineBuf;
	  while (*tbuf != ' ' && *tbuf != '\t')
	    tbuf++;
	  
	  // save the char off
	  char tc = *tbuf;
	  
	  // null terminate the string
	  *tbuf = 0;
	  
	  // save the string off
	  strncpy(tNode->name, lineBuf, 255);

	  // restore the char
	  *tbuf = tc;
	}

	// find oid info specification
	lineBuf = lineBuffer;
	tbuf = strchr(lineBuf, '{');

	// did not find it, check next line
	if (tbuf == NULL)  
	{

	  //read next line of file
	  char* lineBuf = lineBuffer;
	  memset(lineBuffer, 0, 4096);
	  ifile->getline(lineBuffer, 4096);

	  tbuf = strchr(lineBuf, '{');

	  if (tbuf == NULL) {
	    // 10/20/14 - fix for parsing liebert and other mib format
	    // now look for the "::=" marker, removing all lines in between
	    while ((tbuf = strstr(lineBuf, "::=")) == NULL)
	    {
	      if (strstr(lineBuf, "DESCRIPTION"))
		skipDescription(lineBuf, ifile);
	      else 
	      {
		lineBuf = lineBuffer;	
		memset(lineBuffer, 0, 4096);
		ifile->getline(lineBuffer, 4096);
	      }
	    
	      // ignore comments
	      if (isComment(lineBuf))
		continue;
	    }	
	    tbuf += 3;
	    // end of 10/20/14 fix
	  }

	}

	// do we have the oid info now?
	if (tbuf != NULL)
	{
	  tbuf++;
	  // skip white space
	  while (*tbuf == ' ' || *tbuf == '\t' || *tbuf=='{')
	    tbuf++;
	  char* t2buf = strchr(tbuf, '}');
	  if (t2buf)
	  {
	    t2buf--;
	    // skip white space
	    while (*t2buf == ' ' || *t2buf == '\t')
	      t2buf--;
	    *++t2buf = 0;
	    ///////////////////////////////////////////////212
	    int index = 0;
	    char* paren = NULL;
	    char* space = NULL;
	    BOOL hadParen = FALSE;
	    while (tbuf <= t2buf)
	    {
	      // let's see what we have
	      paren = strchr(tbuf, '(');
	      space = strchr(tbuf, ' ');

	      if (paren == NULL)
	      {
		if (hadParen)
		  strcat(tNode->oid, tbuf);
		else
		  strncpy(tNode->oid, tbuf, 256);
		tNode->insert(tNode);
		// done
		break;
	      }
	      else
		hadParen = TRUE;

	      // what's first, a space or a '('?
	      if (space && space < paren) // a space is first
	      {
		*space = 0;
		strcat(tNode->oid, tbuf); 
		strcat(tNode->oid, " ");
		tbuf = ++space; // past the space (' ')
		// keep looking
		continue;
	      }

	      // we are here because the first thing of 
	      // interest is a open paren "("
	      paren++;
	      while (*paren == ' ' || *paren == '\t')
		paren++;
	      char* closeParen = strchr(paren, ')');
	      if (closeParen == NULL) // you are fucking kidding me
	      {
		delete tNode;
		break;
	      }
	      char* tempPtr = closeParen;
	      --tempPtr;
	      while (*tempPtr == ' ' || *tempPtr == '\t')
		--tempPtr;
	      *++tempPtr = 0;
	      strcat(tNode->oid, paren);
	      strcat(tNode->oid, " ");
	      tbuf = ++closeParen;
	      while (*tbuf == ' ' || *tbuf == '\t')
		tbuf++;
	      continue;
	    }
	    ///////////////////////////////////////////////212
	  } // if (t2buf)
	} // if (tbuf != NULL)
	else
	  if (tNode != NULL)
	    delete tNode;

	continue;
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
MibParser::isObjectId(char* lineBuf)
{
  char* tPtr = strstr(lineBuf, "OBJECT IDENTIFIER");
  if (tPtr != NULL)
    //    if (strstr(tPtr+18, "::="))
      return TRUE;
  return FALSE;
}

#if 0
	    //////////////////////////////////////////////////212
	    // processes stupid shit like iso(1) org(3)
	    if (!strchr(tbuf, '('))
	    {
	    //////////////////////////////////////////////////212
	      strncpy(tNode->oid, tbuf, 256);
	      tNode->insert(tNode);
	    //////////////////////////////////////////////////212
	    }
	    else
	    {
	      char* openParen = NULL;
	      char* closeParen = NULL;
	      openParen = strchr(tbuf, '(');
	      while (openParen != NULL)
	      {
		openParen++;
		while (*openParen == ' ' || *openParen == '\t')
		  openParen++;
		closeParen = strchr(openParen, ')');
		if (closeParen == NULL) // check next line
		{
		  memset(lineBuf, 0, 4096);
		  char* lineBuffer = lineBuf;
		  ifile.getline(lineBuf, 4096);
		  closeParen = strchr(openParen, ')');
		  if (closeParen == NULL)
		    break;
		}		  
		// here we are ok
		*closeParen = 0;
		strcat(tNode->oid, openParen);
		strcat(tNode->oid, " ");
		openParen = strchr(++closeParen, '(');
	      } // while (openParen != NULL)
	      
	      if (closeParen != NULL) // did not find it above
	      {
		// save off the rest
		  while (*closeParen == ' ' || *closeParen == '\t')
		    closeParen++;
		  strcat(tNode->oid, closeParen);
		  tNode->insert(tNode);
	      }
	    }
	    //////////////////////////////////////////////////212
#endif
