#pragma once
#include <string>
#include <vector>
#include <vector>
#include <mutex>
#include "stdlib.h"
#include "constants.h"
#include "datastructs.h"

class File_IO
{
private:
	
	std::mutex m; //protects file operations
	
	std::vector<ORDER> myOrders; //holds all the data read
	std::string filename;        //where orders are stored
	
	/**
	 * pulls in line from file, splits it into order number 
	 * and number of donuts
	 * @param file  where to get orders from
	 * @return SUCCESS
	 */
	int processData(std::ifstream &file);

	/**
	 * attempt to open file 'filename' to read, parse its rows 
	 * into ORDER structs and add these structs to a vector 
	 * sort by order_number
	 * @return SUCCESS if all goes well or COULD_NOT_OPEN_FILE
	 */
	int loadData();
public:
	File_IO(std::string filename = DEFAULT_ORDER_IN_FILE);
	
	/**
	 * reloads all data from filename
	 * @param filename  where to get orders from
	 * @return SUCCESS
	 *         COULD_NOT_OPEN_FILE
	 */
	int resetFilename(std::string filename);

	/**
	 * 
	 * @param anOrder
	 * @return SUCCESS anOrder contains the next order to process
	 *         NO_ORDERS  no more orders to process
	 */
	int getNext(ORDER &anOrder);
};

