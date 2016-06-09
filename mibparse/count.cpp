
#ifndef __MIBPARSER_H__
#include "MibParser.h"
#endif

void
MibParser::count(ifstream* ifile)
{
  while (!ifile->eof())
  {
    try 
    {
      // read each line of file
      char lineBuf[4096];
      ifile->getline(lineBuf, 4096);

      if (isObjectType(lineBuf))
	mNumberOfVariables++;
    }
    catch(...)
    {
    }
  }
  ifile->clear();
  ifile->seekg(0);
}
