#pragma warning(disable: 4786)
//---------------------------------------------------------------------------
//
// Name:        SqliteRecordSet.cpp
// Version:     0.2 Beta
// Author:      Gianni Rossi
// Created:     20/06/2005
// License:     Public Domain
//
//---------------------------------------------------------------------------

#include "SqliteRecordSet.h"

namespace SqliteClass {
          
   SqliteRecordSet::SqliteRecordSet()               
   { 
                                        
   }

   SqliteRecordSet::~SqliteRecordSet()               
   { 
                                                      
   }

   void SqliteRecordSet::NewRow()
      {    
         DataSet.resize(DataSet.size()+ 1);     
         DataSet[DataSet.size()-1].resize(Columns.size());     
      }

   void SqliteRecordSet::MoveFirst()              
      { 
         CurrentRowIndex = 0;
         EndOfRecordSet = false;                                                 
      }

   void SqliteRecordSet::MoveLast()              
      { 
         CurrentRowIndex = DataSet.size()-1;                                                 
      }

   void SqliteRecordSet::MoveNext()              
      { 
         if (CurrentRowIndex<int(DataSet.size()-1)) 
            {
               ++CurrentRowIndex;                                                  
               BeginOfRecordSet = false;
            }
         else
            EndOfRecordSet = true;
      }

   void SqliteRecordSet::MovePrev()              
      { 
         if (CurrentRowIndex>0) 
            {
               --CurrentRowIndex;                                                   
               EndOfRecordSet = false;
            }
         else
            BeginOfRecordSet = true;
      }

   bool SqliteRecordSet::IsEmpty()
      {                                                                                  
         return DataSet.size()==0;                                                  
      }

   int SqliteRecordSet::ColumnsCount()
      { 
         return Columns.size();                                                   
      }

   string SqliteRecordSet::FieldValue(string Field)              
      { 
         return "";                                                  
      }

   string SqliteRecordSet::FieldValue(int FieldIndex)              
      { 
         return DataSet[CurrentRowIndex][FieldIndex];                                                  
      }

   string SqliteRecordSet::ColumnName(int ColumnIndex)              
      { 
         return Columns[ColumnIndex];                                                  
      }

   void SqliteRecordSet::AddColumn(string Name)              
      { 
         Columns.push_back(Name);
      }
}
