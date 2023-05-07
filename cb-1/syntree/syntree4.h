#ifndef SYNTREE_H_INCLUDED
#define SYNTREE_H_INCLUDED

/* *** Strukturen *********************************************************** */
#include <stdlib.h>
#include <stdio.h>

typedef /* muss noch definiert werden */ int SyntreeNodeID;

/**@brief Struktur des abstrakten Syntaxbaumes.
 */
typedef struct SyntreeNode SyntreeNode;

/**@brief Struktur des abstrakten Syntaxbaumes.
 */
typedef struct {
    SyntreeNode *root; // Wurzelknoten des Baumes
} Syntree;

/**@brief Struktur eines Knotens im Syntaxbaum.
 */
struct SyntreeNode {
    SyntreeNodeID id; // ID des Knotens
    int type; // Typ des Knotens (z.B. Operator, Zahl, Variable, ...)
    union {
        int number; // Wert, falls Typ == Zahl
        char *variable; // Name, falls Typ == Variable
    } content;
    int num_children; // Anzahl der Kindknoten
    SyntreeNode **children; // Array von Zeigern auf die Kindknoten
};


/* *** öffentliche Schnittstelle ******************************************** */

/**@brief Initialisiert einen neuen Syntaxbaum.
 * @param self  der zu initialisierende Syntaxbaum
 * @return 0, falls keine Fehler bei der Initialisierung aufgetreten sind,
 *      != 0 ansonsten
 */



int syntreeInit(Syntree *self) {
    self->root = NULL;
    return 0;
}


void freeSyntreeNode(SyntreeNode *node) {
    if (node->num_children > 0) {
        for (int i = 0; i < node->num_children; i++) {
            freeSyntreeNode(node->children[i]);
        }
        free(node->children);
    }
    if (node->type == content.number) {
        // Falls der Knoten eine Zahl speichert, muss der Speicher für die Zahl freigegeben werden
        free(node->content.variable);
    }
    free(node);
}
void syntreeRelease(Syntree *self) {
    if (self->root != NULL) {
        freeSyntreeNode(self->root);
        self->root = NULL;
    }
}



/**@brief Erstellt einen neuen Knoten mit einem Zahlenwert als Inhalt.
 * @param self    der Syntaxbaum
 * @param number  die Zahl
 * @return ID des neu erstellten Knoten
 */
SyntreeNodeID syntreeNodeNumber(Syntree *self, int number) {
    // Erstelle einen neuen Knoten und initialisiere ihn
    SyntreeNode *new_node = (SyntreeNode*)malloc(sizeof(SyntreeNode));
    new_node->type = this.number;
    new_node->content.number = number;
    new_node->num_children = 0;
    new_node->children = NULL;

    // Füge den neuen Knoten als Kindknoten zur Wurzel hinzu
    if (self->root == NULL) {
        self->root = new_node;
    } else {
        self->root->num_children++;
        self->root->children = (SyntreeNode**)realloc(self->root->children,
            self->root->num_children * sizeof(SyntreeNode*));
        self->root->children[self->root->num_children - 1] = new_node;
    }

    // Gib die ID des neuen Knotens zurück
    return new_node->id;
}

static int find_node_by_id(SyntreeNode *current_node, SyntreeNodeID id, SyntreeNode **result_node) {
    if (current_node->id == id) {
        *result_node = current_node;
        return 1;
    }
    for (int i = 0; i < current_node->num_children; i++) {
        if (find_node_by_id(current_node->children[i], id, result_node)) {
            return 1;
        }
    }
    return 0;
}


SyntreeNodeID syntreeNodeTag(Syntree *self, SyntreeNodeID id) {
    // Suche den Knoten mit der gegebenen ID
    SyntreeNode *node = NULL;
    find_node_by_id(self->root, id, &node);

    // Wenn der Knoten gefunden wurde, gib seinen Typ zurück
    if (node != NULL) {
        return node->type;
    } else {
        return -1; // Fehler: Knoten mit gegebener ID nicht gefunden
    }
}


/**@brief Kapselt zwei Knoten innerhalb eines Knoten.
 * @param self  der Syntaxbaum
 * @param id1   erster Knoten
 * @param id2   zweiter Knoten
 * @return ID des neu erstellten Knoten
 */

void add_child(SyntreeNode *parent, SyntreeNode *child) {
    parent->num_children++;
    parent->children = realloc(parent->children, parent->num_children * sizeof(SyntreeNode *));
    parent->children[parent->num_children - 1] = child;
}

void remove_child(SyntreeNode *parent, int index) {
    if (index < 0 || index >= parent->num_children) {
        fprintf(stderr, "Invalid child index\n");
        return;
    }
    memmove(&parent->children[index], &parent->children[index + 1], (parent->num_children - index - 1) * sizeof(SyntreeNode *));
    parent->num_children--;
    parent->children = realloc(parent->children, parent->num_children * sizeof(SyntreeNode *));
}



extern SyntreeNodeID syntreeNodePair(Syntree *self, SyntreeNodeID id1, SyntreeNodeID id2) {
    SyntreeNode *node1, *node2;
    if (!find_node_by_id(self->root, id1, &node1) || !find_node_by_id(self->root, id2, &node2)) {
        return -1; // ID(s) not found
    }

    // create new node
    SyntreeNode *new_node = (SyntreeNode *)malloc(sizeof(SyntreeNode));
    if (!new_node) {
        return -1; // allocation failed
    }

    new_node->id = next_node_id++;
    new_node->type = NODE_TYPE_PAIR;
    new_node->num_children = 2;
    new_node->children = (SyntreeNode **)malloc(sizeof(SyntreeNode *) * new_node->num_children);
    if (!new_node->children) {
        free(new_node);
        return -1; // allocation failed
    }

    new_node->children[0] = node1;
    new_node->children[1] = node2;

    // replace node1 and node2 with new node in parent's child list
    SyntreeNode *parent = node1->parent;
    for (int i = 0; i < parent->num_children; i++) {
        if (parent->children[i]->id == node1->id) {
            parent->children[i] = new_node;
        }
        else if (parent->children[i]->id == node2->id) {
            parent->children[i] = new_node;
        }
    }

    // remove node1 and node2 from their parent's child list
    remove_child(node1);
    remove_child(node2);

    // add new node as child of node1's parent
    add_child(parent, new_node);

    return new_node->id;
}

/**@brief Hängt einen Knoten an das Ende eines Listenknotens.
 * @param self  der Syntaxbaum
 * @param list  Listenknoten
 * @param elem  anzuhängender Knoten
 * @return ID des Listenknoten
 */


/**@brief Hilfsfunktion: Liefert den Knoten mit der angegebenen ID oder NULL, falls er nicht existiert.
 * @param node Ein Knoten, dessen ID überprüft werden soll.
 * @param id Die gesuchte ID.
 * @return Der Knoten mit der gesuchten ID oder NULL, falls er nicht existiert.
 */
static SyntreeNode *get_node_by_id(SyntreeNode *node, SyntreeNodeID id) {
    if (node == NULL) {
        return NULL;
    }
    if (node->id == id) {
        return node;
    }
    for (int i = 0; i < node->num_children; i++) {
        SyntreeNode *child = node->children[i];
        SyntreeNode *result = get_node_by_id(child, id);
        if (result != NULL) {
            return result;
        }
    }
    return NULL;
}

/**@brief Hilfsfunktion: Liefert eine neue, eindeutige Knoten-ID.
 * @param self Der Syntaxbaum.
 * @return Eine neue, eindeutige Knoten-ID.
 */
static SyntreeNodeID get_new_node_id(Syntree *self) {
    SyntreeNodeID id = self->next_node_id;
    self->next_node_id++;
    return id;
}



extern SyntreeNodeID syntreeNodeAppend(Syntree *self, SyntreeNodeID list, SyntreeNodeID elem) {
    // Finde den Knoten mit der ID list
    SyntreeNode *list_node;
    if (!find_node_by_id(self->root, list, &list_node)) {
        return -1; // Fehler: Knoten mit der ID list existiert nicht
    }

    // Wenn die Liste noch keine Kinder hat, erstelle einen neuen Knoten mit einem leeren Array von Kindknoten
    if (list_node->num_children == 0) {
        SyntreeNode *new_node = malloc(sizeof(SyntreeNode));
        new_node->id = get_new_node_id(self);
        new_node->type = NODE_LIST;
        new_node->content.number = 0; // Der Inhalt ist hier nicht relevant
        new_node->num_children = 1;
        new_node->children = malloc(sizeof(SyntreeNode *));
        new_node->children[0] = get_node_by_id(self, elem);
        list_node->children = &new_node;
        return new_node->id;
    }

    // Füge den Knoten elem am Ende des Arrays von Kindknoten an
    SyntreeNode **new_children = realloc(list_node->children, (list_node->num_children + 1) * sizeof(SyntreeNode *));
    if (!new_children) {
        return -1; // Fehler: Kein Speicherplatz mehr verfügbar
    }
    new_children[list_node->num_children] = get_node_by_id(self, elem);
    list_node->children = new_children;
    list_node->num_children++;
    return list_node->id;
}


/**@brief Hängt einen Knoten an den Anfang eines Listenknotens.
 * @param self  der Syntaxbaum
 * @param elem  anzuhängender Knoten
 * @param list  Listenknoten
 * @return ID des Listenknoten
 */
extern SyntreeNodeID syntreeNodePrepend(Syntree *self, SyntreeNodeID elem, SyntreeNodeID list){
    SyntreeNode *node, *list_node;
    if (!find_node_by_id(self->root, list, &list_node)) {
        return -1; // list not found
    }
    if (!get_node_by_id(self, elem, &node)) {
        return -1; // elem not found
    }
    if (node->num_children > 0) {
        return -1; // elem already has children
    }
    SyntreeNode *first_child = list_node->children[0];
    add_child(list_node, node, 0);
    add_child(node, first_child, 1);
    return node->id;
}


/**@brief Gibt alle Zahlenknoten rekursiv (depth-first) aus.
 * @param self  der Syntaxbaum
 * @param root  der Wurzelknoten
 */


static void print_node(const SyntreeNode *node, int depth);

void syntreePrint(const Syntree *self, SyntreeNodeID root) {
    const SyntreeNode *node = find_node_by_id(self->root, root);
    if (node == NULL) {
        printf("Node with ID %d not found!\n", root);
        return;
    }
    print_node(node, 0);
}

static void print_node(const SyntreeNode *node, int depth) {
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    printf("ID: %d, Type: %d", node->id, node->type);
    if (node->type == NODE_TYPE_NUMBER) {
        printf(", Value: %d\n", node->content.number);
    } else if (node->type == type.variable) {
        printf(", Name: %s\n", node->content.variable);
    } else {
        printf("\n");
    }
    for (int i = 0; i < node->num_children; i++) {
        print_node(node->children[i], depth + 1);
    }
}


#endif /* SYNTREE_H_INCLUDED */
