#include <sys/stat.h>
#include <malloc.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
    while (buffer[*idx] != '{' && buffer[*idx] != '"' && 
           buffer[*idx] != '}' && buffer[*idx] != '\0')
        (*idx)++;

    return;
}

static ISERROR bufferParser (node *Node, const char *buffer)
{
    // First we need to process tree root.
    // Then we will parse buffer recursively.

    // We need to find first "word" and move pointer.
    // Then we need to remove close bracket for this "word".

    // I'll do it like this:
    // 1) Skip all bad symbols from '\0' to '}'
    // 2) Change '}' to '\0'

    // Done we have buffer with subtrees: "{...}(maybe bad symbol e.t.c)...{...}"

    size_t idx = 0;
    while (buffer[idx] != '\0')
    {
        skipBadSymbols(buffer, &idx);

        CHECKERROR(buffer[idx] == '{' &&
                   "Uncorrect bracket sequence.",
                   FILECONTENTERROR);
    }

    return;
}

static ISERROR parseFile (tree *Tree, FILE *file)
{
    struct stat fileStatBuffer;

    fstat(fileno(file), &fileStatBuffer);

    size_t symbolsCount = fileStatBuffer.st_size; //TODO call func from onegin //TODO fix TODO highlighting

    // I'm using symbolsCount + 1 as size, because we need '\0' at the end of buffer.
    char *buffer = (char *) calloc (symbolsCount + 1, sizeof(char)); // TODO func from onegin

    CHECKERROR(buffer != NULL &&
               "Can't create buffer for file.",
               ALLOCATIONERROR);

    fread(buffer, symbolsCount, symbolsCount, file);

    bufferParser(treeRoot(Tree), buffer);

    free(buffer);

    return NOTERROR;
}

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
