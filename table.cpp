#include "table.h"

Table::Table(const char* nameT, int idn)
{
    strncpy(this->name, nameT, sizeof(this->name) -1);

    this->name[19] = '\0';
    id= idn;
    cantR=0;
    firstBlock=-1;
    currentBlock=-1;
    //numB=-1;
    f=0;
    LongR=0;
    cantC=0;

}

Table::~Table()
{
    //dtor
}
char* Table::toChar()
{
    char* data = new char[44];
    int pos=0;
    memcpy(&data[pos], &name[pos], 20);
    pos+=20;
    memcpy(&data[pos], &id, sizeof(int));
    pos+=4;
    memcpy(&data[pos], &firstBlock, sizeof(int));
    pos+=4;
    memcpy(&data[pos], &currentBlock, sizeof(int));
    pos+=4;
    memcpy(&data[pos], &cantR, sizeof(int));
    pos+=4;
    memcpy(&data[pos], &cantC, sizeof(int));
    pos+=4;
    memcpy(&data[pos], &LongR, sizeof(int));
    pos+=4;
    return data;
}


void Table::initFromChar(char* data)
{
    int pos=0;
    memcpy(&name[pos], &data[pos], 20);
    pos+=20;
    memcpy(&id, &data[pos], sizeof(int));
    pos+=4;
    memcpy(&firstBlock, &data[pos], sizeof(int));
    pos+=4;
    memcpy(&currentBlock, &data[pos], sizeof(int));
    pos+=4;
    memcpy(&cantR, &data[pos], sizeof(int));
    pos+=4;
    memcpy(&cantC, &data[pos], sizeof(int));
    pos+=4;
     memcpy(&LongR, &data[pos], sizeof(int));
    pos+=4;

    cargarCampo();
}

void Table::cargarCampo()
{

    int i = firstBlock;
    int x=0;

    Block* bc = new Block(i, this->f);
    bc->cargar();
    for(int k = 0; k < bc->fieldCount; k++)
    {campos.push_back(bc->campos.at(k));

    }

}
Table::Table()
{

}
