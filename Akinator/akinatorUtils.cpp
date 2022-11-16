#include <sys/stat.h>
#include <stdlib.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "akinatorUtils.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

    else
        Node->data = NULL;

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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

ISERROR pushRootToTree (tree *Tree, node *Root)
{
    CHECKERROR(Tree != NULL && 
               "You are trying to push in nullpointer.", 
               NULLPOINTER);

    CHECKERROR(Root != NULL &&
               "You can't initialize root pointer as NULL.",
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
    
    // Leaf can be NULL.

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

// static void skipBadSymbols (FILE *file)
// {
//     // We don't need to skip bad symbols if syntax is strict.
//     if (strictBaseSyntax)
//         return;

//     int symbol = getc(file);
//     while (symbol != '{' && symbol != '"' && 
//            symbol != '}' && symbol != EOF)
//         symbol = getc(file);

//     ungetc(symbol, file); 
    
//     // For example: we skipped all bad symbols, but last symbol from stream is '{', '}', '"'.
//     // So we need to put it back to stream.

//     return;
// }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node* nodeReadFromFile (node *parentNode, FILE *file)
{   
    int firstSymbol = fgetc(file);
    
    if ((char) firstSymbol == '{')
    {
        CHECKERROR((char) fgetc(file) == '"' &&
                   "First quote is missing.",
                   NULL);

        wchar_t *element = NULL;

        if (fscanf(file, "%ml[^\"]", &element) != 0)
        {
            CHECKERROR(element != NULL &&
                       "Can't allocate memory for string.",
                       NULL);

            node *currentNode = nodeConstructor(element, parentNode);

            if (fgetc(file) != '"')
            {
                PUTERROR("Incorrect quote sequence.");

                free(element);
                element = NULL;

                return NULL;
            }

            int lastSymbol = fgetc(file);

            // If it is leaf
            if (lastSymbol == '}')
                goto freeAndReturn;
            
            ungetc(lastSymbol, file);

            // Pushing to left  subtree.
            pushLeafToNode(currentNode, nodeReadFromFile(currentNode, file));

            // Pushing to right subtree.
            pushLeafToNode(currentNode, nodeReadFromFile(currentNode, file));

            if (fgetc(file) != '}')
            {
                PUTERROR("Incorrect bracket sequence.");

                free(element);
                element = NULL;

                return NULL;
            }
            
            freeAndReturn:
                free(element);
                element = NULL;

                return currentNode;
        }
    }
  
    return NULL;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR parseFile (tree *Tree, const char *filename)
{
    FILE *file = fopen(filename, "r");

    CHECKERROR(file != NULL &&
               "Can't open file.",
               NULLPOINTER);

    pushRootToTree(Tree, nodeReadFromFile(NULL, file));

    fclose(file);

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

tree *openAndParseFile (const char *filename)
{
    CHECKERROR(filename != NULL &&
               "Can't open file with nullpointer to filename.",
               NULL);

    tree *Tree = treeConstructor();

    CHECKERROR(Tree != NULL &&
               "Can't create tree.",
               NULL);

    // We have a guarantee that if parser have error, tree is destructed. 
    // So we can return NULL if error has occured.
    CHECKERROR(parseFile(Tree, filename) == NOTERROR &&
               "Wrong akinator data base.",
               NULL);

    return Tree;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static node *createSubtreeOnLeaf(node *Leaf, wchar_t *leftLeafData, 
                                             wchar_t *rightLeafData,
                                             wchar_t *newNodeData)
{
    node *newNode  = nodeConstructor(newNodeData,   Leaf->parent);
    newNode->left  = nodeConstructor(leftLeafData,  newNode);
    newNode->right = nodeConstructor(rightLeafData, newNode);
    
    if (Leaf->parent->left == Leaf)
        Leaf->parent->left  = newNode;

    else
        Leaf->parent->right = newNode;

    subtreeDestructor(Leaf);

    return newNode;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void endGame (node *Node, bool answer)
{
    if (answer)
    {
        printf("База. Я опять выиграл!\n");

        return;
    }
    
    else
    {   //TODO tts, definition, compare.     
        wchar_t *compareSign = (wchar_t *) calloc (maxUserDataLength, sizeof(wchar_t));

        CHECKERROR(compareSign != NULL && 
                   "Can't allocate memory for compare sign.", 
                   (void) NULL);

        wchar_t *Answer      = (wchar_t *) calloc (maxUserDataLength, sizeof(wchar_t));

        CHECKERROR(Answer != NULL && 
                   "Can't allocate memory for answer.", 
                   (void) NULL);

        printf("О, нет!!! Жалкий человечишка одолел меня!\nТак что же ты загадал? ");
        scanf("%l[^\n]s", Answer);

        // Reading \n from stream.
        getchar();

        printf("Чем оно отличается от \"%ls\"? ", Node->data);
        scanf("%l[^\n]s", compareSign);

        // Reading \n from stream.
        getchar();
        
        createSubtreeOnLeaf(Node, Answer, Node->data, compareSign);

        free(compareSign);
        free(Answer);

        return;
    }

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static bool getAnswer(void)
{
    wchar_t answer = 0;
    scanf("%lc", &answer);

    while (getchar() != '\n') {}

    if (answer == L'д' || answer == L'Д')
        return true;
    
    if (answer == L'н' || answer == L'Н')
        return false;
    
    printf("Неправильный ввод!\n[(Д)а\\(Н)ет]: ");

    return getAnswer();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void recursiveGame (node *Node)
{
    printf("Это %ls?\n[(Д)a\\(Н)ет]: ", Node->data);

    bool answer = getAnswer();

    if (Node->left != NULL && answer)
        recursiveGame(Node->left);

    else if (Node->right != NULL && !answer)
        recursiveGame(Node->right);

    else
        return endGame(Node, answer);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR startGame (tree *Tree)
{
    CHECKERROR(Tree != NULL &&
               "Can't start game without tree.",
               NULLPOINTER);

    recursiveGame(Tree->root);

    return NOTERROR;
}

ISERROR changeDataBase (tree *Tree, const char *filename)
{
    FILE *file = fopen(filename, "w");

    CHECKERROR(file != NULL &&
               "Can't open file.",
               NULLPOINTER);

    CHECKERROR(preorderPrintTree(Tree, file) == NOTERROR &&
               "Can't print tree in file.",
               ERROR);

    fclose(file);

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
