#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

typedef struct
{
	int Id_Thread;
	int fromidx, length;
	int *nums;
} thread_arg, *ptr_thread_arg;


void *thread_func(void *arg) 
{
	int i, endidx;
	long long localsum = 0;
   	ptr_thread_arg argument = (ptr_thread_arg)arg;
	long long *ret = malloc(sizeof(long long));

	endidx = argument->fromidx + argument->length;
	for(i=argument->fromidx; i < endidx; i++) 
		localsum += argument->nums[i];
	printf("Thread %d trabalhou e conseguiu %lld\n" ,  argument->Id_Thread , localsum);
	*ret = localsum;
	pthread_exit((void*)ret);
}

int main(int argc, char **argv) 
{
	int i, length, remainder, *nums;
	unsigned int VETSIZE , NUMTHREADS;
	long long sum = 0;
    clock_t start, end;

	VETSIZE = atoi(argv[1]);
	NUMTHREADS = atoi(argv[2]);

		
	pthread_t *threads;
	thread_arg *arguments;
	
	threads = malloc(sizeof(pthread_t) * NUMTHREADS);
	arguments = malloc(sizeof(thread_arg) * NUMTHREADS);
	

	nums = malloc(sizeof(int) * VETSIZE);
	length = VETSIZE / NUMTHREADS;
	remainder = VETSIZE % NUMTHREADS;
	
	for (i = 0 ; i < VETSIZE ; i++)
    {
		nums[i] = i;
	}
    
	long long somacorreta = 0;
    start = clock();// tempo inicial
	for (i = 0 ; i < VETSIZE ; i++)
	{
		somacorreta += nums[i];
	}
    end = clock();// tempo inicial
    printf ( "tempo gasto %f milisegundos\n", ((double)( end - start ) / ((double)CLOCKS_PER_SEC ))); //impress�o do tempo gasto
    printf("teste A soma CORRETA dos numeros do vetor eh %lld\n", somacorreta);

    for(i=0; i < NUMTHREADS; i++) 
	{
		 arguments[i].Id_Thread = i;
		 arguments[i].fromidx = i * length;
		 arguments[i].length = length;
		 arguments[i].nums = nums;
		 if (i == (NUMTHREADS - 1)) 
			 arguments[i].length += remainder;
    }
    
    
    start = clock();// tempo inicial
	for(i=0; i < NUMTHREADS; i++) 
	{
		 pthread_create(threads+i, NULL, thread_func,	arguments+i);
	}
	for(i=0; i<NUMTHREADS; i++) 
	{
		void *vid;
		long long *tmp1;
		pthread_join(threads[i], &vid);
		tmp1 = (long long *)vid;
		sum += *tmp1;
		free(vid);
	}
    end = clock();// tempo inicial
    printf ( "tempo gasto %f milisegundos\n", ((double)( end - start ) / ((double)CLOCKS_PER_SEC ))); //impress�o do tempo gasto
    printf("teste A soma dos numeros do vetor com threads eh %lld\n", sum);
	
	
	free(nums);
	free(threads);
	free(arguments);

	return 0;
}