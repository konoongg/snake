#include <stdlib.h>
#include <stdio.h>

struct TListNode {
    int X;
    int Y;
    struct TListNode* Next;
    struct TListNode* Last;
};


struct TList {
    struct TListNode* Head;
    struct TListNode* Tail;
    int LastMove;
    int Size;
};

void Crawl(struct TList* list, int moves, int* full) {
    struct TListNode* current = list->Head;
    list->Head = (struct TListNode*)malloc(sizeof(struct TListNode));
    if (list->LastMove != NULL && (moves - list->LastMove == 2 || moves - list->LastMove == -2)) {
        moves = list->LastMove;
    }
    printf(" move %d \n",moves);
    if (moves == 0) {
        list->Head->X = current->X - 1;
        list->Head->Y = current->Y;
    }
    else if (moves == 1) {
        list->Head->X = current->X;
        list->Head->Y = current->Y + 1;
    }
    else if (moves == 2) {
        list->Head->X = current->X + 1;
        list->Head->Y = current->Y;
    }
    else if (moves == 3) {
        list->Head->X = current->X;
        list->Head->Y = current->Y - 1;
    }

    current->Last = list->Head;
    list->Head->Next = current;
    list->Head->Last = NULL;

    if (*full == 0) {
        struct TListNode* predTail = list->Tail->Last;
        predTail->Next = NULL;
        //free(list->Tail);
        list->Tail = predTail;
    }
    else {
        list->Size += 1;
        *full = 0;
    }
    list->LastMove = moves;
}

int Alive(struct TList* list, int n, int m) {
    struct TListNode* X = list->Head->X;
    struct TListNode* Y = list->Head->Y;
    if (X >= n || Y >= m) {
        printf("Met wall\n%d", list->Size);
        return 1;
    }
    if (OnSnake(list, X, Y)) {
        printf("Met self\n%d", list->Size);
        return 1;
    }
    return 0;
}


int OnSnake(struct TList* list,int X, int Y) {
    struct TListNode* current = list->Head->Next;
    while (current != NULL) {
        //printf("while %d %d \n",);
        if (X == current->X && Y == current->Y) {
            return 1;
        }
        current = current->Next;
    }
    return 0;
}

void UpdateBerry(struct TList* list, int* berryX, int* berryY,int n, int m) {
    *berryX = (*berryX + 16127) % n;
    *berryY = (*berryY + 16843) % m;
    if (OnSnake(list, *berryX, *berryY)) {
        UpdateBerry(list,berryX, berryY, n, m);
    }
}


int main() {
    struct TList list;
    list.Head = (struct TListNode*)malloc(sizeof(struct TListNode));
    list.Head->Last = NULL;
    list.LastMove = NULL;
    list.Size = 2;
    int n, m;
    scanf_s("%d%d", &n, &m);
    int hX, hY;
    scanf_s("%d%d", &hX, &hY);
    list.Head->X = hX;
    list.Head->Y = hY;
    list.Tail = list.Head->Next = (struct TListNode*)malloc(sizeof(struct TListNode));
    int sX, sY;
    scanf_s("%d%d", &sX, &sY);
    list.Tail->X = sX;
    list.Tail->Y = sY;
    list.Tail->Next = NULL;
    list.Tail->Last = list.Head;
    int berryX, berryY;
    scanf_s("%d%d", &berryX, &berryY);
    int l;
    scanf_s("%d", &l);
    int moves = 0;
    int full = 0;
    for (int i = 0; i < l; ++i) {
        scanf_s("%d", &moves);
        Crawl(&list, moves, &full);
        if (Alive(&list, n, m)) {
            return 0;
        }
        if (list.Head->X == berryX && list.Head->Y == berryY) {
            UpdateBerry(&list,&berryX, &berryY, n, m);
            full = 1;
        }
    }

    printf("%d", list.Size);
    return 0;
}