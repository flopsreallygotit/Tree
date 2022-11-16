#include "treeUtils.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Functions for output for element of type elem_t.
// We don't know type of element (because it's typedef);
// We don't know printf specificator;
// That's why we need function overload.

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void elementOutput (const char element,     FILE *file) 
{
    fprintf(file, "%c", element);

    return;
}

static void elementOutput (const short element,    FILE *file) 
{
    fprintf(file, "%hd", element);

    return;
}

static void elementOutput (const int element,      FILE *file)
{
    fprintf(file, "%d", element);

    return;
}

static void elementOutput (const long element,     FILE *file)
{
    fprintf(file, "%ld", element);

    return;
}

static void elementOutput (const double element,   FILE *file)
{
    fprintf(file, "%lg", element);

    return;
}

static void elementOutput (const wchar_t *element, FILE *file)
{
    fprintf(file, "\"%ls\"", element);

    return;
}

static void elementOutput (const char *element,    FILE *file)
{
    fprintf(file, "\"%s\"", element);

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

    fprintf(file, HTMLBLUE "\nTree dump at %s:%d in %s\n" HTMLRESET 
            HTMLBOLD "Tree %s[%p] " HTMLBOLDRESET, 
            filename, line, function, treename, Tree);

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

    fprintf(tmp, 
            "digraph BST\n{\n"
            "    node%p [shape=box3d, style=\"filled\" fillcolor=\"%s\","
            "fontname=\"Arial\", label = ",
            Tree->root, fillColor);
    
    elementOutput(Tree->root->data, tmp);
    fprintf(tmp, "]\n");
    

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
