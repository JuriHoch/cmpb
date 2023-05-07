#include <stdlib.h>
#include <stdio.h>
#ifndef SYNTREE_H_INCLUDED
#define SYNTREE_H_INCLUDED


/* *** Strukturen *********************************************************** */
/* Double Linked List: https://www.geeksforgeeks.org/doubly-linked-list/ */

typedef struct Node {
    int data;           // Index
    struct Node* next;
    struct Node* prev;
}Node;

typedef struct node_t{
    int value;         //
    int contain_value; // 0 if contain Value otherwise not
    Node* head;         // List of nodes in the node
}node_t;

typedef int/* muss noch definiert werden */ SyntreeNodeID;

/**@brief Struktur des abstrakten Syntaxbaumes.
 */
typedef struct {
    /* hier sollte noch etwas dazu kommen */
    node_t* node_list;
    int nodelist_cap; // maxlength of list
    int amnt_nodes;   // actual length of list
} Syntree;

/*Hilfsfunktionen für DLL*/
void push(Node** head_ref, int new_data)
{
    /* 1. allocate node */
    Node* new_node = (struct Node*)malloc(sizeof(struct Node));
 
    /* 2. put in the data  */
    new_node->data = new_data;
 
    /* 3. Make next of new node as head and previous as NULL */
    new_node->next = (*head_ref);
    new_node->prev = NULL;
 
    /* 4. change prev of head node to new node */
    if ((*head_ref) != NULL)
        (*head_ref)->prev = new_node;
 
    /* 5. move the head to point to the new node */
    (*head_ref) = new_node;
}

void insertAfter(Node* prev_node, int new_data)
{
    /*1. check if the given prev_node is NULL */
    if (prev_node == NULL) {
        printf("the given previous node cannot be NULL");
        return;
    }
    /* 2. allocate new node */
    Node* new_node = ( Node*)malloc(sizeof( Node));
 
    /* 3. put in the data  */
    new_node->data = new_data;
 
    /* 4. Make next of new node as next of prev_node */
    new_node->next = prev_node->next;
 
    /* 5. Make the next of prev_node as new_node */
    prev_node->next = new_node;
 
    /* 6. Make prev_node as previous of new_node */
    new_node->prev = prev_node;
 
    /* 7. Change previous of new_node's next node */
    if (new_node->next != NULL)
        new_node->next->prev = new_node;
}
 
/* Given a reference (pointer to pointer) to the head
   of a DLL and an int, appends a new node at the end  */
void append(Node** head_ref, int new_data)
{
    /* 1. allocate node */
    struct Node* new_node = ( Node*)malloc(sizeof(struct Node));
 
    struct Node* last = *head_ref; /* used in step 5*/
 
    /* 2. put in the data  */
    new_node->data = new_data;
 
    /* 3. This new node is going to be the last node, so
          make next of it as NULL*/
    new_node->next = NULL;
 
    /* 4. If the Linked List is empty, then make the new
          node as head */
    if (*head_ref == NULL) {
        new_node->prev = NULL;
        *head_ref = new_node;
        return;
    }
 
    /* 5. Else traverse till the last node */
    while (last->next != NULL)
        last = last->next;
 
    /* 6. Change the next of last node */
    last->next = new_node;
 
    /* 7. Make last node as previous of new node */
    new_node->prev = last;
 
    return;
}

/* *** öffentliche Schnittstelle ******************************************** */

/**@brief Initialisiert einen neuen Syntaxbaum.
 * @param self  der zu initialisierende Syntaxbaum
 * @return 0, falls keine Fehler bei der Initialisierung aufgetreten sind,
 *      != 0 ansonsten
 */
extern int
syntreeInit(Syntree *self){
    self->nodelist_cap =16;
    self->amnt_nodes =0;
    self->node_list = (node_t*) malloc (self->nodelist_cap*sizeof(node_t));
    //self->node_list = (Node*) malloc (self->nodelist_cap*sizeof(Node));
};

/**@brief Gibt den Syntaxbaum und alle assoziierten Strukturen frei.
 * @param self  der freizugebende Syntaxbaum
 */
extern void
syntreeRelease(Syntree *self){
    free(self->node_list);
};

/**@brief Erstellt einen neuen Knoten mit einem Zahlenwert als Inhalt.
 * @param self    der Syntaxbaum
 * @param number  die Zahl
 * @return ID des neu erstellten Knoten
 */
extern SyntreeNodeID
syntreeNodeNumber(Syntree *self, int number){
 //check enough space
    int amnt= self->amnt_nodes;
    int cap = self->nodelist_cap;
    if(amnt + 1 >= cap){
        cap = cap*2;
        self->node_list= (node_t*) realloc(self->node_list, cap* sizeof(node_t));
        self->nodelist_cap= cap;
    }
    amnt+1;
    self->node_list[amnt].value = number;       //id1 at index 1, no value for index at 0
    self->node_list[amnt].contain_value = 1;
    self->amnt_nodes= amnt;
    return amnt;
};

/**@brief Kapselt einen Knoten innerhalb eines anderen Knotens.
 * @param self  der Syntaxbaum
 * @param id    der zu kapselnde Knoten
 * @return ID des neu erstellten Knoten
 */
extern SyntreeNodeID
syntreeNodeTag(Syntree *self, SyntreeNodeID id){
   int amnt= self->amnt_nodes;
    int cap = self->nodelist_cap;
    if(amnt + 1 >= cap){
        cap = cap*2;
        self->node_list= (node_t*) realloc(self->node_list, cap* sizeof(node_t));
        self->nodelist_cap= cap;
    }
    amnt+1;
    self->node_list[amnt].contain_value = 0; // wrapped no value
    self->amnt_nodes= amnt;
    append(&self->node_list[amnt].head,id);
    return amnt;
};

/**@brief Kapselt zwei Knoten innerhalb eines Knoten.
 * @param self  der Syntaxbaum
 * @param id1   erster Knoten
 * @param id2   zweiter Knoten
 * @return ID des neu erstellten Knoten
 */
extern SyntreeNodeID
syntreeNodePair(Syntree *self, SyntreeNodeID id1, SyntreeNodeID id2){
    int amnt= self->amnt_nodes;
    int cap = self->nodelist_cap;
        if(amnt + 1 >= cap){
            cap = cap*2;
            self->node_list= (node_t*) realloc(self->node_list, cap* sizeof(node_t));
            self->nodelist_cap= cap;
        }
    amnt+1;
    self->amnt_nodes= amnt;
    self->node_list[amnt].contain_value = 0;
    append(&self->node_list[amnt].head,id1);
    append(&self->node_list[amnt].head,id2);
    return amnt;
};

/**@brief Hängt einen Knoten an das Ende eines Listenknotens.
 * @param self  der Syntaxbaum
 * @param list  Listenknoten
 * @param elem  anzuhängender Knoten
 * @return ID des Listenknoten
 */
extern SyntreeNodeID
syntreeNodeAppend(Syntree *self, SyntreeNodeID list, SyntreeNodeID elem){
    int amnt= self->amnt_nodes;
    int cap = self->nodelist_cap;
     
    self->amnt_nodes= amnt;
    self->node_list[amnt].contain_value = 0;
    append(&self->node_list[list].head,elem);
    return list;
};

/**@brief Hängt einen Knoten an den Anfang eines Listenknotens.
 * @param self  der Syntaxbaum
 * @param elem  anzuhängender Knoten
 * @param list  Listenknoten
 * @return ID des Listenknoten
 */
extern SyntreeNodeID
syntreeNodePrepend(Syntree *self, SyntreeNodeID elem, SyntreeNodeID list){
    int amnt= self->amnt_nodes;
    int cap = self->nodelist_cap;
     
    self->amnt_nodes= amnt;
    self->node_list[amnt].contain_value = 0;
    push(&self->node_list[list].head,elem);
    return list;
};

/**@brief Gibt alle Zahlenknoten rekursiv (depth-first) aus.
 * @param self  der Syntaxbaum
 * @param root  der Wurzelknoten
 */
extern void
syntreePrint(const Syntree *self, SyntreeNodeID root){
    printf("I made it to the end, nice %d\n", root);
};

#endif /* SYNTREE_H_INCLUDED */
