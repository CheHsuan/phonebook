#ifndef _PHONEBOOK_OPT_H
#define _PHONEBOOK_OPT_H

#define MAX_LAST_NAME_SIZE 16
#define HASH_TABLE_SIZE 42737

#include <stdint.h>
typedef struct __PHONE_BOOK_ENTRY {
    char lastName[MAX_LAST_NAME_SIZE];
    char firstName[16];
    char email[16];
    char phone[10];
    char cell[10];
    char addr1[16];
    char addr2[16];
    char city[16];
    char state[2];
    char zip[5];
    struct __PHONE_BOOK_ENTRY *pNext;
} entry;

typedef struct HashTable {
    int flag;
    struct __PHONE_BOOK_ENTRY *head;
    struct __PHONE_BOOK_ENTRY *tail;
} HashTable;

entry *findName(char lastname[], entry *e);
entry *append(char lastName[], HashTable *ht);
uint16_t hashIndex(char lastName[]);

#endif
