#include <iostream>
#include <list>
#include <functional>
#include <stdlib.h>
#include <cstring>
#include <chrono>
#include <pthread.h> 

int user_main(int argc, char **argv);

/* Demonstration on how to pass lambda as parameter.
 * "&&" means r-value reference. You may read about it online.
 */
void demonstration(std::function<void()> && lambda) {
  lambda();
}

int main(int argc, char **argv) {
  /* * Declaration of a sample C++ lambda function
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


typedef struct {
    int low;
    int high;
    std::function<void(int)> lambda;
} pfor1_args;

void* pfor1_func(void* ptr){
    pfor1_args* args = ((pfor1_args*) ptr);
    for (int i = args->low; i < args->high; i++){
        args->lambda(i);
    }
    return NULL;
}

void parallel_for(int low, int high, std::function<void(int)> &&lambda, int numThreads){
    if (numThreads <= 0){
        //error checking for thread count
        perror("Need > 0 threads!");
        exit(1);
    }

    if (high<low){
        perror("loop range can't be negative!");
        exit(1);
    }

    if (high==low){
        printf("Time Taken by Parallel For: 0 microsec (Zero range)\n");
        return;
    }

    pthread_t *tids = nullptr;
    pfor1_args *args = nullptr;

    try {
        //if 'new' fails, it throws std::bad_alloc, handled by the catch
        tids = new pthread_t[numThreads-1];
        args = new pfor1_args[numThreads];
    } catch (const std::bad_alloc& e) {
        perror("memory allocation failed for threads or arguments");
        exit(1);
    }
    int chunk = (high-low)/ numThreads;
    int extra = (high-low) % numThreads;

    int curr = low;

    auto start_time = std::chrono::high_resolution_clock::now(); //record start time

    for (int i = 0; i < numThreads-1; i++){
        args[i].low = curr;
        curr += chunk;
        if (extra > 0){
            curr++;
            extra--;
        }
        args[i].high = curr;
        args[i].lambda = lambda;

        //create a new Pthread
        if (pthread_create(&tids[i], NULL, pfor1_func, (void*)&args[i]) != 0){
            perror("pthread_create failed!");
            //cleanup previous successful threads
            for(int j = 0; j < i-1; j++) {
                pthread_join(tids[j], NULL);
            }

            //cleanup dynamic memory
            delete[] tids;
            delete[] args;

            exit(1);
        }
    }

	args[numThreads-1].low = curr;
	curr+=chunk;
	if (extra > 0){
		curr++;
		extra--;
	}
	args[numThreads-1].high = curr;
	args[numThreads-1].lambda = lambda;

	pfor1_func((void*)&args[numThreads-1]);

    for (int i = 0; i < numThreads-1; i++){
        //join (wait for) all created Pthreads
        if (pthread_join(tids[i],NULL) ){
            perror("pthread_join failed");
            delete[] tids;
            delete[] args;
            exit(1);
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now(); //record end time
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    printf("Time Taken by Parallel For: %ld microsec\n", duration); //print execution time
    delete[] tids;
    delete[] args;
}

//2D parallel for rund outer loop in parallel, using the 1D parallel for and passing an appropriate lambda function
void parallel_for(int low1, int high1, int low2, int high2, std::function<void(int,int)> &&lambda, int numThreads){
	parallel_for(low1,high1,[&](int i){
		for (int j = low2; j < high2; j++){
			lambda(i,j);
		}
	}, numThreads);
}

#define main user_main
