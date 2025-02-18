/*Turcanu Catalin-313CD*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Initializarea benzii

typedef struct BandNode{
    char value;
    struct BandNode *next;
    struct BandNode *prev;
} BandNode;

typedef struct Band{
    BandNode *sentinel;
    BandNode *last;
    BandNode *finger;
} Band;

BandNode *createBandNode(char value){ //Functia de creare a nodului din banda
    BandNode *node = (BandNode*)malloc(sizeof(BandNode));
    node->value = value;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

Band *createBand(){ //Functia de creare a benzii
    Band *band = (Band*)malloc(sizeof(Band));
    BandNode *sentinel = createBandNode('\0');
    BandNode *node = createBandNode('#');
    band->sentinel = sentinel;
    band->last = node;
    band->sentinel->next = node;
    band->last->prev = sentinel;
    band->finger = band->sentinel->next;
    return band;
}

void clearBand(Band *band){ //Functia de curatare a benzii
    BandNode *curr = band->sentinel;
    while (curr != NULL){
        BandNode *temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(band);
}

//Functiile de modificare a benzii

void MOVE_LEFT(Band *band){ //Mutare la stanga
    if (band->finger->prev != band->sentinel){
        band->finger = band->finger->prev;
    }
}

void MOVE_RIGHT(Band *band){ //Mutare la dreapta
    BandNode *iter = band->finger;
    if (band->finger->next == NULL){
        BandNode *new = (BandNode*)malloc(sizeof(BandNode));
        iter->next = new;
        new->prev = iter;
        new->value = '#';
        band->last = new;
        band->finger = new;
    }else{
        band->finger = iter->next;
        band->finger->prev = iter;
    }
}

void MOVE_LEFT_CHAR(Band *band, char c, FILE *file){ //Mutare la stanga pana gasim caracterul
    BandNode *iter = band->finger;
    while (iter != NULL && iter->value != c){
        iter = iter->prev;
    }
    if (iter == NULL){
        fprintf(file,"ERROR\n");
        return;
    }else{
        band->finger = iter;
    }
}

void MOVE_RIGHT_CHAR(Band *band, char c){ //Mutare la dreapta pana gasim caracterul
    BandNode *iter = band->finger;
    while (iter != NULL && iter->value != c){
        iter = iter->next;
    }
    if (iter == NULL){
        BandNode *new = (BandNode*)malloc(sizeof(BandNode));
        new->value = '#';
        new->prev = NULL;
        new->next = NULL;
        band->finger = new;
        if (band->last != NULL){
            iter = band->last;
            iter->next = new;
            new->prev = iter;
            band->last = new;
        }else{
            band->sentinel = new;
            band->last = new;
        }
    }else{
        band->finger = iter;
    }
}

void WRITE(Band *band, char c){ //Scriere caracter in nod
    band->finger->value = c;
}

void INSERT_LEFT(Band *band, char c,FILE *file){ //Inserare caracter la stanga
    BandNode *new = (BandNode*)malloc(sizeof(BandNode));
    if (band->finger->prev == band->sentinel){
        fprintf(file,"ERROR\n");
    }else{
        new->prev = band->finger->prev;
        new->next = band->finger;
        band->finger->prev->next = new;
        band->finger->prev = new;
        band->finger = new;
        new->value = c;
    }
}

void INSERT_RIGHT(Band *band, char c){ //Inserare caracter la dreapta
    BandNode *new = (BandNode*)malloc(sizeof(BandNode));
    if (band->finger->next == NULL){
        band->finger->next = new;
        new->prev = band->finger;
        new->value = c;
        band->last = new;
        new->next = NULL;
        band->finger = new;   
    }else{
        new->next = band->finger->next;
        new->prev = band->finger;
        band->finger->next->prev = new;
        band->finger->next = new;
        band->finger = new;
        new->value = c;
    }
}

void SHOW_CURRENT(Band *band, FILE *file){ //Afisarea caracterului curent
    fprintf(file,"%c\n",band->finger->value);
}

void SHOW(Band *band, FILE *file){ //Afisarea benzii
    BandNode *iter = band->sentinel->next;
    while (iter != band->finger){
        fprintf(file,"%c",iter->value);
        iter = iter->next;
    }
    fprintf(file,"|%c|",band->finger->value);
    BandNode *iter2 = band->finger->next;
    while (iter2 != NULL){
        fprintf(file,"%c",iter2->value);
        iter2 = iter2->next;
    }
    fprintf(file,"\n");
}

//Functia EXECUTE

typedef struct Operation{
    char *type;
    char value;
    struct Operation *next;
} Operatie;

typedef struct Queue{
    Operatie *head;
    Operatie *tail;
    int size;
} Coada;

Operatie *initOperatie(char *tip, char value){ //Initializarea nodului cozii de comenzi
    Operatie *op = malloc(sizeof(Operatie));
    op->type = malloc(strlen(tip) + 1); 
    strcpy(op->type, tip);
    op->value = value;
    op->next = NULL;
    return op;
}

Coada *initCoada(){ //Initializarea cozii de comanzi
    Coada *coada = (Coada*)malloc(sizeof(Coada));
    coada->head = NULL;
    coada->tail = NULL;
    coada->size = 0;
    return coada;
}

Coada *enqueue(Coada *coada, char *tip, char value){ //Adaugarea comenzilor
    Operatie *op = initOperatie(tip, value);
    if (coada->head == NULL){
        coada->head = coada->tail = op;
    }else{
        coada->tail->next = op;
        coada->tail = op;
    }
    coada->size++;
    return coada;
}

Operatie *dequeue(Coada *coada){ //Extragerea comenzilor
    if (coada->head == NULL){
        return NULL; 
    }
    Operatie *op = coada->head;
    coada->head = coada->head->next;
    coada->size--;
    return op;
}

void freeQueue(Coada *coada){ //Eliberarea memoriei pentru coada
    Operatie *op = coada->head;
    while (op != NULL){
        Operatie *next = op->next;
        free(op->type);
        free(op);
        op = next;
    }
    free(coada);
}

void EXECUTE(Coada *coada, Band *band, FILE *file){ //Executarea comenzilor
    if (coada->size == 0){
        return;
    }
    Operatie *op = dequeue(coada);
    if (strcmp(op->type, "MOVE_RIGHT") == 0){
        MOVE_RIGHT(band);
    }
    if (strcmp(op->type, "MOVE_LEFT") == 0){
        MOVE_LEFT(band);
    }
    if (strcmp(op->type, "WRITE") == 0){
        WRITE(band, op->value);
    }
    if (strcmp(op->type, "INSERT_RIGHT") == 0){
        INSERT_RIGHT(band, op->value);
    }
    if (strcmp(op->type, "INSERT_LEFT") == 0){
        INSERT_LEFT(band, op->value,file);
    }
    if (strcmp(op->type, "MOVE_RIGHT_CHAR") == 0){
        MOVE_RIGHT_CHAR(band, op->value);
    }
    if (strcmp(op->type, "MOVE_LEFT_CHAR") == 0){
        MOVE_LEFT_CHAR(band,op->value,file);
    }
}

//Comenzile UNDO si REDO

typedef struct StackNode{
    char command[256];
    struct StackNode *next;
} StackNode;

typedef struct CommandStack{
    StackNode *top;
} CommandStack;

CommandStack *initStack(){ //Initializarea stivelor
    CommandStack *stack = (CommandStack*)malloc(sizeof(CommandStack));
    stack->top = NULL;
    return stack;
}

StackNode *push(CommandStack *stack, char *command){ //Adaugarea comezilor
    StackNode *node = (StackNode*)malloc(sizeof(StackNode));
    strcpy(node->command, command);
    node->next = stack->top;
    stack->top = node;
    return node;
}

char *pop(CommandStack *stack){ //Extragerea comenzilor
    if (stack->top == NULL){
        return NULL;
    }
    StackNode *temp = stack->top;
    stack->top = stack->top->next;
    char *command = strdup(temp->command);
    free(temp);
    return command;
}

void clearStack(CommandStack *stack){ //Eliberarea memoriei pentru stive
    StackNode *temp;
    while (stack->top != NULL){
        temp = stack->top;
        stack->top = stack->top->next;
        free(temp);
    }
    free(stack);
}


void UNDO(Band* band, CommandStack *undo, CommandStack *redo){ //Operatia UNDO
    char *command = pop(undo);
    if (command == NULL){
        return;
    }
    if (strcmp(command, "MOVE_LEFT") == 0){
        MOVE_RIGHT(band);
    }
    else if (strcmp(command, "MOVE_RIGHT") == 0){
        MOVE_LEFT(band);
    }
    push(redo, command);
}

void REDO(Band *band, CommandStack *undo, CommandStack *redo){ //Operatia REDO
    char *command = pop(redo);
    if (command == NULL){
        return;
    }
    if (strcmp(command, "MOVE_LEFT") == 0){
        MOVE_LEFT(band);
    } 
    else if (strcmp(command, "MOVE_RIGHT") == 0){
        MOVE_RIGHT(band);
    }
    push(undo,command);
}

