#include "dbmanager.h"

DbManager::DbManager()
{
    CreateDirectory("Databases_DBCLI", NULL);
    currentDB=""; //ctor
    myFile=0;
}
void DbManager::createDatabase(string name, unsigned int db_size)
{
    if(name.size() > 20 ){
        printMsg("Error: El nombre deberia ser menos de 20 caracteres");
        return;
    }
    int DB_SIZE= from_MB(db_size);
    if(DB_SIZE > 134217728){
        printMsg("Error: Database Deberia ser menor de 120Mb");
        return;
    }
    currentDB=name;
    name += ".dat";
    string db_path=  PATH + name;
    char *path_t = new char[db_path.length() + 1];
    strcpy(path_t, db_path.c_str());
    myFile = new DataFile(path_t);
    myFile->myFile= fopen(path_t, "wb+");
    myFile->closeFile();
    //SUPERBLOCK----------

    MasterBlock *mblock= new MasterBlock(myFile, currentDB.c_str(), DB_SIZE);

    //WRITING SUPERBLOCK----------
    mblock->escribir();
    //END OF FILE
    myFile->openFile();
    char block[BLOCK_SIZE];
    memset(block,0, BLOCK_SIZE);
    fseek(myFile->myFile, DB_SIZE, SEEK_SET );
    fwrite(block, sizeof(char), DB_SIZE, myFile->myFile);
    myFile->closeFile();
/*
    ofstream out_file(db_path.c_str(), ios::binary);
    char block[BLOCK_SIZE];
    memset(block,0, BLOCK_SIZE);
    out_file.seekp(DB_SIZE-BLOCK_SIZE, ios::beg);
    out_file.write(block, BLOCK_SIZE);
    out_file.close();*/

    printMsg("Database: "+name+" Creada!");
    mblock->file->closeFile();
}



void DbManager::dropDatabase(string name)
{

    string db_path= PATH + name + ".dat";
    if (remove(db_path.c_str())!=0)
        printMsg("Error al dropear  "+name);
    else{
        currentDB="";
        printMsg("la database "+ name + " ha sido dropeada");
    }
}

void DbManager::setDatabase(string name)
{

    currentDB=name;

}

void DbManager::createTable(string name, string pk)
{
    Table* t= new Table(name.c_str(), 1);
    string db_path=  PATH + currentDB +".dat";
    char *path_t = new char[db_path.length() + 1];
    strcpy(path_t, db_path.c_str());
    myFile = new DataFile(path_t);
    MB= new MasterBlock(myFile);
    MB->cargar();
    Block* b= new Block(MB->nextFreeBlock, myFile);
    MB->nextFreeBlock++;
    t->firstBlock= b->numBlock;
    t->currentBlock= -1;
    t->cantC=campos.size();
    int lonR=0;
    for(int i=0; i<campos.size(); i++){
        int leng, type;
        if (campos[i].second==4){
            leng=4;
            type=0;
        }
        else if (campos[i].second==8){
            leng=8;
            type=1;
        }
        else {
            type=2;
            leng=campos[i].second;
        }
        bool PK= pk.compare(campos[i].first)==0? true : false;
        b->addField(new Field(campos[i].first.c_str(), type, leng, i, PK));
        lonR+=leng;
    }
    t->LongR=lonR+1;
    MB->addTable(t);
    MB->escribir();
    b->escribir();
    delete t;
}

void DbManager::dropTable(string t_name)
{
    //Table* t= new Table(name.c_str(), 1);
    string db_path=  PATH + currentDB +".dat";
    char *path_t = new char[db_path.length() + 1];
    strcpy(path_t, db_path.c_str());
    myFile = new DataFile(path_t);
    MB= new MasterBlock(myFile);
    MB->cargar();
    Table* t= MB->findTable(t_name);
    char * data =new char[4096];
    memset(data,0, BLOCK_SIZE);
    myFile->escribir(data, t->firstBlock*BLOCK_SIZE, BLOCK_SIZE);
    delete t;
    MB->removeTable(t_name);
    MB->escribir();
}

DbManager::~DbManager()
{
    //dtor
}

void DbManager::insertRegister(string tname, char* registro)
{
    string db_path=  PATH + currentDB +".dat";
    char *path_t = new char[db_path.length() + 1];
    strcpy(path_t, db_path.c_str());
    myFile = new DataFile(path_t);
    MB= new MasterBlock(myFile);
    MB->cargar();
    Table* t= MB->findTable(tname);
    RecordBlock* b;

    if(t->currentBlock==-1){
        b=new RecordBlock(MB->nextFreeBlock, myFile);
        MB->nextFreeBlock++;
        b->escribir();
    }else{
        b=new RecordBlock(t->currentBlock, myFile);
    }
    b->lonR=t->LongR;
    b->cargar();
    int pos=0;



    if((4084-(b->entryCount*t->LongR))<4096){
        if(b->entryCount==0){
            pos= 12;
        }else{
            pos= (b->entryCount*t->LongR)+12;
            cout << pos << endl;
        }

        if((4096-pos)>(t->LongR)){

            b->addRecord(registro);
        }
    }

    if(t->currentBlock==-1){
    for(int i=0; i< MB->tables.size(); i++){
        if(MB->tables.at(i)->id==t->id){
            MB->tables.at(i)->currentBlock=b->numBlock;
        }
    }
    }

    b->escribir();
    MB->escribir();

    delete t;
    delete b;
    proyeccion.clear();
    campos.clear();
    values.clear();

}
void DbManager::selectStmt(string tname, string sel, string colS)
{
    string db_path=  PATH + currentDB +".dat";
    char *path_t = new char[db_path.length() + 1];
    strcpy(path_t, db_path.c_str());
    myFile = new DataFile(path_t);
    MB= new MasterBlock(myFile);
    MB->cargar();
    Table* t= MB->findTable(tname);
    RecordBlock* b;
    string fields="";
    if(proyeccion.size()==0){
        for(int i=0; i< t->campos.size(); i++){
                fields+=t->campos.at(i)->nombre;
                fields+= " | ";
        }
    }else{
        for(int i=0; i< proyeccion.size(); i++){
                fields+=proyeccion.at(i);
                fields+= " | ";
        }
    }
    cout << fields << endl;
    cout << "------------"<< endl;

    if(t->currentBlock==-1){
        printMsg("No hay registros en la tabla");
        return;
    }
    b=new RecordBlock(t->currentBlock, myFile);
    b->lonR=t->LongR;
    b->cargar();
    do{
        for(int k=0; k< b->regs.size(); k++){
            char * data= b->regs.at(k);
            int tt;
            double dd;
            stringstream ss;
            std::ostringstream sd;
            int pos=0;
            string line="";
            bool selectB=true;
            for(int i=0; i< t->campos.size(); i++){
                string fname=string(t->campos.at(i)->nombre);

                bool show=false;
                for(int j=0; j<proyeccion.size(); j++){
                    if ((fname.compare(proyeccion.at(j)))==0)
                        show=true;
                }
                if(proyeccion.size()==0)
                    show=true;

                if(t->campos.at(i)->tipo==0){
                    memcpy(&tt,&data[pos], sizeof(int));
                    pos+=4;
                    if (show){
                        if(fname.compare(colS)==0){
                            if(atoi(sel.c_str())!=tt)
                                selectB=false;
                        }
                        ss << tt;
                        line+= ss.str();
                        line+= "|";
                    } //cout << tt << "|";
                }else if(t->campos.at(i)->tipo==1){
                        memcpy(&dd,&data[pos], sizeof(double));
                        pos+=8;
                        if (show){
                            if(fname.compare(colS)==0){
                                if(strtod(sel.c_str(), NULL)!=dd)
                                    selectB=false;
                            }
                            sd << dd;
                            line+= sd.str();
                            line+= "|";
                        }
                    }else{
                        char s[t->campos.at(i)->longitud];
                        memcpy(&s[0],&data[pos], t->campos.at(i)->longitud);
                        pos+=t->campos.at(i)->longitud;
                        if (show){
                            if(fname.compare(colS)==0){
                                if((sel.compare(string(s)))!=0)
                                    selectB=false;
                            }
                            line+=s;
                            line+= "|";
                        }
                    }
            }
            if (selectB){
                char rm;
                memcpy(&rm,&data[104], 1);
                if( rm == 'N')
                    cout <<line<< endl;
            }
        }
    }while(b->next!=-1);
    proyeccion.clear();
    campos.clear();
    values.clear();

}

void DbManager::deleteRegister(string tname, string sel, string colS)
{
    string db_path=  PATH + currentDB +".dat";
    char *path_t = new char[db_path.length() + 1];
    strcpy(path_t, db_path.c_str());
    myFile = new DataFile(path_t);
    MB= new MasterBlock(myFile);
    MB->cargar();
    Table* t= MB->findTable(tname);
    RecordBlock* b;
    string fields="";
    if(t->currentBlock==-1){
        printMsg("No hay registros en la tabla");
        return;
    }
    b=new RecordBlock(t->currentBlock, myFile);
    b->lonR=t->LongR;
    b->cargar();
    do{
        for(int k=0; k< b->regs.size(); k++){
            char * data= b->regs.at(k);
            int tt;
            double dd;
            stringstream ss;
            std::ostringstream sd;
            int pos=0;
            string line="";
            bool found=false;
            for(int i=0; i< t->campos.size(); i++){
                string fname=string(t->campos.at(i)->nombre);

                if(t->campos.at(i)->tipo==0){
                    memcpy(&tt,&data[pos], sizeof(int));
                    pos+=4;
                    if(fname.compare(colS)==0){
                        if(atoi(sel.c_str())==tt)
                            found=true;
                    }

                }else if(t->campos.at(i)->tipo==1){
                        memcpy(&dd,&data[pos], sizeof(double));
                        pos+=8;
                        if(fname.compare(colS)==0){
                            if(strtod(sel.c_str(), NULL)==dd)
                                found=true;
                        }
                }else{
                        char s[t->campos.at(i)->longitud];
                        memcpy(&s[0],&data[pos], t->campos.at(i)->longitud);
                        pos+=t->campos.at(i)->longitud;
                        if(fname.compare(colS)==0){
                            if((sel.compare(string(s)))==0)
                                found=true;
                        }
                }
            }
            if (found){
                char rm='Y';
                memcpy(&data[104], &rm, 1);
                b->regs.at(k)=data;
                b->escribir();
            }
        }
    }while(b->next!=-1);
    proyeccion.clear();
    campos.clear();
    values.clear();
}

void DbManager::updateRegister(string tname, string sel, string colS)
{
    string db_path=  PATH + currentDB +".dat";
    char *path_t = new char[db_path.length() + 1];
    strcpy(path_t, db_path.c_str());
    myFile = new DataFile(path_t);
    MB= new MasterBlock(myFile);
    MB->cargar();
    Table* t= MB->findTable(tname);
    RecordBlock* b;
    string fields="";
    if(t->currentBlock==-1){
        printMsg("No hay registros en la tabla");
        return;
    }
    b=new RecordBlock(t->currentBlock, myFile);
    b->lonR=t->LongR;
    b->cargar();
    do{
        for(int k=0; k< b->regs.size(); k++){
            char * data= b->regs.at(k);
            int tt;
            double dd;
            stringstream ss;
            std::ostringstream sd;
            int pos=0;
            string line="";
            bool found=false;
            for(int i=0; i< t->campos.size(); i++){
                string fname=string(t->campos.at(i)->nombre);
                if(t->campos.at(i)->tipo==0){
                    memcpy(&tt, &data[pos], sizeof(int));
                    pos+=4;
                    if(fname.compare(colS)==0){
                        if(atoi(sel.c_str())==tt)
                            found=true;
                    }

                }else if(t->campos.at(i)->tipo==1){
                        memcpy(&dd,&data[pos], sizeof(double));
                        pos+=8;
                        if(fname.compare(colS)==0){
                            if(strtod(sel.c_str(), NULL)==dd)
                                found=true;
                        }
                }else{
                        char s[t->campos.at(i)->longitud];
                        memcpy(&s[0],&data[pos], t->campos.at(i)->longitud);
                        pos+=t->campos.at(i)->longitud;
                        if(fname.compare(colS)==0){
                            if((sel.compare(string(s)))==0)
                                found=true;
                        }
                }
            }
            if (found){

                char rm;
                memcpy(&rm,&data[104], 1);
                if( rm == 'N'){
                    int pos=0;
                    for(int i=0; i< t->campos.size(); i++){

                        string fname=string(t->campos.at(i)->nombre);
                        for(int j=0; j<proyeccion.size(); j++){
                            if ((fname.compare(proyeccion.at(j)))==0){

                                if(t->campos.at(i)->tipo==0){
                                    int val= atoi(values.at(j).c_str());
                                    memcpy(&data[pos], &val, 4);
                                }else if(t->campos.at(i)->tipo==1){
                                    double dval= strtod(values.at(j).c_str(), NULL);
                                    memcpy(&data[pos],&dval, 8);
                                }else{

                                    char s[t->campos.at(i)->longitud];

                                    const char* d= (values.at(0)).c_str();

                                    strncpy(s,d, t->campos.at(i)->longitud-1);
                                    s[t->campos.at(i)->longitud-1]= '\0';
                                    memcpy(&data[pos], &s[0], t->campos.at(i)->longitud);
                                }
                            }
                        }
                        pos+=t->campos.at(i)->longitud;
                    }
                    b->regs.at(k)=data;
                    b->escribir();
                }
            }
        }
    }while(b->next!=-1);
    proyeccion.clear();
    campos.clear();
    values.clear();
}

void DbManager::listTables()
{
    string db_path=  PATH + currentDB +".dat";
    char *path_t = new char[db_path.length() + 1];
    strcpy(path_t, db_path.c_str());
    myFile = new DataFile(path_t);
    MB= new MasterBlock(myFile);
    MB->cargar();
    cout << "-----------LISTANDO TABLAS EN DATABASE " << currentDB << " ----------"<<endl;
    for(int i=0; i<MB->tables.size(); i++){
        cout << MB->tables.at(i)->id << " | " << MB->tables.at(i)->name << endl; ;
    }
}

void DbManager::showTable(string tname)
{
    string db_path=  PATH + currentDB +".dat";
    char *path_t = new char[db_path.length() + 1];
    strcpy(path_t, db_path.c_str());
    myFile = new DataFile(path_t);
    MB= new MasterBlock(myFile);
    MB->cargar();
    cout << "-----------MOSTRANDO LA TABLA: " << tname << " ----------"<<endl;
    Table* t=MB->findTable(tname);
    if (t==0)
        printMsg("Table does not exits.");
    else{
        string pkn="";
        cout << "Table " << tname << "(" <<endl;
        for (int i=0; i< t->campos.size(); i++){
            string type= "char";
            if(t->campos.at(i)->tipo==0)
                type= "int";
            if(t->campos.at(i)->tipo==1)
                type= "double";
            string pk=t->campos.at(i)->PK=='T'? ") not null ": ")";
            cout << '\t'<< t->campos.at(i)->nombre << " \t" << type << "(" << t->campos.at(i)->longitud << pk <<endl;
            if( t->campos.at(i)->PK=='T'){
                pkn= string(t->campos.at(i)->nombre);
            }
        }
        cout << "\tprimary key (" << pkn << ")" << endl;
        cout << ")" << endl;
    }

}

char* DbManager::createRecord(string tname)
{

    string db_path=  PATH + currentDB +".dat";
    char *path_t = new char[db_path.length() + 1];
    strcpy(path_t, db_path.c_str());
    myFile = new DataFile(path_t);
    MB= new MasterBlock(myFile);
    MB->cargar();
    Table* t=MB->findTable(tname);
    char* data= new char(t->LongR);
    int pos=0;

    for(int i=0; i<t->campos.size(); i++){
        if(t->campos.at(i)->tipo==0){
            int k=atoi(values.at(i).c_str());
            memcpy(&data[pos], &k, sizeof(int));
            pos+=4;
        }
        else if(t->campos.at(i)->tipo==0){
            double d=strtod(values.at(i).c_str(), NULL);
            memcpy(&data[pos], &d, sizeof(double));
            pos+=8;
        }
        else{
            values.at(i)=values.at(i)+'\0';
            const char* n=values.at(i).c_str();
            char name[t->campos.at(i)->longitud];
            strncpy(name, n, t->campos.at(i)->longitud-1);
            name[t->campos.at(i)->longitud-1]='\0';
            memcpy(&data[pos], &name[0], t->campos.at(i)->longitud);
            pos+=t->campos.at(i)->longitud;
        }
    }
    char rm='N';
    memcpy(&data[t->LongR-1], &rm, 1);
    values.clear();
    proyeccion.clear();
    return data;

}


