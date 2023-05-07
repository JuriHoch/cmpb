/***************************************************************************//**
 * Martina Scholz
 * Martrikelnr: 547342
 * Compilerbau SS 2022
 * @file syntree.c
 * @brief Implementation of an syntax tree.
 ******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "syntree.h"
#include <assert.h>

// Initialisiert einen neuen Syntaxbaum und liefert den Rückgabewert 0, falls keine Fehler bei der
// Initialisierung aufgetreten sind und andernfalls einen Fehlercode.
int syntreeInit(Syntree *self) {
    self->cap = SIZE;
    self->len = 0;
    self->nodes = (SyntreeNode *) malloc(sizeof(SyntreeNode) * self->cap);
    if (self->nodes == NULL) {
        fputs("unable-to-allocate-memory error\n", stderr);
        exit(-1);
    }
    return 0;
}

// Gibt den Syntaxbaum und alle damit assoziierten Strukturen frei.
void syntreeRelease(Syntree *self) {
    ListNode *tmp;
    for (int i = 0; i < self->len; i++) {
        if (self->nodes[i].tag == NODE) {
            while (self->nodes[i].node.children != NULL) {
                tmp = self->nodes[i].node.children;
                self->nodes[i].node.children = self->nodes[i].node.children->next;
                free(tmp);
            }
            free(self->nodes[i].node.children);
        }
    }
    free(self->nodes);
}

// Erstellt einen neuen Knoten mit einem Zahlenwert als Inhalt und gibt dessen ID zurück.
SyntreeNodeID syntreeNodeNumber(Syntree *self, int number) {
    SyntreeNode *node = (SyntreeNode *) malloc(sizeof(SyntreeNode));
    if (node == NULL) {
        fputs("unable-to-allocate-memory error\n", stderr);
        exit(-1);
    }
    node->id = self->len;
    node->tag = NUMBER;
    node->val = number;

    if (self->len == self->cap - 1) {
        self->cap *= 2;
        self->nodes = (SyntreeNode *) realloc(self->nodes, sizeof(SyntreeNode) * self->cap);
        if (self->nodes == NULL) {
            fputs("unable-to-allocate-memory error\n", stderr);
            exit(-1);
        }
    }
    self->nodes[self->len++] = *node;
    free(node);
    return self->len - 1;
}

// Kapselt einen Knoten innerhalb eines anderen Knotens und gibt dessen ID zurück.
SyntreeNodeID syntreeNodeTag(Syntree *self, SyntreeNodeID id) {
    SyntreeNode *node = (SyntreeNode *) malloc(sizeof(SyntreeNode));
    if (node == NULL) {
        fputs("unable-to-allocate-memory error\n", stderr);
        exit(-1);
    }
    node->id = self->len;
    node->tag = NODE;
    node->node.entry = "num";
    node->node.children = (ListNode *) malloc(sizeof(ListNode));
    if (node->node.children == NULL) {
        fputs("unable-to-allocate-memory error\n", stderr);
        exit(-1);
    }
    node->node.children->id = id;
    node->node.children->next = NULL;
    node->node.numOfChildren++;

    if (self->len == self->cap - 1) {
        self->cap *= 2;
        self->nodes = (SyntreeNode *) realloc(self->nodes, sizeof(SyntreeNode) * self->cap);
        if (self->nodes == NULL) {
            fputs("unable-to-allocate-memory error\n", stderr);
            exit(-1);
        }
    }
    self->nodes[self->len++] = *node;
    free(node);

    return self->len - 1;
}

// Kapselt ein Knotenpaar innerhalb eines Knotens und gibt dessen ID zurück.
SyntreeNodeID syntreeNodePair(Syntree *self, SyntreeNodeID id1, SyntreeNodeID id2) {
    SyntreeNode *node = (SyntreeNode *) malloc(sizeof(SyntreeNode));
    if (node == NULL) {
        fputs("unable-to-allocate-memory error\n", stderr);
        exit(-1);
    }
    node->id = self->len;
    node->tag = NODE;
    node->node.entry = "op";
    node->node.children = (ListNode *) malloc(sizeof(ListNode));
    if (node->node.children == NULL) {
        fputs("unable-to-allocate-memory error\n", stderr);
        exit(-1);
    }
    node->node.children->id = id1;
    node->node.children->next = (ListNode *) malloc(sizeof(ListNode));
    if (node->node.children->next == NULL) {
        fputs("unable-to-allocate-memory error\n", stderr);
        exit(-1);
    }
    node->node.children->next->id = id2;
    node->node.children->next->next = NULL;
    node->node.numOfChildren = node->node.numOfChildren + 2;

    if (self->len == self->cap - 1) {
        self->cap *= 2;
        self->nodes = (SyntreeNode *) realloc(self->nodes, sizeof(SyntreeNode) * self->cap);
        if (self->nodes == NULL) {
            fputs("unable-to-allocate-memory error\n", stderr);
            exit(-1);
        }
    }
    self->nodes[self->len++] = *node;

    free(node);

    return self->len - 1;
}

// Hängt einen Knoten an das Ende eines Listenknotens und gibt dessen ID zurück.
SyntreeNodeID syntreeNodeAppend(Syntree *self, SyntreeNodeID list, SyntreeNodeID elem) {
    if (self->nodes[list].node.children == NULL) {
        fputs("NULL error\n", stderr);
        exit(-1);
    }
    ListNode *current = self->nodes[list].node.children;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = (ListNode *) malloc(sizeof(ListNode));
    current->next->id = elem;
    current->next->next = NULL;

    self->nodes[list].node.numOfChildren++;
    return list;
}

// Hängt einen Knoten an den Anfang eines Listenknotens und gibt dessen ID zurück.
SyntreeNodeID syntreeNodePrepend(Syntree *self, SyntreeNodeID elem, SyntreeNodeID list) {
    ListNode *newFirstChild;
    newFirstChild = (ListNode *) malloc(sizeof(ListNode));
    if (newFirstChild == NULL) {
        fputs("unable-to-allocate-memory error\n", stderr);
        exit(-1);
    }
    newFirstChild->id = elem;
    newFirstChild->next = self->nodes[list].node.children;
    self->nodes[list].node.children = newFirstChild;

    self->nodes[list].node.numOfChildren++;
    return list;
}

// Gibt alle Zahlenknoten zwischen runden und alle zusammengesetzten Knoten zwischen
// geschweiften Klammern rekursiv (depth-first) auf der Standardausgabe aus.
void syntreePrint(const Syntree *self, SyntreeNodeID root) {
    ListNode *current;

    if(self->nodes[root].tag != NUMBER){
        printf("{");
        current = self->nodes[root].node.children;
        while(current != NULL){
            syntreePrint(self, current->id);
            current = current->next;
        }
        printf("}");
    }

    if (self->nodes[root].tag == NUMBER)
        printf("(%i)", self->nodes[root].val);
}
