#include "cli.h"

cli::cli()
{
    dbm= new DbManager();
}

cli::~cli()
{
    //dtor
}

void cli::evalStatement()
{

    string cmd="";
    while(true){
        getline(cin, cmd);

        if(cmd.compare("exit")==0){
            return;
        }

        string p= cmd.substr(0,11);

        if(!(p.compare ("Connect to "))){
            string db= cmd.substr(11);
            dbm->setDatabase(db);
            continue;
        }

        p= cmd.substr(0,16);

        if (p.compare("Create database ")==0){
            string db= cmd.substr(16);
            string name= db.substr(0, db.find(" "));
            string n= db.substr(db.find(" "));
            dbm->createDatabase(name,atoi(n.c_str()));
            continue;
        }

        p= cmd.substr(0,14);

        if (!(p.compare("Drop database "))){
            dbm->dropDatabase(cmd.substr(14));
            continue;
        }

        p = cmd.substr(0,13);
        if (!(p.compare("Create table "))){
            string name =cmd.substr(13,cmd.find(" -c"));
            name =name.substr(0,name.find(" -c"));

            vector<pair<string, int> > columnas;
            int cantC=0;
            string campos = cmd.substr(cmd.find("-columns=")+9,cmd.find("-"));
            campos = campos.substr(0,campos.find("-"));
            string c="";

            while (!(campos.empty())){
                 c= campos.substr(0, campos.find(","));
                 columnas.push_back(make_pair(c, 0));
                 campos= campos.substr(campos.find(",")+1);
                 if(!(campos.compare(c)))
                    campos.erase();
                 c= campos.substr(0,7);
                 cantC++;
            }

            string tipos = cmd.substr(cmd.find("-columns=")+9);
            tipos= tipos.substr(tipos.find(" ")+8);
            int type=2;

            for(int i=0; i<cantC; i++){
                 c= tipos.substr(0, tipos.find(","));
                 if(!(c.compare("int")))
                    type=4;
                 else if(!(c.compare("double")))
                    type=8;
                 else
                    type=atoi((c.substr(c.find("(")+1,c.find(")"))).c_str());
                 columnas[i].second=type;
                 tipos= tipos.substr(tipos.find(",")+1);
                 if(!(tipos.compare(c)))
                    tipos.erase();
            }

            string pk=cmd.substr(cmd.find("-key=")+5);
            cout << pk << endl;
            dbm->campos=columnas;
            dbm->createTable(name, pk);

            continue;
        }

        p= cmd.substr(0,11);
        if (!(p.compare("Drop table "))){
            dbm->dropTable(cmd.substr(11));
            continue;
        }

        p= cmd.substr(0,7);
        if (!(p.compare("Insert "))){
            string name =cmd.substr(7,cmd.find(" -c"));
            name =name.substr(0,name.find(" -c"));

            string campos = cmd.substr(cmd.find("-values=")+8);
            campos = campos.substr(0);
            string c="";
            while (!(campos.empty())){
                 c= campos.substr(0, campos.find(","));
                 dbm->values.push_back(c);
                 campos= campos.substr(campos.find(",")+1);
                 if(!(campos.compare(c)))
                    campos.erase();
            }

            char* data=dbm->createRecord(name);
            dbm->insertRegister(name, data);

        }

//Insert Places -columns=key,name -values=1,La Ceiba

    }
}
