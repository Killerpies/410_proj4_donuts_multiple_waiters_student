#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <string>
#include <atomic>

#include "../includes/box.h"
#include "../includes/constants.h"
#include "../includes/waiter.h"
#include "../includes/baker.h"
#include "../includes/datastructs.h"
#include "../includes/PRINT.h"

using namespace std;
int total_pts=4;		//start with 2 points for compiling
const int DEFAULT_PTS = 4;

//*************************************************
//NOTE:  most of these globals are needed
//by both bakers and waiters, so be sure to include
//externs.h in appropriate cpp implementations
//*************************************************
//the mutexes
mutex mutex_order_inQ;
mutex mutex_order_outQ;

//where all the orders come from 
File_IO m_io;

//the condition variable
condition_variable cv_order_inQ;

//tracks number of waiters
atomic<int> num_working_waiters(0);	

//waiters will process all the orders
//when the last one leaves it will set 
//all_work_done to true
atomic<bool> all_work_done(false);

//where orders are stored
queue<ORDER> order_in_Q;
vector<ORDER> order_out_Vector;

//used to make SAFEPRINT statements work properly
mutex printMutex;

//*************************************************
//runs waiter until orders all read and placed
//on order_in_Q then exits
void doWaiter(int id) {
	Waiter myWaiter(id);
	myWaiter.beWaiter();
}

//takes orders from order_in_Q, processes
//them and then puts them on order_out_Vector
void doBaker(int id) {
	Baker myBaker(id);
	myBaker.beBaker();
}
bool verify(int calculated,int correct, string s){
	bool bres=(calculated==correct);
	if(bres)
		SAFEPRINT("CORRECT Total " + s + " made   = "+to_string(calculated));
	else
		SAFEPRINT("!!!!!!!!!!!!!!!!!!!!FAIL Total " + s + " made   = "+to_string(calculated) + " Should have made " + to_string(correct));
	return bres;
}

//prints what is in order_out_Vector
//DO NOT CALL THIS WHEN MULTIPLE THREADS ARE ACCESSING order_out_Vector
void audit_results(std::string fn) {
	std::vector<ORDER>::iterator itOrder;
	std::vector<Box>::iterator itBox;

	int total_donuts = 0;
	int total_orders = 0;
	int total_boxes  = 0;

	//first go through every order
	for (itOrder = order_out_Vector.begin(); itOrder != order_out_Vector.end(); itOrder++) {
		int numDonuts = 0;
		total_orders++;

		//for each order go through all the boxes and add up all the donuts
		for (itBox = (itOrder->boxes).begin(); itBox != (itOrder->boxes).end(); ++itBox) {
			total_boxes++;
			numDonuts += itBox->size();
			total_donuts += itBox->size();
		}

		//if one order was screwed up say so
		if (numDonuts != itOrder->number_donuts) 
			SAFEPRINT("ERROR Order"+to_string( itOrder->order_number)+ " Expected " + to_string(itOrder->number_donuts) + " found "+ to_string(numDonuts));
	}
	
	//quick check against original orders
	int num_donuts = 0;
	int num_orders = 0;
	int num_boxes = 0;
	ORDER anOrder;
	File_IO mio(fn);
	while(mio.getNext(anOrder) == SUCCESS){
		num_donuts+=anOrder.number_donuts;
		num_orders+=1;
		int tmp = anOrder.number_donuts/DOZEN; //integer div
		if (anOrder.number_donuts%DOZEN>0)
			tmp+=1;
		num_boxes+=tmp;
	}
	SAFEPRINT("\n For file " + fn);
	if (verify(total_donuts,num_donuts,"donuts")) total_pts+=DEFAULT_PTS;
	if (verify(total_orders,num_orders,"orders"))total_pts+=DEFAULT_PTS;
	if (verify(total_boxes,num_boxes,"boxes"))total_pts+=DEFAULT_PTS;
}

void testfile(string fn, int numwaiters=DEFAULT_NUM_WAITERS, int numbakers=DEFAULT_NUM_BAKERS ) {
	//reset these
	order_out_Vector.clear();
	all_work_done=false;
	num_working_waiters =0;
	
	//lets open a file of interest
	m_io.resetFilename(fn);
	//add some bakers and waiters
	std::vector<thread> thds;
	for (int j = 0; j < numwaiters; j++)
		thds.push_back(thread(doWaiter, j));
	for (int j = 0; j < numbakers; j++)
		thds.push_back(thread(doBaker, j));
	//wait for threads to exit
	for (auto &thd : thds)
		thd.join();
	audit_results(fn);
}
void test(int numwaiters=DEFAULT_NUM_WAITERS, int numbakers=DEFAULT_NUM_BAKERS){
	testfile(DEFAULT_ORDER_IN_FILE,numwaiters,numbakers );
//	testfile("in0.txt",numwaiters,numbakers );
	testfile("in1.txt",numwaiters,numbakers );
}

int main()
{
	test(1,1);
	test(1,4);
	test(4,1);
	test(4,4);
	
	cout<<"Total points="<<to_string(total_pts)<<endl;

	return SUCCESS;
}

