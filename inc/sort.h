#ifndef _SORT_H_

    char *ALGS[] = {
        "MergeSort Serial",
        "MergeSort com MPI",
        "MergeSort com OMP",
        "MergeSort com Odd-Even",
        "BubbleSort com Odd-Even"
    };

    int MergeSort(int *, int);

    int MergeSortMPI();

    int MergeSortOMP();

    int MergeSortOddEven();
    
    int BubbleSortOddEven();

#endif
