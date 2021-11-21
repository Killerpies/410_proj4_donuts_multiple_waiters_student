#pragma once
#include <iostream>
#include <string>

//default files
const std::string	DEFAULT_ORDER_IN_FILE = "inlots.txt";
const int DOZEN = 12;

//default number threads
const int DEFAULT_NUM_WAITERS=3;
const int DEFAULT_NUM_BAKERS=3;

//SUCCESS CODES
const int 			SUCCESS = 0;

//used when reading orders from a file
const char			CHAR_TO_SEARCH_FOR = ',';

//FAILURE CODES
const int 			FAIL = SUCCESS - 1;
const int 			COULD_NOT_OPEN_FILE = SUCCESS - 3;
const int			UNINITIALIZED = SUCCESS - 5;
const int			NO_ORDERS = SUCCESS - 7;

//I have 3 types of donut but for now only use GLAZED
//It adds a lot of complexity to have mixed types
enum type { GLAZED, SPRINKLED, CAKE };
struct DONUT {
	type myType = type::GLAZED;
};

