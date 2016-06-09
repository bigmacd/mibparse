//---------------------------------------------------------------------------
//
// Name:        SqliteException.cpp
// Version:     0.2 Beta
// Author:      Gianni Rossi
// Created:     20/06/2005
// License:     Public Domain
//
//---------------------------------------------------------------------------

#include "SqliteException.h"

namespace SqliteClass {
          
   SqliteException::SqliteException(string Statement,int ErrNumber,string ErrDescription)               
   { 
      _Statement = Statement;
      _ErrNumber = ErrNumber;
      _ErrDescription = ErrDescription;                                     
   }

   SqliteException::~SqliteException()               
   { 
                                                      
   }
   string SqliteException::GetStatement()               
   { 
      return _Statement;                                                      
   }
   int SqliteException::GetErrNumber()               
   { 
      return _ErrNumber;                                                      
   }
   string SqliteException::GetErrDescription()               
   { 
      return _ErrDescription;                                                                          
   }
}
