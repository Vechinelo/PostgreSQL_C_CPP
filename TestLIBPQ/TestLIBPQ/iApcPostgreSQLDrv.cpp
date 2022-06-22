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

  int iApcPostgreSQLDrv::executeSQL( const std::string& astrSQL)
  {
      PGresult* res = PQexec(conn, astrSQL.c_str());
      if (PQresultStatus(res) != PGRES_COMMAND_OK) {
          
          PQclear(res);
         std:: cout << "Select failed: " << PQresultErrorMessage(res) << std::endl;
          return 1;
      }
      PQclear(res);
      return 0;
  }

