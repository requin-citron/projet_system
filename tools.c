
#include "tools.h"

void ask(const char *intro, char *str) {
    printf("%s",intro);
    fgets(str,SIZE_IN,stdin);
    fflush(stdin);
}

int isWord(const char *word, const char *str) {
    char tmp[SIZE_IN];
    snprintf(tmp,SIZE_IN,"%c\n",word[0]);
    if (!strcmp(str,tmp))
        return 1;
    snprintf(tmp,SIZE_IN,"%c\n",word[0]+'A'-'a');
    if (!strcmp(str,tmp))
        return 1;
    snprintf(tmp,SIZE_IN,"%s\n",word);
    if (!strcmp(str,tmp))
        return 1;
    tmp[0]+='A'-'a';
    if (!strcmp(str,tmp))
        return 1;
    return 0;
}

void color(char *name, const char *deb, const char *fin) {
    char tmp[SIZE_COM];
    strcpy(tmp,name);
    snprintf(name,SIZE_COM,"%s%s%s",deb,tmp,fin);
}
