#include "p3140121-p3150087-p3140087-pizza2.h"

//Mutexes

pthread_mutex_t mutex_available_cooks = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutex_available_ovens = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutex_output = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutex_total_time_delivery = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutex_max_time_delivery = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutex_total_time_freezing = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutex_max_time_freezing = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutex_flag = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutex_available_deliverers = PTHREAD_MUTEX_INITIALIZER;

//Conditions

pthread_cond_t cond_no_available_cook = PTHREAD_COND_INITIALIZER;

pthread_cond_t cond_no_available_oven = PTHREAD_COND_INITIALIZER;

pthread_cond_t cond_flag = PTHREAD_COND_INITIALIZER;

pthread_cond_t cond_no_available_deliverer = PTHREAD_COND_INITIALIZER;

//Variables

int total_time_delivery;

int max_time_delivery;

int total_time_freezing;

int max_time_freezing;



int available_cooks;

int available_ovens;

unsigned int seed;

int flag;

int available_deliverers; 


//execute the orders each time

void* order(void *order_id) {

	int oid = *(int*) order_id;

	int rc;

	struct timespec order_started;
	
	struct timespec bake_finished;

	struct timespec order_finished;



	if (oid != 1) {

		rc = pthread_mutex_lock(&mutex_flag);

		check_succesful_mutex_lock_unlock(rc);

		while (flag == 1) {

			rc = pthread_cond_wait(&cond_flag, &mutex_flag);

			if (rc != 0) {

				printf("ERROR: return code from pthread_cond_wait() is %d\n",

						rc);

				pthread_exit(&rc);

			}



		}

		flag = 1;

		int delay_time = rand_r(&seed) % time_order_high + time_order_low;





		sleep(delay_time);

		flag = 0;

		rc = pthread_cond_signal(&cond_flag);

		if (rc != 0) {

			printf("ERROR: return code from pthread_cond_signal() is %d\n", rc);

			pthread_exit(&rc);

		}

		rc = pthread_mutex_unlock(&mutex_flag);

		check_succesful_mutex_lock_unlock(rc);

	}

	int order_time;
	
	int freezing_time;

	clock_gettime(CLOCK_REALTIME, &order_started);



	//Lock cooks

	rc = pthread_mutex_lock(&mutex_available_cooks);

	check_succesful_mutex_lock_unlock(rc);



	while (available_cooks <= 0) {

		rc = pthread_cond_wait(&cond_no_available_cook, &mutex_available_cooks);

		if (rc != 0) {

			printf("ERROR: return code from pthread_cond_wait() is %d\n", rc);

			pthread_exit(&rc);

		}

	}



	available_cooks--;



	//Unlock cooks

	rc = pthread_mutex_unlock(&mutex_available_cooks);

	check_succesful_mutex_lock_unlock(rc);



	//Peak a random number for pizzas

	int pizzas = rand_r(&seed) % no_pizzas_high + no_pizzas_low;





	sleep(pizzas * time_prep);



	//Lock ovens

	rc = pthread_mutex_lock(&mutex_available_ovens);

	check_succesful_mutex_lock_unlock(rc);



	while (available_ovens <= 0) {

		rc = pthread_cond_wait(&cond_no_available_oven, &mutex_available_ovens);

		if (rc != 0) {

			printf("ERROR: return code from pthread_cond_wait() is %d\n", rc);

			pthread_exit(&rc);

		}

	}



	available_ovens--;



	//Unlock ovens

	rc = pthread_mutex_unlock(&mutex_available_ovens);

	check_succesful_mutex_lock_unlock(rc);


	//Lock cooks

		rc = pthread_mutex_lock(&mutex_available_cooks);

		check_succesful_mutex_lock_unlock(rc);

		
		
		available_cooks++;
		
		
		
	//Unlock cooks and signal the thread waiting for available cooks

		rc = pthread_cond_signal(&cond_no_available_cook);

		if (rc != 0) {

			printf("ERROR: return code from pthread_cond_signal() is %d\n", rc);

			pthread_exit(&rc);

		}

		rc = pthread_mutex_unlock(&mutex_available_cooks);

		check_succesful_mutex_lock_unlock(rc);

			

	sleep(time_bake);
	
	clock_gettime(CLOCK_REALTIME, &bake_finished);
	
	
	

	//Lock deliverer
	
	rc = pthread_mutex_lock(&mutex_available_deliverers);

	check_succesful_mutex_lock_unlock(rc);
	
	while (available_deliverers <= 0) {

			rc = pthread_cond_wait(&cond_no_available_deliverer, &mutex_available_deliverers);

			if (rc != 0) {

				printf("ERROR: return code from pthread_cond_wait() is %d\n", rc);

				pthread_exit(&rc);

			}

		}
	
	available_deliverers--;
	
	
	//Unlock deliverers
	
	rc = pthread_mutex_unlock(&mutex_available_deliverers);

	check_succesful_mutex_lock_unlock(rc);
	


	//Lock ovens

	rc = pthread_mutex_lock(&mutex_available_ovens);

	check_succesful_mutex_lock_unlock(rc);


	

	available_ovens++;



	//Unlock ovens and signal the thread waiting for available ovens

	rc = pthread_cond_signal(&cond_no_available_oven);

	if (rc != 0) {

		printf("ERROR: return code from pthread_cond_signal() is %d\n", rc);

		pthread_exit(&rc);

	}

	rc = pthread_mutex_unlock(&mutex_available_ovens);

	check_succesful_mutex_lock_unlock(rc);
	
	
	int delivery_time = rand_r(&seed) % time_delivery_high + time_delivery_low;
	
	sleep(delivery_time);
	
	clock_gettime(CLOCK_REALTIME, &order_finished);
	
	
	sleep(delivery_time);
	
	rc = pthread_mutex_lock(&mutex_available_deliverers);

		check_succesful_mutex_lock_unlock(rc);
		
		available_deliverers++;
		
		//Unlock deliverers and signal the thread waiting for available deliverers

			rc = pthread_cond_signal(&cond_no_available_deliverer);

			if (rc != 0) {

				printf("ERROR: return code from pthread_cond_signal() is %d\n", rc);

				pthread_exit(&rc);

			}

			rc = pthread_mutex_unlock(&mutex_available_deliverers);

			check_succesful_mutex_lock_unlock(rc);
		
		


	



	order_time = order_finished.tv_sec - order_started.tv_sec; //method for the structure of time
	
	



	//Lock total_time

	rc = pthread_mutex_lock(&mutex_total_time_delivery);

	check_succesful_mutex_lock_unlock(rc);



	total_time_delivery += order_time;



	//Unlock total_time

	rc = pthread_mutex_unlock(&mutex_total_time_delivery);

	check_succesful_mutex_lock_unlock(rc);



	freezing_time = order_finished.tv_sec - bake_finished.tv_sec; //method for the structure of time


	//Lock total_time_freezing
	
	rc = pthread_mutex_lock(&mutex_total_time_freezing);

	check_succesful_mutex_lock_unlock(rc);
	
	
	
	total_time_freezing += freezing_time;
	
	
	//Unlock total_time_freezing
	
	rc = pthread_mutex_unlock(&mutex_total_time_freezing);

	check_succesful_mutex_lock_unlock(rc);

	
	

	//Lock max_time

	rc = pthread_mutex_lock(&mutex_max_time_delivery);

	check_succesful_mutex_lock_unlock(rc);



	if (max_time_delivery < order_time) {

		max_time_delivery = order_time;

	}



	//Unlock max_time

	rc = pthread_mutex_unlock(&mutex_max_time_delivery);

	check_succesful_mutex_lock_unlock(rc);


	//Lock max_time_freezing

		rc = pthread_mutex_lock(&mutex_max_time_freezing);

		check_succesful_mutex_lock_unlock(rc);



		if (max_time_freezing < freezing_time) {

			max_time_freezing = freezing_time;

		}



		//Unlock max_time_freezing

		rc = pthread_mutex_unlock(&mutex_max_time_freezing);

		check_succesful_mutex_lock_unlock(rc);

	
	

	//Lock console

	rc = pthread_mutex_lock(&mutex_output);

	check_succesful_mutex_lock_unlock(rc);



	printf("The order with the id: %d was delivered in %d minutes and was cooling for %d minutes .\n", oid, order_time,freezing_time);



	//Unlock console

	rc = pthread_mutex_unlock(&mutex_output);

	check_succesful_mutex_lock_unlock(rc);



	pthread_exit(NULL);



}



//the main function

int main(int argc, char *argv[]) {

	//->The user inserts the number of pizzas' orders=customers and the generator's seed



	//check if the number of arguments is valid

	int rc;



	if (argc != 3) {

		printf("ERROR: You have to give 2 arguments.One for the number of orders and one for the generator's seed.)\n\n\n");

		exit(-1);

	}

	//Take the argument for the number of pizzas' orders and check if the value is valid

	int no_orders = atoi(argv[1]);

	if (no_orders <= 0) {

		printf("ERROR:The second parameter must be a number and greater than zero.\n");

		exit(-1);

	}



	total_time_delivery = 0;

	max_time_delivery = 0;
	
	total_time_freezing = 0;
	
	max_time_freezing = 0;

	




	available_cooks = no_cooks;

	available_ovens = no_ovens;
	
	available_deliverers = no_deliverers;

	flag = 0;



	//Take the argument for the generator's seed

	seed = atoi(argv[2]);



	//Declare an array with the id of orders

	int order_id[no_orders];

	for (int i = 0; i < no_orders; ++i) {

		order_id[i] = i + 1;

	}



	//Create as many threads as the number of pizzas' orders

	pthread_t tid[no_orders];



	//Creation o first thread

	rc = pthread_create(&tid[0], NULL, &order, &order_id[0]);



	//Check if threads are created properly

	if (rc != 0) {

		printf("ERROR: return code from pthread_create() is %d\n", rc);

		exit(-1);

	}



	for (int i = 1; i < no_orders; ++i) {



		rc = pthread_create(&tid[i], NULL, &order, &order_id[i]);



		//Check if threads are created properly

		if (rc != 0) {

			printf("ERROR: return code from pthread_create() is %d\n", rc);

			exit(-1);

		}

	}



	for (int i = 0; i < no_orders; ++i) {

		rc = pthread_join(tid[i], NULL);

		if (rc != 0) {

			printf("ERROR: return code from pthread_join() is %d\n", rc);

			exit(-1);

		}



	}

	double avg_delivery = (double) total_time_delivery / no_orders;
	double avg_freezing = (double) total_time_freezing / no_orders;

	//Max times and Average times

	printf("The max delivery time was %d and the average delivery time was %lf.\n", max_time_delivery, avg_delivery);
	printf("The max cooling time was %d and the average cooling time was %lf.\n", max_time_freezing, avg_freezing);



//Destroy mutexes and conditions.



	rc = pthread_mutex_destroy(&mutex_available_cooks);

	if (rc != 0) {

		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);

		exit(-1);

	}



	rc = pthread_mutex_destroy(&mutex_available_ovens);

	if (rc != 0) {

		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);

		exit(-1);

	}



	rc = pthread_mutex_destroy(&mutex_output);

	if (rc != 0) {

		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);

		exit(-1);

	}

	rc = pthread_mutex_destroy(&mutex_total_time_delivery);

	if (rc != 0) {

		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);

		exit(-1);

	}
	
	rc = pthread_mutex_destroy(&mutex_total_time_freezing);

	if (rc != 0) {

		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);

		exit(-1);

	}

	rc = pthread_mutex_destroy(&mutex_max_time_delivery);

	if (rc != 0) {

		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);

		exit(-1);

	}
	
	rc = pthread_mutex_destroy(&mutex_max_time_freezing);

		if (rc != 0) {

			printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);

			exit(-1);

		}
	
	

	rc = pthread_mutex_destroy(&mutex_flag);

	if (rc != 0) {

		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);

		exit(-1);

	}
	
	rc = pthread_mutex_destroy(&mutex_available_deliverers);

		if (rc != 0) {

			printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);

			exit(-1);

		}
		
		rc = pthread_cond_destroy(&cond_no_available_deliverer);

		if (rc != 0) {

			printf("ERROR: return code from pthread_cond_destroy() is %d\n", rc);

			exit(-1);

		}

	rc = pthread_cond_destroy(&cond_no_available_cook);

	if (rc != 0) {

		printf("ERROR: return code from pthread_cond_destroy() is %d\n", rc);

		exit(-1);

	}

	rc = pthread_cond_destroy(&cond_no_available_oven);

	if (rc != 0) {

		printf("ERROR: return code from pthread_cond_destroy() is %d\n", rc);

		exit(-1);

	}

	rc = pthread_cond_destroy(&cond_flag);

	if (rc != 0) {

		printf("ERROR: return code from pthread_cond_destroy() is %d\n", rc);

		exit(-1);

	}



	return 0;

}



//Check if the mutexes' lock/unlock are done succesfully

void check_succesful_mutex_lock_unlock(int rc) {

	if (rc != 0) {

		printf("ERROR: Wrong lock/unlock with error code %d\n", rc);

		pthread_exit(&rc);

	}

}

