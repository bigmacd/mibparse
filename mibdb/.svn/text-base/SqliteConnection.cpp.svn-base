#pragma warning(disable: 4786)

//---------------------------------------------------------------------------
//
// Name:        SqliteConnection.cpp
// Version:     0.2 Beta
// Author:      Gianni Rossi
// Created:     20/06/2005
// License:     Public Domain
//
//---------------------------------------------------------------------------

#include "SqliteConnection.h"

namespace SqliteClass {
   SqliteConnection::SqliteConnection()               
   { 
      ErrMsg=0;  
      RecordSet = NULL;                                               
   }

   SqliteConnection::~SqliteConnection()               
   {  
      if (RecordSet!=NULL) delete RecordSet;                                                                                                           
   }

   int SqliteConnection::Open(char *DBFile)  
   {
      rc = sqlite3_open(DBFile, &db);  
      if( rc )
      {
         ErrMsg = (char*)sqlite3_errmsg(db);
         sqlite3_close(db);       
      }
      return rc;
   }

   int SqliteConnection::_Execute(char *Statement)  
   {
      if (RecordSet!=NULL) 
         {
            delete RecordSet;
            RecordSet = NULL;
         }   
                           
      rc = sqlite3_exec(db, Statement, CallBack, this, &ErrMsg);
      
      if (RecordSet==NULL) RecordSet = new SqliteRecordSet();
      
      if( rc!=SQLITE_OK ) throw SqliteException(Statement,rc,GetError());       
      
      return rc;
   }
   
   int SqliteConnection::Execute(string Statement)  
   {      
      _Execute((char*)Statement.c_str());      
      return rc;
   }
   
   int SqliteConnection::Execute(string Statement,SqliteRecordSet*& RecordsAffected)  
   {
      _Execute((char*)Statement.c_str());
      RecordsAffected = RecordSet;      
      return rc;
   }
   
   char* SqliteConnection::GetError()
   {
      return ErrMsg;
   }    

   void SqliteConnection::LoadResult(int argc, char **argv, char **azColName)
   {
      int i=0;
        
      if (RecordSet==NULL) 
         {
          RecordSet = new SqliteRecordSet;
          
          for(i=0; i<argc; i++) RecordSet->AddColumn(azColName[i]);
          
          }
      
      RecordSet->NewRow();

      for(i=0; i<argc; i++) 
              RecordSet->DataSet[RecordSet->DataSet.size()-1][i] = argv[i]?argv[i]:"NULL";
   }  

   void SqliteConnection::Close()
   {
      sqlite3_close(db);
   }     
   
   int SqliteConnection::BeginTrans()
   {
      TransStarted = true; 
      return Execute("BEGIN"); 
      
   }     
   
   int SqliteConnection::CommitTrans()
   {
      TransStarted = false;
      return Execute("COMMIT");
   }     

   int SqliteConnection::RollbackTrans()
   {
      TransStarted = false;
      return Execute("ROLLBACK"); 
   }     

}
