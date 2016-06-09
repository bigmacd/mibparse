#ifndef __MIBPARSER_H__
#define __MIBPARSER_H__

#ifndef __NODE_H__
#include "Node.h"
#endif

#ifndef __MIBDB_H__
#include "MibDb.h"
#endif

#ifndef _WIN32
#define BOOL bool
#endif

//#include <fstream.h>
#include <stdio.h>

#include <fstream>
using namespace std;

class MibParser
{

  private:

    char             mMibName[128];

    unsigned int     mNumberOfVariables;
    unsigned int     mNumberOfObjectIds;

    BOOL             mDebug;

    BOOL             isComment(char* lineBuf);
    BOOL             isObjectId(char* lineBuf);
    BOOL             isObjectType(char* lineBuf);
    BOOL             isDefinition(char* lineBuf);
    BOOL             isModuleIdentity(char* lineBuf);
    BOOL             isImport(char* lineBuf);    
    BOOL             isTrapType(char* lineBuf);
    BOOL             isObjectIdentity(char* lineBuf);

    void             substitute();

  //#ifdef _WIN32
    void             skipDescription(char* currentLine, std::ifstream* ifile);
    void             skipImports(std::ifstream* ifile);
    void             count(std::ifstream* ifile);
    void             doNotificationTypes(std::ifstream* ifile);
    void             doObjectIds(std::ifstream* ifile);
    void             doObjectTypes(std::ifstream* ifile);
    void             doMibName(std::ifstream* ifile);
    void             doModuleIdentity(std::ifstream* ifile);
    void             doTrapTypes(std::ifstream* ifile);
    void             doImports(std::ifstream* ifile);
    void             doObjectIdentity(std::ifstream* ifile);

    void             DebugOut();
    //    void             reorder();
    //    node*            findallchildren(char* name);

  protected:

  public:

    MibParser();
    ~MibParser();

    bool Parse(MibDb* db, char* filename);

    node* Nodes();

    char* MibName() { return mMibName; }

    unsigned int     NumberOfVariables() { return mNumberOfVariables; }
    unsigned int     NumberOfObjectIds() { return mNumberOfObjectIds; }

    void             Debug(BOOL onOff) { mDebug = onOff; };


};
#endif










