#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_NUM 100
int array[MAX_NUM];
size_t array_size = 0;

typedef struct {
    size_t start;
    size_t end;
} indexes;

//Initialization of array from user input.
void init_array();

//Print the values of array.
void print_array(int *arr, size_t size);

//Sort the array using thread 1 and thread2.
void *thread_sort(void *index);

//Required function for "qsort" in "thread_sort()".
int cmp(const void  *a, const void *b);

//Merge the array using thread 3.
void merge_array(int *result);

int main() {
    //Initialization of 3 Threads.
    pthread_t *tid = (pthread_t*)malloc(3 * sizeof(pthread_t));
    pthread_attr_t attr; /* set of attributes for the thread */
    pthread_attr_init(&attr);

    //Initialization of arraies from user input.
    init_array();
    printf("Original array:\n");
    print_array(array, array_size);

    indexes data[2];
    data[0].start = 0, data[0].end = array_size / 2;
    data[1].start = array_size / 2, data[1].end = array_size;

    //Create threads to start sorting.
    pthread_create(&tid[0], &attr, thread_sort, &data[0]);
    pthread_create(&tid[1], &attr, thread_sort, &data[1]);
    //Wait.
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    //Show the results of the 2 sorted array.
    printf("Thread 0:\n");
    print_array(array, data[0].end - data[0].start);
    printf("Thread 1:\n");
    print_array(array + data[1].start, data[1].end - data[1].start);

    int *sorted_array = malloc(sizeof(int) * array_size);
    pthread_create(&tid[2], &attr, merge_array, sorted_array);
    pthread_join(tid[2], NULL);

    //Show the result of final sorted array
    printf("After merging:\n");
    print_array(sorted_array, array_size);

    return 0;
}

//Initialization of array from user input.
void init_array() {
    printf("Please enter the number of elements:");
    scanf("%ld", &array_size);
    for(size_t i = 0; i != array_size; ++i) {
        scanf("%d", &array[i]);
    }
}

//Print the values of array.
void print_array(int *arr, size_t size) {
    for(size_t i = 0; i != size; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

//Sort the array using thread 1 and thread2.
void *thread_sort(void *index) {
    size_t start, end;
    start = ((indexes *)index)->start;
    end = ((indexes *)index)->end;
    qsort(array + start, end - start, sizeof(int), cmp);
    pthread_exit(0);
}

//Required function for "qsort" above.
int cmp(const void  *a, const void *b) {
    return *((int *)a) - *((int *)b);
}

//Merge the array using thread 3.
void merge_array(int *result) {
    size_t start1 = 0, end1 = array_size / 2;
    size_t start2 = array_size / 2, end2 = array_size;
    size_t i = 0;
    while(start1 < end1 && start2 < end2) {
        if(array[start1] < array[start2]) {
            result[i++] = array[start1++];
        } else {
            result[i++] = array[start2++];
        }
    }
    if(start2 < end2) {
        start1 = start2, end1 = end2;
    }
    while(start1 < end1) {
        result[i++] = array[start1++];
    }
    pthread_exit(0);
}
