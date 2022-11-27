#include <string>
#include "stdlib.h"
#include "../includes/waiter.h"
#include "../includes/externs.h"
#include "../includes/PRINT.h"
#include "../includes/box.h"

using namespace std;

/**
	 * Make sure you increment num_working_waiters to indicate that a waiter has started
	 * num_working_waiters is a global but it is atomic so you do not need to protect it
	 * Also set all_work_done = false to indicate that there is work to do
	 * all_work_done is a global but it is atomic so you do not need to protect it
	 * @param id a number used to identify this particular waiter
	 */
Waiter::Waiter(int id):id(id){
	num_working_waiters +=1;
	all_work_done = false;
}

/**
	 * gets next Order from global file_IO object
	 * file_IO has its own internal locking so you
	 * do not need to synchronize access to it in this function
	 * @param anOrder
	 * @return if SUCCESS then anOrder contains new order
	 *         otherwise return contains fileIO error
	 */
int Waiter::getNext(ORDER &anOrder){
	return m_io.getNext(anOrder);

}

/**
	 * Runs a loop that calls getNext(ORDER &anOrder) which
	 * gets orders from global file_IO one at a time
	 * puts them in order_in_Q then signals baker(s) using cv_order_inQ
	 * so they can be consumed by baker(s)
	 * when finished exits loop, decrements num_working_waiters checks to see if
	 * num_working_waiters==0 and if so sets all_work_done=true
	 * then signals baker(s) using cv_order_inQ that it is done
	 */
void Waiter::beWaiter() {
	unique_lock<mutex> lck(mutex_order_inQ);
	ORDER temp;
	while(this->getNext(temp) != NO_ORDERS){
		order_in_Q.push(temp);
	}
	num_working_waiters -=1;
	if (num_working_waiters == 0){
		all_work_done=true;
	}
	lck.unlock();
	cv_order_inQ.notify_all();
}


