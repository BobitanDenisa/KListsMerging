/* Bobitan Denisa-Ioana

   Task:

   Implement efficiently k lists merging.

   Observations:

   The algorithm has complexity O(nlgk), where k is the number of lists and n the total number of elements.

   When k is variable and n fixed, the graph presents a logarithmic line. The complexity is ~ O(lgk), n a constant.
   When n is variable and k fixed, the graph presents for all 3 k's a polynomial complexity ~ O(n), k a constant.

*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Profiler.h"

#pragma warning(disable : 4996)

#define MAX_SIZE 10000

FILE *f = fopen("output.txt", "w");

Profiler profiler("DemoLAB4");
int len = 0;

int assignK5, assignK10, assignK100;
int compK5, compK10, compK100;

int assign, comp;

typedef struct nod
{
	int key;
	struct nod *next;
} Node;

typedef struct
{
	int len;
	Node *first;
	Node *last;
} List; //doubly linked list

int left(int i)
{
	return 2 * i + 1;
}

int right(int i)
{
	return 2 * i + 2;
}

void swapElements(int *a, int i, int j) //swaps elements from position i and j in array a
{
	int aux = a[i];
	a[i] = a[j];
	a[j] = aux;
}

void printList(List *l) //function to display the elements of a list
{
	Node *walker = l->first;
	while (walker != NULL)
	{
		fprintf(f, "%d ", walker->key);
		walker = walker->next;
	}
	fprintf(f, "\n");
}

void heapify(List **l, int *a, int n, int i)
{
	int s = left(i);
	int d = right(i);
	int min;
	comp++;
	if ((s < n) && (l[a[s]]->first->key < l[a[i]]->first->key))
	{
		min = s;
	}
	else
	{
		min = i;
	}
	comp++;
	if ((d < n) && (l[a[d]]->first->key < l[a[min]]->first->key))
	{
		min = d;
	}
	if (min != i)
	{
		swapElements(a, i, min);
		assign += 3;
		heapify(l, a, n, min);
	}
}

void buildMinHeap(List **l, int *a, int n) //bottom up
{
	for (int i = n / 2 - 1; i >= 0; i--)
	{
		heapify(l, a, n, i);
	}
}

List *initList(int size)
{
	List *l = (List*)malloc(size * sizeof(List));
	l->first = l->last = NULL;
	l->len = 0;
	return l;
}

Node *newNode(int key)
{
	Node *p = (Node*)malloc(sizeof(Node));
	p->key = key;
	p->next = NULL;
	return p;
}

void insertElementInList(List *l, int key) //insert element at the back of the list
{
	Node *nd = newNode(key);
	if (l->len == 0)
	{
		l->first = nd;
		l->last = nd;
	}
	else
	{
		l->last->next = nd;
		l->last = nd;
	}
	l->len++;
}

void deleteFirstFromList(List **l) //delete first element from the list
{
	if ((*l)->len > 0)
	{
		(*l)->first = (*l)->first->next;
		(*l)->len--;
	}
	if ((*l)->len == 0)
	{
		(*l)->first = (*l)->last = NULL;
	}
}

List **generateArrays(int k, int n)
{
	int len;
	int *a = (int*)malloc(n * sizeof(int));

	List **l = (List**)malloc(k * sizeof(List*));

	for (int i = 0; i < k; i++)
	{
		//fill the k arrays with random data, ordered ascendingly
		if (i < n % k) //n mod k lists with more elements than the others
		{
			len = n / k + 1;
			FillRandomArray(a, len, 10, 100, true, 1);
		}
		else //lists with n div k elements
		{
			len = n / k;
			FillRandomArray(a, len, 10, 100, true, 1);
		}
		l[i] = initList(len);
		for (int j = 0; j < len; j++)
		{
			insertElementInList(l[i], a[j]);
		}
		printList(l[i]);
	}
	return l;
}

int *buildHeapArray(List **l, int k) //k is the number of lists
{ 
	int *a = (int*)malloc(k*sizeof(int));
	for (int i = 0; i < k; i++) //for each list of elements we take the element on position index
	{
			a[i] = i; //populate array a with the indexes of the lists the elements belong to
			assign++;
	}
	return a;
}

void deleteElementFromArray(int *a, int *n) //put the last element first, then decrease the length of array
{
	a[0] = a[(*n) - 1]; //move the last element on the first position
	assign++;
	(*n)--; //decrement the length of the array
}

void mergeLists(int k, int n)
{
	List **l = generateArrays(k, n);
	List *final = initList(n);
	int *a; //contains indexes of the lists each element belongs to
	a = buildHeapArray(l, k);
	buildMinHeap(l, a, k);
	int i = 0;
	while (i < n) //while there are still elements left in lists
	{
		insertElementInList(final, l[a[0]]->first->key); //insert minimum in the final list
		deleteFirstFromList(&(l[a[0]])); //delete first element from list a[0]
		if (l[a[0]]->first == NULL) //if there are elements in the list a[0]
		{
			deleteElementFromArray(a, &k); //if not, delete the position from the heap array
		}
		heapify(l, a, k, 0);
		i++;
	}
	printList(final); //uncomment only when test function is called in main
}

void testMergeKLists()
{
	int k, n;
	printf("Number of lists: ");
	scanf("%d", &k);
	printf("Number of elements: ");
	scanf("%d", &n);
	mergeLists(k, n);
	
	fclose(f);
}

void profilerReport()
{
	for (int i = 10; i <= 500; i += 10)
	{
		assignK5 = assignK10 = assignK100 = compK5 = compK10 = compK100 = assign = comp = 0;
		mergeLists(i, MAX_SIZE);
		
		profiler.countOperation("Total_asign", i, assign + comp);
	}

	profiler.createGroup("TotalOperationsKVariabil", "Total_asign");

	for (int n = 100; n < MAX_SIZE; n += 100)
	{
		assignK5 = assignK10 = assignK100 = compK5 = compK10 = compK100 = assign = comp = 0;
		
		mergeLists(5, n);
		assignK5 = assign;
		compK5 = comp;

		assign = comp = 0;
		mergeLists(10, n);
		assignK10 = assign;
		compK10 = comp;

		assign = comp = 0;
		mergeLists(100, n);
		assignK100 = assign;
		compK100 = comp;

		profiler.countOperation("k=5", n, assignK5 + compK5);
		profiler.countOperation("k=10", n, assignK10 + compK10);
		profiler.countOperation("k=100", n, assignK100 + compK100);
	}

	profiler.createGroup("TotalOperationsNVariable", "k=5", "k=10", "k=100");
	profiler.showReport();
}

int main()
{
	//testMergeKLists();

	profilerReport();

	return 0;
}