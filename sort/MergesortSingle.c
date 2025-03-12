#include <sys/types.h>
#include <sys/unistd.h>

#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "unistd.h"
#include "string.h"
# define CLOCK_MONOTONIC		1
void swap(long long *x, long long *y) {
    *x += *y;
    *y = *x - *y;
    *x -= *y;
    return;
}
void mergeSort(long long *arr, const int L, const int R) {
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
    mergeSort(arr, L, mid);
    mergeSort(arr, mid + 1, R);

    // Merge two halves
    long long tmp[R - L + 1];
    int i = L, j = mid + 1, k = 0;

    // Merge while both halves are valid
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

    // Copy leftover from left half
    while (i <= mid) {
        tmp[k++] = arr[i++];
    }
    // Copy leftover from right half
    while (j <= R) {
        tmp[k++] = arr[j++];
    }

    // Write merged data back to arr
    for (int p = 0; p < k; p++) {
        arr[L + p] = tmp[p];
    }

    return;
}

int main() {
    long long *arr;
    int n;
    scanf("%d", &n);
    arr = (long long *)malloc(n * sizeof(long long));
    for (int i = 0; i < n; i++) {
        scanf("%lld", &arr[i]);
    }

    struct timespec begin, finish;  
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &begin);
    mergeSort(arr, 0, n - 1);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - begin.tv_sec) + (finish.tv_nsec - begin.tv_nsec) / 1000000000.0;
    fprintf(stderr, "Time elapsed: %f seconds.\n", elapsed);
    for (int i = 0; i <= n - 1; i++) {
        printf("%lld ", arr[i]);
    }
    free(arr);
    return 0;
}
