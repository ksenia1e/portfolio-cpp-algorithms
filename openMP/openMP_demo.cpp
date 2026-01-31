#include "stdafx.h"

void max_threads()
{
	cout << "Max numbers of threads " << omp_get_max_threads();
}

int num_threads(int quantity)
{
	quantity = omp_get_num_threads();
	return quantity;
}

void HelloWorld()
{
	int quantity = 0;
	int rank = 0;
#pragma omp parallel
	{
		if ( omp_get_thread_num() == 0 ) quantity = num_threads(quantity);
		rank = omp_get_thread_num();
		printf_s("\nHello World %d", rank);
	}
	cout << "\nNumber of threads in parallel section " << quantity << endl;
}

void cycle(int arr[])
{
	int rank = 0;
	int Max_treads = omp_get_max_threads();
	cout << "\nCycle without schedule";
#pragma omp parallel num_threads(Max_treads)
	{
		#pragma omp for
			for ( int i = 0; i < 20; i++ )
			{
				rank = omp_get_thread_num();
				arr[i] = rank;
				printf_s("\n %d - %d", rank, i);
			}
	}
	
}

void cycle_static(int arr[])
{
	int rank = 0;
	int Max_treads = omp_get_max_threads();
	cout << "\nCycle with static schedule";
#pragma omp parallel num_threads(Max_treads)
	{
#pragma omp for schedule(static, 10)
		for ( int i = 0; i < 20; i++ )
		{
			rank = omp_get_thread_num();
			arr[i] = rank;
			printf_s("\n %d - %d", rank, i);
		}
	}

}

void cycle_dynamic(int arr[])
{
	int rank = 0;
	int Max_treads = omp_get_max_threads();
	cout << "\nCycle with dynamic schedule";
#pragma omp parallel num_threads(Max_treads)
	{
#pragma omp for schedule(dynamic, 10)
		for ( int i = 0; i < 20; i++ )
		{
			rank = omp_get_thread_num();
			arr[i] = rank;
			printf_s("\n %d - %d", rank, i);
		}
	}
}

void Print(int arr[])
{
	printf_s("\n \n Array of threads");
	int Max_treads = omp_get_max_threads();
#pragma omp parallel num_threads(Max_treads)
	{
#pragma omp for 
		for ( int i = 0; i < 20; i++ )
		{
			printf_s("\n %d - %d", arr[i], i);
		}
	}
	printf_s("\n ");
}

int main()
{
	int arr[20] = { 0 };
	max_threads();
	HelloWorld();
	cycle(arr);
	Print(arr);
	cycle_static(arr);
	Print(arr);
	cycle_dynamic(arr);
	Print(arr);
}