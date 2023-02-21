#include <stdio.h>

void pushSort(int arr[], int n)
{
    int i, j, temp;
    for (i = 1; i < n; i++)
    {
        temp = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > temp)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
    }
}

int main()
{
    int arr[] = {3, 5, 8, 1, 2, 9, 4, 7, 6};
    int n = sizeof(arr) / sizeof(arr[0]);
    pushSort(arr, n);
    for (int i = 0; i < n; i++)
    {
        printf("%d(MISSING) ", arr[i]);
    }
    return 0;
}