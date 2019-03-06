#ifndef FIELD_H
#define FIELD_H
#include "tools.h"

class Field
{
    public:
        Field();
        Field(const char *name, int t, int size_f, int idx, bool pk);
        virtual ~Field();
        int index;
        char nombre[20];
        int tipo;
        int longitud;
        char PK;
        char* toChar();
        void initFromChar(char* data);
    protected:

    private:
};

#endif // FIELD_H
