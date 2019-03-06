#include "masterblock.h"

MasterBlock::MasterBlock(DataFile*  fil, const char *name, int dsize)
{
    strncpy(this->dbname, name, sizeof(this->dbname) -1);
    this->dbname[19] = '\0';
    dbsize =dsize;;
    tableCount =0;
    blocksFree=(8*4096)-2;
    nextFreeBlock=1;
    numBlock=0;
    file=fil;//ctor
}

MasterBlock::MasterBlock(DataFile* fil)
{

    dbsize=0;
    tableCount =0;
    blocksFree=((4096*8)-2);
    nextFreeBlock=1;
    numBlock=0;
    file=fil;//ctor
}


MasterBlock::~MasterBlock()
{
    //dtor
}
void MasterBlock::escribir()
{
    char* data= this->toChar();
    int pos= numBlock*BLOCK_SIZE;
    file->escribir(data, pos,BLOCK_SIZE);
}

void MasterBlock::cargar()
{
    int pos= numBlock*BLOCK_SIZE;
    char* data= new char[BLOCK_SIZE];
    data= file->leer(pos, BLOCK_SIZE);

    charToBloque(data);

}

char* MasterBlock::toChar()
{
    char* data = new char[BLOCK_SIZE];
    int pos=0;
    memcpy(&data[pos], &dbname[pos], 20);
    pos+=20;
    memcpy(&data[pos], &dbsize, sizeof(int));
    pos+=4;
    memcpy(&data[pos], &tableCount, sizeof(int));
    pos+=4;
    memcpy(&data[pos], &blocksFree, sizeof(int));
    pos+=4;
    memcpy(&data[pos], &nextFreeBlock, sizeof(int));
    pos+=4;
    memcpy(&data[pos], &numBlock, sizeof(int));
    pos+=4;
    for(int i=0; i<tableCount; i++){
        char * entrydata= tables.at(i)->toChar();
        memcpy(&data[pos], &entrydata[0], 44);
        pos+=44;
    }
    return data;
}

void MasterBlock::charToBloque(char* data)
{

    int pos=0;
    memcpy(&dbname[pos], &data[pos], 20);
    pos+=20;
    memcpy(&dbsize, &data[pos], sizeof(int));
    pos+=4;
    memcpy(&tableCount, &data[pos], sizeof(int));
    pos+=4;
    memcpy(&blocksFree, &data[pos], sizeof(int));
    pos+=4;
    memcpy(&nextFreeBlock, &data[pos], sizeof(int));
    pos+=4;
    memcpy(&numBlock, &data[pos], sizeof(int));
    pos+=4;
    for(int i=0; i<tableCount; i++){
        Table* t= new Table();
        t->f=this->file;
        t->initFromChar(&data[pos]);
        tables.push_back(t);
        pos+=44;
    }
}

void MasterBlock::addTable(Table* t)
{
    tables.push_back(t);
    tableCount++;
}
Table* MasterBlock::findTable(string name)
{
    for (int i=0; i<tableCount; i++){
        if(!(name.compare(tables.at(i)->name))){
            return tables.at(i);
        }
    }
    return 0;
}

void MasterBlock::removeTable(string name)
{
    for (int i=0; i<tableCount; i++){
        if(!(name.compare(tables.at(i)->name))){
            tables.erase(tables.begin()+i);
            tableCount--;
        }
    }


}

