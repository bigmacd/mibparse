
#ifndef __MIBPARSER_H__
#include "MibParser.h"
#endif

void
MibParser::doModuleIdentity(ifstream* ifile)
{
  skipImports(ifile);

  // first let's do MODULE-IDENTITY
  while (!ifile->eof())
  {
    try 
    {
      // read each line of file
      char lineBuffer[4096];
      char* lineBuf = lineBuffer;
      memset(lineBuffer, 0, 4096);

      ifile->getline(lineBuffer, 4096);

      // ignore comments
      if (isComment(lineBuf))
	continue;

      
      if (isModuleIdentity(lineBuf))
      {
	// skip white space
	while (*lineBuf == ' ' || *lineBuf == '\t')
	  lineBuf++;

	// find first blank after non-white space
	char* tbuf = lineBuf;
	while (*tbuf != ' ' && *tbuf != '\t')
	  tbuf++;

	// save the char off
	char tc = *tbuf;

	// null terminate the string
	*tbuf = 0;
	node* tNode = new node;

	// save the string off
	strncpy(tNode->name, lineBuf, 256);

	// restore the char
	*tbuf = tc;
	lineBuf = lineBuffer;

	////////////////////////////////////
	while (!strstr(lineBuf, "::="))
	{
	  memset(lineBuffer, 0, 4096);
	  lineBuf = lineBuffer;	
	  ifile->getline(lineBuffer, 4096);
	}
	////////////////////////////////////

	tbuf = strchr(lineBuf, '{');
	if (tbuf)
	{
	  tbuf++;
	  // skip white space
	  while (*tbuf == ' ' || *tbuf == '\t')
	    tbuf++;
	  char* t2buf = strchr(tbuf, '}');
	  if (t2buf)
	  {
	    t2buf--;
	    // skip white space
	    while (*t2buf == ' ' || *t2buf == '\t')
	      t2buf--;
	    *++t2buf = 0;
	    strncpy(tNode->oid, tbuf, 256);
	    tNode->insert(tNode);
	  }
	}

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
MibParser::isModuleIdentity(char* lineBuf)
{
  char* tPtr = strstr(lineBuf, "MODULE-IDENTITY");
  if (tPtr != NULL)
      return TRUE;
  return FALSE;
}
