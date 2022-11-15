#include <stdlib.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "treeUtils.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Structs are declared in header and they are defined here to avoid encapsulation.
// Static functions can be used in not static functions. That's why they security level is lower.

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

struct node_
{
    node_ *parent;

    node_ *left;

    elem_t data;

    node_ *right;
} node_;

struct tree_
{
    node *root;

    #ifdef BIRTHINFO

    size_t      birthLine;
    const char *birthFile;

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

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *nodeConstructor (elem_t element, node *parent)
{                                          
    node *Node = (node *) calloc (1, sizeof(node));

    CHECKERROR(Node != NULL && 
               "Can't allocate memory for node.", 
               NULL); 

    Node->parent = parent;
    Node->data   = element;

    Node->left  = NULL;                                                   
    Node->right = NULL;

    return Node;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

tree *treeConstructorFunction (const char *filename, const int line)
{
    tree *Tree = (tree *) calloc (1, sizeof(tree));

    CHECKERROR(Tree != NULL && 
               "Can't allocate memory for tree.",
               NULL);

    Tree->root = nodeConstructor(rootPoison, NULL);

    CHECKERROR(Tree->root != NULL, NULL);

    #ifdef BIRTHINFO

    Tree->birthFile = filename;
    Tree->birthLine = line;

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

    Node->parent = NULL;
    Node->left   = NULL;
    Node->right  = NULL;

    free(Node);
    Node = NULL;

    return;
}

void treeDestructor (tree *Tree)
{
    CHECKERROR(Tree != NULL && 
               "You are trying to destroy nullpointer.", 
               (void) NULL);

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

node *nodeParent (node *Node)
{
    CHECKERROR(Node != NULL &&
               "Can't execute parent node of nullpointer.", 
               NULL);

    return Node->parent;
}

node *leftNode (node *Node)
{
    CHECKERROR(Node != NULL &&
               "Can't execute left node of nullpointer.", 
               NULL);

    return Node->left;
}

node *rightNode (node *Node)
{
    CHECKERROR(Node != NULL &&
               "Can't execute right node of nullpointer.", 
               NULL);

    return Node->right;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

elem_t nodeData (node *Node)
{
    CHECKERROR(Node != NULL &&
               "Can't execute data of nullpointer.", 
               NULL);

    return Node->data;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *insertRoot (tree *Tree, elem_t element) 
{
    CHECKERROR(Tree != NULL &&
               "Can't push to nullpointer.", 
               NULL);

    CHECKERROR(treeVerifier(Tree) == NOTERROR   &&
               "Tree is wrong.",
               NULL);

    CHECKWARNING(Tree->root->data == rootPoison &&
                 "Tree already contains data in root.");

    Tree->root->data = element;

    return Tree->root;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *insertLeftLeaf (node *Node, elem_t element)
{
    CHECKERROR(Node->left == NULL &&
               "Can't push a value in left node, because it already contains it.",
               NULL);

    Node->left = nodeConstructor(element, Node);

    return Node->left;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *insertRightLeaf (node *Node, elem_t element)
{
    CHECKERROR(Node->right == NULL &&
               "Can't push a value in right node, because it already contains it.",
               NULL);

    Node->right = nodeConstructor(element, Node);

    return Node->right;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define CREATENODEANDEXIT(Node, element, parent) \
    do                                           \
    {                                            \
        Node = nodeConstructor(element, parent); \
        return Node;                             \
    }                                            \
    while (0)
    
static node *recursiveTreePusher (node *Node, elem_t element)
{
    if (Node->left == NULL && Node->right == NULL) // We find a leaf.
    {
        // Default push to left.

        CREATENODEANDEXIT(Node->left, element, Node);
    }

    else if (Node->left  != NULL && Node->right == NULL)
    {
        // 1) If element < Node->left->data, recursion to Node->left.
        // 2) Else create new node on Node->right.

        if (element < Node->left->data)
            return recursiveTreePusher(Node->left, element);

        else
            CREATENODEANDEXIT(Node->right, element, Node);
    }

    else if (Node->right != NULL && Node->left  == NULL)
    {
        // 1) If element > Node->right->data, recursion to Node->right.
        // 2) Else create new node on Node->left.

        if (element < Node->right->data)
            return recursiveTreePusher(Node->right, element);

        else
            CREATENODEANDEXIT(Node->left, element, Node);
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
               "Tree is wrong.",
               NULL);

    return recursiveTreePusher(Tree->root, element);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR pushRootToTree (tree *Tree, node *Root)
{
    CHECKERROR(Tree != NULL && 
               "You are trying to push in nullpointer.", 
               NULLPOINTER);

    CHECKERROR(Root != NULL &&
               "You can't initialize root pointer as NULL.",
               NULLPOINTER);

    CHECKERROR(treeVerifier(Tree) == NOTERROR &&
               "Tree is wrong.",
               NULLPOINTER);

    node *leftSubtree  = Tree->root->left;
    node *rightSubtree = Tree->root->right;

    free(Tree->root);
    Tree->root = Root;

    if (leftSubtree  != NULL)
        leftSubtree->parent  = Root;
        
    if (rightSubtree != NULL)
        rightSubtree->parent = Root;

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR pushLeafToNode (node *Node, node *Leaf)
{
    CHECKERROR(Node != NULL && 
               "You are trying to push in nullpointer.", 
               NULLPOINTER);

    CHECKERROR(Leaf != NULL && 
               "You are trying to push in nullpointer.", 
               NULLPOINTER);

    if (Node->left == NULL)
        Node->left  = Leaf;

    else if (Node->right == NULL)
        Node->right = Leaf;
    
    else
        CHECKERROR(NULL &&
                   "Both nodes aren't NULL. Can't push.",
                   ERROR);

    return NOTERROR;    
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Functions for output for element of type elem_t.
// We don't know type of element (because it's typedef);
// We don't know printf specificator;
// That's why we need function overload.

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void elementOutput (const char element,    FILE *file) 
{
    fprintf(file, "%c", element);

    return;
}

static void elementOutput (const short element,   FILE *file) 
{
    fprintf(file, "%hd", element);

    return;
}

static void elementOutput (const int element,     FILE *file)
{
    fprintf(file, "%d", element);

    return;
}

static void elementOutput (const long element,    FILE *file)
{
    fprintf(file, "%ld", element);

    return;
}

static void elementOutput (const double element,  FILE *file)
{
    fprintf(file, "%lg", element);

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// If required specificator not found.

static void elementOutput (const void *element, FILE *file) 
{
    PUTWARNING("Unsupported type!");

    fprintf(file, "ERR");

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void recursivePreorderPrintTree (const node *Node, FILE *file)
{
    putc('{', file);

    elementOutput(Node->data, file);

    if (Node->left  != NULL)
        recursivePreorderPrintTree(Node->left,  file);

    if (Node->right != NULL)
        recursivePreorderPrintTree(Node->right, file);

    putc('}', file);

    return;
}

ISERROR preorderPrintTree (tree *Tree, FILE *file)
{
    CHECKERROR(Tree != NULL &&
               "Can't print nullpointer.",
               NULLPOINTER);

    CHECKERROR(treeVerifier(Tree) == NOTERROR &&
               "Tree is wrong.",
               WRONGTREE);

    recursivePreorderPrintTree(Tree->root, file);

    putc('\n', file);

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define PRINTNODE(Node, file)                                       \
    do                                                              \
    {                                                               \
        fprintf(file, "    Node   pointer: " "%p\n", Node);         \
        fprintf(file, "    Parent pointer: " "%p\n", Node->parent); \
        fprintf(file, "    Left   pointer: " "%p\n", Node->left);   \
        fprintf(file, "    Right  pointer: " "%p\n", Node->right);  \
        fprintf(file, "    Element: ");                             \
        elementOutput(Node->data, file);                            \
        fprintf(file, "\n\n");                                      \
    }                                                               \
    while (0)


static void recursivePrintTree (const node *Node, FILE *file)
{
    PRINTNODE(Node, file);

    if (Node->left  != NULL)
        recursivePrintTree(Node->left,  file);

    if (Node->right != NULL)
        recursivePrintTree(Node->right, file);

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

    putc('\n', file);

    if (Tree->root)
        recursivePrintTree(Tree->root, file);

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
    if (Tree->root->data == rootPoison)
        fprintf(tmp, 
            "digraph BST\n{\n"
            "    node%p [shape=point];\n", Tree->root);

    else
    {
        fprintf(tmp, 
            "digraph BST\n{\n"
                "    node%p [shape=box3d, style=\"filled\" fillcolor=\"%s\","
                "fontname=\"Arial\", label = ",
                Tree->root, fillColor);
        
        elementOutput(Tree->root->data, tmp);
        fprintf(tmp, "]\n");
    }

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

    snprintf(command, maxCommandSize, "mkdir -p ./TreeDumpSources/");
    system(command);

    snprintf(command, maxCommandSize, "dot tmp.txt -T png -o ./TreeDumpSources/graph%d.png", dumpNumber);
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

    FILE *tmp = fopen("tmp.txt", "w");

    CHECKERROR(tmp != NULL &&
               "Can't open temporary file for graph dump.",
               ALLOCATIONERROR);

    fillTemporaryFile(Tree, tmp);
    fclose(tmp);

    CHECKERROR(makeSourcesImages() == NOTERROR, ERROR);

    fprintf(file, "<img src = ./TreeDumpSources/graph%d.png>\n", dumpNumber);
    dumpNumber++;

    system("rm -rf tmp.txt");

    fprintf(file, "</hr>\n</pre>\n");

    return treeError;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
