#include "recordblock.h"

RecordBlock::RecordBlock()
{
    entryCount=0;
    numBlock=-1;
    next=-1;
    file=0;
    lonR=0;
}

RecordBlock::~RecordBlock()
{
    //dtor
}
RecordBlock::RecordBlock(int num, DataFile* fil)
{
    entryCount=0;
    numBlock=num;
    next=-1;
    file=fil;
    lonR=0;
}

char* RecordBlock::toChar()
{
    char* data = new char[BLOCK_SIZE];
    int pos=0;
    memcpy(&data[pos], &numBlock, sizeof(int));
    pos+=4;
    memcpy(&data[pos], &next, sizeof(int));
    pos+=4;
    memcpy(&data[pos], &entryCount, sizeof(int));
    pos+=4;
    for(int i=0; i<regs.size(); i++){
        char * entrydata= regs.at(i);
        memcpy(&data[pos], &entrydata[0], lonR);
        pos+=lonR;
    }
    return data;
}

void RecordBlock::charToBloque(char* data)
{
    int pos=0;
    memcpy(&numBlock, &data[pos], sizeof(int));
    pos+=4;
    memcpy(&next, &data[pos], sizeof(int));
    pos+=4;
    memcpy(&entryCount, &data[pos], sizeof(int));
    pos+=4;
    for(int i=0; i<entryCount; i++){
        char * entrydata= new char[lonR];
        memcpy(&entrydata[0], &data[pos], lonR);
        if(entrydata[lonR-1]=='N')
            regs.push_back(entrydata);
        pos+=lonR;
    }
}

void RecordBlock::escribir()
{
    char* data= this->toChar();
    int pos= numBlock*BLOCK_SIZE;
    file->escribir(data, pos,BLOCK_SIZE);
}

void RecordBlock::cargar()
{
    int pos= numBlock*BLOCK_SIZE;
    char* data= new char[BLOCK_SIZE];
    data= file->leer(pos, BLOCK_SIZE);
    charToBloque(data);
}

void RecordBlock::addRecord(char* r)
{
    regs.push_back(r);
    entryCount++;
}




