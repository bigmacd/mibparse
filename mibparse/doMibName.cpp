
#ifndef __MIBPARSER_H__
#include "MibParser.h"
#endif

void
MibParser::doMibName(ifstream* ifile)
{
  // first let's find the line with the name
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

      if (isDefinition(lineBuf))
      {
	char* def = strstr(lineBuf, "DEFINITIONS");
	if (def != NULL)
	{
	  def--;
	  while (def != lineBuf)
	  {
            // trim off trailing whitespace
	    if (*(def - 1) == ' ' || *(def - 1) == '\t') 
	      def--;
	    else
	      break;
	  } // while (def != lineBuf)
	  *def = 0;
	  // now trim off leading whitespace
	  while (*lineBuf == ' ' || *lineBuf == '\t')
	    lineBuf++;
	  strncpy(mMibName, lineBuf, 127);
	} // if (def != NULL)
	break; // out of while loop
      } // if (isDefinition(lineBuf))
    }
    catch(...)
    {
    }
  } // while (!ifile.eof())

  ifile->clear();
  ifile->seekg(0);
}

BOOL
MibParser::isDefinition(char* lineBuf)
{
  char* tPtr = strstr(lineBuf, "DEFINITIONS");
  if (tPtr != NULL)
    if (strstr(tPtr+11, "::="))
      return TRUE;
  return FALSE;
}
