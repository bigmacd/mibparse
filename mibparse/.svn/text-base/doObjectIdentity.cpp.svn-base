#pragma warning(disable: 4786)

#ifndef __MIBPARSER_H__
#include "MibParser.h"
#endif

void
MibParser::doObjectIdentity(ifstream* ifile)
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
      if (isObjectIdentity(lineBuf))
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
MibParser::isObjectIdentity(char* lineBuf)
{
  if (strstr(lineBuf, "OBJECT-IDENTITY"))
    return TRUE;
  return FALSE;
}

