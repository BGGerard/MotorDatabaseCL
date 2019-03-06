#ifndef MASTERBLOCK_H
#define MASTERBLOCK_H
#define BLOCK_SIZE 4096
#include "datafile.h"
#include "table.h"
#include <vector>


class MasterBlock
{
    public:
        MasterBlock(DataFile*  fil);
        MasterBlock(DataFile*  fil, const char *name, int dsize);
        virtual ~MasterBlock();
        DataFile *file;
        char dbname[20];
        int dbsize;
        int tableCount;
        int blocksFree;
        int nextFreeBlock;
        int numBlock;
        void escribir();
        void cargar();
        char* toChar();
        void charToBloque(char* data);
        void addTable(Table *t);
        vector <Table*> tables;
        Table* findTable(string name);
        void removeTable(string name);
    protected:

    private:
};

#endif // MASTERBLOCK_H
