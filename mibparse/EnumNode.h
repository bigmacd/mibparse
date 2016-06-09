#ifndef __ENUMNODE_H__
#define __ENUMNODE_H__

#ifdef _WIN32
#include <windows.h>
#else
#define NULL 0L
#include <string.h>
#endif

class EnumNode
{
  public:

    EnumNode();
    EnumNode(EnumNode* n);
    ~EnumNode();

    unsigned int mValue;
    char mName[256];

    EnumNode* mNext;
      
};


#endif
