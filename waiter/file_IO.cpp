#include <numeric>  
#include <math.h> 
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "../includes/file_IO.h"

using namespace std;
//if myString does not contain a string rep of number returns o
//if int not large enough has undefined behaviour, very fragile
int stringToInt(const char *myString) {
	return atoi(myString);
}

//sorting functions
bool sort_order_number(const ORDER &o1, const ORDER &o2) {
	return o1.order_number < o2.order_number;
}

File_IO::File_IO(std::string filename)
{
	resetFilename(filename);
}

/**
 * reloads all data from filename
 * @param filename
 * @return SUCCESS
 *         COULD_NOT_OPEN_FILE
 */
int File_IO::resetFilename(std::string filename){
	this->filename=filename;
	return loadData();	
}

int File_IO::processData(std::ifstream &file) {
	std::string line;
	std::string token;
	ORDER anOrder;
	stringstream ss;

	while (!file.eof()) {
		std::getline(file, line); //get a line from the file 
		ss.str(line);
		
		if (line.empty())
			continue;

		//!!!!!! NO ERROR CHECK HERE DONT ASSUMME PERFECT DATA 		!!!!!!
		//!!!!!! if any of the following fail retValue should		!!!!!!
		//!!!!!! be set to false, break out of loop, then return;	!!!!!!

		//get order_number
		if (std::getline(ss, token, CHAR_TO_SEARCH_FOR))
			anOrder.order_number = stringToInt(token.c_str());

		//get number_donuts
		if (std::getline(ss, token, CHAR_TO_SEARCH_FOR))
			anOrder.number_donuts = stringToInt(token.c_str());

		//finally add to vector (makes a COPY of myMeasurement)
		myOrders.push_back(anOrder);

		//clear stream so it will work for next read
		ss.clear();
	}

	//sort it by order_number
	std::sort(myOrders.begin(), myOrders.end(), sort_order_number);

	return SUCCESS;
}

int File_IO::loadData() {
	
	lock_guard<mutex> lck(m);
	myOrders.clear();

	int iRet = FAIL;	//pessimism

	ifstream myInFile;
	myInFile.open(filename);
	if (!myInFile.is_open())
		return COULD_NOT_OPEN_FILE;

	//processfile contents
	iRet = processData(myInFile);
	if (iRet != SUCCESS)
		return iRet;

	//close file
	if (myInFile.is_open())
		myInFile.close();

	return SUCCESS;
}

//return the first struct in the vector
//then deletes it from the vector
int File_IO::getNext(ORDER &anOrder) {
	int iRet = FAIL;	//pessimism

	lock_guard<mutex> lck(m);
	//if no other orders say so
	if (myOrders.empty())
		return NO_ORDERS;

	//return the next one
	anOrder = myOrders[0];
	myOrders.erase(myOrders.begin());
	
	return SUCCESS;
}


