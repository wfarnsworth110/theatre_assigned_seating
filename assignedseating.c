//Programming Assignment 2
//Walker Farnsworth
//wa934080

#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#define INITSIZE 10
#define MAXLEN 51
#define MAXROWS 100000

typedef struct order{
	
	int s_seat;
	int e_seat;
	char * name;
	
}order;

typedef struct theaterrow{
	
	order ** list_orders;
	int max_size;
	int cur_size;
	
}theaterrow;

order * make_order(int start, int end, char * this_name);
theaterrow * make_empty_row();
int conflict(order * order1, order * order2);
int can_add_order(theaterrow * this_row, order * this_order); // Should run in linear time
int add_order(theaterrow * this_row, order * this_order); // Should run in linear time
char * get_owner(theaterrow ** theater, int row, int seat_num); // Should run in linear time
char * get_row_owner(theaterrow * this_row, int seat_num); // Should run in linear time
int contains(order * myorder, int seat_no);
void free_order(order * this_order);
void free_row(theaterrow * this_row);
void chomp(char word []);

int main()
{
	theaterrow ** amc = calloc(MAXROWS + 1, sizeof(theaterrow*)); // Given
	
	//fail-safe
	if(amc == NULL)
	{
		printf("calloc error in main...program exiting\n");
		exit(1);
	}
	
	// Initialize every theater row
	for(int i = 0; i < MAXROWS + 1; ++i)
		amc[i] = make_empty_row();
	
	int quitFlag = 1;
	
	do{
		char select[MAXLEN + INITSIZE] = {};
		char option[INITSIZE] = {};
		
		char buy1[INITSIZE] = {};
		char buy2[INITSIZE] = {};
		char buy3[INITSIZE] = {};
		int buyRow = 0;
		int buyStart = 0;
		int buyEnd = 0;
		char buyName[MAXLEN] = {};
		
		char lookup1[INITSIZE] = {};
		char lookup2[INITSIZE] = {};
		int lookupRow = 0;
		int lookupSeat = 0;
		char * name;
		
		int spaceCheck = 0;
		int checkPoint = 0;
		
		// User input
		fgets(select, MAXLEN, stdin);
		chomp(select);
		int inputSize = strlen(select);
		
		for(int i = 0; i < inputSize; ++i)
		{
			if(select[i] == ' ')
			{
				++spaceCheck;
				strncpy(option, select, i);
				break;
			}
		}
		
		if(spaceCheck == 0)
		{
			strcpy(option, select);
		}
		
		for(int i = 0; i < strlen(option); ++i)
		{
			option[i] = tolower(option[i]);
		}
		
		if(strcmp(option, "buy") == 0)
		{				
			// Collect row input
			spaceCheck = 0;
			checkPoint = strlen(option) + 1;
			
			for(int i = checkPoint; i < inputSize; ++i)
			{
				if(select[i] == ' ')
				{
					++spaceCheck;
					strncpy(buy1, &select[checkPoint], i - checkPoint);
					buyRow = atoi(buy1);
					break;
				}
			}
			
			if(spaceCheck == 0 || buyRow < 1 || buyRow > MAXROWS)
			{
				printf("Invalid input, please try again\n");
				continue;
			}
			
			// Collect start input
			spaceCheck = 0;
			checkPoint = strlen(option) + strlen(buy1) + 2;
			
			for(int i = checkPoint; i < inputSize; ++i)
			{
				if(select[i] == ' ')
				{
					++spaceCheck;
					strncpy(buy2, &select[checkPoint], i - checkPoint);
					buyStart = atoi(buy2);
					break;
				}
			}
			
			if(spaceCheck == 0 || buyStart < 1 || buyStart > MAXROWS)
			{
				printf("Invalid input, please try again\n");
				continue;
			}
			
			// Collect end input
			spaceCheck = 0;
			checkPoint = strlen(option) + strlen(buy1) + strlen(buy2) + 3;
			
			for(int i = checkPoint; i < inputSize; ++i)
			{
				if(select[i] == ' ')
				{
					++spaceCheck;
					strncpy(buy3, &select[checkPoint], i - checkPoint);
					buyEnd = atoi(buy3);
					break;
				}
			}
			
			if(spaceCheck == 0 || buyEnd < 1 || buyEnd < buyStart || buyEnd > MAXROWS)
			{
				printf("Invalid input, please try again\n");
				continue;
			}
			
			// Collect name input
			checkPoint = strlen(option) + strlen(buy1) + strlen(buy2) + strlen(buy3) + 4;
			strcpy(buyName, &select[checkPoint]);
			
			// Execute buy functions
			order * newOrder = make_order(buyStart, buyEnd, buyName);
			
			int canAddOrder = can_add_order(amc[buyRow], newOrder);
			if(canAddOrder == 0)
			{
				printf("FAILURE\n");
				continue;
				free_order(newOrder);
			}
			
			int addOrder = add_order(amc[buyRow], newOrder);
			if(addOrder == 0)
			{
				printf("FAILURE\n");
				continue;
				free_order(newOrder);
			}
			
			free_order(newOrder);
			
			printf("SUCCESS\n");
		}
		else if(strcmp(option, "lookup") == 0)
		{
			// Collect row input
			spaceCheck = 0;
			checkPoint = strlen(option) + 1;
			
			for(int i = checkPoint; i < inputSize; ++i)
			{
				if(select[i] == ' ')
				{
					++spaceCheck;
					strncpy(lookup1, &select[checkPoint], i - checkPoint);
					lookupRow = atoi(lookup1);
					break;
				}
			}
			
			if(spaceCheck == 0 || lookupRow < 1 || lookupRow > MAXROWS)
			{
				printf("Invalid input, please try again\n");
				continue;
			}
			
			// Collect seat number
			checkPoint = strlen(option) + strlen(lookup1) + 2;
			strcpy(lookup2, &select[checkPoint]);
			lookupSeat = atoi(lookup2);
			
			if(lookupSeat == 0 || lookupSeat > MAXROWS)
			{
				printf("Invalid input, please try again\n");
				continue;
			}
			
			// Execute lookup functions			
			name = get_owner(amc, lookupRow, lookupSeat);
			
			if(name != NULL)
				printf("%s\n", name);
			else
				printf("No one\n");
		}
		else if(strcmp(option, "quit") == 0)
		{
			// Free memory
			for(int i = 0; i < MAXROWS + 1; ++i)
			{	
				if(amc[i] != NULL)
					free_row(amc[i]);
			}
			
			free(amc);
			amc = NULL;
			quitFlag = 0;
		}
		else
		{
			printf("Invalid input, please try again\n");
		}
	}while(quitFlag);
	
	return 0;
}

order * make_order(int start, int end, char * this_name)
{
	/*
		Returns a pointer to a dynamically allocated order storing the given 
		start row, end row, and the name this_name. Note: strcpy should be 
		used to copy the contents into the struct after its name field is 
		dynamically allocated.
	*/
	
	order * newOrder = malloc(sizeof(order));
	
	//fail-safe
	if(newOrder == NULL)
	{
		printf("malloc error in make_order...program exiting\n");
		exit(1);
	}
	
	newOrder->s_seat = start;
	newOrder->e_seat = end;
	newOrder->name = malloc(sizeof(char) * MAXLEN);
	
	//fail-safe
	if(newOrder->name == NULL)
	{
		printf("malloc error in make_order...program exiting\n");
		exit(1);
	}
	
	strcpy(newOrder->name, this_name);
	
	return newOrder;
}

theaterrow * make_empty_row()
{
	/*
		This function will allocate the memory for one theaterrow, including 
		allocating its array of orders to a default maximum size of 10, and 
		setting its current size to 0.
	*/
	
	theaterrow * newTheaRow = malloc(sizeof(theaterrow));
	
	//fail-safe
	if(newTheaRow == NULL)
	{
		printf("malloc error in make_empty_row...program exiting\n");
		exit(1);
	}
	
	newTheaRow->list_orders = calloc(INITSIZE, sizeof(order*)); //was the previous allocation the issue?
	
	//fail-safe
	if(newTheaRow->list_orders == NULL)
	{
		printf("calloc error in make_empty_row...program exiting\n");
		exit(1);
	}
	
	newTheaRow->max_size = INITSIZE;
	newTheaRow->cur_size = 0;
	
	return newTheaRow;
}

int conflict(order * order1, order * order2)
{
	/*
		Assuming that order1 and order2 point to orders on the same row, this 
		function returns 1 if the orders conflict, meaning that they share 
		at least 1 seat in common, and returns 0 otherwise.
	*/
	
	// Order 1 will be the new order, order 2 will be the veteran row
	if(order1->s_seat >= order2->s_seat && order1->s_seat <= order2->e_seat)
		return 1;
	else if(order1->e_seat >= order2->s_seat && order1->e_seat <= order2->e_seat)
		return 1;
	else
		return 0;
}

int can_add_order(theaterrow * this_row, order * this_order) // Should run in linear time
{
	/*
		Returns 1 if the order pointed to buy this_order can be added to the 
		row pointed to by this_row. Namely, 1 is returned if and only if 
		this_order does NOT have any seats in it that are part of any order 
		already on this_row.
	*/
	
	if(this_row->cur_size == 0)
		return 1;
	
	for(int i = 0; i < this_row->cur_size; ++i)
	{
		if(conflict(this_order, this_row->list_orders[i]) == 1)
			return 0;
	}
	
	return 1;
}

// Changed from void to int to return 0 or 1
int add_order(theaterrow * this_row, order * this_order) // Should run in linear time
{
	/*
		This function tries to add this_order to this_row. If successful, 
		the order is added and 1 is returned. Otherwise, 0 is returned and 
		no change is made to this_row. If the memory for this_row is full, 
		this function will double the maximum # of orders allocated for the 
		row (via realloc), before adding this_order, and adjust max_size and 
		cur_size of this_row.
	*/
	if(this_row->cur_size == 100)
		return 0;
	
	if(this_row->cur_size == this_row->max_size)
	{
		int newMaxSize = this_row->max_size * 2;
		if(newMaxSize >= 100)
			newMaxSize = 100;
		this_row->list_orders = realloc(this_row->list_orders, sizeof(order*) * newMaxSize);
	}
	
	this_row->list_orders[this_row->cur_size] = make_order(this_order->s_seat, this_order->e_seat, this_order->name);
	
	this_row->cur_size++;
	
	return 1;
}

char * get_owner(theaterrow ** theater, int row, int seat_num) // Should run in linear time
{
	/*
		If seat_num seat number in row number row is occupied, return a 
		pointer to the owner’s name. Otherwise, return NULL.
	*/
	
	return get_row_owner(theater[row], seat_num);
}

char * get_row_owner(theaterrow * this_row, int seat_num) // Should run in linear time
{
	/*
		If seat_num in the row pointed to by this_row is occupied, return a 
		pointer to the string storing the owner’s name. If no one is sitting 
		in this seat, return NULL.
	*/
	
	for(int i = 0; i < this_row->cur_size; ++i)
		if(contains(this_row->list_orders[i], seat_num) == 1)
			return this_row->list_orders[i]->name;
	
	return NULL;
}

int contains(order * myorder, int seat_no)
{
	/*
		This function returns 1 if the seat number seat_no is contained in
		the range of seats pointed to by myorder, and returns 0 otherwise.
	*/
	
	if(myorder->s_seat <= seat_no && myorder->e_seat >= seat_no)
		return 1;
	else
		return 0;
}

void free_order(order * this_order)
{
	/*
		Frees all memory associated with this_order.
	*/
	
	free(this_order->name);
	free(this_order);
}

void free_row(theaterrow * this_row)
{
	/*
		Frees all memory associated with this_row.
	*/
	
	for(int i = 0; i < (this_row->cur_size); ++i)
	{
		if(this_row->list_orders[i] != NULL)
		{
			free_order(this_row->list_orders[i]);
		}
	}
	
	free(this_row->list_orders);
	free(this_row);
}

void chomp(char word [])
{
	if(word[strlen(word) - 1] == '\n')
		word[strlen(word) - 1] = '\0';
}