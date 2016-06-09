#pragma warning(disable: 4786)

#ifndef __MIBPARSER_H__
#include "MibParser.h"
#endif

void
MibParser::doImports(ifstream* ifile)
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

      ifile->getline(lineBuffer, 4096);

      // ignore comments
      if (isComment(lineBuf))
	continue;

      if (isImport(lineBuf))
      {
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
MibParser::isImport(char* lineBuf)
{
  char* tPtr = strstr(lineBuf, "IMPORTS");
  if (tPtr != NULL)
      return TRUE;
  return FALSE;
}
