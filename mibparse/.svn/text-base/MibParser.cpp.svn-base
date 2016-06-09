#pragma warning(disable: 4786)

#ifndef __MIBPARSER_H__
#include "MibParser.h"
#endif

#include "knownoids.h"

#include <fstream>
using namespace std;

#ifndef _WIN32
#define TRUE true
#define FALSE false
#endif

MibParser::MibParser()
          :mNumberOfVariables(0),
	   mNumberOfObjectIds(0)
{
  memset(mMibName, 0, 128);
}

MibParser::~MibParser()
{

}

bool
MibParser::Parse(MibDb* db, char* filename)
{
  bool retVal = false;

  std::ifstream ifile(filename, ios::binary);

  if (ifile.good())
  {
    count(&ifile);
    doMibName(&ifile);
    doModuleIdentity(&ifile);
    doImports(&ifile);
    doObjectIds(&ifile);
    doObjectIdentity(&ifile);
    doObjectTypes(&ifile);
    doTrapTypes(&ifile);
    doNotificationTypes(&ifile);

    char inoid[256];
    memset(inoid, 0, 256);
    char dotOid[256];
    memset(dotOid, 0, 256);
    char* inOID = inoid;

    char topOid[64];

    node nd;
    node* n = nd.NodeList();


    if (n != NULL)
    {
      // locate the topmost oid
      if (!(n->oid[0] >= '1' && n->oid[0] <= '9'))
      {
	char* dot = strchr(n->oid, '.');
	if (dot != NULL)
	  *dot = 0;
	
	memset(topOid, 0, 64);
	strcpy(topOid, n->oid); 
	
	if (dot != NULL)
	{
	  *dot = '.';
	  strcpy(dotOid, dot);
	}
	
	int index = 0;
	char* knownOid;
	while ((knownOid = knownoids[index][0]) != NULL)
	{
	  if (!strcmp(topOid, knownOid))
	  {	  
	    if (dot != NULL)
	      sprintf(n->oid, "%s%s", knownoids[index][1], dotOid);
	    else
	      sprintf(n->oid, "%s", knownoids[index][1]);
	    break;
	  }
	  index++;
	}
      }
    } // if (n != NULL)

    for (int x = 0; x < 20; x++) // why 10?  hmmm...
      substitute();
  }
  ifile.close();

  // do database inserts here
  //db->Connect("Mibdb.db");
  if (db->InsertMibName(mMibName))
  {

    retVal = true;

    db->FindMib(mMibName);
    int mibId = db->MibId();

    node* n = Nodes();
    while (NULL != n)
    {
      db->InsertOid(n->oid,
		    n->name,
		    mibId,
		    (n->mEnumNodeList == NULL) ? 0 : 1,
		    n->trap,
		    n->specificType);
      
      if (n->mEnumNodeList != NULL)
      {
	db->FindOid(n->oid);
	int oidId = db->OidId();
	EnumNode* en = n->mEnumNodeList;
	while (en != NULL)
	{
	  db->InsertEnum(oidId,
			en->mValue,
			en->mName);
	  en = en->mNext;
	} // while (en != NULL)
      } // if (n->mEnumNodeList != NULL)
      
      n = n->next;
    } // while (NULL != n)
  } // if (db->InsertMibName(mMibName))

  DebugOut();

  return retVal;
}
  
void
MibParser::DebugOut()
{
  if (FALSE == mDebug) return;

  node* n = Nodes(); //firstNode;
  while(n != NULL)
  {
    if (n->mEnumNodeList != NULL)
    {
      EnumNode* en = n->mEnumNodeList;
      while (en != NULL)
      {
	std::cout << "enum list insert:" << endl;
	std::cout << "\t" << MibName() << endl;
	std::cout << "\t" << n->oid << endl;
	std::cout << "\t" << n->name << endl;
	std::cout << "\t" << en->mValue << endl;
	std::cout << "\t" << en->mName << endl;
	en = en->mNext;
      } // while (en != NULL)
    } // if (n->mEnumNodeList != NULL)
    else
    {
      std::cout << "object insert:" << endl;
      if (n->trap)
      {
	std::cout << "\tTrap" << endl;
	std::cout << "\tSpecificType: " << n->specificType << endl;
      }
      std::cout << "\t" << MibName() << endl;
      std::cout << "\t" << n->oid << endl;
      std::cout << "\t" << n->name << endl;
    }
    n = n->next;
  }
}

node* 
MibParser::Nodes()
{
  node n;
  return n.NodeList();
}

BOOL
MibParser::isComment(char* lineBuf)
{
  BOOL retVal = FALSE;

  while(1)
  {
    if (*lineBuf == 0)
      break;

    if (*lineBuf == ' ')
    {
      lineBuf++;
      continue;
    }

    if (*lineBuf == '\t')
    {
      lineBuf++;
      continue;
    }

    if (*lineBuf == '-')
    {
      lineBuf++;
      if (*lineBuf == '-')
      {
	retVal = TRUE;
	break;
      }
      continue;
    }
    //    lineBuf++;
    break;
  }
  
  return retVal;
}

void
MibParser::skipImports(std::ifstream* ifile)
{
  char lineBuffer[4096];
  char* lineBuf = lineBuffer;
  memset(lineBuffer, 0, 4096);
  ifile->getline(lineBuffer, 4096);

  while (!ifile->eof())
  {
    memset(lineBuffer, 0, 4096);
    ifile->getline(lineBuffer, 4096);

    if (isComment(lineBuf))
      continue;

    if (strstr(lineBuf, "IMPORTS"))
    {
      while (!ifile->eof())
      {
	lineBuf = lineBuffer;
	memset(lineBuffer, 0, 4096);
	ifile->getline(lineBuffer, 4096);

	if (isComment(lineBuf))
	  continue;

	if (strchr(lineBuf, ';'))
	{
	  lineBuf = lineBuffer;
	  memset(lineBuffer, 0, 4096);
	  ifile->getline(lineBuffer, 4096);
	  break;
	}
      }
      break;
    }
  }
}

void
MibParser::skipDescription(char* currentLine,
			   ifstream* ifile)
{
//  char lineBuffer[4096];
//  char* lineBuf = lineBuffer;
  char* quotePtr = NULL;

  int loop = 0;

  char* tqPtr = NULL;
  if (tqPtr = strchr(currentLine, '\"'))  // found opening quote
  {
    tqPtr++;
    loop = 1;
    if (tqPtr = strchr(currentLine, '\"')) // found closing quote
    {
      // advance to next line and get out
//      lineBuf = lineBuffer;
//      memset(lineBuffer, 0, 4096);
//      ifile->getline(lineBuffer, 4096);
      memset(currentLine, 0, 4096);
      ifile->getline(currentLine, 4096);
      return;
    }
  }

  // we are here if we found zero or one quotes
  while (!ifile->eof())
  {
    quotePtr = NULL;
//    lineBuf = lineBuffer;
//    memset(lineBuffer, 0, 4096);
	memset(currentLine, 0, 4096);
//    ifile->getline(lineBuffer, 4096);
	ifile->getline(currentLine, 4096);
//    if ((quotePtr = strchr(lineBuf, '\"')) != NULL)
    if ((quotePtr = strchr(currentLine, '\"')) != NULL)
    {
      loop++; // count how many we found
      quotePtr++;
      if (strchr(quotePtr, '\"')) // found second one on same line
	loop++;

      if (loop >= 2) // should never be more than two, but just in case
	break;
    }
  }
}

void
MibParser::substitute()
{
  node nd;
  node* Node = nd.NodeList();
  while (Node != NULL)
  {
    // if this oid is non-numeric
    if (Node->oid != NULL && (Node->oid[0] < '0' || Node->oid[0] > '9'))
    {
      char remainder[128];
      // chop off after the symbolic name part and save it
      char* dot = strchr(Node->oid, '.');
      if (dot != NULL)
      {
	memset(remainder, 0, 128);
	strncpy(remainder, dot, 128);
	*dot = 0;
      }

      // go through list and find a name that matches this oid
      // if (knownOid == NULL) // was not found in knownoids list
      ////////////////////////////////////213 check the current list first
      BOOL found = FALSE;

      node nd2;
      node* tnode = nd2.NodeList();
      while(tnode != NULL)
      {
	// is this symbolic oid 
	if (!strcmp(tnode->name, Node->oid))
	{
	  memset(Node->oid, 0, 256);
	  strcpy(Node->oid, tnode->oid);
	  if (dot != NULL)
	    strcat(Node->oid, remainder);
	  found = TRUE;
	  break;
	}
	tnode = tnode->next;
      }

      if (!found)
      {
	// is this a knownoid?
	int index = 0;
	char* knownOid;
	while ((knownOid = knownoids[index][0]) != NULL)
	{
	  if(!strcmp(knownOid, Node->oid))
	  {
	    memset(Node->oid, 0, 256);
	    strcpy(Node->oid, knownoids[index][1]);
	    if (dot != NULL)
	      strcat(Node->oid, remainder);
	    found = TRUE;
	    break;
	  }
	  index++;
	}
      }
      if (!found)
	if (dot != NULL)
	  *dot = '.';

    } // if (Node->oid != NULL && (Node->oid[0] < '0' || Node->oid[0] > '9'))
    Node = Node->next;
  }
}
