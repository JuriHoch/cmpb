#ifndef SYNTREE_H_INCLUDED
#define SYNTREE_H_INCLUDED

/* *** Strukturen *********************************************************** */
/* Falls die Compilierung fehlschlaegt, bitte in die README Datei gucken.*/

typedef int SyntreeNodeID;

/**@brief Struktur des abstrakten Syntaxbaumes.
 */
typedef struct ListenKnoten {
    SyntreeNodeID id;
    struct ListenKnoten *next;
} ListenKnoten;

typedef struct SyntreeNode {
    SyntreeNodeID id;
    enum {
        nummer, knoten
    } tag;
    union {
        int wert;
        struct {
            char *Startknoten;
            int AnzahlKinder;
            ListenKnoten *Kinder;
        } node;
    };
} SyntreeNode;

typedef struct {
    int laenge;
    int AnzahlmaximalerKnoten;
    SyntreeNode *nodes;
} Syntree;

/* *** öffentliche Schnittstelle ******************************************** */




// Initialisiert einen neuen Syntaxbaum und liefert den Rückgabewert 0, falls keine Fehler bei der
// Initialisierung aufgetreten sind und andernfalls einen Fehlercode.
extern int syntreeInit(Syntree *self);

// Gibt den Syntaxbaum und alle damit assoziierten Strukturen frei.
extern void syntreeRelease(Syntree *self);

// Erstellt einen neuen Knoten mit einem Zahlaengewert als Inhalt und gibt dessen ID zurück.
extern SyntreeNodeID syntreeNodeNumber(Syntree *self, int number) ;

extern SyntreeNodeID syntreeNodeTag(Syntree *self, SyntreeNodeID id);


// Kapselt ein Knotenpaar innerhalb eines Knotens und gibt dessen ID zurück.
extern SyntreeNodeID syntreeNodePair(Syntree *self, SyntreeNodeID id1, SyntreeNodeID id2);
    


// Hängt einen Knoten an das Ende eines Listenknotens und gibt dessen ID zurück.
extern SyntreeNodeID syntreeNodeAppend(Syntree *self, SyntreeNodeID list, SyntreeNodeID elem);
    


// Hängt einen Knoten an den Anfang eines Listenknotens und gibt dessen ID zurück.
extern SyntreeNodeID syntreeNodePrepend(Syntree *self, SyntreeNodeID elem, SyntreeNodeID list);
   


// Gibt alle Zahlaengeknoten zwischen runden und alle zusammengesetzten Knoten zwischen
// geschweiften Klammern rekursiv (depth-first) auf der Standardausgabe aus.
extern void syntreePrint(const Syntree *self, SyntreeNodeID root);
    


#endif /* SYNTREE_H_INCLUDED */  