#include "stdlib.h"

node* node::mFirstNode = NULL;
node* node::mLastNode = NULL;
node* node::mFirstTrap = NULL;
node* node::mLastTrap = NULL;

node::node()
     :used(false),
      trap(false),
      next(NULL),
      prev(NULL),
      syntax(-1),
      mMinValue(-1),
      mMaxValue(-1),
      mEnumNodeList(NULL),
      specificType(0)
{
  memset(oid, 0, 256);
  memset(name, 0, 256);
}

node::node(node* n)
{
  if (n != NULL)
  {
#ifdef _WIN32
    strcpy_s(oid, 255, n->oid);
    strcpy_s(name, 255, n->name);
#else
    strcpy(oid, n->oid);
    strcpy(name, n->name);
#endif
    used = n->used;
    trap = n->trap;
    specificType = n->specificType;
    syntax = n->syntax;
  }
  next = NULL;
  prev = NULL;
}

void       
node::insert(node* Node)
{
  normalize(Node);
  //  if(Node->trap == FALSE)

  if (TRUE == Node->trap)
  {
    char* st = strrchr(Node->oid, '.');
    if (NULL != st)
      Node->specificType = atoi(++st);
  }

  
  if(1)
  {
    if (mFirstNode == NULL)
    {
      mFirstNode = Node;
      mLastNode = Node;
    }
    else
    {
      mLastNode->next = Node;
      Node->prev = mLastNode;
      mLastNode = Node;
    }
  }
#if 0
  else
  {
    if (mFirstTrap == NULL)
    {
      mFirstTrap = Node;
      mLastTrap = Node;
    }
    else
    {
      mLastTrap->next = Node;
      Node->prev = mLastTrap;
      mLastTrap = Node;
    }
  }
#endif
  Node->next = NULL;
}

void
node::normalize(node* Node)
{
  char tbuf[256];
  memcpy(tbuf, Node->oid, 256);

  memset(Node->oid, 0, 256);
  int index1 = 0;
  int index2 = 0;
  
  BOOL lastWasSpace = FALSE;
  while(tbuf[index1] != 0)
  {
    if (tbuf[index1] != ' ' && tbuf[index1] != '\t') // not a space or a tab
    {
      Node->oid[index2++] = tbuf[index1++]; // just copy it
      lastWasSpace = FALSE;
      continue;
    }
    if (!lastWasSpace)
    {
      lastWasSpace = TRUE;
      Node->oid[index2++] = '.'; // replace first space with dot
      index1++;
      continue;
    }      
    index1++; // skip it
  }

#if 0
  if (strlen(Node->oid))
  {
    for (unsigned int x = 0; x < 256; x++)
    {
      if (Node->oid[x] == 0)
	break;
      if (Node->oid[x] == ' ')
	Node->oid[x] = '.';
    }
  }
#endif
}

