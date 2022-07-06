#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include <stdint.h>



typedef uint32_t DWORD;   // DWORD = unsigned 32 bit value
typedef uint16_t WORD;    // WORD = unsigned 16 bit value
typedef uint8_t BYTE;     // BYTE = unsigned 8 bit value


const std::string logfilename = "logfile.txt";
static std::ofstream fout;
const std::string strConnect = "host=localhost port=5432 dbname=tesdb user=postgres password=1234567890";

//начальные значения
//TLXVariantType::lxVTShort
const short c_nShort = -32768; // –32768 - 32767
//TLXVariantType::lxVTBool
const bool c_fBool = true;  //меняем отрицанием
//TLXVariantType::lxVTByte
const BYTE c_bByte = 0; //0 - 255
//TLXVariantType::lxVTWord
const WORD c_wWord = 0; //0 - 65535
//TLXVariantType::lxVTDWord
const DWORD c_dwDWORD = 0; //0 - 4 294 967 295
//TLXVariantType::lxVTChar
const char c_chChar = 0; // 0 - 255
//TLXVariantType::lxVTSChar
const signed char c_schSChar = -128;  // -128 - 127
//TLXVariantType::lxVTDouble
const double c_dDouble = 0;
//TLXVariantType::lxVTInt64
const int64_t c_n64 = 0;  // -9 223 372 036 854 775 808 - 9 223 372 036 854 775 807
const int64_t c_n64Step = 372036854775LL;
//TLXVariantType::lxVTUInt64
const uint64_t c_un64 = 0; // 0 - 18 446 744 073 709 551 615
const uint64_t c_un64Step = 744073709551LL;
//float
const float c_fltFloat = 0;

class tApcWorkObj
{
public:
	tApcWorkObj();
	~tApcWorkObj();

	int start();

	int test1();
	int test2();
	int clear();


private:
	struct tm* myTime;

};

