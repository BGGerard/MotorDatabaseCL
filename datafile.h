#ifndef DATAFILE_H
#define DATAFILE_H
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;
class DataFile
{
    public:
        FILE* myFile;
        DataFile(char* path);
        char* data, *pat;
        virtual ~DataFile();
        void openFile();
        void closeFile();
        void escribir(char* data, int pos, int longitud);
        char* leer(int pos, int longitud);
        bool isEmpty();
    protected:

    private:
};

#endif // DATAFILE_H
