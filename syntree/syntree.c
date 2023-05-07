
#include <stdlib.h>
#include <stdio.h>
#include "syntree.h"
#include <assert.h>

/* Falls die Compilierung fehlschlaegt, bitte in die README Datei gucken.*/

int syntreeInit(Syntree *self) {
    self->AnzahlmaximalerKnoten = 100;
    self->laenge = 0;
    self->nodes = (SyntreeNode *) malloc(sizeof(SyntreeNode) * self->AnzahlmaximalerKnoten);
    if (self->nodes == NULL) {
        fputs("Fehler, kein Speicher vorhanden\n", stderr);
        exit(EXIT_FAILURE);
    }
    return 0;
}


void syntreeRelease(Syntree *self) {
    ListenKnoten *tmp;
    for (int i = 0; i < self->laenge; i++) {
        if (self->nodes[i].tag == knoten) {
            while (self->nodes[i].node.Kinder != NULL) {
                tmp = self->nodes[i].node.Kinder;
                self->nodes[i].node.Kinder = self->nodes[i].node.Kinder->next;
                free(tmp);
            }
            free(self->nodes[i].node.Kinder);
        }
    }
    free(self->nodes);
}


SyntreeNodeID syntreeNodeNumber(Syntree *self, int number) {
    SyntreeNode *node = (SyntreeNode *) malloc(sizeof(SyntreeNode));
    if (node == NULL) {
        fputs("Fehler, kein Speicher vorhanden\n", stderr);
        exit(EXIT_FAILURE);
    }
    node->id = self->laenge;
    node->tag = nummer;
    node->wert = number;

    if (self->laenge == self->AnzahlmaximalerKnoten - 1) {
        self->AnzahlmaximalerKnoten *= 2;
        self->nodes = (SyntreeNode *) realloc(self->nodes, sizeof(SyntreeNode) * self->AnzahlmaximalerKnoten);
        if (self->nodes == NULL) {
            fputs("Fehler, kein Speicher vorhanden\n", stderr);
            exit(EXIT_FAILURE);
        }
    }
    self->nodes[self->laenge++] = *node;
    free(node);
    return self->laenge - 1;
}


SyntreeNodeID syntreeNodeTag(Syntree *self, SyntreeNodeID id) {
    SyntreeNode *node = (SyntreeNode *) malloc(sizeof(SyntreeNode));
    if (node == NULL) {
        fputs("Fehler, kein Speicher vorhandenn", stderr);
        exit(EXIT_FAILURE);
    }
    node->id = self->laenge;
    node->tag = knoten;
    node->node.Startknoten = "num";
    node->node.Kinder = (ListenKnoten *) malloc(sizeof(ListenKnoten));
    if (node->node.Kinder == NULL) {
        fputs("Fehler, kein Speicher vorhanden\n", stderr);
        exit(EXIT_FAILURE);
    }
    node->node.Kinder->id = id;
    node->node.Kinder->next = NULL;
    node->node.AnzahlKinder++;

    if (self->laenge == self->AnzahlmaximalerKnoten - 1) {
        self->AnzahlmaximalerKnoten *= 2;
        self->nodes = (SyntreeNode *) realloc(self->nodes, sizeof(SyntreeNode) * self->AnzahlmaximalerKnoten);
        if (self->nodes == NULL) {
            fputs("Fehler, kein Speicher vorhanden\n", stderr);
            exit(EXIT_FAILURE);
        }
    }
    self->nodes[self->laenge++] = *node;
    free(node);

    return self->laenge - 1;
}


SyntreeNodeID syntreeNodePair(Syntree *self, SyntreeNodeID id1, SyntreeNodeID id2) {
    SyntreeNode *node = (SyntreeNode *) malloc(sizeof(SyntreeNode));
    if (node == NULL) {
        fputs("Fehler, kein Speicher vorhanden\n", stderr);
        exit(EXIT_FAILURE);
    }
    node->id = self->laenge;
    node->tag = knoten;
    node->node.Startknoten = "op";
    node->node.Kinder = (ListenKnoten *) malloc(sizeof(ListenKnoten));
    if (node->node.Kinder == NULL) {
        fputs("Fehler, kein Speicher vorhanden\n", stderr);
        exit(EXIT_FAILURE);
    }
    node->node.Kinder->id = id1;
    node->node.Kinder->next = (ListenKnoten *) malloc(sizeof(ListenKnoten));
    if (node->node.Kinder->next == NULL) {
        fputs("Fehler, kein Speicher vorhanden\n", stderr);
        exit(EXIT_FAILURE);
    }
    node->node.Kinder->next->id = id2;
    node->node.Kinder->next->next = NULL;
    node->node.AnzahlKinder = node->node.AnzahlKinder + 2;

    if (self->laenge == self->AnzahlmaximalerKnoten - 1) {
        self->AnzahlmaximalerKnoten *= 2;
        self->nodes = (SyntreeNode *) realloc(self->nodes, sizeof(SyntreeNode) * self->AnzahlmaximalerKnoten);
        if (self->nodes == NULL) {
            fputs("Fehler, kein Speicher vorhanden\n", stderr);
            exit(EXIT_FAILURE);
        }
    }
    self->nodes[self->laenge++] = *node;

    free(node);

    return self->laenge - 1;
}


SyntreeNodeID syntreeNodeAppend(Syntree *self, SyntreeNodeID list, SyntreeNodeID elem) {
    if (self->nodes[list].node.Kinder == NULL) {
        fputs("Fehler\n", stderr);
        exit(EXIT_FAILURE);
    }
    ListenKnoten *current = self->nodes[list].node.Kinder;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = (ListenKnoten *) malloc(sizeof(ListenKnoten));
    current->next->id = elem;
    current->next->next = NULL;

    self->nodes[list].node.AnzahlKinder++;
    return list;
}


SyntreeNodeID syntreeNodePrepend(Syntree *self, SyntreeNodeID elem, SyntreeNodeID list) {
    ListenKnoten *neuerErsterKinderknoten;
    neuerErsterKinderknoten = (ListenKnoten *) malloc(sizeof(ListenKnoten));
    if (neuerErsterKinderknoten == NULL) {
        fputs("Fehler, kein Speicher vorhanden\n", stderr);
        exit(EXIT_FAILURE);
    }
    neuerErsterKinderknoten->id = elem;
    neuerErsterKinderknoten->next = self->nodes[list].node.Kinder;
    self->nodes[list].node.Kinder = neuerErsterKinderknoten;

    self->nodes[list].node.AnzahlKinder++;
    return list;
}


void syntreePrint(const Syntree *self, SyntreeNodeID root) {
    ListenKnoten *current;

    if(self->nodes[root].tag != nummer){
        printf("{");
        current = self->nodes[root].node.Kinder;
        while(current != NULL){
            syntreePrint(self, current->id);
            current = current->next;
        }
        printf("}");
    }

    if (self->nodes[root].tag == nummer)
        printf("(%i)", self->nodes[root].wert);
}

