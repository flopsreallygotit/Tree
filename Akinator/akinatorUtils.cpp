#include <sys/stat.h>
#include <stdlib.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "akinatorUtils.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// TODO Tree pushes.

static node *nodeConstructor (wchar_t *Data, node *Parent)
{
    node *Node = (node *) calloc (1, sizeof(node));

    CHECKERROR(Node != NULL && 
               "Can't allocate memory for node.", 
               NULL); 

    Node->parent = Parent;
    Node->left   = NULL;                                                   
    Node->right  = NULL;

    if (Data != NULL)
    {
        Node->data = (elem_t) calloc (wcslen(Data) + 1, sizeof(wchar_t));

        CHECKERROR(Node->data != NULL &&
                "Can't allocate memory for data of node.",
                NULL);

        wcscpy(Node->data, Data);
    }

    return Node;
}

static void subtreeDestructor (node *Node)
{
    node *leftSubtree  = Node->left;
    node *rightSubtree = Node->right;

    free(Node->data);
    Node->data = NULL;

    Node->left   = NULL;
    Node->right  = NULL;
    Node->parent = NULL;

    free(Node);
    Node = NULL;

    if (leftSubtree  != NULL)
        subtreeDestructor(leftSubtree);
    
    if (rightSubtree != NULL)
        subtreeDestructor(rightSubtree);
    
    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static tree *treeConstructor (void)
{
    tree *Tree = (tree *) calloc (1, sizeof(tree));

    CHECKERROR(Tree != NULL && 
               "Can't allocate memory for tree.",
               NULL);

    Tree->root = nodeConstructor(NULL, NULL);

    CHECKERROR(Tree->root != NULL, NULL);

    return Tree;
}

void treeDestructor (tree *Tree)
{
    CHECKERROR(Tree != NULL && 
               "Can't destruct nullpointer.",
               (void) NULL);

    if (Tree->root != NULL)
        subtreeDestructor(Tree->root);

    free(Tree);
    Tree = NULL;

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

const char *simpleCommandLineParser (const int argc, const char *argv[])
{
    CHECKERROR(argv != NULL &&
               "Argv pointer can't be NULL.",
               NULL);

    CHECKERROR(argc != 1 &&
               "You haven't entered file name."
               "Example: ./main base.txt", 
               NULL);

    CHECKWARNING(argc == 2 &&
                 "You have entered more than two arguments."
                 "Only second argument is taken as the filename.");

    return argv[1];
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void skipBadSymbols (FILE *file)
{
    // We don't need to skip bad symbols if syntax is strict.
    if (strictBaseSyntax)
        return;

    int symbol = getc(file);
    while (symbol != '{' && symbol != '"' && 
           symbol != '}' && symbol != EOF)
        symbol = getc(file);

    ungetc(symbol, file); 
    
    // For example: we skipped all bad symbols, but last symbol from stream is '{', '}', '"'.
    // So we need to put it back to stream.

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node* nodeReadFromFile (node *parentNode, size_t *nodeCount, FILE *file)
{   
    int firstSymbol = fgetc(file);
    
    if ((char) firstSymbol == '{')
    {
        wchar_t *element = NULL;

        if (fscanf(file, "%ml[^{}]", &element) != 0)
        {
            node *currentNode = nodeConstructor(element, parentNode);

            (*nodeCount)++;

            int lastSymbol = fgetc(file);

            // TODO subtree destructor.

            if ((char) lastSymbol == '}')
                goto freeAndReturn;
            
            ungetc(lastSymbol, file);

            // Pushing to left  subtree.
            pushLeafToNode(currentNode, nodeReadFromFile(currentNode, nodeCount, file));

            // Pushing to right subtree.
            pushLeafToNode(currentNode, nodeReadFromFile(currentNode, nodeCount, file));

            CHECKERROR(fgetc(file) == '}' &&
                       "Incorrect bracket sequence.",
                       NULL);

        freeAndReturn:
            free(element);
            element = NULL;
            return currentNode;
        }
    }
  
    return NULL;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR parseFile (tree *Tree, FILE *file)
{
    size_t nodeCount = 0;

    pushRootToTree(Tree, nodeReadFromFile(NULL, &nodeCount, file));

    fclose(file);

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

tree *openAndParseFile (const char *filename)
{
    CHECKERROR(filename != NULL &&
               "Can't open file with nullpointer to filename.",
               NULL);

    FILE *file = fopen(filename, "r");

    CHECKERROR(file != NULL &&
               "Can't open file.",
               NULL);

    tree *Tree = treeConstructor();

    CHECKERROR(Tree != NULL &&
               "Can't create tree.",
               NULL);

    // We have a guarantee that if parser have error, tree is destructed. 
    // So we can return NULL if error has occured.
    CHECKERROR(parseFile(Tree, file) == NOTERROR &&
               "Wrong akinator data base.",
               NULL);

    return Tree;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
