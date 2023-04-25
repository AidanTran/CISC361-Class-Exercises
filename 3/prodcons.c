#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#define MAX_ITEMS_TO_PRODUCE 10
#define MAX_ITEMS_TO_CONSUME 10
#define BUFFER_SIZE 5
#define NumProducer 1
#define NumConsumer 1
sem_t empty;
sem_t full;
int in = 0;
int out = 0;
int buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
typedef struct arg_data
{
    int thread_number;
} arg_data;
void *producer(void *arg)
{
    int item;
    arg_data *current_thread_data = (arg_data *)arg;
    for (int i = 0; i < MAX_ITEMS_TO_PRODUCE; i++)
    {
        item = rand();
        // start of critical section
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = item;
        printf("Producer %d: Inserted item %d into index %d\n", current_thread_data->thread_number, item,
               in);
        in = (in + 1) % 5;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        // end of critical section
    }
    return NULL;
}

void *consumer(void *arg)
{
    arg_data *current_thread_data = (arg_data *)arg;
    for (int i = 0; i < MAX_ITEMS_TO_CONSUME; i++)
    {
        // start of critical section
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int item = buffer[out];
        printf("Consumer %d: Removed item %d from index %d\n", current_thread_data->thread_number,
               item, out);
        out = (out + 1) % 5;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        // end of critical section
    }
    return NULL;
}

int main()
{
    pthread_t producers[NumProducer], consumers[NumConsumer];
    arg_data producer_arg_arr[NumProducer];
    arg_data consumer_arg_arr[NumConsumer];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    for (int thread_no = 1; thread_no <= NumProducer; thread_no++)
    {
        producer_arg_arr[thread_no - 1].thread_number = thread_no;
        pthread_create(&producers[thread_no - 1], NULL, producer, &producer_arg_arr[thread_no - 1]);
    }
    for (int thread_no = 1; thread_no <= NumConsumer; thread_no++)
    {
        consumer_arg_arr[thread_no - 1].thread_number = thread_no;
        pthread_create(&consumers[thread_no - 1], NULL, consumer, &consumer_arg_arr[thread_no - 1]);
    }
    for (int i = 0; i < NumProducer; i++)
    {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < NumConsumer; i++)
    {
        pthread_join(consumers[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    return 0;
}
