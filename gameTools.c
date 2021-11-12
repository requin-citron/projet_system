
#include "gameTools.h"

void initListCards(int* allCards) {
    for (int t=0; t<100; t++)
        allCards[t] = t+1;
}
void pickCards(int* allCards, int* cards) {
    int nb=0;
    while (nb<NB_CARD) {
        int pick = rand()%100;
        if (allCards[pick]==-1)
            continue;
        cards[nb++] = allCards[pick];
        allCards[pick] = -1;
    }
    sort(cards);
}
void printCards(int* cards) {
    int pos = cardsNotEmpty(cards);
    if (pos==-1) return;
    printf("[%d",cards[pos]);
    while (++pos<NB_CARD)
        if (cards[pos]!=-1)
            printf(",%d",cards[pos]);
    printf("]");
}
int cardsNotEmpty(int *cards) {
    for (int t=0; t<NB_CARD; t++)
        if (cards[t]!=-1)
            return t;
    return -1;
}
void sort(int *cards) {
    for (int t=0; t<NB_CARD-1; t++)
        for (int h=t+1; h<NB_CARD; h++)
            if (cards[t]>cards[h]) {
                int tmp = cards[t];
                cards[t] = cards[h];
                cards[h] = tmp;
            }
}
int playCard(int *cards) {
    int pos = cardsNotEmpty(cards);
    if (pos==-1) return -1;
    int tmp = cards[pos];
    cards[pos] = -1;
    return tmp;
}
