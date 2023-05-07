#ifndef SYNTREE_H_INCLUDED
#define SYNTREE_H_INCLUDED

/* *** Strukturen *********************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct SyntreeNode {
    int value;
    struct SyntreeNode **children;
    size_t num_children;
} SyntreeNode;

typedef /* muss noch definiert werden */ int SyntreeNodeID;
/**@brief Struktur des abstrakten Syntaxbaumes.
 */

typedef struct {
     /* hier sollte noch etwas dazu kommen */
    SyntreeNode *root;
} Syntree;


/* *** öffentliche Schnittstelle ******************************************** */

/**@brief Initialisiert einen neuen Syntaxbaum.
 * @param self  der zu initialisierende Syntaxbaum
 * @return 0, falls keine Fehler bei der Initialisierung aufgetreten sind,
 *      != 0 ansonsten
 */
extern int
syntreeInit(Syntree *self){
    if (self == NULL) {
        return -1; 
    }
    self->root = NULL;
    return 0;
};

/**@brief Gibt den Syntaxbaum und alle assoziierten Strukturen frei.
 * @param self  der freizugebende Syntaxbaum
 */
 void releaseNode(SyntreeNode *node) {
    if (node == NULL) {
        return;
    }
    // Freigabe aller Kinder des Knotens
    for (size_t i = 0; i < node->num_children; i++) {
        releaseNode(node->children[i]);
    }
    // Freigabe des aktuellen Knotens
    free(node->children);
    free(node);
}


extern void syntreeRelease(Syntree *self) {
    releaseNode(self->root);
    self->root = NULL; // Setzen des Wurzelzeigers auf NULL
}


/**@brief Erstellt einen neuen Knoten mit einem Zahlenwert als Inhalt.
 * @param self    der Syntaxbaum
 * @param number  die Zahl
 * @return ID des neu erstellten Knoten
 */
extern  SyntreeNodeID syntreeNodeNumber(Syntree *self, int number) {
    SyntreeNode *newNode = (SyntreeNode *) malloc(sizeof(SyntreeNode));
    if (newNode == NULL) {
        return -1;
    }
    newNode->value = number;
    newNode->num_children = 0;
    newNode->children = NULL;

    self->root = newNode;

    return 0;
}

SyntreeNodeID syntreeNodeAdd(Syntree *self, SyntreeNode *node) {
    if (self->root == NULL) {
        // If the tree is empty, set the new node as the root
        self->root = node;
        return 0; // The ID of the root node is always 0
    } else {
        // Otherwise, add the new node as a child of an existing node
        SyntreeNode *parent = self->root;
        int index = node->value < parent->value ? 0 : parent->num_children;
        while (index < parent->num_children && node->value > parent->children[index]->value) {
            index++;
        }
        // Insert the new node at the correct position in the children array
        parent->num_children++;
        parent->children = realloc(parent->children, parent->num_children * sizeof(SyntreeNode *));
        for (int i = parent->num_children - 1; i > index; i--) {
            parent->children[i] = parent->children[i - 1];
        }
        parent->children[index] = node;
        return index;
    }
}

SyntreeNode *syntreeNodeGet(Syntree *self, SyntreeNodeID id) {
    if (self->root == NULL || id == 0) {
        // If the tree is empty or the ID is 0 (which corresponds to the root node),
        // return the root node
        return self->root;
    } else {
        // Otherwise, search for the node with the given ID
        SyntreeNode *parent = self->root;
        SyntreeNode *node = NULL;
        int index = id;
        while (parent != NULL && index >= parent->num_children) {
            // Subtract the number of children of each ancestor from the index
            index -= parent->num_children;
            parent = parent->num_children > 0 ? parent->children[parent->num_children - 1] : NULL;
        }
        if (parent != NULL) {
            node = parent->children[index];
        }
        return node;
    }
}

/**@brief Kapselt einen Knoten innerhalb eines anderen Knotens.
 * @param self  der Syntaxbaum
 * @param id    der zu kapselnde Knoten
 * @return ID des neu erstellten Knoten

 

 */
extern SyntreeNodeID syntreeNodeTag(Syntree *self, SyntreeNodeID id) {
    SyntreeNode *parent = malloc(sizeof(SyntreeNode));
    parent->num_children = 1;
    parent->children = malloc(sizeof(SyntreeNode *));
    parent->children[0] = syntreeNodeGet(self, id);
    parent->value = -1; // set a dummy value for the parent node

    // Add the new parent node to the tree and return its ID
    return syntreeNodeAdd(self, parent);
}


/**@brief Kapselt zwei Knoten innerhalb eines Knoten.
 * @param self  der Syntaxbaum
 * @param id1   erster Knoten
 * @param id2   zweiter Knoten
 * @return ID des neu erstellten Knoten
 */



SyntreeNode *syntreeGetNode(const Syntree *self, SyntreeNodeID id1) {
    if (id1 == -1) {
        return NULL;
    }
    return self->root->children[id1];
}

SyntreeNode *syntreeNodeNew() {
    SyntreeNode *new_node = (SyntreeNode *)malloc(sizeof(SyntreeNode));
    if (new_node == NULL) {
        perror("Error: Memory allocation for new node failed");
        exit(EXIT_FAILURE);
    }
    new_node->value = 0;
    new_node->num_children = 0;
    new_node->children = NULL;
    return new_node;
}






extern SyntreeNodeID syntreeNodePair(Syntree *self, SyntreeNodeID id1, SyntreeNodeID id2) {
    SyntreeNode *node1 = syntreeGetNode(self, id1);
    SyntreeNode *node2 = syntreeGetNode(self, id2);
    SyntreeNode *new_node = syntreeNodeNew();
    new_node->num_children = 2;
    new_node->children = (SyntreeNode **) malloc(2 * sizeof(SyntreeNode *));
    new_node->children[0] = node1;
    new_node->children[1] = node2;
    self->root = new_node;
    return 0;  // assume the ID of the new node is always 0
}




/**@brief Hängt einen Knoten an das Ende eines Listenknotens.
 * @param self  der Syntaxbaum
 * @param list  Listenknoten
 * @param elem  anzuhängender Knoten
 * @return ID des Listenknoten
 */
extern SyntreeNodeID syntreeNodeAppend(Syntree *self, SyntreeNodeID list, SyntreeNodeID elem) {
    SyntreeNode *list_node = syntreeGetNode(self, list);
    

    SyntreeNode *elem_node = syntreeGetNode(self, elem);
    if (elem_node == NULL) {
        return -1;  // Error: element node not found
    }

    // Increase the size of the children array in the list node by 1
    size_t new_size = list_node->num_children + 1;
    list_node->children = realloc(list_node->children, new_size * sizeof(SyntreeNode *));
    if (list_node->children == NULL) {
        return -1;  // Error: memory allocation failed
    }

    // Add the element node to the end of the children array in the list node
    list_node->children[new_size - 1] = elem_node;
    list_node->num_children = new_size;

    return elem;
}


/**@brief Hängt einen Knoten an den Anfang eines Listenknotens.
 * @param self  der Syntaxbaum
 * @param elem  anzuhängender Knoten
 * @param list  Listenknoten
 * @return ID des Listenknoten
 */





extern SyntreeNodeID syntreeNodePrepend(Syntree *self, SyntreeNodeID elem, SyntreeNodeID list) {
    // get the node for the given element and list IDs
    SyntreeNode *elem_node = syntreeGetNode(self, elem);
    SyntreeNode *list_node = syntreeGetNode(self, list);

    // create a new list node and set its child to the given element node
    SyntreeNode *new_list_node = syntreeNodeNew();
    new_list_node->num_children = 1;
    new_list_node->children = malloc(sizeof(SyntreeNode *));
    new_list_node->children[0] = elem_node;

    // if the original list node has no children, set its child to the new list node
    if (list_node->num_children == 0) {
        list_node->num_children = 1;
        list_node->children = malloc(sizeof(SyntreeNode *));
        list_node->children[0] = new_list_node;
    }
    // otherwise, insert the new list node at the beginning of the original list node's children
    else {
        size_t new_num_children = list_node->num_children + 1;
        SyntreeNode **new_children = malloc(new_num_children * sizeof(SyntreeNode *));
        new_children[0] = new_list_node;
        memcpy(&new_children[1], list_node->children, list_node->num_children * sizeof(SyntreeNode *));
        free(list_node->children);
        list_node->children = new_children;
        list_node->num_children = new_num_children;
    }

    // return the ID of the new list node
    return syntreeNodeAdd(self, new_list_node);
}


/**@brief Gibt alle Zahlenknoten rekursiv (depth-first) aus.
 * @param self  der Syntaxbaum
 * @param root  der Wurzelknoten
 */

 
void printNode(const SyntreeNode *node, int depth) {
    if (node == NULL) {
        return;
    }

    // Print the current node's value and indent appropriately
    printf("%*s%d\n", depth * 4, "", node->value);

    // Recursively print all of the node's children
    for (size_t i = 0; i < node->num_children; i++) {
        printNode(node->children[i], depth + 1);
    }
}

void syntreePrint(const Syntree *self, SyntreeNodeID root) {
    // Find the root node based on its ID
    SyntreeNode *node = self->root;
    while (node != NULL && root != 0) {
        if (node - self->root == root - 1) {
            break;
        }
        node++;
    }

    // Print the subtree rooted at the root node
    printNode(node, 0);
}




#endif /* SYNTREE_H_INCLUDED */  
