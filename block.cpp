#include "block.h"

Block::Block(int num, DataFile* fil)
{
    fieldCount=0;
    numBlock=num;
    next=-1;
    file=fil;
}

Block::~Block()
{
    //dtor
}
char* Block::toChar()
{
    char* data = new char[BLOCK_SIZE];
    int pos=0;
    memcpy(&data[pos], &numBlock, sizeof(int));
    pos+=4;
    memcpy(&data[pos], &next, sizeof(int));
    pos+=4;
    memcpy(&data[pos], &fieldCount, sizeof(int));
    pos+=4;
    for(int i=0; i<fieldCount; i++){
        char * entrydata= campos.at(i)->toChar();
        memcpy(&data[pos], &entrydata[0], 33);
        pos+=33;
    }
    return data;
}

void Block::charToBloque(char* data)
{
    int pos=0;
    memcpy(&numBlock, &data[pos], sizeof(int));
    pos+=4;
    memcpy(&next, &data[pos], sizeof(int));
    pos+=4;
    memcpy(&fieldCount, &data[pos], sizeof(int));
    pos+=4;
    for(int i=0; i<fieldCount; i++){
        Field* f= new Field();
        f->initFromChar(&data[pos]);
        campos.push_back(f);
        pos+=33;
    }
}

void Block::escribir()
{
    char* data= this->toChar();
    int pos= numBlock*BLOCK_SIZE;
    file->escribir(data, pos,BLOCK_SIZE);
}

void Block::cargar()
{
    int pos= numBlock*BLOCK_SIZE;
    char* data= new char[BLOCK_SIZE];
    data= file->leer(pos, BLOCK_SIZE);
    charToBloque(data);
}

void Block::addField(Field* f)
{
    campos.push_back(f);
    fieldCount++;
}
