/*Turcanu Catalin-313CD*/

#include "MagicBand.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 255

int main(){

    //Initializarea structurilor de date necesare

    Band *band = createBand();
    Coada *coada = initCoada();
    CommandStack *undo = initStack();
    CommandStack *redo = initStack();

    //Citirea comenzilor din fisier, adaugarea acestora in structurile de date necesare si afisarea outputului in alt fisier

    FILE *finput = fopen("tema1.in","r");
    FILE *foutput = fopen("tema1.out","w");
    char *number = (char*)malloc(sizeof(char));
    fgets(number,MAX,finput);
    int nrcommands = atoi(number);
    char command[256];
    for(int i = 0; i < nrcommands; i++){
        fgets(command,MAX,finput);
        command[strcspn(command, "\r\n")] = 0;
        if (strcmp(command,"MOVE_RIGHT") == 0){
            push(undo,"MOVE_RIGHT");
            enqueue(coada,"MOVE_RIGHT",'\0');
        }
        if (strcmp(command,"MOVE_LEFT") == 0){
            push(undo,"MOVE_LEFT");
            enqueue(coada,"MOVE_LEFT",'\0');
        }
        if (strncmp(command,"WRITE",5) == 0){
            char value = command[6];
            enqueue(coada,"WRITE",value);
        }
        if (strncmp(command,"INSERT_RIGHT",12) == 0){
            char value = command[13];
            enqueue(coada,"INSERT_RIGHT",value); 
        }
        if (strncmp(command,"INSERT_LEFT",11) == 0){
            char value = command[12];
            enqueue(coada,"INSERT_LEFT",value);
        }
        if (strncmp(command,"MOVE_RIGHT_CHAR",15) == 0){
            char value = command[16];
            enqueue(coada,"MOVE_RIGHT_CHAR",value);
        }
        if (strncmp(command,"MOVE_LEFT_CHAR",14) == 0){
            char value = command[15];
            enqueue(coada,"MOVE_LEFT_CHAR",value);
        }
        if (strcmp(command,"EXECUTE") == 0){
            EXECUTE(coada,band,foutput);  
        }
        if (strcmp(command,"SHOW_CURRENT") == 0){
            SHOW_CURRENT(band,foutput);
        }
        if (strcmp(command,"SHOW") == 0){
            SHOW(band,foutput);
        }
        if (strcmp(command,"UNDO") == 0){
            UNDO(band,undo,redo);
        }
        if (strcmp(command,"REDO") == 0){
            REDO(band,undo,redo);
        }
    }

    //Eliberrarea memoriei

    clearBand(band);
    freeQueue(coada);
    clearStack(undo);
    clearStack(redo);
    free(number);
    fclose(finput);
    fclose(foutput);

    return 0;
}