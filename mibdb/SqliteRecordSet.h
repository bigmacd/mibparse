//---------------------------------------------------------------------------
//
// Name:        SqliteRecordSet.h
// Version:     0.2 Beta
// Author:      Gianni Rossi
// Created:     20/06/2005
// License:     Public Domain
//
//---------------------------------------------------------------------------

#ifndef SQLITERECORDSET_H
#define SQLITERECORDSET_H

#include <vector>
#include <string>
using namespace std;

namespace SqliteClass {
   
   class SqliteRecordSet
   {
      private:
         vector< string > Columns;
         
      protected:      
         
      public:
         vector< vector<string> > DataSet;    
                      
         SqliteRecordSet();  
         ~SqliteRecordSet();       
 
         void NewRow();
                  
         int CurrentRowIndex;
         
         int ColumnsCount();
         int RecordsCount();
         
         bool EndOfRecordSet;
         bool BeginOfRecordSet;
         bool IsEmpty();
         
         void MoveFirst();
         void MoveLast();
         void MoveNext();
         void MovePrev();
         
         void AddColumn(string Name);
         string ColumnName(int ColumnIndex);
         
         string FieldValue(string Field);
         string FieldValue(int FieldIndex);
   };

}
#endif
