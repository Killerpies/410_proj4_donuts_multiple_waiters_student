/*
 * SAFEPRINT.cpp
 *
 *  Created on: Nov 18, 2021
 *      Author: keith
 */
#include <mutex>
#include <iostream>
#include "../includes/externs.h"
#include "../includes/PRINT.h"

using namespace std;

//used to make following SAFEPRINT statement work properly
//in a multithreaded environment
extern std::mutex printMutex;

void SAFEPRINT(string txt){
	lock_guard<mutex> lck(printMutex);
	std::cout<<txt<<std::endl;
};

