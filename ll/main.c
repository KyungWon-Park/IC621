#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

struct list_node
{
	pthread_mutex_t mutex;
	int data;
	struct list_node *next;
}; 

struct list_node *ptr_head;

int List_Member(int lookup_data)
{
	int found = 0;
	pthread_mutex_lock(&(*ptr_head).mutex);
	struct list_node *curr = ptr_head;
	while (curr != NULL && (*curr).data < lookup_data) 
	{
		pthread_mutex_lock(&(*(*curr).next).mutex);
		pthread_mutex_unlock(&(*curr).mutex);
		curr = (*curr).next;
	}

	if (curr != NULL && (*curr).data == lookup_data)
	{	found = 1;	}
	pthread_mutex_unlock(&(*curr).mutex);

	return found;
}

void List_Print(void)
{
	struct list_node *curr = ptr_head;
	printf("\n\nPrinting list... \n\n");
	while (curr != NULL)
	{
		printf("%d\n", (*curr).data);
		curr = (*curr).next;
	}

	return;
}

void *Worker(void *rank);

void List_Insert(int inserted_data) 
{
	pthread_mutex_lock(&(*ptr_head).mutex);	// Lock current
	struct list_node *curr = ptr_head;
	struct list_node *prev = NULL;
	while (1)
	{
		if ((*curr).next != NULL) 
		{
			pthread_mutex_lock(&(*(*curr).next).mutex);
			if ((*(*curr).next).data < inserted_data)
			{
				prev = curr;
				curr = (*curr).next;
				pthread_mutex_unlock(&(*prev).mutex);
			}
			else 
			{
				pthread_mutex_unlock(&(*(*curr).next).mutex);
				break;
			}
		}
		else 
		{
			break;	// Escape with desired node in curr
		}
	}
	// WARNING: MALLOC 1
	struct list_node *inserted = malloc(sizeof(struct list_node));
	assert(inserted != NULL && "Malloc failed!\n");

	pthread_mutex_init(&(*inserted).mutex, NULL);
	(*inserted).data = inserted_data;
	(*inserted).next = (*curr).next;
	(*curr).next = inserted;
	pthread_mutex_unlock(&(*curr).mutex);
	return;
}

int main(void)
{
	// Generate a list data structure
	// WARNING: MALLOC 0
	ptr_head = malloc(sizeof(struct list_node));
	assert(ptr_head != NULL && "Malloc failed!");
	// Initialize 
	pthread_mutex_init(&(*ptr_head).mutex, NULL);
	(*ptr_head).data = 0;
	(*ptr_head).next = NULL;

	// Threads
	long threads = 4;
	pthread_t thread_handles[4];

	for (long thread = 0; thread < threads; thread++)
	{
		pthread_create(&thread_handles[thread], NULL, Worker, (void *) thread);
	}

	
	for (long thread = 0; thread < threads; thread++)
	{
		pthread_join(thread_handles[thread], NULL);
	}

	List_Print();

	return 0;
}

void *Worker(void *rank) 
{
	long my_rank = (long) rank;
	int i = my_rank + 1;
	for(;i < 101;i = i + 4)
	{
		List_Insert(i);
		printf("Worker %ld inserted %d in list\n", my_rank, i); 
	}

	return NULL;
}
