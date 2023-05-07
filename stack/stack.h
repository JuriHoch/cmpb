#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED
/* Falls die Compilierung fehlschlaegt, bitte in die README Datei gucken.*/

/* *** Strukturen *********************************************************** */
//#include "stack.c"
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
stackInit(IntStack *self);

/**@brief Gibt den Stack und alle assoziierten Strukturen frei.
 * @param self  der Stack
 */
extern void
stackRelease(IntStack *self);

/**@brief Legt einen Wert auf den intstack.
 * @param self  der intstack
 * @param i     der Wert
 */
extern void
stackPush(IntStack *self, int i);

/**@brief Entfernt und liefert das oberste Element des Stacks.
 * @param self  der Stack
 * @return das oberste Element
 */
extern int
stackPop(IntStack *self);
/**@brief Gibt zurück, ob der Stack leer ist.
 * @param self  der Stack
 * @return 0, falls nicht leer,
        != 0, falls leer
 */
extern int
stackIsEmpty(const IntStack *self);

/**@brief Gibt den Inhalt des Stacks beginnend mit dem obersten Element auf der
 * Standardausgabe aus.
 * @param self  der Stack
 */
extern void
stackPrint(const IntStack *self);

#endif /* STACK_H_INCLUDED */
