#ifndef GELASIA_COMPACT_H
#define GELASIA_COMPACT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

typedef struct _gelasia_compacter gelasia_compacter;

int gelasia_compacter_add_number(gelasia_compacter *comp, long long number);
unsigned char *gelasia_compacter_get_sequence(gelasia_compacter *comp, int *size_pointer);
int gelasia_compacter_get_number_count(gelasia_compacter *comp);

gelasia_compacter *new_gelasia_compacter(int with_sign);
void delete_gelasia_compacter(gelasia_compacter *comp);

typedef struct _gelasia_reader gelasia_reader;

gelasia_reader *new_gelasia_reader(int with_sign, int numbers);
int gelasia_reader_feed(gelasia_reader *readr, unsigned char *buffer, int size_receibed, int *readed);
long long *delete_gelasia_reader_and_get_numbers(gelasia_reader *readr);

void gelasia_compacter_show(gelasia_compacter *comp);

#endif