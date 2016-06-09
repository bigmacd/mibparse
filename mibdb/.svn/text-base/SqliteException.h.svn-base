//---------------------------------------------------------------------------
//
// Name:        SqliteException.h
// Version:     0.2 Beta
// Author:      Gianni Rossi
// Created:     20/06/2005
// License:     Public Domain
//
//---------------------------------------------------------------------------

#ifndef SQLITEEXCEPTION_H
#define SQLITEEXCEPTION_H

#include <string>
using namespace std;

namespace SqliteClass {
   
   class SqliteException
   {
         private:
            string _Statement;
            int _ErrNumber;
            string _ErrDescription;
         public:
            SqliteException(string Statement,int ErrNumber,string ErrDescription);  
            ~SqliteException();  
            string GetStatement();                 
            int GetErrNumber();
            string GetErrDescription();                        
   };
} //end namespace      
#endif
