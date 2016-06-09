#define __NODE_H__

#include "EnumNode.h"

#ifndef _WIN32
#define BOOL bool
#define TRUE true
#define FALSE false
#endif

class node
{
  private:

    static node* mFirstNode;
    static node* mLastNode;
    static node* mFirstTrap;
    static node* mLastTrap;

    void         normalize(node* Node);

  protected:
 

   public:

    node();
    node(node* n);

    char oid[256];
    char name[256];

    BOOL used;
    BOOL trap;
    int  specificType;

    int  mMinValue;
    int  mMaxValue;

    int  syntax;

    node* next;
    node* prev;

    EnumNode* mEnumNodeList;

    node*     NodeList() { return mFirstNode; }

    void      insert(node* Node);
      
};

#endif
