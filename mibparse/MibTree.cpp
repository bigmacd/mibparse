
#ifndef __MIBTREE_H__
#include "MibTree.h"
#endif

#ifndef _WIN32
#define TRUE true
#define FALSE false
#endif

MibTree::MibTree()
        :mRootNode(NULL)
{

//  BuildKnownTree(char* filename);



}


MibTree::~MibTree()
{


}

void
MibTree::BuildKnownTree(char* filename)
{
  BOOL done = FALSE;
  MibNode* currentNode = mRootNode;
  while (!done)
  {
    currentNode = new MibNode();


  } // while (!done)
}

    
BOOL 
MibTree::Import(char* filename)
{

return FALSE;
}

char*         
MibTree::NameToOid(char* name)
{

return NULL;
}


MibNode*       
MibTree::FindOid(char* oid)
{

return NULL;
}

MibNode*         
MibTree::FindName(char* name)  
{

return NULL;
}

