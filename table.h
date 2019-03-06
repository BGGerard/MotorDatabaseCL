#ifndef TABLE_H
#define TABLE_H
#include <stdio.h>
#include <string.h>
#include "vector"
#include "datafile.h"
#include "field.h"
#include "block.h"
class Table
{
    public:
        Table(const char* nameT, int idn);
        Table ();
        virtual ~Table();
        char name[20];
        int id;
        int firstBlock;
        int currentBlock;
        int cantR;
        int LongR;
        int cantC;
        std::vector<Field*> campos;
        char* toChar();
        void initFromChar(char* data);
        void cargarCampo();
        DataFile* f;

    protected:

    private:
};

#endif // TABLE_H
