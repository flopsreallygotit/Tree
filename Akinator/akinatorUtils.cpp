#include <sys/stat.h>
#include <malloc.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// TODO recursion read and add to tree

#include "akinatorUtils.h"

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

static ISERROR parseFile (tree *Tree, FILE *file)
{
    // First we need to initialize root of tree.
    // Then we are parsing recursively.

    skipBadSymbols(file);

    int symbol = getc(file);
    if ((char) symbol != '{')
    {
        PUTERROR("Uncorrect bracket sequence.");

        treeDestructor(Tree);
        fclose(file);

        return FILECONTENTERROR;
    }

    else if (symbol == EOF)
    {
        PUTERROR("Empty data base.");

        treeDestructor(Tree);
        fclose(file);

        return EMPTY;
    }

    else
        ungetc('{', file);

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

    tree *Tree = treeConstructor;

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
