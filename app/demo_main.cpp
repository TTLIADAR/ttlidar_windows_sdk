/*
*  TTldiar System
*  Driver Interface
*
*  Copyright 2021 TT Team
*  All rights reserved.
*
*	Author: Orangepi  2021-2-21
*
*/


#include <stdio.h>
#include <stdlib.h>
#include "ttlidar_driver.h"

using namespace std;
using namespace ttlidar;


bool start_scan(TTlidarDriver * drv)
{
	u_result     result;

	result = drv->startScan(SCAN_MODE,5000);
	if (IS_OK(result)) { 
		printf("TTldiar start scan ok!\n");
	}
	else {
		printf("Error, cannot make the lidar start scan: %x\n", result);
		return false;
	}
	return true;
}

bool stop_scan(TTlidarDriver * drv)
{
	u_result     result;

	result = drv->stopScan();
	if (IS_OK(result)) { 
		printf("TTldiar stop scan ok!\n");
	}
	else {
		printf("Error, cannot make the lidar stop scan: %x\n", result);
		return false;
	}
	return true;
}


int main(int argc, const char * argv[]) {
    const char * opt_com_path = NULL;
    _u32         opt_com_baudrate = 115200;
    u_result     result;

	int i = 0;
	int n = 0;
	size_t count = 0;

    // read serial port from the command line...
    if (argc>1) opt_com_path = argv[1]; 

    // read baud rate from the command line if specified...
    if (argc>2) opt_com_baudrate = strtoul(argv[2], NULL, 10);


    if (!opt_com_path) {
#ifdef _WIN32
        // use default com port
        opt_com_path = "\\\\.\\com5";
#else
        opt_com_path = "/dev/ttyUSB0";
#endif
    }

    // create the driver instance
    TTlidarDriver *drv =   new TTlidarDriver();
    
    if (!drv) {
        fprintf(stderr, "insufficent memory, exit\n");
        exit(-2);
    }

    // make connection...
    if (IS_FAIL(drv->connect(opt_com_path, opt_com_baudrate))) {
        fprintf(stderr, "Error, cannot bind to the specified serial port %s.\n"
            , opt_com_path);
        goto on_finished;
    }

	printf("TTldiar connected!\n");

	//start scan
	if (!start_scan(drv))
	{
		//goto on_finished;
	}


	while (true)
	{
		LIDAR_SCAN_INFO_T nodes[360 *6];
		count = _countof(nodes);

		result = drv->grabScanData(nodes, count);

		if (IS_OK(result)) 
		{
			
			printf("TTlidar count = %d\n", count);
			printf("100th Point: angle= %.2f,dist= %.3fm\n",nodes[100].angle,nodes[100].distance);
			
		}
		else
		{
			printf("TTlidar grabScanData error: %x\n", result);
		}
	}

    // done!
on_finished:
	stop_scan(drv);
    drv->disconnect(); 
    delete drv;
    return 0;
}

