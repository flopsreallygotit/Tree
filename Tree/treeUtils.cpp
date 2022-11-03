#include <math.h>
#include <malloc.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "treeUtils.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Structs are declaredin header and they are defined here to avoid encapsulation.
// Static functions can be used in not static functions. That's why they security level is lower.

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

struct node_
{
    node_ *left;

    elem_t data;

    node_ *right;
}node_;

struct tree_
{
    #ifdef STRUCTCANARY

    canary_t leftCanary;

    #endif

    node *root;

    #ifdef BIRTHINFO

    size_t      birthLine;
    const char *birthFile;

    #endif

    #ifdef STRUCTCANARY

    canary_t rightCanary;

    #endif
}tree_;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR treeVerifier (const tree *Tree)
{
    if (Tree->root == NULL)
    {
        PUTERROR("Memory for tree is allocated, but root is nullpointer");

        return WRONGTREE;
    }

    #ifdef STRUCTCANARY

    if (Tree->leftCanary  != Canary1)
    {
        PUTERROR("Left canary is dead");

        return LEFTCANARY;
    }

    if (Tree->rightCanary != Canary2)
    {
        PUTERROR("Right canary is dead");

        return RIGHTCANARY;
    }

    #endif

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static node *nodeConstructor (elem_t element)
{                                          
    node *Node = (node *) calloc (1, sizeof(node));

    CHECKERROR(Node != NULL && "Can't allocate memory for node.", NULL); 

    Node->data  = element;

    Node->left  = NULL;                                                   
    Node->right = NULL;

    return Node;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

tree *treeConstructorFunction (const char *filename, int line)
{
    tree *Tree = (tree *)   calloc (1, sizeof(tree));

    CHECKERROR(Tree != NULL && "Can't allocate memory for tree.", NULL);

    Tree->root = nodeConstructor(rootPoison);

    CHECKERROR(Tree->root != NULL, NULL);

    #ifdef BIRTHINFO

    Tree->birthFile = filename;
    Tree->birthLine = line;

    #endif

    #ifdef STRUCTCANARY

    Tree->leftCanary  = Canary1;
    Tree->rightCanary = Canary2;

    #endif

    return Tree;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void recursiveTreeDestroyer (node *Node)
{
    if (Node == NULL)
        return;

    if (Node->left  != NULL)
        recursiveTreeDestroyer(Node->left);
    
    if (Node->right != NULL)
        recursiveTreeDestroyer(Node->right);

    free(Node);
    Node = NULL;

    return;
}

void treeDestructor (tree *Tree)
{
    CHECKERROR(Tree != NULL && "You are trying to destroy nullpointer.", (void) NULL);
    CHECKERROR(treeVerifier(Tree) == NOTERROR && "Tree is wrong.",       (void) NULL);

    recursiveTreeDestroyer(Tree->root);

    free(Tree);
    Tree = NULL;

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define CREATENODEANDEXIT(Node, element)           \
    do                                             \
    {                                              \
        Node = nodeConstructor(element);           \
        CHECKERROR(Node != NULL, ALLOCATIONERROR); \
        return NOTERROR;                           \
    }                                              \
    while (0)
    
static ISERROR recursiveTreePusher (node *Node, elem_t element)
{
    if (Node->left == NULL && Node->right == NULL) // We find a leaf.
    {
        // If default push mode is set to left than create new node on Node->left, else on Node->right.

        if (pushMode == LEFT)
            CREATENODEANDEXIT(Node->left,  element);

        else
            CREATENODEANDEXIT(Node->right, element);

        return NOTERROR;
    }

    else if (Node->left != NULL) // && Node->right == NULL.
    {
        // 1) If element < Node->left->data, recursion to Node->left.
        // 2) Else create new node on Node->right.

        if (element < Node->left->data)
            return recursiveTreePusher(Node->left, element);

        else
            CREATENODEANDEXIT(Node->right, element);
    }

    else if (Node->right != NULL) // && Node->left == NULL.
    {
        // 1) If element > Node->right->data, recursion to Node->right.
        // 2) Else create new node on Node->left.

        if (element < Node->right->data)
            return recursiveTreePusher(Node->right, element);

        else
            CREATENODEANDEXIT(Node->left, element);
    }

    else // Node->left != NULL && Node->right != NULL.
    {
        // 1) If element > Node->left->data, recursion to Node->right.
        // 2) Else recursion to Node->left.

        if (element > Node->left->data)
            return recursiveTreePusher(Node->right, element);
        else
            return recursiveTreePusher(Node->left,  element);
    }

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ISERROR pushLeafToNode (node *Node, elem_t element)
// {
//     CHECKERROR(Node != NULL && 
//                "You are trying to push in nullpointer.", 
//                NULLPOINTER);

//     CHECKERROR(isfinite(element) && 
//                "You are trying to insert not finite value in tree. It's not allowed", 
//                ISNOTFINITE);

//     recursiveTreePusher(Node, element);

//     return NOTERROR;
// }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR treeInsert (tree *Tree, elem_t element)
{
    CHECKERROR(Tree != NULL && 
               "You are trying to insert in nullpointer.", 
               NULLPOINTER);

    CHECKERROR(isfinite(element) && 
               "You are trying to insert not finite value in tree. It's not allowed.", 
               ISNOTFINITE);

    recursiveTreePusher(Tree->root, element);

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Functions for output for element of type elem_t.
// We don't know type of element (because it's typedef);
// We don't know printf specifier;
// That's why we need function overload.

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void elementOutput (const int element,    FILE *file)
{
    fprintf(file, "%d", element);

    return;
}

static void elementOutput (const double element, FILE *file)
{
    fprintf(file, "%lg", element);

    return;
}

static void elementOutput (const char *element,  FILE *file)
{
    fprintf(file, "%s", element);

    return;
}

static void elementOutput (const char element,   FILE *file)
{
    fprintf(file, "%c", element);

    return;
}

static void elementOutput (const long element,   FILE *file)
{
    fprintf(file, "%ld", element);

    return;
}

static void elementOutput (const short element,  FILE *file) 
{
    fprintf(file, "%hd", element);

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// If required specificator not found, element outputs in hex.

static void elementOutput (const void *element) 
{
    const elem_t Element = *(const elem_t *) element;

    printf("%x", Element);

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void printTree (const node *Node, FILE *file)
{
    if (Node->left  != NULL)
    {
        putc('(', file);
        return printTree(Node->left,  file);
        putc(')', file);
    }

    elementOutput(Node->data, file);
    putc(' ', file);

    if (Node->right != NULL)
    {
        putc('(', file);
        return printTree(Node->right, file);
        putc(')', file);
    }

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR simpleTreeDumpFunction (const tree *Tree, const char *treename, const char *filename, 
                                const int   line, const char *function, FILE *file)
{
    CHECKERROR(Tree     != NULL && 
               "Can't dump nullpointer.", 
               NULLPOINTER);

    CHECKERROR(treename != NULL &&
               "Can't dump with nullpointer to treename.",
               NULLPOINTER);

    CHECKERROR(filename != NULL &&
               "Can't dump with nullpointer to filename.",
               NULLPOINTER);

    CHECKERROR(function != NULL &&
               "Can't dump with nullpointer to function.", 
               NULLPOINTER);

    ISERROR treeError = treeVerifier(Tree);

    fprintf(file, BOLD MAGENTA "\nTree dump at %s:%d in %s\n" RESET BOLD "Tree %s[%p] " RESET, 
            filename, line, function, treename, Tree);

    if (treeError == NOTERROR)
        fprintf(file, BOLD GREEN "(OK)\n" RESET);

    else
        fprintf(file, BOLD RED "(ERROR: %d)\n" RESET, (int) treeError);

    #ifdef BIRTHINFO

    fprintf(file, BOLD "Birth at %s:%ld\n" RESET, Tree->birthFile, Tree->birthLine);

    #endif

    printf("{\n");

    #ifdef STRUCTCANARY

    fprintf(file, BOLD "    Left  struct canary: %llx %s\n    Right struct canary: %llx %s\n" RESET, 
            Tree->leftCanary,  (treeError == LEFTCANARY  ? RED "(DEAD)" RESET BOLD : GREEN "(OK)" RESET BOLD), 
            Tree->rightCanary, (treeError == RIGHTCANARY ? RED "(DEAD)" RESET      : GREEN "(OK)" RESET     ));

    #endif

    if (Tree->root)
    {
        fprintf(file, BOLD "    Print of tree:\n    " RESET);

        printTree(Tree->root, file);

        putc(')', file);
    }

    else
        fprintf(file, "    Tree hasn't root.");

    fprintf(file, "\n}\n\n");

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// TODO remove
// TODO recursive printTree

int main ()
{
    tree Tree2 = {0};
    simpleTreeDump(&Tree2, stdout);

    tree *Tree1 = treeConstructor;
    simpleTreeDump(Tree1, stdout);

    treeInsert(Tree1, 10);
    treeInsert(Tree1, 11);

    simpleTreeDump(Tree1, stdout);

    treeDestructor(Tree1);

    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
