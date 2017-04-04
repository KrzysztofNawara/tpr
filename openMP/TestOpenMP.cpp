// TestOpenMP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <omp.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <ctime>

using namespace std;

struct predicate {
	bool operator() (int i, int j) { return (i<j); }
} pred;

//Prints array
// Parameters:
// data - array of integers to search from
// count - elements in data array
static void print_array(int *data, int count) {
	printf("[");
	for (int i = 0; i < count; i++)
		if (i != count - 1) {
			printf("%d, ", data[i]);
		}
		else {
			printf("%d", data[i]);
		}

		printf("]");
}

// Finds minimum and maximum value in data array
// Parameters:
// data - array of integers to search from
// count - elements in data array
// Returns:
// minMax[0] - minium value
// minMax[1] - maximum value
static int* find_min_max(int *data, int count) {
	int minMax[2] = { data[0], data[0] };

	for (int i = 1; i < count; i++)
	{
		if (data[i] > minMax[1])
			minMax[1] = data[i];
		if (data[i] < minMax[0])
			minMax[0] = data[i];
	}
	return minMax;
}

static void bucket_sort(int* data, int count) {
	int maxValue = find_min_max(data, count)[1];

	int bucketCount = count;
	vector<int>* bucket = new vector<int>[bucketCount];

	// Creates all buckets
	#pragma omp parallel for
	for (int i = 0; i < bucketCount; i++)
	{
		bucket[i] = vector<int>();
	}

	// Populates buckets with data
	for (int i = 0; i < count; i++)
	{
		int selectedBucket = (data[i] * count) / (maxValue + 1);
		bucket[selectedBucket].push_back(data[i]);
	}

	#pragma omp parallel for
	// Sort all buckets
	for (int i = 0; i < bucketCount; i++)
	{
		sort(bucket[i].begin(), bucket[i].end(), pred);
	}

	// Merges buckets to array
	int insertedElements = 0;
	for (int i = 0; i < bucketCount; i++)
	{
		int bucketSize = bucket[i].size();
		if (bucketSize > 0)
		{
			// Copy all values from bucket to array
			for (int j = 0; j < bucketSize; j++)
			{
				data[insertedElements] = bucket[i][j];
				insertedElements++;
			}
		}
	}
}


int* generate_random_array(int size, int from, int to) {

	std::random_device rd;
	std:mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(from, to);

	int *arr = new int[size];
	for (int i = 0; i < size; i++) {
		arr[i] = uni(rng);
	}
	return arr;
}


static const int ARRAY_SIZE = 1000000;
int main()
{
	int *unsorted = generate_random_array(ARRAY_SIZE, 1, 1000);

	//print_array(unsorted, ARRAY_SIZE);
	//printf("\n");

	const clock_t begin_time = clock();
	bucket_sort(unsorted, ARRAY_SIZE);
	const clock_t end_time = clock();
	printf("Sorted %d elements in %f seconds\n", ARRAY_SIZE, (float (end_time - begin_time))/1000);
	
	//print_array(unsorted, ARRAY_SIZE);

	getchar();
	return 0;
}

