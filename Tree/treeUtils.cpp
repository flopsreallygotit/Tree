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

    recursiveTreeDestroyer(Tree->root);

    free(Tree);
    Tree = NULL;

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *treeRoot (tree *Tree)
{
    CHECKERROR(Tree != NULL &&
               "Can't execute root of nullpointer.", 
               NULL);

    return Tree->root;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define CREATENODEANDEXIT(Node, element) \
    do                                   \
    {                                    \
        Node = nodeConstructor(element); \
        return Node;                     \
    }                                    \
    while (0)
    
static node *recursiveTreePusher (node *Node, elem_t element)
{
    if (Node->left == NULL && Node->right == NULL) // We find a leaf.
    {
        // Default push to left.

        CREATENODEANDEXIT(Node->left,  element);
    }

    else if (Node->left  != NULL && Node->right == NULL)
    {
        // 1) If element < Node->left->data, recursion to Node->left.
        // 2) Else create new node on Node->right.

        if (element < Node->left->data)
            return recursiveTreePusher(Node->left, element);

        else
            CREATENODEANDEXIT(Node->right, element);
    }

    else if (Node->right != NULL && Node->left  == NULL)
    {
        // 1) If element > Node->right->data, recursion to Node->right.
        // 2) Else create new node on Node->left.

        if (element < Node->right->data)
            return recursiveTreePusher(Node->right, element);

        else
            CREATENODEANDEXIT(Node->left, element);
    }

    else // Node->left != NULL && Node->right != NULL
    {
        // 1) If element > Node->left->data, recursion to Node->right.
        // 2) Else recursion to Node->left.

        if (element > Node->left->data)
            return recursiveTreePusher(Node->right, element);

        else
            return recursiveTreePusher(Node->left,  element);
    }

    return NULL;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *treeInsert (tree *Tree, elem_t element)
{
    CHECKERROR(Tree != NULL && 
               "You are trying to insert in nullpointer.", 
               NULL);

    CHECKERROR(isfinite(element) && 
               "You are trying to insert not finite value in tree. It's not allowed.", 
               NULL);

    return recursiveTreePusher(Tree->root, element);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *pushLeafToNode (node *Node, elem_t element)
{
    CHECKERROR(Node != NULL && 
               "You are trying to push in nullpointer.", 
               NULL);

    CHECKERROR(isfinite(element) && 
               "You are trying to insert not finite value in tree. It's not allowed", 
               NULL);

    return recursiveTreePusher(Node, element);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Functions for output for element of type elem_t.
// We don't know type of element (because it's typedef);
// We don't know printf specificator;
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

static void elementOutput (const void *element,  FILE *file) 
{
    const elem_t Element = *(const elem_t *) element;

    fprintf(file, "%x", Element);

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define printNode(Node, file)                                                \
    do                                                                       \
    {                                                                        \
        fprintf(file, BOLD "    Node  pointer: " RESET "%p\n", Node);        \
        fprintf(file, BOLD "    Left  pointer: " RESET "%p\n", Node->left);  \
        fprintf(file, BOLD "    Right pointer: " RESET "%p\n", Node->right); \
        fprintf(file, BOLD "    Element: " RESET);                           \
        elementOutput(Node->data, file);                                     \
        fprintf(file, "\n\n");                                               \
    }                                                                        \
    while (0)

static void printTree (const node *Node, FILE *file)
{
    printNode(Node, file);

    if (Node->left  != NULL)
        printTree(Node->left,  file);

    if (Node->right != NULL)
        printTree(Node->right, file);

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

    putc('\n', file);

    if (Tree->root)
        printTree(Tree->root, file);

    else
        fprintf(file, "    Tree hasn't root.\n");

    fprintf(file, "}\n\n");

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
