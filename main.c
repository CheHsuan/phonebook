#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include<unistd.h>

#include IMPL

#define DICT_FILE "./dictionary/words.txt"
static void addnull(char *line)
{
	int i = 0;
	while (line[i] != '\0')	
		i++;
	line[i - 1] = '\0';
	i = 0;	
}
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

int main(int argc, char *argv[])
{
    printf("pid : %d\n",getpid());
//    sleep(10);
    FILE *fp;
    char line[MAX_LAST_NAME_SIZE];
    struct timespec start, end;
    double cpu_time1, cpu_time2;
    /* check file opening */
    fp = fopen(DICT_FILE, "r");
    if (fp == NULL) {
        printf("cannot open the file\n");
        return -1;
    }

    /* build the entry */
#if defined(_PHONEBOOK_OPT_H)
    HashTable ht[HASH_TABLE_SIZE];
#else
    entry *pHead, *e;
    pHead = (entry *) malloc(sizeof(entry));
    printf("size of entry : %lu bytes\n", sizeof(entry));
    e = pHead;
    e->pNext = NULL;
#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
#endif
    clock_gettime(CLOCK_REALTIME, &start);
    while (fgets(line, sizeof(line), fp)) {
#if defined(_PHONEBOOK_OPT_H)
		append(line, hashfunction(line, ht, 0));
#else
		addnull(line);
        e = append(line, e);
#endif
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);
    /* close file as soon as possible */
    fclose(fp);

    /* the givn last name to find */
    char input[MAX_LAST_NAME_SIZE] = "zyxel";
    /* compute the execution time */
    clock_gettime(CLOCK_REALTIME, &start);
#if defined(_PHONEBOOK_OPT_H)
    if(findName(input, hashfunction(input, ht, 1)) != NULL)
	printf("finded\n");
#else 
    e = pHead;
    assert(findName(input, e) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, e)->lastName, "zyxel"));

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    if(findName(input, e) != NULL)
	printf("finded\n");
#endif
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time2 = diff_in_second(start, end);

    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %lf sec\n", cpu_time2);
#if defined(_PHONEBOOK_OPT_H) != 1
    /* FIXME: release all allocated entries */
    free(pHead);
#endif

    return 0;
}
