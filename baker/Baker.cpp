#include <mutex>

#include "../includes/baker.h"
#include "../includes/externs.h"
#include "../includes/PRINT.h"
#include "../includes/box.h"
using namespace std;


Baker::Baker(int id):id(id)
{
}

/**
	 * gets the number of donuts to make from anOrder, and packages that
	 * number in boxes, 12 to a box, with a possibly partially filled box
	 * ex. if anOrder.number_donuts =13, this function will create 2 boxes;
	 *     1 with 12 donuts, 1 with 1
	 * boxes then appended to anOrder.boxes
	 * anOrder is then complete and should be appended to order_out_Vector
	 * @param anOrder  the order to bake box and append to order_out_Vector
	 */
void Baker::bake_and_box(ORDER &anOrder) {
	unique_lock<mutex> lck(mutex_order_outQ);
	Box temp;
	for (int i = 0; i<anOrder.number_donuts; i++){
		DONUT tempDonut;
		temp.addDonut(tempDonut);
		if (temp.size() == 12 || i== anOrder.number_donuts- 1) {
			anOrder.boxes.push_back(temp);
			temp.clear();
		}
	}
	temp.clear();
	order_out_Vector.push_back(anOrder);

}

/**
	 * runs a loop
	 * efficiently sleeps as long as their are no orders to process and all_work_done is false
	 * will be awakened whenever a waiter places an order or when a waiter finishes and then;
	 *   if orders in order_in_Q, gets an order, and passes it to bake_and_box above
	 *   if all waiters are finished (all_work_done==true) and there are no more orders then baker exits
	 *
	 * You will use cv_order_inQ to be notified by waiters
	 * all_work_done handles the case where a baker starts before any waiters
	 * (in which case there are no orders in the queue but all_work_done indicates
	 * some are coming when the waiters start) as well as when all the waiters have finished
	 */
void Baker::beBaker() {
	unique_lock<mutex> lck(mutex_order_inQ);
	while (order_in_Q.empty() && !all_work_done) {
		cv_order_inQ.wait(lck);
	}
	while (!order_in_Q.empty()){
		ORDER temp = order_in_Q.front();
		order_in_Q.pop();
		this->bake_and_box(temp);
	}
	lck.unlock();
	cv_order_inQ.notify_all();
}
