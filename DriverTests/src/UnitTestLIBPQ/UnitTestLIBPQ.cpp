

#include <iostream>

#include "tApcWorkObj.h"
#include <stdio.h>
#include <time.h>



int main()
{
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

