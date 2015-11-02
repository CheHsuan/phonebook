#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "smaz.h"
#include "phonebook_opt.h"

/* compress lastName version */
entry *findName(char lastname[], entry *pHead)
{
	char dest[100];
	int decomprlen = 0;
    while (pHead != NULL) {
		decomprlen = smaz_decompress(pHead->comprData,pHead->comprlen,dest,sizeof(dest));
		dest[decomprlen] = '\0';
		if (strcasecmp(lastname, dest) == 0)
		  	return pHead;
        pHead = pHead->pNext;
    }
    return NULL;
}

entry *append(char lastName[], entry *e)
{
	char out[4096];
	char d[4096];
	int comprlen,decomprlen;
	int length;
    /* allocate memory for the new entry and put lastName */
    e->pNext = (entry *) malloc(sizeof(entry));
    e = e->pNext;
    length = addnull(lastName);
	/* compress the lastName */
	comprlen = smaz_compress(lastName,length,out,sizeof(out));
	assert(length != (unsigned)(decomprlen = smaz_decompress(out,comprlen,d,sizeof(d) && "Error compressing")));
	assert(memcmp(lastName,d,decomprlen));
	e->comprlen = comprlen;
	e->comprData = malloc(comprlen * sizeof(char));
	strcpy(e->comprData,out); 
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
int addnull(char *line)
{
    int i = 0;
    while (line[i] != '\0')
        i++;
    line[i - 1] = '\0';
    return i-1;
}
