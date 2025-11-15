#include <iostream>
#include <list>
#include <functional>
#include <stdlib.h>
#include <cstring>
#include <chrono>

int user_main(int argc, char **argv);

/* Demonstration on how to pass lambda as parameter.
 * "&&" means r-value reference. You may read about it online.
 */
void demonstration(std::function<void()> && lambda) {
  lambda();
}

int main(int argc, char **argv) {
  /* 
   * Declaration of a sample C++ lambda function
   * that captures variable 'x' by value and 'y'
   * by reference. Global variables are by default
   * captured by reference and are not to be supplied
   * in the capture list. Only local variables must be 
   * explicity captured if they are used inside lambda.
   */
  int x=5,y=1;
  // Declaring a lambda expression that accepts void type parameter
  auto /*name*/ lambda1 = /*capture list*/[/*by value*/ x, /*by reference*/ &y](void) {
    /* Any changes to 'x' will throw compilation error as x is captured by value */
    y = 5;
    std::cout<<"====== Welcome to Assignment-"<<y<<" of the CSE231(A) ======\n";
    /* you can have any number of statements inside this lambda body */
  };
  // Executing the lambda function
  demonstration(lambda1); // the value of x is still 5, but the value of y is now 5

  int rc = user_main(argc, argv);
 
  auto /*name*/ lambda2 = [/*nothing captured*/]() {
    std::cout<<"====== Hope you enjoyed CSE231(A) ======\n";
    /* you can have any number of statements inside this lambda body */
  };
  demonstration(lambda2);
  return rc;
}

void parallel_for(int low, int high, std::function<void(int)> &&lambda, int numThreads){
}


typedef struct {
	int low1;
	int high1;
	int low2;
	int high2;
	std::function<void(int,int)> lambda;
} pfor2_args;

void* pfor2_func(void* ptr){
	pfor2_args* args = ((pfor2_args*) ptr);
	for (int i = args->low1; i < args->high1; i++){
		for (int j = args->low2; j < args->high2; j++){
			args->lambda(i,j);
		}
	}
	return NULL;
}

void parallel_for(int low1, int high1, int low2, int high2, std::function<void(int,int)> &&lambda, int numThreads){

	//struct for args
	//need numThreads threads
	//divide high1-low1 into numThreads chunks for parallel execution
	//record start time for parallel for
	//start parallel execution using a for loop
	//join() to wait for results
	//record end time for parallel for and print it
	if (numThreads <= 0){
		perror("Need > 0 threads!");
		exit(1);
	}

	pthread_t *tids = new pthread_t[numThreads];
        pfor2_args *args = new pfor2_args[numThreads];
	int chunk = (high1 - low1)/numThreads;
	int extra = (high1 - low1)%numThreads;

	int curr = low1;

	auto start_time = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < numThreads; i++){
		args[i].low1 = curr;
		curr += chunk;
		if (extra > 0){
			curr++;
			extra--;
		}
		args[i].high1 = curr;
		args[i].low2 = low2;
		args[i].high2 = high2;
		args[i].lambda = lambda;
		if (pthread_create(&tids[i], NULL, pfor2_func, (void*)&args[i]) != 0){
			perror("pthread_create failed!");
			exit(1);
		}
	}

	for (int i = 0; i < numThreads; i++){
		if (pthread_join(tids[i],NULL) ){
			perror("pthread_join failed");
			exit(1);
		}
	}

	auto end_time = std::chrono::high_resolution_clock::now();
    	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

	printf("Time Taken by Parallel For: %ld microsec\n", duration);
	delete[] tids;
	delete[] args;
}

#define main user_main

