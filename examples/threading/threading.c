#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include<errno.h>
// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{

    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    thread_func_args->thread_complete_success=false;
    fprintf(stderr,"Debuuuuuuuuuuuuuuuuuuuuuuug------------- threadfunc started\n");
        fprintf(stderr,"Debuuuuuuuuuuuuuuuuuuuuuuug------------- start_thread_obtaining_mutex ID of thread is %lu  \n",pthread_self());

        fprintf(stderr,"Debuuuuuuuuuuuuuuuuuuuuuuug------------- wait to obtain %d\n",thread_func_args->wait_to_obtain_ms);
    usleep(thread_func_args->wait_to_obtain_ms*1000);
    int rc = pthread_mutex_lock(thread_func_args->mutex);
    if(rc!=0){
        fprintf(stderr,"Debuuuuuuuuuuuuuuuuuuuuuuug------------- unable to lock the mutex \n");
        //fflush(stdout); 
           // thread_func_args->thread_complete_success=false;
               return thread_param;

    }
    fprintf(stderr,"Debuuuuuuuuuuuuuuuuuuuuuuug------------- wait to release %d\n",thread_func_args->wait_to_release_ms);
    usleep(thread_func_args->wait_to_release_ms*1000);
   int rw = pthread_mutex_unlock(thread_func_args->mutex); 
    if(rw!=0){
        fprintf(stderr,"Debuuuuuuuuuuuuuuuuuuuuuuug------------- unable to unlock the mutex \n");
        //fflush(stdout); 
            thread_func_args->thread_complete_success=false;
                return thread_param;
    }
    else{
            fprintf(stderr,"Debuuuuuuuuuuuuuuuuuuuuuuug------------- thread executed succesfully \n");
           // fflush(stdout); 
    } 
        thread_func_args->thread_complete_success=true;
    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
     struct thread_data* data = malloc(sizeof(struct thread_data));
     if (data == NULL) {  // ✅ ADDED: Null check
    fprintf(stderr, "Debug: malloc failed\n");
    return false;
}
        fprintf(stderr,"Debuuuuuuuuuuuuuuuuuuuuuuug------------- start_thread_obtaining_mutex started\n");
     data->wait_to_obtain_ms=wait_to_obtain_ms;
     data->wait_to_release_ms=wait_to_release_ms;
     data->mutex= mutex;
    //pthread_mutex_init(&mutex,NULL);
    int ret =pthread_create(thread,NULL,threadfunc,data);
     fprintf(stderr,"Debuuuuuuuuuuuuuuuuuuuuuuug------------- pthread_create ret of thread is %d  \n",ret);

    if (ret!=0)
    {
        errno = ret;
       // perror("pthread_create");
       // fflush(stdout); 
                fprintf(stderr,"Debuuuuuuuuuuuuuuuuuuuuuuug------------- unable to create the thread \n");
       free(data);
        return false;
    }
    // if(data->thread_complete_success==true){
    //     //free(data);
    //             fprintf(stderr,"Debuuuuuuuuuuuuuuuuuuuuuuug------------- thread_complete_success==true\n");
    //     return false;
    // }


    return true;
}

