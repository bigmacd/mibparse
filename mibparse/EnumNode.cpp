
EnumNode::EnumNode()
        :mNext(NULL),
         mValue(0)
{
  memset(mName, 0, 256);
}

EnumNode::EnumNode(EnumNode* n)
        :mNext(NULL),
         mValue(0)
{
  if (n != NULL)
  {
    if (n->mName != NULL)
#ifdef _WIN32
      strcpy_s(mName, 255, n->mName);
#else
      strcpy(mName, n->mName);
#endif
    mValue = n->mValue;
  }
}

EnumNode::~EnumNode()
{
  EnumNode* n = mNext;
  while (n != NULL)
  {
    delete n;
    n = n->mNext;
  }
}
