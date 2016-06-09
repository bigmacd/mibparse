#define __MIBTREE_H__

#include "MibNode.h"

// RO read only
// RW read write
// M mandatory
// C current
// NA not accessible

struct mibData
{
  unsigned int oid;
  char         name[256];
  int          syntax;
  int          access;
  int          status;
  int          childCount;
};

class MibTree
{

  private:

    MibNode*          mRootNode;



  protected:



  public:


    MibTree();
    ~MibTree();


    BOOL              Import(char* filename);


    char*             NameToOid(char* name);
    MibNode*          FindOid(char* oid);
    MibNode*          FindName(char* name);    

    void              BuildKnownTree(char* filename);


};

#endif
