#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// A utility function to swap two elements
void swap(int* a, int* b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
   array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */

int partition (int array[], int low, int high)
{
	int pivot = array[high];    // pivot
	int i = (low -1);  // Index of smaller element


	for (int j = low; j <= high- 1; j++)
    {
		if (array[j] <= pivot)
      {
				i++;
				swap(&array[i], &array[j]);
      }
    }
	swap(&array[i + 1], &array[high]);
	return (i + 1);
}



/* The main function that implements QuickSort
   array[] --> array to be sorted,
   low  --> Starting index,
   high  --> Ending index */
void quickSort(int array[], int low, int high)
{

	if (low < high) {
		/* pi is partitioning index, array[p] is now
		   at right place */

		int pi = partition(array, low, high);

		if (high - low < 10000) {

			quickSort(array, low, pi - 1);
			quickSort(array, pi + 1, high);

		} else {

			#pragma omp task default(none) firstprivate(array,low,pi)
			{
				// Separately sort elements
				quickSort(array, low, pi - 1);
				}
				#pragma omp task default(none) firstprivate(array,high,pi)
				{
						quickSort(array, pi + 1, high);
				}
		}
  }
}

// Function to print an array
void printArray(int array[], int size)
{
	int i;
	for (i=0; i < size; i++)
		printf("%d ", array[i]);
	printf("\n");
}

// Main to test above functions
int main()
{
	int i,SIZE = 1000000;
	int *array = (int*) malloc(SIZE*sizeof(int));

	for(i=0; i < SIZE; i++)
		array[i] = rand()%SIZE;


	double time1= omp_get_wtime();

	#pragma omp parallel num_threads(64) default(none) shared(array,SIZE)
	{
		//printf("Numero de threads= %d\n", omp_get_num_threads());
		#pragma omp single nowait
		{
		quickSort(array, 0, SIZE-1);}

	}

	// Runtime of the parallel quicksort section.
	printf("time= %f\n", omp_get_wtime()-time1);

	// Testing the proper execution of quicksort.
	for(i=0;i<SIZE-1;i++){
		if(array[i]>array[i+1]){
			printf("array[%d] = %d, array[%d]= %d\n",i, array[i], (i+1), array[i+1] );
			printf("Not working\n");
      return -1;
		}
	}

	/* printf("Sorted array: \n"); */
	/* printArray(array, SIZE); */
	free(array);
	return 0;
}
