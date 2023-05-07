#include <stdio.h>
#include "stack.h"
#include <stdlib.h>

/* Falls die Compilierung fehlschlaegt, bitte in die README Datei gucken.*/


int stackInit(IntStack *self){
    if(self ==NULL){
        fprintf(stderr,"Fehler bei der Initialisierung!\n ");
        return -1;
    }
    self->top=-1;
    self->groesse=0;
    self->daten=NULL;

    return 0;
}

void stackRelease(IntStack *self){
    free(self->daten);
    self->top=-1;
    self->groesse=0;
    self->daten=NULL;
}

void stackPush(IntStack *self, int i){

if (self->top==self->groesse-1) {
        // Wenn der Stack voll ist, erhöhen wir die Größe des Arrays
        self->groesse+=1;
        self->daten = (int *) realloc(self->daten, self->groesse * sizeof(int));
        if (self->daten == NULL) {
            printf("freier Speicher konnte nicht gefunden werden\n");
            exit(EXIT_FAILURE);
        }
    }

    self->top+=1;
    
    self->daten[self->top]=i;
}


int stackTop(const IntStack *self){
    if(self->top==-1){
        printf("Der Stack ist leer, er hat kein oberstes Element!\n");
        exit(EXIT_FAILURE);
    }

    return self->daten[self->top];
}


int stackPop(IntStack *self){

    if(self->top==-1){
        printf("Der Stack ist leer, er hat kein oberstes Element!\n");
        exit(EXIT_FAILURE);
    }

    int obersterWert=self->daten[self->top];
    self->top-=1;

    if (self->groesse> 1 && self->top < self->groesse / 2) {
        self->groesse /= 2;
        self->daten = (int *) realloc(self->daten, self->groesse * sizeof(int));
    }
    return obersterWert;
}

int stackIsEmpty(const IntStack *self){
    return self->top==-1;
}
