#include "datafile.h"

DataFile::DataFile(char* path)
{
    pat=path;
}

DataFile::~DataFile()
{
    //dtor
}
void DataFile::openFile()
{
    myFile= fopen(pat, "rb+");
}


void DataFile::closeFile()
{
    fclose(myFile);
}

void DataFile::escribir(char* data, int pos, int longitud)
{
    openFile();
    fseek (myFile , pos , SEEK_SET);
    fwrite(data, sizeof(char), longitud, myFile);
    fflush(myFile);

}

char* DataFile::leer(int pos, int longitud)
{
    openFile();
    fseek (myFile , pos , SEEK_SET);
    data= new char[longitud];
    fread(data, sizeof(char), longitud, myFile);
    return data;
}

bool DataFile::isEmpty()
{
    fseek (myFile, 0, SEEK_END);
    int s=ftell (myFile);
    return (s==0) ? true : false;
}


