
#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE_IN 50
#define PORT 23
#define SIZE_COM 500

#define NB_CARD 10

#define RED "\033[37;41m"
#define GREEN "\033[37;42m"
#define BLUE "\033[30;44m"
#define FALLBACK "\033[39;49m"

void ask(const char*,char*);
int isWord(const char*,const char*);
void color(char*,const char*,const char*);

#endif
