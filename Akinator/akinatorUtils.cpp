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

static void skipBadSymbols (const char *buffer, size_t *idx)
{
    if (strictBaseSyntax)
        return;

    while (buffer[*idx] != '{' && buffer[*idx] != '"' && 
           buffer[*idx] != '}' && buffer[*idx] != '\0')
        (*idx)++;

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR getToQuestion (const char *buffer, size_t *idx)
{
    skipBadSymbols(buffer, idx);

    CHECKERROR(buffer[*idx] == '{' &&
                "Uncorrect bracket sequence.",
                FILECONTENTERROR);
    
    (*idx)++; // Skipping '{'

    skipBadSymbols(buffer, idx);   

    CHECKERROR(buffer[*idx] == '"' &&
                "Uncorrect quotes sequence.",
                FILECONTENTERROR); 

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static char *scanString (const char *string)
{
    char *newString = NULL;

    sscanf(string, "%ms[^{}]", newString);

    return newString;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR bufferParser (node *Node, const char *buffer)
{
    size_t idx = 0;
    while (buffer[idx] != '\0')
    {
        CHECKERROR(getToQuestion(buffer, &idx) == NOTERROR, ERROR);

        char *newString = scanString(buffer + idx);
    }

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static size_t symbolsCounter (FILE *file)
{
    struct stat fileStatBuffer;

    fstat(fileno(file), &fileStatBuffer);

    size_t symbolsCount = (size_t) fileStatBuffer.st_size;

    return symbolsCount;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR parseFile (tree *Tree, FILE *file)
{
    size_t symbolsCount = symbolsCounter(file);

    // I'm using symbolsCount + 1 as size, because we need '\0' at the end of buffer.
    char *buffer = (char *) calloc (symbolsCount + 1, sizeof(char)); 

    CHECKERROR(buffer != NULL &&
               "Can't create buffer for file.",
               ALLOCATIONERROR);

    fread(buffer, symbolsCount, symbolsCount, file);

    bufferParser(treeRoot(Tree), buffer);

    free(buffer);

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

    parseFile(Tree, file);

    fclose(file);

    return Tree;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
