#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "phonebook_opt.h"

/* FILL YOUR OWN IMPLEMENTATION HERE! */
entry *findName(char lastName[], entry *e)
{
    while (e != NULL) {
        if (strcasecmp(lastName, e->lastName) == 0) {
            return e;
        }
        e = e->pNext;
    }

    return NULL;
}

entry *append(char lastName[], HashTable *ht)
{
    int index = hashIndex(lastName);
    if(ht[index].flag == 0) {
        ht[index].head = (entry *)malloc(sizeof(entry));
        strcpy(ht[index].head->lastName,lastName);
        ht[index].head->pNext = NULL;
        ht[index].tail = ht[index].head;
        ht[index].flag = 1;
        return ht[index].head;
    } else {
        ht[index].tail->pNext = (entry *) malloc(sizeof(entry));
        ht[index].tail = ht[index].tail->pNext;
        strcpy(ht[index].tail->lastName, lastName);
        ht[index].tail->pNext = NULL;
        return ht[index].tail;
    }
}

uint16_t hashIndex(char *lastName)
{
    unsigned int hashValue  = 1;
    while(*lastName != '\0') {
        if(*lastName == '\n') {
            *lastName = '\0';
            lastName++;
            continue;
        }
        hashValue = (hashValue) + ( *lastName++);
        //hashValue = (hashValue << 5 )  + *lastName++;
    }
    return hashValue % HASH_TABLE_SIZE;
}

