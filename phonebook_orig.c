#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "phonebook_orig.h"

/* original version */
entry *findName(char lastname[], entry *pHead)
{
    while (pHead != NULL) {
        if (strcasecmp(lastname, pHead->lastName) == 0)
            return pHead;
        pHead = pHead->pNext;
    }
    return NULL;
}

entry *append(char lastName[], entry *e)
{
    /* allocate memory for the new entry and put lastName */
    e->pNext = (entry *) malloc(sizeof(entry));
    e = e->pNext;
    addnull(lastName);
    strcpy(e->lastName, lastName);
    e->pNext = NULL;

    return e;
}

entry *initialize(entry *pHead)
{
    pHead = (entry*) malloc(sizeof(entry));
    pHead->pNext = NULL;
    return pHead;
}

//replace \n with \0
void addnull(char *line)
{
    int i = 0;
    while (line[i] != '\0')
        i++;
    line[i - 1] = '\0';
    return;
}
