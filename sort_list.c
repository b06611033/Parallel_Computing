//
// Sorts a list using multiple threads
//

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>

#define MAX_THREADS 65536
#define MAX_LIST_SIZE 300000000

#define DEBUG 0

// Thread variables
//
// VS: ... declare thread variables, mutexes, condition varables, etc.,
// VS: ... as needed for this assignment
//

pthread_mutex_t p_lock;
pthread_mutex_t list_lock;
pthread_mutex_t work_lock;
pthread_attr_t p_attr;
pthread_barrier_t pb;
pthread_t p_threads[MAX_THREADS];
pthread_cond_t p_cond[MAX_THREADS];

// argument needed for each thread
struct p_thread_args
{
    int thread_id;
    int *ptr;
};
typedef struct p_thread_args p_thread_args;

struct barrier_node
{
    pthread_mutex_t lock;
    pthread_cond_t sync;
    pthread_cond_t release;
    int count;
};
typedef struct barrier_node barrier_node;
typedef struct barrier_node barriers[MAX_THREADS];
barriers b_arr;

void barrier_init(barriers b)
{
    int i;
    for (i = 0; i < MAX_THREADS; i++)
    {
        b[i].count = 0;
        pthread_mutex_init(&(b[i].lock), NULL);
        pthread_cond_init(&(b[i].sync), NULL);
        pthread_cond_init(&(b[i].release), NULL);
    }
}

void barrier_sync(barriers b, int thread_id, int blk_id, int level)
{
    int target_count = (1 << (level + 1)); // number of threads in this block
    // pick 1 thread (thread_id % target_count == 0) to wait under b[blk_id].sync condition
    if (thread_id % target_count == 0)
    {
        pthread_mutex_lock(&(b[blk_id].lock));
        b[blk_id].count++;
        while (b[blk_id].count < target_count)
        {
            pthread_cond_wait(&(b[blk_id].sync), &(b[blk_id].lock));
            // after being signaled, this thread will enter the barrier_release function and releas others
        }
        pthread_mutex_unlock(&(b[blk_id].lock));
    }
    else
    {
        // all other threads wait under b[blk_id].release condition
        pthread_mutex_lock(&(b[blk_id].lock));
        b[blk_id].count++;
        if (b[blk_id].count == target_count)
        {
            // When last thread arrives, it signals the thread waiting under b[blk_id].sync condition
            pthread_cond_signal(&(b[blk_id].sync));
        }
        while (pthread_cond_wait(&(b[blk_id].release), &(b[blk_id].lock)) != 0)
            ;
        pthread_mutex_unlock(&(b[blk_id].lock));
    }
}

void barrier_release(barriers b, int thread_id, int blk_id, int level)
{
    pthread_mutex_lock(&(b[blk_id].lock));
    int target_count = (1 << (level + 1));
    if (thread_id == blk_id && b[blk_id].count == target_count)
    {
        // the particular thread will be responsible to releas others, and initialize count to 0
        b[blk_id].count = 0;
        pthread_cond_broadcast(&(b[blk_id].release));
    }
    pthread_mutex_unlock(&(b[blk_id].lock));
}

void barrier_destroy(barriers b)
{
    int i;
    for (i = 0; i < MAX_THREADS; i++)
    {
        pthread_mutex_destroy(&(b[i].lock));
        pthread_cond_destroy(&(b[i].sync));
        pthread_cond_destroy(&(b[i].release));
    }
}

// Global variables
int num_threads;  // Number of threads to create - user input
int list_size;    // List size
int *list;        // List of values
int *work;        // Work array
int *list_orig;   // Original list of values, used for error checking
int globalQ;      // the total level - user input
int sublist_size; // the sublist size - determined by user input

// Print list - for debugging
void print_list(int *list, int list_size)
{
    int i;
    for (i = 0; i < list_size; i++)
    {
        printf("[%d] \t %16d\n", i, list[i]);
    }
    printf("--------------------------------------------------------------------\n");
}

// Comparison routine for qsort (stdlib.h) which is used to
// a thread's sub-list at the start of the algorithm
int compare_int(const void *a0, const void *b0)
{
    int a = *(int *)a0;
    int b = *(int *)b0;
    if (a < b)
    {
        return -1;
    }
    else if (a > b)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// Return index of first element larger than or equal to v in sorted list
// ... return last if all elements are smaller than v
// ... elements in list[first], list[first+1], ... list[last-1]
//
//   int idx = first; while ((v > list[idx]) && (idx < last)) idx++;
//
int binary_search_lt(int v, int *list, int first, int last)
{
    // Linear search code
    // int idx = first; while ((v > list[idx]) && (idx < last)) idx++; return idx;

    int left = first;
    int right = last - 1;

    if (list[left] >= v)
        return left;
    if (list[right] < v)
        return right + 1;
    int mid = (left + right) / 2;
    while (mid > left)
    {
        if (list[mid] < v)
        {
            left = mid;
        }
        else
        {
            right = mid;
        }
        mid = (left + right) / 2;
    }
    return right;
}
// Return index of first element larger than v in sorted list
// ... return last if all elements are smaller than or equal to v
// ... elements in list[first], list[first+1], ... list[last-1]
//
//   int idx = first; while ((v >= list[idx]) && (idx < last)) idx++;
//
int binary_search_le(int v, int *list, int first, int last)
{

    // Linear search code
    // int idx = first; while ((v >= list[idx]) && (idx < last)) idx++; return idx;

    int left = first;
    int right = last - 1;

    if (list[left] > v)
        return left;
    if (list[right] <= v)
        return right + 1;
    int mid = (left + right) / 2;
    while (mid > left)
    {
        if (list[mid] <= v)
        {
            left = mid;
        }
        else
        {
            right = mid;
        }
        mid = (left + right) / 2;
    }
    return right;
}

// Sort list via parallel merge sort
//
// VS: ... to be parallelized using threads ...
//
void *sort_list(void *_args)
{

    int i, level;
    int my_list_size;

    struct p_thread_args *args = (struct p_thread_args *)_args;
    int thread_id = args->thread_id;
    int *ptr = args->ptr;

    int my_own_blk, my_own_idx;
    int my_blk_size, my_search_blk, my_search_idx, my_search_idx_max;
    int my_write_blk, my_write_idx;
    int my_search_count;
    int idx, i_write;

    my_list_size = ptr[thread_id + 1] - ptr[thread_id];
    qsort(&list[ptr[thread_id]], my_list_size, sizeof(int), compare_int);

    for (level = 0; level < globalQ; level++)
    {
        // Scatter list elements into work array
        my_blk_size = sublist_size * (1 << level); // block size of current level
        // find the starting position of the block the thread belongs to
        my_own_blk = (thread_id >> level) << level;
        my_own_idx = ptr[my_own_blk];

        // find the starting position of the adjacent block
        my_search_blk = ((thread_id >> level) << level) ^ (1 << level);
        my_search_idx = ptr[my_search_blk];
        my_search_idx_max = my_search_idx + my_blk_size;

        // find the starting position of the double sized block going to be written
        my_write_blk = ((thread_id >> (level + 1)) << (level + 1));
        my_write_idx = ptr[my_write_blk];

        idx = my_search_idx;

        my_search_count = 0;

        // need a barrier to wait for other thread quick sort their sublist
        barrier_sync(b_arr, thread_id, my_write_blk, level);
        barrier_release(b_arr, thread_id, my_write_blk, level);

        // Bianry search for 1st element
        // use lt and le to avoid different threads writing into same position due to equal values
        if (my_search_blk > my_own_blk)
        {
            idx = binary_search_lt(list[ptr[thread_id]], list, my_search_idx, my_search_idx_max);
        }
        else
        {
            idx = binary_search_le(list[ptr[thread_id]], list, my_search_idx, my_search_idx_max);
        }
        my_search_count = idx - my_search_idx;
        i_write = my_write_idx + my_search_count + (ptr[thread_id] - my_own_idx); // work array position to write

        work[i_write] = list[ptr[thread_id]];

        // Linear search for 2nd element onwards
        for (i = ptr[thread_id] + 1; i < ptr[thread_id + 1]; i++)
        {
            if (my_search_blk > my_own_blk)
            {
                while ((list[i] > list[idx]) && (idx < my_search_idx_max))
                {
                    idx++;
                    my_search_count++;
                }
            }
            else
            {
                while ((list[i] >= list[idx]) && (idx < my_search_idx_max))
                {
                    idx++;
                    my_search_count++;
                }
            }
            i_write = my_write_idx + my_search_count + (i - my_own_idx);

            work[i_write] = list[i];
        }
        // needs a barrier because other threads might not yet updated the value to the working array positions the current thread is responsible of
        barrier_sync(b_arr, thread_id, my_write_blk, level);
        barrier_release(b_arr, thread_id, my_write_blk, level);

        // Copy work into list
        for (i = ptr[thread_id]; i < ptr[thread_id + 1]; i++)
        {
            list[i] = work[i];
        }
        pthread_barrier_wait(&pb); // wait for all threads to synchronize

        // if (DEBUG) printf("Thread %d, level = %d\n", my_id, level);
        // if (DEBUG) print_list(list, list_size);
    }
    pthread_exit(NULL);
}

// Main program - set up list of random integers and use threads to sort the list
//
// Input:
//	k = log_2(list size), therefore list_size = 2^k
//	q = log_2(num_threads), therefore num_threads = 2^q
//
int main(int argc, char *argv[])
{
    struct timespec start, stop, stop_qsort;
    double total_time, time_res, total_time_qsort;
    int k, q, j, error;

    // Read input, validate
    if (argc != 3)
    {
        printf("Need two integers as input \n");
        printf("Use: <executable_name> <log_2(list_size)> <log_2(num_threads)>\n");
        exit(0);
    }
    k = atoi(argv[argc - 2]);
    if ((list_size = (1 << k)) > MAX_LIST_SIZE)
    {
        printf("Maximum list size allowed: %d.\n", MAX_LIST_SIZE);
        exit(0);
    };
    q = atoi(argv[argc - 1]);
    if ((num_threads = (1 << q)) > MAX_THREADS)
    {
        printf("Maximum number of threads allowed: %d.\n", MAX_THREADS);
        exit(0);
    };
    if (num_threads > list_size)
    {
        printf("Number of threads (%d) < list_size (%d) not allowed.\n",
               num_threads, list_size);
        exit(0);
    };

    // Allocate list, list_orig, and work

    list = (int *)malloc(list_size * sizeof(int));
    list_orig = (int *)malloc(list_size * sizeof(int));
    work = (int *)malloc(list_size * sizeof(int));

    //
    // VS: ... May need to initialize mutexes, condition variables,
    // VS: ... and their attributes
    //
    globalQ = q;                            // assign q to global q
    sublist_size = list_size / num_threads; // assign sublist size t0 global sublist size
    // nums_threads + 1 is to add list_size + 1 to the last position for the last threads convenience
    int ptr[num_threads + 1]; // array to save the starting position of each thread
    struct p_thread_args *args[MAX_THREADS];
    int thread_id;

    pthread_mutex_init(&work_lock, NULL);
    pthread_mutex_init(&list_lock, NULL);
    pthread_mutex_init(&p_lock, NULL);
    pthread_barrier_init(&pb, NULL, num_threads);
    pthread_attr_init(&p_attr);
    pthread_attr_setdetachstate(&p_attr, PTHREAD_CREATE_JOINABLE);

    for (thread_id = 0; thread_id < num_threads; thread_id++)
    {
        pthread_cond_init(&p_cond[thread_id], NULL);
    }

    barrier_init(b_arr);
    // Initialize list of random integers; list will be sorted by
    // multi-threaded parallel merge sort
    // Copy list to list_orig; list_orig will be sorted by qsort and used
    // to check correctness of multi-threaded parallel merge sort
    srand48(0); // seed the random number generator
    for (j = 0; j < list_size; j++)
    {
        list[j] = (int)lrand48();
        list_orig[j] = list[j];
    }
    // duplicate first value at last location to test for repeated values
    list[list_size - 1] = list[0];
    list_orig[list_size - 1] = list_orig[0];

    // Create threads; each thread executes find_minimum
    clock_gettime(CLOCK_REALTIME, &start);

    //
    // VS: ... may need to initialize mutexes, condition variables, and their attributes
    //

    // Serial merge sort
    // VS: ... replace this call with multi-threaded parallel routine for merge sort
    // VS: ... need to create threads and execute thread routine that implements
    // VS: ... parallel merge sort

    // compute starting position for each thread
    for (thread_id = 0; thread_id < num_threads; thread_id++)
    {
        ptr[thread_id] = thread_id * sublist_size;
    }
    ptr[num_threads] = list_size; // additional ending position for the convenience of last thread

    // Create threads and assign tasks
    for (thread_id = 0; thread_id < num_threads; thread_id++)
    {
        args[thread_id] = (struct p_thread_args *)malloc(sizeof(struct p_thread_args));
        args[thread_id]->thread_id = thread_id;
        args[thread_id]->ptr = ptr;
        pthread_create(&p_threads[thread_id], &p_attr, sort_list, (void *)args[thread_id]);
    }
    // After tasks are finished, join all threads
    for (thread_id = 0; thread_id < num_threads; thread_id++)
    {
        pthread_join(p_threads[thread_id], NULL);
    }

    // Compute time taken
    clock_gettime(CLOCK_REALTIME, &stop);
    total_time = (stop.tv_sec - start.tv_sec) + 0.000000001 * (stop.tv_nsec - start.tv_nsec);

    // Check answer
    qsort(list_orig, list_size, sizeof(int), compare_int);
    clock_gettime(CLOCK_REALTIME, &stop_qsort);
    total_time_qsort = (stop_qsort.tv_sec - stop.tv_sec) + 0.000000001 * (stop_qsort.tv_nsec - stop.tv_nsec);

    error = 0;
    for (j = 1; j < list_size; j++)
    {
        if (list[j] != list_orig[j])
            error = 1;
    }

    if (error != 0)
    {
        printf("Houston, we have a problem!\n");
    }

    // Print time taken
    printf("List Size = %d, Threads = %d, error = %d, time (sec) = %8.4f, qsort_time = %8.4f\n",
           list_size, num_threads, error, total_time, total_time_qsort);

    // VS: ... destroy mutex, condition variables, etc.
    free(list);
    free(work);
    free(list_orig);
    pthread_mutex_destroy(&p_lock);
    pthread_mutex_destroy(&list_lock);
    pthread_mutex_destroy(&work_lock);
    pthread_attr_destroy(&p_attr);
    pthread_barrier_destroy(&pb);
    for (thread_id = 0; thread_id < num_threads; thread_id++)
    {
        pthread_cond_destroy(&p_cond[thread_id]);
    }
    barrier_destroy(b_arr);
}
