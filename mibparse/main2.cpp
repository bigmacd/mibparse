
#include <iostream>
using namespace std;

extern char* knownoids[][2];
//extern char* syntaxes[];

void main(int argc, char** argv);

void
main(int argc, char** argv)
{
  if (argc != 2)
    return;
  MibDb db("Mibdb.db");
  MibParser mp;
  mp.Debug(TRUE);
  if (mp.Parse(&db, argv[1]))
  {
    // output the results
    std::cout << "Number of variables: " << mp.NumberOfVariables() << endl;
    std::cout << "Number of object ids: " << mp.NumberOfOjectIds() << endl;
    std::cout << "MIB name is: " << mp.MibName() << endl;
  }
  else
    std::cout << "parse failed, possible duplicates" << endl;
}
