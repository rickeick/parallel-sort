#ifndef _SORT_H_

    char *ALGS[] = {
        "MergeSort Serial",
        "MergeSort com MPI",
        "MergeSort com OMP",
        "MergeSort com Odd-Even",
        "BubbleSort com Odd-Even"
    };

    void MergeSort(int *, int);

    void MergeSortMPI();

    void MergeSortOMP();

    void MergeSortOddEven();
    
    void BubbleSortOddEven();

#endif
