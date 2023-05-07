#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

/* *** Strukturen *********************************************************** */
#include <stdlib.h>
#include <stdio.h>
/**@brief Struktur des Stacks.
 */
typedef struct {
    /* hier sollte noch etwas dazu kommen */
    int groesse;
    int top;
    int *daten;
} IntStack;

/* *** öffentliche Schnittstelle ******************************************** */

/**@brief Initialisiert einen neuen Stack.
 * @param self  der zu initialisierende Stack
 * @return 0, falls keine Fehler bei der Initialisierung aufgetreten sind,
 *      != 0, ansonsten
 */
extern int
stackInit(IntStack *self){
    self->top=-1;
    self->groesse=0;
    self->daten=NULL;

    return 0;
}

/**@brief Gibt den Stack und alle assoziierten Strukturen frei.
 * @param self  der Stack
 */
extern void
stackRelease(IntStack *self){
    free(self->daten);
    self->top=-1;
    self->groesse=0;
    self->daten=NULL;
}

/**@brief Legt einen Wert auf den intstack.
 * @param self  der intstack
 * @param i     der Wert
 */
extern void
stackPush(IntStack *self, int i){

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

/**@brief Gibt das oberste Element des Stacks zurück.
 * @param self  der Stack
 * @return das oberste Element
 */
extern int
stackTop(const IntStack *self){
    if(self->top==-1){
        printf("Der Stack ist leer, er hat kein oberstes Element!");
        exit(EXIT_FAILURE);
    }

    return self->daten[self->top];
}

/**@brief Entfernt und liefert das oberste Element des Stacks.
 * @param self  der Stack
 * @return das oberste Element
 */
extern int
stackPop(IntStack *self){

    int obersterWert=self->daten[self->top];
    self->top-=1;

    if (self->groesse> 1 && self->top < self->groesse / 2) {
        self->groesse /= 2;
        self->daten = (int *) realloc(self->daten, self->groesse * sizeof(int));
    }
    return obersterWert;
}

/**@brief Gibt zurück, ob der Stack leer ist.
 * @param self  der Stack
 * @return 0, falls nicht leer,
        != 0, falls leer
 */
extern int
stackIsEmpty(const IntStack *self){
    return self->top==-1;
}

/**@brief Gibt den Inhalt des Stacks beginnend mit dem obersten Element auf der
 * Standardausgabe aus.
 * @param self  der Stack
 */
extern void
stackPrint(const IntStack *self);

#endif /* STACK_H_INCLUDED */
