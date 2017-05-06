#ifndef __MIBNODE_H__
#include "MibNode.h"
#endif

MibNode::MibNode()
        :mUsed(false),
         mTrap(false),
         mChild(NULL),
         mParent(NULL),
         mSyntax(0),
         mAccess(0),
         mStatus(0),
         mOid(0),
         mNumChildren(0)
{
  memset(mName, 0, 256);
  memset(mDescription, 0, 256);
}

MibNode::MibNode(MibNode* n)
        :mUsed(false),
         mTrap(false),
         mChild(NULL),
         mParent(NULL),
         mSyntax(0),
         mAccess(0),
         mStatus(0),
         mOid(0),
         mNumChildren(0)
{
  if (n != NULL)
  {
    if (n->mName != NULL)
#ifdef _WIN32
      strcpy_s(mName, 255, n->mName);
#else
      strcpy(mName, n->mName);
#endif
    if (n->mDescription != NULL)
#ifdef _WIN32
      strcpy_s(mDescription, 255, n->mDescription);
#else
      strcpy(mDescription, n->mDescription);
#endif
    mUsed = n->mUsed;
    mTrap = n->mTrap;
    mSyntax = n->mSyntax;
    mAccess = n->mAccess;
    mStatus = n->mStatus;
    mOid = n->mOid;
  }
}

