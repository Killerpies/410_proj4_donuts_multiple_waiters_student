#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <atomic>
#include "datastructs.h"
#include "file_IO.h"

//the mutexes
extern std::mutex mutex_order_inQ;
extern std::mutex mutex_order_outQ;

//the condition variable
extern std::condition_variable cv_order_inQ;

//initialized to number of waiter threads
//when ==0, baker can quit when there 
//are no more orders in order_in_Q
extern std::atomic<int> num_working_waiters;

//waiters will process all the orders
//when the last one leaves it will set 
//all_work_done to true
extern std::atomic<bool> all_work_done;


//where orders come from
extern File_IO m_io;

//where orders are stored
extern std::queue<ORDER> order_in_Q;
extern std::vector<ORDER> order_out_Vector;
