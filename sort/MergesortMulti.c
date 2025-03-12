#include <sys/types.h>
#include <sys/unistd.h>

#include "math.h"
#include "pthread.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/time.h"
#include "unistd.h"
#define CLOCK_MONOTONIC 1
int MAX_THREADS = 2;
int MAX_DEPTH = 1;
long long *arr;

void merge2parts(const int L, const int R, const int mid) {
    long long tmp[R - L + 1];
    int i = L, j = mid + 1, k = 0;
    while (i <= mid && j <= R) {
        if (arr[i] < arr[j]) {
            tmp[k++] = arr[i++];
        } else if (arr[i] > arr[j]) {
            tmp[k++] = arr[j++];
        } else {
            tmp[k++] = arr[i++];
            tmp[k++] = arr[j++];
        }
    }
    while (i <= mid) {
        tmp[k++] = arr[i++];
    }
    while (j <= R) {
        tmp[k++] = arr[j++];
    }
    for (int p = 0; p < k; p++) {  // copy the sorted array back to the original array
        arr[L + p] = tmp[p];
    }
}
void swap(long long *x, long long *y) {
    *x += *y;
    *y = *x - *y;
    *x -= *y;
    return;
}

void mergeSort(const int L, const int R) {
    if (L == R) {
        return;
    }
    if (R - L == 1) {
        if (arr[L] > arr[R]) {
            swap(&arr[L], &arr[R]);
            return;
        }
    }

    int mid = (L + R) / 2;
    mergeSort(L, mid);
    mergeSort(mid + 1, R);

    // Merge two halves
    merge2parts(L, R, mid);
    return;
}

void *mergeSortThread(void *arguments) {
    // Cast the passed pointer to an int array
    int *args = (int *)arguments;
    int start = args[0];
    int end = args[1];
    free(args);  // free dynamically allocated memory

    mergeSort(start, end);
    return NULL;  // return from pthread function
}

int min(int x, int y) {
    if (x < y) {
        return x;
    }
    return y;
}

typedef struct Slot {
    int L, R;
} Slot;

void mergeSlot(const Slot *slot, int left, int right) {
    if (left == right) {
        return;
    }
    if (left == right - 1) {  // merge two nerbouring slots
        int L = slot[left].L;
        int R = slot[right].R;
        int mid = slot[left].R;

        merge2parts(L, R, mid);
        return;
    }

    int midSlotIdx = (left + right) / 2;
    mergeSlot(slot, left, midSlotIdx);
    mergeSlot(slot, midSlotIdx + 1, right);

    int L = slot[left].L;
    int mid = slot[midSlotIdx].R;
    int R = slot[right].R;

    merge2parts(L, R, mid);
    return;
}
int main(int argc, char *argv[]) {
    struct timespec begin, finish;
    double time_elapsed;

    if (argc > 1) {
        MAX_THREADS = atoi(argv[1]);
        MAX_DEPTH = log(MAX_THREADS) / log(2);
    }

    int n;
    scanf("%d", &n);
    arr = (long long *)malloc(n * sizeof(long long));
    for (int i = 0; i < n; i++) {
        scanf("%lld", &arr[i]);
    }

    clock_gettime(CLOCK_MONOTONIC, &begin);  // timing start , after input reading

    Slot *slot = (Slot *)malloc((n / MAX_THREADS + 3) * sizeof(Slot));
    // slot stands for each divided part of the array

    int slotCnt = 0;
    pthread_t threads[MAX_THREADS * 2];
    size_t stride = n / MAX_THREADS;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    for (int i = 0; i * stride < n; i++) {
        // printf("Creating thread %d\n", i);
        int start = stride * i;
        int end = min(stride * (i + 1) - 1, n - 1);
        slot[i].L = start;
        slot[i].R = end;
        slotCnt++;
        int *targs = (int *)malloc(2 * sizeof(int));
        targs[0] = start;
        targs[1] = end;

        pthread_create(&threads[i], &attr, mergeSortThread, (void *)targs);
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }  // wait for each threads to complete

    mergeSlot(slot, 0, slotCnt - 1);

    clock_gettime(CLOCK_MONOTONIC, &finish);  // timing end
    time_elapsed = ((finish.tv_sec - begin.tv_sec) + (finish.tv_nsec - begin.tv_nsec) / 1e9);
    fprintf(stderr, "Time elapsed: %f\n", time_elapsed);
    for (int i = 0; i <= n - 1; i++) {
        printf("%lld ", arr[i]);
    }

    free(arr);
    free(slot);
    return 0;
}
