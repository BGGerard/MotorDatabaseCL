#ifndef RECORDBLOCK_H
#define RECORDBLOCK_H
#include "datafile.h"
#include "tools.h"
#include <vector>

class RecordBlock
{
    public:
        RecordBlock(int num, DataFile* fil);
        RecordBlock();
        virtual ~RecordBlock();
        int numBlock;
        int next;
        int fieldCount;
        int entryCount;
        int lonR;
        std::vector<char *> regs;
        char* toChar();
        void charToBloque(char* data);
        void escribir();
        void cargar();
        DataFile *file;
        void addRecord(char* r);

    protected:

    private:
};

#endif // RECORDBLOCK_H
