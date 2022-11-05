#include <stdlib.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "treeUtils.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Structs are declared in header and they are defined here to avoid encapsulation.
// Static functions can be used in not static functions. That's why they security level is lower.

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

struct node_
{
    node_ *left;

    elem_t data;

    node_ *right;
} node_;

struct tree_
{
    #ifdef STRUCTCANARY

    canary_t leftCanary; // *((char*) tree_ + 9) = 0 // TODO zachem?

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

tree *treeConstructorFunction (const char const *filename, const int line) // TODO check consts
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

node *treeRootPush (tree *Tree, elem_t element) 
{
    CHECKERROR(Tree != NULL &&
               "Can't push to nullpointer.", 
               NULL);

    CHECKERROR(treeVerifier(Tree) == NOTERROR &&
               "Tree is wrong.",
               NULL);

    Tree->root->data = element;

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

    CHECKERROR(treeVerifier(Tree) == NOTERROR &&
            "Tree is wrong.", // TODO DSL
            NULL);

    return recursiveTreePusher(Tree->root, element);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *pushLeafToNode (tree *Tree, node *Node, elem_t element)
{
    CHECKERROR(Node != NULL && 
               "You are trying to push in nullpointer.", 
               NULL);

    CHECKERROR(treeVerifier(Tree) == NOTERROR &&
        "Tree is wrong.",
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

#define PRINTNODE(Node, file)                                     \
    do                                                            \
    {                                                             \
        fprintf(file, "    Node  pointer: " "%p\n", Node);        \
        fprintf(file, "    Left  pointer: " "%p\n", Node->left);  \
        fprintf(file, "    Right pointer: " "%p\n", Node->right); \
        fprintf(file, "    Element: ");                           \
        elementOutput(Node->data, file);                          \
        fprintf(file, "\n\n");                                    \
    }                                                             \
    while (0)

static void printTree (const node *Node, FILE *file)
{
    PRINTNODE(Node, file);

    if (Node->left  != NULL)
        printTree(Node->left,  file);

    if (Node->right != NULL)
        printTree(Node->right, file);

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR simpleTreeDumpFunction (const tree *Tree, const char *treename, const char *filename, 
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

    CHECKERROR(file     != NULL &&
               "Can't dump with nullpointer to file.", 
               NULLPOINTER);

    ISERROR treeError = treeVerifier(Tree);

    fprintf(file, HTMLBLUE "\nTree dump at %s:%d in %s\n" HTMLRESET 
            HTMLBOLD "Tree %s[%p] " HTMLBOLDRESET, 
            filename, line, function, treename, Tree);

    if (treeError == NOTERROR)
        fprintf(file, HTMLGREEN "(OK)\n" HTMLRESET);

    else
        fprintf(file, HTMLRED "(ERROR: %d)\n" HTMLRESET, (int) treeError);

    #ifdef BIRTHINFO

    fprintf(file, HTMLBOLD "Birth at %s:%ld\n" HTMLBOLDRESET, Tree->birthFile, Tree->birthLine);

    #endif

    fprintf(file, "{\n");

    #ifdef STRUCTCANARY

    fprintf(file, "    Left  struct canary: %llx %s\n    Right struct canary: %llx %s\n", 
            Tree->leftCanary,  (treeError == LEFTCANARY  ? "(DEAD)" : "(OK)"), 
            Tree->rightCanary, (treeError == RIGHTCANARY ? "(DEAD)" : "(OK)"));

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

// Only address of current node or it's parent allow us to distinguish them.
// So I use address for this.

#define DEFINENODE(NodeParent, Node, tmp)                                              \
    do                                                                                 \
    {                                                                                  \
        if (Node != NULL)                                                              \
        {                                                                              \
            fprintf(tmp, "    node%p [shape=box3d, style=\"filled\" fillcolor=\"%s\"," \
                    "fontname=\"Arial\", label = ",                                    \
                    Node, fillColor);                                                  \
            elementOutput(Node->data, tmp);                                            \
            fprintf(tmp, "]\n");                                                       \
            fprintf(tmp, "    node%p -> node%p\n", NodeParent, Node);                  \
        }                                                                              \
    }                                                                                  \
    while (0)

static void recursiveFileFiller (const node *Node, FILE *tmp)
{
    DEFINENODE(Node, Node->left,  tmp);
    DEFINENODE(Node, Node->right, tmp);

    if (Node->left  != NULL)
        recursiveFileFiller(Node->left,  tmp);

    if (Node->right != NULL)
        recursiveFileFiller(Node->right, tmp);
    
    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void fillTemporaryFile (const tree *Tree, FILE *tmp)
{
    fprintf(tmp, 
        "digraph BST\n{\n"
        "    node%p [shape=point];\n", Tree->root);

    recursiveFileFiller(Tree->root, tmp);

    putc('}', tmp);  
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Html output need sources of images.
// => We need to save all dump images.
// => We need a number of current image:
static int dumpNumber = 0;

static ISERROR makeSourcesImages(void)
{
    char *command = (char *) calloc (maxCommandSize, sizeof(char));

    CHECKERROR(command != NULL &&
               "Can't allocate memory for command.",
               ALLOCATIONERROR);

    snprintf(command, maxCommandSize, "mkdir -p %s", dumpDirectory);
    system(command);

    snprintf(command, maxCommandSize, "dot %s -T png -o %sgraph%d.png", 
             tmpFilename, dumpDirectory, dumpNumber);
    system(command);

    free(command);

    return NOTERROR;
}

ISERROR treeDumpFunction (const tree *Tree,     const char *message,
                          const char *treename, const char *filename, 
                          const int   line,     const char *function, 
                          FILE *file)
{
    CHECKERROR(Tree     != NULL && 
               "Can't dump nullpointer.", 
               NULLPOINTER);

    CHECKERROR(message  != NULL && 
               "Can't dump with nullpointer to message.", 
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

    CHECKERROR(file     != NULL &&
               "Can't dump with nullpointer to file.", 
               NULLPOINTER);

    fprintf(file, 
            "\n<hr>\n<pre>\n"
            "<h2>%s</h2>\n"
            VERDANA, 
            message);

    ISERROR treeError = simpleTreeDumpFunction(Tree, treename, filename,
                                               line, function, file);
    fprintf(file, "</p>\n");

    FILE *tmp = fopen(tmpFilename, "w");

    CHECKERROR(tmp != NULL &&
               "Can't open temporary file for graph dump.",
               ALLOCATIONERROR);

    fillTemporaryFile(Tree, tmp);
    fclose(tmp);

    CHECKERROR(makeSourcesImages() == NOTERROR, ERROR);

    fprintf(file, "<img src = %sgraph%d.png>\n", dumpDirectory, dumpNumber);
    dumpNumber++;

    fprintf(file, "</hr>\n</pre>\n");

    return treeError;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
