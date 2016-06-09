#include "MibParser.h"

extern char* syntaxes[];

void main(int argc, char** argv);

void
main(int argc, char** argv)
{
  if (argc != 3)
    return;

  MibParser mp;
  mp.Parse(argv[1]);


  // output the results
  cout << "Number of variables: " << mp.NumberOfVariables() << endl;
  cout << "Number of object ids: " << mp.NumberOfOjectIds() << endl;
  cout << "MIB name is: " << mp.MibName() << endl;
  node* n = mp.Nodes();//firstNode;
  while(n != NULL)
  {
    //
    cout << n->oid << '\t' << n->name;
    if (n->syntax >= 0)
    {
      cout << "\t" << syntaxes[n->syntax];
      //      if (!strcmp(syntaxes[n->syntax], "INTEGER"))
      switch(n->syntax)
      {
       case 1:
       case 13:
       case 15:
       case 16:
       case 17:
       case 18:
       case 21:
       case 23:
       case 24:
       case 25:
       case 27:
       {
	if (n->mMinValue != -1)
	  cout << ' ' << n->mMinValue << ' ' << n->mMaxValue;
	
	if (n->mEnumNodeList != NULL)
	{
	  EnumNode* en = n->mEnumNodeList;
	  while (en != NULL)
	  {
	    cout << endl;
	    cout << "\t\t\t" << en->mName << '\t' << en->mValue;
	    en = en->mNext;
	  }
	}
       }
       break;
      } // switch (n->syntax)
    }
    if (n->trap)
      cout << "\t TRAP";

    cout << endl;
    n = n->next;
  }
  

}
