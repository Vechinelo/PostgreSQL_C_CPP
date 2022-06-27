#include "iApcPostgreSQLDrv.h"
#include <libpq-fe.h>
#include <string>
#include <iostream>

iApcPostgreSQLDrv::iApcPostgreSQLDrv()
{
	
}
//-----------------------------------------------------------------------------

iApcPostgreSQLDrv::~iApcPostgreSQLDrv()
{
    if (PQstatus(conn) == CONNECTION_OK) 
    {
        disconnect();
      
        std::cout << "DESTRUCTOR"<< std::endl;
    }
    
}

  int iApcPostgreSQLDrv::connect(const std::string& strConnect)
{ 
     conn= PQconnectdb(strConnect.c_str());
     if (PQstatus(conn) == CONNECTION_BAD)
     {
         PQfinish(conn);
         return 1;
     }
     
     
     return 0;
     
}

  int iApcPostgreSQLDrv::disconnect()
  {
      PQfinish(conn); //nullptr is no problem
      return 0;
  }

  int iApcPostgreSQLDrv::startTransaction()
  {
      PGresult* res = PQexec(conn, "BEGIN");
      if (PQresultStatus(res) != PGRES_COMMAND_OK)
      {
          
          PQclear(res);
          return 1;
      }
      PQclear(res);
      return 0;
  }

  int iApcPostgreSQLDrv::commit()
  {
      PGresult* res = PQexec(conn, "COMMIT");
      if (PQresultStatus(res) != PGRES_COMMAND_OK)
      {

          PQclear(res);
          return 1;
      }
      PQclear(res);
      return 0;
  }

  int iApcPostgreSQLDrv::rollback()
  {
      PGresult* res = PQexec(conn, "ROLLBACK");
      if (PQresultStatus(res) != PGRES_COMMAND_OK)
      {

          PQclear(res);
          return 1;
      }
      PQclear(res);
      return 0;
  }

  int iApcPostgreSQLDrv::executeSQL(const std::string& astrSQL)
  {
      PGresult* res = PQexec(conn, astrSQL.c_str());
      if (PQresultStatus(res) != PGRES_COMMAND_OK) {
          
          PQclear(res);
       //  std:: cout << "Select failed: " << PQresultErrorMessage(res) << std::endl;
          return 1;
      }
      PQclear(res);
      return 0;
  }

  int iApcPostgreSQLDrv::openQuery(const std::string& astrSQL)
  {
      if (!PQsendQuery(conn, astrSQL.c_str())) {
          return 1;
      }
      
      if (!PQsetSingleRowMode(conn)) {
          return 1;
      }
      PGresult* res = PQgetResult(conn);
      while (res != nullptr)
      {
          if (PQresultStatus(res) == PGRES_FATAL_ERROR) {
              return 1;
          }
      /*    for (int i = 0; i < PQntuples(res); i++) {
              for (int j = 0; j < PQnfields(res); j++) {
                  std::cout << PQgetvalue(res, i, j) << "   ";
              }
              std::cout << std::endl;
          }*/
          PQclear(res);
          res = PQgetResult(conn);
              
      }

      PQclear(res);
      return 0;

  }

