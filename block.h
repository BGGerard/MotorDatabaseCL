#ifndef BLOCK_H
#define BLOCK_H
#include "datafile.h"
#include "field.h"
#include "tools.h"
#include <vector>

class Block
{
    public:
        Block(int num, DataFile* fil);
        virtual ~Block();
        int numBlock;
        int next;
        int fieldCount;
        char* toChar();
        std::vector<Field *> campos;
        //std::vector<char *> regs;
        void charToBloque(char* data);
        void escribir();
        void cargar();
        DataFile *file;
        //void addRecord(char* r);
        void addField(Field* f);
    protected:

    private:
};

#endif // BLOCK_H
