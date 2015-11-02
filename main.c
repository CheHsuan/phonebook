#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

#include IMPL

#define DICT_FILE "./dictionary/words.txt"
#define NUM_OF_THREADS 5

typedef struct thread_info {
	int id;
	entry *pHead;
	entry *e;
} thread_info;

pthread_mutex_t lock;
int readend = 0;
FILE *fp;

static double diff_in_second(struct timespec t1, struct timespec t2)
{
	struct timespec diff;
	if (t2.tv_nsec-t1.tv_nsec < 0) {
		diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
		diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
	} else {
		diff.tv_sec  = t2.tv_sec - t1.tv_sec;
		diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
	}
	return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

void reader(void *information)
{
	char buffer[MAX_LAST_NAME_SIZE];
	thread_info *info = (thread_info *)information;
	
	while(readend == 0) {
		pthread_mutex_lock(&(lock));
		if(fgets(buffer,MAX_LAST_NAME_SIZE,fp) == NULL)
			readend = 1;
		pthread_mutex_unlock(&(lock));
		info->e = append(buffer, info->e);
	}
	pthread_exit(0);
}

int main(int argc, char *argv[])
{
	struct timespec start, end;
	double cpu_time1, cpu_time2;
	thread_info info[NUM_OF_THREADS];
	pthread_t thread[NUM_OF_THREADS];
	entry *pHead = NULL;

	printf("size of entry : %lu bytes\n", sizeof(entry));	

	fp = fopen(DICT_FILE, "r");
	if (fp == NULL) {
		printf("cannot open the file\n");
		return -1;
	}

	//initialize the mutex
	pthread_mutex_init(&(lock), NULL);

#if defined(__GNUC__)
	__builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
	clock_gettime(CLOCK_REALTIME, &start);

	//create threads	
	for(int i = 0; i < NUM_OF_THREADS; i++) {
		info[i].id = i + 1;
		info[i].pHead = initialize(info[i].pHead);
		info[i].e = info[i].pHead;
		pthread_create(&thread[i], NULL, (void *)reader, (void *)&info[i]);
	}
	for(int i = 0; i < NUM_OF_THREADS; i++)
		pthread_join(thread[i], NULL);
	
	//combine linked-lists to a list
	for(int i = 0; i< NUM_OF_THREADS-1; i++){
		info[i].e->pNext = info[i+1].pHead;
	}
	pHead = info[0].pHead;

	clock_gettime(CLOCK_REALTIME, &end);
	cpu_time1 = diff_in_second(start, end);
	/* the given last name to find */
	char input[MAX_LAST_NAME_SIZE] = "zyxel";
	/* compute the execution time */
	clock_gettime(CLOCK_REALTIME, &start);
#if defined(__GNUC__)
	__builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
	findName(input, pHead);

	clock_gettime(CLOCK_REALTIME, &end);
	cpu_time2 = diff_in_second(start, end);

	printf("execution time of append() : %lf sec\n", cpu_time1);
	printf("execution time of findName() : %lf sec\n", cpu_time2);

	fclose(fp);	
	free(pHead);	
	return 0;
}
