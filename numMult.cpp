#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

//Global Constants
const int MAX_SIZE = 256;

//Global Variables
int sizeNum1;
int sizeNum2;
int in1 = 0;

int numThreads;
unsigned short int num1 [MAX_SIZE];
unsigned short int num2 [MAX_SIZE];
unsigned short int finalNum [MAX_SIZE*2];

//mutex for setting up multiplication
sem_t initial_loc_sem;
//mutex for accessing the finalNumber
sem_t result_sem;
//array of thread
pthread_t* tid;

//stores the number in the num  array
//returns size of the number read in
int readNumber(unsigned short int* num, FILE* fp)
{
 char c = getc(fp);
 int count = 0;
 while(c != '\n' && c != EOF)
 {
       	num[count++] = (unsigned short int)c - '0';
	c = getc(fp);
 }
 return count;
}

void* multiply(void *)
{
//represents the solo digit from num2 that will be multiplied along m1
int localNumber;
//represents the location to add to the final num
int local_in_one;

//get mutex on intializing numbers
sem_wait(&initial_loc_sem);

while(sizeNum2 > 0)
{
	//get number to multiply and location
	sizeNum2--;
	localNumber = num2[sizeNum2];
	local_in_one = in1;
	in1++;
	//release mutex on initializing numbers
	sem_post(&initial_loc_sem);
	
	int carry = 0;
	int in2 = 0;
	
	int i;
	for(int i = sizeNum1 - 1; i >= 0; i--)
	{
		//wait to access finalNum
		sem_wait(&result_sem);
		int multNum = num1[i];
		int sum = multNum * localNumber + finalNum[local_in_one + in2] + carry; 
		finalNum[local_in_one+in2] = sum%10;
		sem_post(&result_sem);
		//release lock on finalNum
		carry = sum/10;
		in2++;
	}
	
	//add carry if necessary
	if(carry > 0)
	{
		sem_wait(&result_sem);
		finalNum[local_in_one + in2] += carry; 	
		sem_post(&result_sem);
	}
	//wait to get the lock on intialzing
	sem_wait(&initial_loc_sem);
}
//release lock on intializing
sem_post(&initial_loc_sem); 
return NULL;                                                                     }                                                                                                                                                                                
void outputNumbers()
{
 FILE* output;
 output = fopen("output.txt", "w");
 int i;
 bool start = false;
 for(i = MAX_SIZE*2-1; i >= 0; i--)
 {
    int c = finalNum[i];
    if(!start && c != 0)
    	start = true;
    if(start)
    	fprintf(output, "%d", c);
 }
 fclose(output);
}

int main()
{
 FILE* input;
 input = fopen("input.txt","r");
 //read in number of threads
 fscanf(input, "%d", &numThreads);
 //discard newline
 getc(input);
 
 //read in firt number
 sizeNum1 = readNumber(num1, input);
  //read in second number
 sizeNum2 = readNumber(num2, input);

 //use only the required number of threads
 if(numThreads > sizeNum2)
 {
 	numThreads = sizeNum2;
 }

 pthread_t array[numThreads];
 tid = array;

 int i;
 sem_init(&initial_loc_sem, 0, 1);
 sem_init(&result_sem, 0, 1);
 //create threads
 for( i = 0; i < numThreads; i++ )
 	pthread_create(&tid[ i ], NULL, multiply, NULL);
 
 for( i = 0; i < numThreads; i++ ) 
	pthread_join( tid[ i ], NULL );
 
 outputNumbers();

 fclose(input);
 return 0;
}
