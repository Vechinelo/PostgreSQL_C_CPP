
#include <iostream>
//#include "iApcPostgreSQLDrv.h"
#include <string>
#include <chrono>
#include<fstream>
#include "tApcWorkObj.h"

int main()
{
   
    tApcWorkObj* spWorkObj = new tApcWorkObj();
    if (!spWorkObj)
    {
        std::cout << "Can't create spWorkObj." << std::endl;
        return -33;
    }

    spWorkObj->start();

    return 0;
}

