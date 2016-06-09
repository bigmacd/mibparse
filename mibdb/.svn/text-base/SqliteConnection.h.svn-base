//---------------------------------------------------------------------------
//
// Name:        SqliteConnection.h
// Version:     0.2 Beta
// Author:      Gianni Rossi
// Created:     20/06/2005
// License:     Public Domain
//
//---------------------------------------------------------------------------

#ifndef SQLITECONNECTION_H
#define SQLITECONNECTION_H

#include "sqlite3.h"
#include "SqliteRecordSet.h"
#include "SqliteException.h"

#include <iostream>
using namespace std;

namespace SqliteClass {
   class SqliteConnection
   {
      private:
         sqlite3* db;
         char* ErrMsg;
         int rc;
         SqliteRecordSet* RecordSet;
         int _Execute(char* Statement);
         bool TransStarted;
      protected:      
         
      public:         
         SqliteConnection();  
         ~SqliteConnection();       
         
         int Open(char* DBFile);
         void Close();  
         int BeginTrans();
         int CommitTrans();
         int RollbackTrans();         
         int Execute(string Statement);
         int Execute(string Statement,SqliteRecordSet*& RecordsAffected);
         
         char* GetError();  
         
         void LoadResult(int argc, char **argv, char **azColName); 

   };

   static int CallBack(void *SqliteConnection_Istance, int argc, char **argv, char **azColName)
   {       
      SqliteConnection *aSqliteConnection = (SqliteConnection*)SqliteConnection_Istance;
      if (aSqliteConnection != NULL)
         {  
            aSqliteConnection->LoadResult(argc, argv,azColName);                
         }
   
      return 0;
   }
}
#endif
