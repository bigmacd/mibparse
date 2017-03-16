#ifndef __MIBDB_H__
#define __MIBDB_H__

#include "SqliteConnection.h"
#include "SqliteRecordSet.h"

using namespace std;
using namespace SqliteClass;
#include <string>

// data sets
#define MIBDATASET 0
#define OIDDATASET 1
#define ENUMDATASET 2


class MibDb
{

  private:

    string               mErrorString;

    SqliteConnection     mConnection;
    SqliteRecordSet*     mRecordSet;

    bool                 connected;

  // MibName table
    int                  mMibNameId;
    string               mMibName;

  // Oids table
    int                  mOidsId;
    string               mOid;
    string               mOidsName;
    int                  mOidsMibNameId;
    bool                 mEnumFlag;
    bool                 mTrapFlag;
    int                  mSpecificType;
    string               mTrapName;

  // Enum table
    string               mEnumName;
    int                  mEnumValue;

    void                 PopulateOidData();
    bool                 FindOidsByMibId(int mibId);
    void                 DeleteEnums(int oidId);

  protected:



  public:

    MibDb();
    MibDb(std::string dbName);
    ~MibDb();

    bool               Connect(std::string dbName);

    // MibName stuff
    bool               InsertMibName(string name);
    bool               DeleteMib(string name);             // deletes OIDs from this MIB too
    bool               FindMib(string name);               // checks if MIB is already loaded
    bool               GetAllMIBs();                       // for looping through all names

    int                MibId();                            // column accessor
    string             MibName();                          // column accessor

    // Oids stuff
    bool               InsertOid(string oid,
				 string name, 
				 int mibnameId,
				 int enumFlag,
				 int trapFlag,
				 int trapSpecificType);

    bool               FindOid(string oid);                // looks up OID 
    bool               FindNearestOid(string oid);         // looks up OID 
    bool               FindTrap(string oid, int sType);    // looks up OID as trap

    int                OidId();                            // column accessor
    string             Oid();                              // column accessor
    string             OidsName();                         // column accessor
    int                OidsMibNameId();                    // column accessor
    bool               IsTrap();                           // column accessor
    bool               IsEnum();                           // column accessor
    int                SpecificType();                     // column accessor
    string             TrapName();                         // column accessor

    // Enum stuff
    bool               InsertEnum(int oidId,
				  int value,
				  string name);
    bool               FindEnum(string oid, int value);           // looks up OID as Enum
    bool               FindNearestEnum(string oid, int value);    // looks up OID as Enum
    string             EnumName();                                // column accessor
//  int                EnumValue();                               // column accessor


    bool               Next(int dataset);                  // go to the next row

    string             ErrorString() { return mErrorString; }


};

#endif





