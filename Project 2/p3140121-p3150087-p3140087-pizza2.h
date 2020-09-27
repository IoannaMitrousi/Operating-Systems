#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h> 

//const variables
const int no_cooks=2;
const int no_ovens=5;
const int no_deliverers=10;

const int time_order_low =1;
const int time_order_high=5;

const int no_pizzas_low=1;
const int no_pizzas_high=5;

const int time_prep=1;
const int time_bake=10;

const int time_delivery_low=5;
const int time_delivery_high=15;

//functions
void check_succesful_mutex_lock_unlock(int rc);
