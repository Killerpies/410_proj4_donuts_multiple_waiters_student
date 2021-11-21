#pragma once
#include <string>

#include "constants.h"
#include "datastructs.h"
#include "file_IO.h"

class Waiter
{
private:
	int id;

public:
	/**
	 * Make sure you increment num_working_waiters to indicate that a waiter has started
	 * num_working_waiters is a global but it is atomic so you do not need to protect it
	 * Also set all_work_done = false to indicate that there is work to do
	 * all_work_done is a global but it is atomic so you do not need to protect it
	 * @param id a number used to identify this particular waiter 
	 */
	Waiter(int id);

	/**
	 * gets next Order from global file_IO object
	 * file_IO has its own internal locking so you 
	 * do not need to synchronize access to it in this function
	 * @param anOrder
	 * @return if SUCCESS then anOrder contains new order
	 *         otherwise return contains fileIO error
	 */
	int getNext(ORDER &anOrder);

	/**
	 * Runs a loop that calls getNext(ORDER &anOrder) which 
	 * gets orders from global file_IO one at a time 
	 * puts them in order_in_Q then signals baker(s) using cv_order_inQ 
	 * so they can be consumed by baker(s) 
	 * when finished exits loop, decrements num_working_waiters checks to see if
	 * num_working_waiters==0 and if so sets all_work_done=true
	 * then signals baker(s) using cv_order_inQ that it is done 
	 */
	void beWaiter();
};
