#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#define MAX_NO_OF_THREADS 2
#define MAX_NO_OF_ELEMENTS 1000000000
typedef struct arg_data {
    int thread_number;
} arg_data;
//shared data on which threads will work concurrently
//array which will be summed
//sum variable that will store the total sum
static long long int sum;
void* worker_sum(void* arg)
{
    arg_data* current_thread_data = (arg_data*)arg;
 
    printf("Current thread no is : %d\n", current_thread_data->thread_number);
 
    //Determine the bounds
    int width = MAX_NO_OF_ELEMENTS / MAX_NO_OF_THREADS;
    long long i = current_thread_data->thread_number;
    //Generate the sum
    long long current_thread_sum = 0;
    for (i; i <  MAX_NO_OF_ELEMENTS; i+= MAX_NO_OF_THREADS) {
        current_thread_sum += i + 1;
    }
 
    return current_thread_sum;
}

int main()
{
    //let the array consists of first MAX_NO_OF_ELEMENTS integers,
    //1 to MAX_NO_OF_ELEMENTS
   
    //pthread objects
    pthread_t id[MAX_NO_OF_THREADS];
   
    //argument data to send in worker functions
    arg_data arg_arr[MAX_NO_OF_THREADS];
    for (int i = 0; i < MAX_NO_OF_THREADS; i++) {
        arg_arr[i].thread_number = i;
    }
    //total number of threads we will create
    int no_of_threads = MAX_NO_OF_THREADS;
    printf("Creating %d number of threads...\n", no_of_threads);
//Setup timing
    clock_t start, end;
    double cpu_time_taken;
    start = clock();
    int thread_no = 1;
   
    //creating the child threads
    for (int i = 0; i < MAX_NO_OF_THREADS; i++) {
        pthread_create(&id[i], NULL, worker_sum, &arg_arr[i]);
    }
    //joining the threads one by one
    for (int i = 0; i < MAX_NO_OF_THREADS; i++) {
        void * recv;
        pthread_join(id[i], &recv);
        sum += (long long) recv;
    }
    end = clock();
    cpu_time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("All child threads has finished their works...\n");
    printf("Total sum: %lld\n", sum);
    printf("Time taken to sum all the numbers are %lf\n", cpu_time_taken);
    return 0;
}