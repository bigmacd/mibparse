#define __VALUERANGENODE_H__

#ifdef _WIN32
#include <windows.h>
#else
#define NULL 0L
#endif

class node;

class ValueRangeNode
{
  public:

    ValueRangeNode();
    ValueRangeNode(ValueRangeNode* n);
    ~ValueRangeNode();

    unsigned int mMinValue;
    unsigned int mMaxValue;

    node* mNext;
      
};


#endif
