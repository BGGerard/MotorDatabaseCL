#include "field.h"

Field::Field()
{
    tipo= 0;
    longitud= 0;
    index=0;
}

Field::Field(const char* name, int t, int size_f, int idx, bool pk)
{
    strncpy(this->nombre, name, sizeof(this->nombre) -1);
    this->nombre[19] = '\0';
    tipo= t;
    longitud= size_f;
    index=idx;
    PK= pk==true? 'T' :'F';
}

char* Field::toChar()
{
    char* data = new char[33];
    int pos=0;
    memcpy(&data[pos], &index, sizeof(int));
    pos+=4;
    memcpy(&data[pos], &nombre[0], 20);
    pos+=20;
    memcpy(&data[pos], &tipo, sizeof(int));
    pos+=4;
    memcpy(&data[pos], &longitud, sizeof(int));
    pos+=4;
    memcpy(&data[pos], &PK, sizeof(char));
    pos+=1;
    return data;
}

void Field::initFromChar(char* data)
{
    int pos=0;
    memcpy(&index, &data[pos], sizeof(int));
    pos+=4;
    memcpy(&nombre[0], &data[pos], 20);
    pos+=20;
    memcpy(&tipo, &data[pos], sizeof(int));
    pos+=4;
    memcpy(&longitud, &data[pos], sizeof(int));
    pos+=4;
    memcpy(&PK, &data[pos], sizeof(char));
    pos+=1;
}


Field::~Field()
{
    //dtor
}
