#include "record.h"

Record::Record(char* data, int sizeR, int col)
{
    datav= data;
    cantC=col;
    longitud=sizeR;
}

Record::~Record()
{

}

char* Record::toChar()
{
    return datav;
}

void Record::initFromChar(char* data)
{
    int pos=0;
    datav=data;

}

