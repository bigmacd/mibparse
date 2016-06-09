#pragma warning(disable: 4786)

#ifndef __MIBDB_H__
#include "MibDb.h"
#endif

//#ifdef _WIN32
//#include "afx.h"
//#endif

#include <stdio.h>
#include <stdlib.h>

// MibName info
#define MIBNAMEID 0
#define MIBNAMENAME 1

// Oids info
#define OIDSID 0
#define OIDSOID 1
#define OIDSNAME 2
#define OIDSMIBNAMEID 3
#define ENUMFLAG 4
#define TRAPFLAG 5
#define TRAPSPECIFICTYPE 6

// Enums info
#define ENUMOIDID 0
#define ENUMVALUE 1
#define ENUMNAME 2

MibDb::MibDb()
      :mRecordSet(NULL),
       connected(false)
{

}

MibDb::MibDb(std::string dbName)
      :mRecordSet(NULL)
{
  Connect(dbName);
  connected = true;
}

MibDb::~MibDb()
{
  mConnection.Close(); 
  connected = false;
}

bool       
MibDb::Connect(std::string dbName)
{
  // open already returns true unless something strange happens.

  // see if the file exists, if not, open and then initialize
  bool exists = false;
  FILE* fp = fopen(dbName.c_str(), "r");
  if (NULL != fp)
  {
    exists = true;
    fclose(fp);
  }
      
  bool retVal = false;
  int status = mConnection.Open((char*)dbName.c_str());
  if (!status) 
    retVal = true;
  else
    mErrorString = mConnection.GetError();

  if (exists == false)
  {
    SqliteRecordSet* result = NULL;
    try {
      std::string sql;

      sql = "CREATE TABLE MibName (id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(64) UNIQUE)";
      mConnection.Execute(sql.c_str(), result);
      
      sql = "CREATE TABLE Oids (id INTEGER PRIMARY KEY AUTOINCREMENT, oid VARCHAR(128), name VARCHAR(64), mibNameId INTEGER, enumFlag INTEGER, trapFlag INTEGER, specificType INTEGER)";
      mConnection.Execute(sql.c_str(), result);

      sql = "CREATE TABLE Enums (oidId INTEGER, value INTEGER, name VARCHAR(64))";
      mConnection.Execute(sql.c_str(), result);

    }
    catch (SqliteException e) {
    }
    
  }
  return retVal;
}


    // MibName stuff
bool               
MibDb::InsertMibName(string name)
{
  bool retVal = false;
  SqliteRecordSet* result = NULL;
  try {
    std::string sql;
    char temp[512];
    sprintf(temp, 
	    "insert into MibName (name) VALUES ('%s')", 
	    name.c_str());
    sql = temp;
    mConnection.Execute(sql.c_str(), result);
    if (result)
    {
      retVal = true;
    }
  }
  catch (SqliteException e) {
  }
  return retVal;
}

bool               
MibDb::DeleteMib(string name)             // deletes OIDs from this MIB too
{
  bool retVal = false;

  retVal = FindMib(name);
  if (false == retVal) 
    return retVal;
  
  // need the id to find all the oids
  int id = MibId();

  // find the oids in this mib, get the id
  retVal = FindOidsByMibId(id);
  while (true == retVal)
  {
    // use the id of the oids to delete the enums
    int oidId = OidId();
    DeleteEnums(oidId);
    retVal = Next(OIDDATASET);
  }

  // delete the oids
  SqliteRecordSet* result = NULL;
  try {
    std::string sql;
    char temp[512];
    sprintf(temp, 
	    "delete from Oids where mibNameId = '%d'", 
	    id);
    sql = temp;
    mConnection.Execute(sql.c_str(), result);

    // delete the mib
    sprintf(temp, 
	    "delete from MibName where name = '%s'", 
	    name.c_str());
    sql = temp;
    mConnection.Execute(sql.c_str(), result);
    if (result)
      retVal = true;
  }
  catch (SqliteException e) {
  }
  return retVal;
}

void                 
MibDb::DeleteEnums(int oidId)
{
  try 
  {
    std::string sql;
    char temp[512];
    sprintf(temp, 
            "delete from Enums where oidId = '%d'",
	    oidId);
    sql = temp;
    mConnection.Execute(sql.c_str(), mRecordSet);
  } 
  catch (SqliteException e) {
  }
}

bool               
MibDb::FindMib(string name)
{

  bool retVal = false;

  mErrorString.empty();

  try {
    std::string sql;
    char temp[512];
    sprintf(temp, 
	    "select * from MibName where name = '%s'",
	    name.c_str());
    sql = temp;
    mConnection.Execute(sql.c_str(), mRecordSet);
    if (mRecordSet && !mRecordSet->IsEmpty())
    {
      mRecordSet->MoveFirst();

      string data = mRecordSet->FieldValue(MIBNAMEID);
      mMibNameId = atoi(data.c_str());
      
      mMibName  = mRecordSet->FieldValue(MIBNAMENAME);
      retVal = true;
    }      
  } 
  catch (SqliteException e) {
    mErrorString = e.GetErrDescription();
  }
  return retVal;
}

bool               
MibDb::GetAllMIBs()
{
  bool retVal = false;

  try {
    std::string sql = "select * from MibName";
    mConnection.Execute(sql.c_str(), mRecordSet);
    if (mRecordSet && !mRecordSet->IsEmpty())
    {
      mRecordSet->MoveFirst();

      string data = mRecordSet->FieldValue(MIBNAMEID);
      mMibNameId = atoi(data.c_str());
      
      mMibName  = mRecordSet->FieldValue(MIBNAMENAME);
      retVal = true;
    }      
  } 
  catch (SqliteException e) {
  }
  return retVal;
}

int                
MibDb::MibId()                            // column accessor
{
  return mMibNameId;
}

string             
MibDb::MibName()                          // column accessor
{
  return mMibName;
}


    // Oids stuff
bool               
MibDb::InsertOid(string oid,
		 string name, 
		 int mibnameId,
		 int enumFlag,
		 int trapFlag,
		 int trapSpecificType)
{
  bool retVal = false;
  SqliteRecordSet* result = NULL;
  try {
    std::string sql;
    char temp[512];
    sprintf(temp, 
	    "insert into Oids (oid, name, mibNameId, enumFlag, trapFlag, specificType) VALUES ('%s', '%s', '%d', '%d', '%d', '%d')", 
	    oid.c_str(),
	    name.c_str(),
	    mibnameId,
	    enumFlag,
	    trapFlag,
	    trapSpecificType);
    sql = temp;
    mConnection.Execute(sql.c_str(), result);
    if (result)
    {
      retVal = true;
    }
  }
  catch (SqliteException e) {
  }
  return retVal;
}

bool               
MibDb::FindNearestOid(string oid)
{
  //select * from Oids where instr("1.3.6.1.2.1.16.4.3.1.4.0.1.2.3", oid) 
  //order by length(oid) desc limit 1;
	return false;
}

bool               
MibDb::FindNearestEnum(string oid, int value)
{
  //select * from Oids where instr("1.3.6.1.2.1.16.4.3.1.4.0.1.2.3", oid) 
  //order by length(oid) desc limit 1;
	return false;
}

bool               
MibDb::FindOid(string oid)                // looks up OID 
{
  bool retVal = false;

  mErrorString.empty();

  try {
    std::string sql;
    char temp[512];
    sprintf(temp, 
	    "select * from Oids where oid = '%s'",
	    oid.c_str());
    sql = temp;
    mConnection.Execute(sql.c_str(), mRecordSet);
    if (mRecordSet && !mRecordSet->IsEmpty())
    {
      mRecordSet->MoveFirst();

      PopulateOidData();
      retVal = true;
    }      
  } 
  catch (SqliteException e) {
    mErrorString = e.GetErrDescription();
  }
  return retVal;
}

bool               
MibDb::FindOidsByMibId(int mibId)          // find all OIDs from a MIB
{
  bool retVal = false;

  try {
    std::string sql;
    char temp[512];
    sprintf(temp, 
	    "select * from Oids where mibNameId = '%'",
	    mibId);
    sql = temp;
    mConnection.Execute(sql.c_str(), mRecordSet);
    if (mRecordSet && !mRecordSet->IsEmpty())
    {
      mRecordSet->MoveFirst();

      PopulateOidData();
      retVal = true;
    }      
  } 
  catch (SqliteException e) {
  }
  return retVal;
}

void
MibDb::PopulateOidData()
{
  string data = mRecordSet->FieldValue(OIDSID);
  mOidsId = atoi(data.c_str());
  
  mOid  = mRecordSet->FieldValue(OIDSOID);
  
  mOidsName = mRecordSet->FieldValue(OIDSNAME);
  
  data = mRecordSet->FieldValue(OIDSMIBNAMEID);
  mOidsMibNameId = atoi(data.c_str());
  
  data = mRecordSet->FieldValue(ENUMFLAG);
  mEnumFlag = (bool)(atoi(data.c_str()));
  
  data = mRecordSet->FieldValue(TRAPFLAG);
  mTrapFlag = (bool)(atoi(data.c_str()));
  
  data = mRecordSet->FieldValue(TRAPSPECIFICTYPE);
  mSpecificType = atoi(data.c_str());
}

bool               
MibDb::FindTrap(string oid, int sType)    // looks up OID as trap
{
  bool retVal = false;

  mErrorString.empty();

  try {
    std::string sql;
    char temp[512];
    sprintf(temp, 
	    "select name from Oids where oid = '%s.%d' AND specificType = '%d' AND trapFlag = '%d'",
	    oid.c_str(),
	    sType,
	    sType,
	    1);
    sql = temp;
    mConnection.Execute(sql.c_str(), mRecordSet);
    if (mRecordSet && !mRecordSet->IsEmpty())
    {
      mRecordSet->MoveFirst();

      mTrapName  = mRecordSet->FieldValue(0);

      retVal = true;
    }      
  } 
  catch (SqliteException e) {
    mErrorString = e.GetErrDescription();
  }
  return retVal;

}

int             
MibDb::OidId()                              // column accessor
{
  return mOidsId;
}

string             
MibDb::Oid()                              // column accessor
{
  return mOid;
}

string             
MibDb::OidsName()                         // column accessor
{
  return mOidsName;
}

int                
MibDb::OidsMibNameId()                    // column accessor
{
  return mOidsMibNameId;
}

bool               
MibDb::IsTrap()                           // column accessor
{
  return mTrapFlag;
}

bool               
MibDb::IsEnum()                           // column accessor
{
  return mEnumFlag;
}

int                
MibDb::SpecificType()                     // column accessor
{
  return mSpecificType;
}

string             
MibDb::TrapName()                         // column accessor
{
  return mTrapName;
}

// enum stuff
bool               
MibDb::InsertEnum(int oidId,
		  int value,
		  string name)
{
  bool retVal = false;
  SqliteRecordSet* result = NULL;
  try {
    std::string sql;
    char temp[512];
    sprintf(temp, 
	    "insert into Enums (oidId, value, name) VALUES ('%d', '%d', '%s')", 
	    oidId,
	    value,
	    name.c_str());
    sql = temp;
    mConnection.Execute(sql.c_str(), result);
    if (result)
    {
      retVal = true;
    }
  }
  catch (SqliteException e) {
  }
  return retVal;
}

bool               
MibDb::FindEnum(string oid, int value)    // looks up OID as Enum
{
  bool retVal = false;

  mErrorString.empty();

  // get the oid's id first
  if (true == (retVal = FindOid(oid)))
  {
    int oidid = OidId();
    retVal = false;

    try {
      std::string sql;
      char temp[512];
      sprintf(temp, 
	      "select name from Enums where oidId = '%d' AND value = '%d'",
	      oidid,
	      value);
      sql = temp;
      mConnection.Execute(sql.c_str(), mRecordSet);
      if (mRecordSet && !mRecordSet->IsEmpty())
      {
	mRecordSet->MoveFirst();
	mEnumName  = mRecordSet->FieldValue(0);
	retVal = true;
      }      
    } 
    catch (SqliteException e) {
      mErrorString = e.GetErrDescription();
    }
  }
  return retVal;
}

string             
MibDb::EnumName()                         // column accessor
{
  return mEnumName;
}


//int                
//MibDb::EnumValue()                        // column accessor
//{
//
//}

bool               
MibDb::Next(int dataset)                  // go to the next row
{
  bool retVal = false;

  if (dataset >= MIBDATASET && dataset <= ENUMDATASET)
  {
    if (mRecordSet && !mRecordSet->IsEmpty())
      mRecordSet->MoveNext();
    if (false == mRecordSet->EndOfRecordSet)
    {
      switch (dataset)
      {
      case MIBDATASET:
	{
	  string data = mRecordSet->FieldValue(MIBNAMEID);
	  mMibNameId = atoi(data.c_str());
	  mMibName  = mRecordSet->FieldValue(MIBNAMENAME);
	  retVal = true;
	  break;
	}
      case OIDDATASET:
	PopulateOidData();
	retVal = true;
	break;
      case ENUMDATASET:
	//	retVal = DiskPopulate();
	break;
      }
    }
  }
  return retVal;
}

