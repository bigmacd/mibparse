#define __MIBNODE_H__

#ifdef _WIN32
#include <windows.h>
#else
#define BOOL bool
#endif

#include "Node.h"
#include "EnumNode.h"


class MibNode
{
  public:

    MibNode();
    MibNode(MibNode* n);
    ~MibNode();

    unsigned int mOid;
    char         mName[256];

    BOOL         mUsed;
    BOOL         mTrap;

    int          mSyntax;
    int          mAccess;
    int          mStatus;
    char         mDescription[256];

    int          mMinValue;
    int          mMaxValue;

    node*        mChild;
    node*        mParent;

    EnumNode*    mEnumList;

    int          mNumChildren;
      
};


#endif
