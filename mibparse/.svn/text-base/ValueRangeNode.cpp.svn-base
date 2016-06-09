#include "ValueRangeNode.h"

ValueRangeNode::ValueRangeNode()
        :mNext(NULL),
         mMinValue(0),
	 mMaxValue(0)
{
}

ValueRangeNode::ValueRangeNode(ValueRangeNode* n)
        :mNext(NULL),
         mMinValue(0),
         mMaxValue(0)
{
  if (n != NULL)
  {
    mMinValue = n->mMinValue;
    mMaxValue = n->mMaxValue;
  }
}

