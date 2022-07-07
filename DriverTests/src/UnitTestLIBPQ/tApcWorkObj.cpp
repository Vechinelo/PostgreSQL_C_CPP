#define _CRT_SECURE_NO_WARNINGS
#include "tApcWorkObj.h"
#include "iApcPostgreSQLDrv.h"
#include <time.h>




tApcWorkObj::tApcWorkObj()
{
	fout.open(logfilename);

    const time_t timer = time(NULL);
    myTime = localtime(&timer);

}

tApcWorkObj::~tApcWorkObj()
{
	fout.close();

    delete myTime;
}

int tApcWorkObj::start()
{
    int nErrorCount = 0;
    
    int nRes = clear();
    if (nRes) {
        nErrorCount++;
    }
    
    nRes = test1();
    if (nRes) {
        nErrorCount++;
        return nErrorCount;
    }

    nRes = test2();
    if (nRes) {
        nErrorCount++;
    }
    
    return nErrorCount;
}

int tApcWorkObj::test1()
{
	fout << "test 1 started..." << std::endl;
	std::unique_ptr<iApcPostgreSQLDrv> spDrv(iApcPostgreSQLDrv::createInstance());

	int nRes = spDrv->connect(strConnect);
	if (nRes) {
		fout << "error in connect" << std::endl;
		return 1;
	}

	const int nTableCount = 5;
	const std::string strCreateTableSQL[nTableCount] =
    {
   "CREATE TABLE CHE( \
      SA BIGINT, \
      FIRST_NAME VARCHAR(50) NOT NULL,\
      SECOND_NAME VARCHAR(50) NOT NULL, \
      CARDID BIGINT, \
      PHOTO BYTEA)",
    "CREATE TABLE TEST( \
      SHORT_NAME SMALLINT, \
      BOOL_NAME SMALLINT, \
      BYTE_NAME SMALLINT, \
      WORD_NAME integer, \
      DWORD_NAME INTEGER, \
      CHAR_NAME SMALLINT, \
      SCHAR_NAME SMALLINT, \
      DOUBLE_NAME DOUBLE PRECISION, \
      FLOAT_NAME FLOAT, \
      INT64_NAME BIGINT, \
      UINT64_NAME BIGINT)",
    "CREATE TABLE ACCOUNT( \
      ID BIGINT NOT NULL, \
      CARDNUM BIGINT NOT NULL, \
      STATUS BIGINT, \
      CREATEDATE TIMESTAMP)",
    "CREATE TABLE FDATETIME( \
      ID BIGINT NOT NULL, \
      DATETIME TIMESTAMP, \
      FDATE TIMESTAMP, \
      FTIME TIME)",
    "CREATE TABLE MULTI( \
      ID BIGINT NOT NULL, \
      STRFIELD VARCHAR(50) NOT NULL)"
    };

    for (int i = 0; i < nTableCount; i++) {
        nRes = spDrv->executeSQL(
            strCreateTableSQL[i]);
        if (nRes) {
            fout << "error in executeSQL: " + strCreateTableSQL[i] << std::endl;
            return 1;
        } 

        fout << "creating table #" + std::to_string(i) + " passed" << std::endl;
    }


    //new cards
    for (int i = 0; i < 1000; i++)
    {
        std::string strSQL = "INSERT INTO ACCOUNT(ID, CARDNUM, STATUS) VALUES(" +
            std::to_string(i) + ", " + std::to_string(i) + ", " + std::to_string(i + 2000) + ")";

        nRes = spDrv->executeSQL(strSQL);
        if (nRes) {
            fout << "error in execiteSQL: " + strSQL << std::endl;
            return 1;
        }
    }
    fout << "insert into ACCOUNT passed" << std::endl;

    for (int i = 0; i < 1000; i++)
    {
        std::string strSQL = "INSERT INTO CHE(FIRST_NAME,SECOND_NAME) VALUES( 'CHEFIRST" +
            std::to_string(i) + "', 'CHESECOND" + std::to_string(i) + "')";
        nRes = spDrv->executeSQL(strSQL);
        if (nRes) {
            fout << "error in executeSQL: " + strSQL << std::endl;
            return 1;
        }
    }
    fout << "insert into CHE passed" << std::endl;

    for (int i = 0; i < 1000; i++)
    {
        char s1[80] = { 0 };
        
        myTime->tm_mday += 1;
        myTime->tm_hour += 1;
        myTime->tm_min += 1;
        myTime->tm_sec += 1;

        time_t next = mktime(myTime);
        myTime = localtime(&next);

        std::string dt, date, time;

        strftime(s1, 80, "%Y%m%d %H%M%S", myTime);
        dt = s1;

        strftime(s1, 80, "%Y%m%d", myTime);
        date = s1;

        strftime(s1, 80, "%H%M%S", myTime);
        time = s1;


        std::string strSQL = "INSERT INTO FDATETIME(ID, DATETIME, FDATE, FTIME) VALUES(" +
            std::to_string(i) + ", '" + dt + "', '" + date + "', '" + time + "')";
       

        nRes = spDrv->executeSQL(strSQL);
        if (nRes) {
            fout << "error in executeSQL: " + strSQL << std::endl;
            return 1;
        }


    }
    fout << "insert into FDATETIME passed" << std::endl;

    std::string strInsTest = "INSERT INTO TEST( \
    SHORT_NAME, \
    BOOL_NAME, \
    BYTE_NAME, \
    WORD_NAME, \
    DWORD_NAME, \
    CHAR_NAME, \
    SCHAR_NAME, \
    DOUBLE_NAME,\
    INT64_NAME, \
    UINT64_NAME, \
    FLOAT_NAME) VALUES($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11)";

    short nShort = c_nShort;
    bool fBool = c_fBool;
    BYTE bByte = c_bByte;
    WORD wWord = c_wWord;
    DWORD dwDWORD = c_dwDWORD;
    char chChar = c_chChar;
    signed char schSChar = c_schSChar;
    double dDouble = c_dDouble;
    int64_t n64 = c_n64;
    uint64_t un64 = c_un64;
    float fltFloat = c_fltFloat;

    for (int i = 0; i < 1000; i++) {
        
        const std::string str[11] =
        {
            std::to_string(nShort),
            std::to_string(fBool),
            std::to_string(bByte),
            std::to_string(wWord),
            std::to_string(dwDWORD),
            std::to_string(chChar),
            std::to_string(schSChar),
            std::to_string(dDouble),
            std::to_string(n64),
            std::to_string(un64),
            std::to_string(fltFloat)
        };
        const char* paramValues[11];
        for (int i = 0; i < 11; i++) paramValues[i] = str[i].c_str();
        

        //заполняем параметрами
        

        const int paramFormats[11]{ 0,0,0,0,0,0,0,0,0,0,0 };   //0 = text, 1 = binary
        const int paramLenghts[11]{ strlen(paramValues[0]), 
                                   strlen(paramValues[1]), 
                                   strlen(paramValues[2]), 
                                   strlen(paramValues[3]),
                                   strlen(paramValues[4]),
                                   strlen(paramValues[5]),
                                   strlen(paramValues[6]),
                                   strlen(paramValues[7]),
                                   strlen(paramValues[8]),
                                   strlen(paramValues[9]),
                                   strlen(paramValues[10]) };

        
        nRes = spDrv->executeWithParams(strInsTest,
                11,              /* params num */
                paramValues,
                paramLenghts,
                paramFormats,
                0);
        if (nRes) {
            fout << "error in executeWithParams"  << std::endl;
            return 1;
        }
        
        nShort += 55000;
        fBool = !fBool;
        bByte++;
        wWord += 60;
        dwDWORD += 1122122;
        chChar++;
        schSChar++;
        dDouble += 12.12;
        n64 += c_n64Step;
        un64 += c_un64Step;
        fltFloat += 0.11;
    }
    fout << "insert into TEST passed" << std::endl;

    fout << "test 1 finished successfully" << std::endl;
	
	return 0;
}

int tApcWorkObj::test2()
{
    fout << "test 2 started..." << std::endl;
    std::unique_ptr<iApcPostgreSQLDrv> spDrv(iApcPostgreSQLDrv::createInstance());

    int nRes = spDrv->connect(strConnect);
    if (nRes) {
        fout << "error in connect" << std::endl;
        return 1;
    }

    //int
    std::string strSQLInt = "UPDATE ACCOUNT SET CARDNUM = $2 WHERE ID = $1";
    int nCardNum = 0;
    for (int i = 0; i < 1000; i++) {

        const std::string str[2] =
        {
            std::to_string(nCardNum),
            std::to_string(nCardNum+1)
            
        };


        const char* paramValues[2];
        for (int i = 0; i < 2; i++) paramValues[i] = str[i].c_str();
       
        const int paramFormats[2]{ 0,0 };   //0 = text, 1 = binary
        const int paramLenghts[2]{ strlen(paramValues[0]), strlen(paramValues[1]) };


        nRes = spDrv->executeWithParams(strSQLInt,
            2,              /* params num */
            paramValues,
            paramLenghts,
            paramFormats,
            0);
        if (nRes) {
            fout << "error in executeWithParams" << std::endl;
            return 1;
        }


        nCardNum++;
    }

    fout << "update int passed" << std::endl;

    std::string strSQLString = "UPDATE CHE SET FIRST_NAME = $1 WHERE SECOND_NAME = $2";
    
    for (int i = 0; i < 1000; i++) {
        
        const int nParams = 2;
       
        const std::string str[nParams] =
        {
            "CHEFIRST" + std::to_string(i + 1),
            "CHESECOND" + std::to_string(i)
        };

        const char* paramValues[nParams];
        for (int i = 0; i < nParams; i++) paramValues[i] = str[i].c_str();

        const int paramFormats[nParams]{ 0,0 };   //0 = text, 1 = binary
        const int paramLenghts[nParams]{ strlen(paramValues[0]), strlen(paramValues[1]) };

        nRes = spDrv->executeWithParams(strSQLString,
            nParams,              /* params num */
            paramValues,
            paramLenghts,
            paramFormats,
            0);

        if (nRes) {
            fout << "error in executeWithParams" << std::endl;
            return 1;
        }
    }
    fout << "update string passed" << std::endl;

    //DateTime
    std::string strSQLDT = "UPDATE ACCOUNT SET CREATEDATE = $1 WHERE CARDNUM = $2";
    for (int i = 0; i < 1000; i++) {

        const int nParams = 2;
        char s1[80] = { 0 };
        myTime->tm_mday += 1;
        time_t next = mktime(myTime);
        myTime = localtime(&next);
        strftime(s1, 80, "%Y%m%d %H%M%S", myTime);
        const std::string dt = s1;


        const std::string str[nParams] =
        {
            dt,
            std::to_string(i)
        };

        const char* paramValues[nParams];
        for (int i = 0; i < nParams; i++) paramValues[i] = str[i].c_str();

        const int paramFormats[nParams]{ 0,0 };   //0 = text, 1 = binary
        const int paramLenghts[nParams]{ strlen(paramValues[0]), strlen(paramValues[1]) };

        nRes = spDrv->executeWithParams(strSQLDT,
            nParams,              /* params num */
            paramValues,
            paramLenghts,
            paramFormats,
            0);

        if (nRes) {
            fout << "error in executeWithParams" << std::endl;
            return 1;
        }
    }

    fout << "update datetime passed" << std::endl;


    FILE* file = fopen("testblob.jpg", "rb");
    if (file == NULL) {
        fout << std::endl << "FILE WAS UNABLE TO BE READED" << std::endl;
        fout << strerror(errno) << std::endl;
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long int fileSize = ftell(file);

    rewind(file);

    unsigned char* buffRead = (unsigned char*)malloc(fileSize);
    size_t bytes_read = fread(buffRead, 1, fileSize, file);
    if (bytes_read != fileSize) std::cout << std::endl << "fread reading Error";
    fclose(file);

    for (int i = 0; i < 1000; i++) {
        std::string strBlobSQL = "UPDATE CHE SET PHOTO = $1::bytea WHERE SECOND_NAME = 'CHESECOND" + std::to_string(i) + "'";
        
        const char* paramValues[1];
        paramValues[0] = reinterpret_cast<const char*>(buffRead);   // type cast required to PQexeParams

        const int paramFormats[1]{ 1 };   //0 = text, 1 = binary
        const int paramLenghts[1]{ fileSize };

        nRes = spDrv->executeWithParams(strBlobSQL,
            1,              /* params num */
            paramValues,
            paramLenghts,
            paramFormats,
            1);

        if (nRes) {
            fout << "error in executeWithParams" << std::endl;
            free(buffRead);
            return 1;
        }
    }

    fout << "update blob passed" << std::endl;

    
    free(buffRead);
    
    fout << "test 2 finished successfully" << std::endl;

    return 0;
}

int tApcWorkObj::clear()
{
    std::string strClear = "DROP TABLE IF EXISTS CHE, TEST, FDATETIME, ACCOUNT, MULTI";
    std::unique_ptr<iApcPostgreSQLDrv> spDrv(iApcPostgreSQLDrv::createInstance());
    int nRes = spDrv->connect(strConnect);
    if (nRes) {
        fout << "error in connect" << std::endl;
        return 1;
    }
    nRes = spDrv->executeSQL(strClear);
    if (nRes) {
        fout << "error in executeSQL: " + strClear << std::endl;
        return 1;
    }


    return 0;
}


