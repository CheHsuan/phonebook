#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "phonebook_opt.h"

/* FILL YOUR OWN IMPLEMENTATION HERE! */
entry *findName(char lastName[], entry *e)
{
    int i = 0;
    while (e != NULL) {
    	if (strcasecmp(lastName, e->lastName) == 0){
	    printf("%d\t%s\n",i,lastName);
	    return e;
	}
        e = e->pNext;
	i++;
    }
    return NULL;
}

entry *append(char lastName[], entry *e)
{
    e->pNext = (entry *) malloc(sizeof(entry));
    e = e->pNext;
    strcpy(e->lastName, lastName);
    e->pNext = NULL;
    return e;
}

int hashIndex(char *lastName){
	unsigned int hashValue  = 1;
        while(*lastName != '\0'){
                hashValue = (hashValue) + ( *lastName++);
		//hashValue = (hashValue << 5 )  + *lastName++;
		}
	return hashValue % HASH_TABLE_SIZE; 
}

entry *hashfunction(char *lastName, HashTable *ht,int flag){
	int index = hashIndex(lastName);
	if(flag == 0){
		if(ht[index].flag == 0){
			ht[index].head = (entry *)malloc(sizeof(entry));
			ht[index].head->pNext = NULL;
			ht[index].tail = ht[index].head;
			ht[index].flag = 1;
		}
        	return ht[index].tail;
	}
	else 
		return ht[index].head;
} 	
