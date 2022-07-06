#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include "tApcWorkObj.h"
#include <stdio.h>
#include <time.h>
#include <string>
//#define ADD_DAYS 300

int main()
{
	/*struct tm* myTime;
	char s1[80] = { 0 };
	const time_t timer = time(NULL);
	myTime = localtime(&timer);
	strftime(s1, 80, "%d.%m.%Y %H:%M:%S ", myTime);
	printf("%s\n", s1);
	myTime->tm_mday += 1;
    myTime->tm_hour += 1;
    myTime->tm_min += 1;
    myTime->tm_sec += 1;
	time_t next = mktime(myTime);
	myTime = localtime(&next);
	strftime(s1, 80, "%d.%m.%Y %H:%M:%S ", myTime);
	printf("%s\n", s1);
	getchar();*/



	/*std::string dt = std::to_string(myTime->tm_year + 1900) + std::to_string(myTime->tm_mon) + std::to_string(myTime->tm_mday) + " " +
		std::to_string(myTime->tm_hour) + std::to_string(myTime->tm_min) + std::to_string(myTime->tm_sec);*/
	/*std::string dt, date, time;
	strftime(s1,80, "%Y%m%d %H%M%S", myTime);

	//printf("%s\n", s1);

	dt = s1;

	strftime(s1, 80, "%Y%m%d", myTime);
	date = s1;

	strftime(s1, 80, "%H%M%S", myTime);
	time = s1;

	std::cout << dt << std::endl <<date << std::endl << time;*/


	tApcWorkObj* spWorkObj = new tApcWorkObj();
	if (!spWorkObj)
	{
		std::cout << "Can't create spWorkObj." << std::endl;
		return -33;
	}

	int nRes = spWorkObj->start();
	if (nRes) {
		return nRes;
	}

	return 0;
}

