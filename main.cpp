#include <iostream>
#include "dbmanager.h"
#include <utility>
#include "cli.h"
#include "tools.h"

using namespace std;

int main()
{
    DbManager* dbm= new DbManager();

    string cmd="";
    //dbm->setDatabase("Test");

    //dbm->proyeccion.push_back("name ");
    char nam[20]= "name";
    nam[19]= '\0';
    string fn= string(nam);
    //cout << fn.compare(dbm->proyeccion.at(0)) << endl;

    //dbm->selectStmt("Places", "", "");
    //dbm->deleteRegister("Places", "14", "key");
    //dbm->listTables();
    //dbm->showTable("Places");
    //dbm->values.push_back("San Pedro Sula");
    //dbm->updateRegister("Places", "14", "key");




    while (true){
        getline(cin, cmd);

        if(cmd.compare("exit")==0){
            break;
        }

        string p= cmd.substr(0,11);

        if(!(p.compare ("Connect to "))){
            string db= cmd.substr(11);
            dbm->setDatabase(db);
            printMsg("Connected to the database "+db);
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

        cout << p <<endl;
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
            continue;
        }

        p= cmd.substr(0,7);
        if (!(p.compare("Delete "))){
            string name =cmd.substr(7,cmd.find(" -w"));
            name =name.substr(0,name.find(" -w"));
            string campos = cmd.substr(cmd.find("-where=")+7);
            campos = campos.substr(0);
            string colS=campos.substr(0, campos.find("="));
            string sel=campos.substr(campos.find("=")+1);
            dbm->deleteRegister(name, sel, colS);
            continue;
        }

        p= cmd.substr(0,7);
        if (!(p.compare("Select "))){
            string name =cmd.substr(13,cmd.find(" -c"));
            name =name.substr(0,name.find(" -c"));
            string campos = cmd.substr(cmd.find("-columns=")+9);
            campos = campos.substr(0,campos.find("-"));
            string c="";
            if(campos.compare("* ")!=0){

                while (!(campos.empty())){
                     c= campos.substr(0, campos.find(","));
                     dbm->proyeccion.push_back(c);
                     campos= campos.substr(campos.find(",")+1);
                     if(!(campos.compare(c)))
                        campos.erase();
                }
            }
            cout << dbm->proyeccion.size() <<endl;
            string colS="", sel="";
            if (cmd.length()>cmd.find("-where=")){
                campos = cmd.substr(cmd.find("-where=")+7);
                colS=campos.substr(0, campos.find("="));
                sel=campos.substr(campos.find("=")+1);
            }

            dbm->selectStmt(name, sel, colS);
            continue;
        }

        p= cmd.substr(0,7);
        if (!(p.compare("Update "))){
            string name =cmd.substr(7,cmd.find(" -c"));
            name =name.substr(0,name.find(" -c"));
            string campos = cmd.substr(cmd.find("-columns=")+9);
            campos = campos.substr(0,campos.find("-"));
            string c="";

            while (!(campos.empty())){
                c= campos.substr(0, campos.find(","));
                dbm->proyeccion.push_back(c);
                campos= campos.substr(campos.find(",")+1);
                if(!(campos.compare(c))){
                    campos.erase();
                }
            }

            string valores = cmd.substr(cmd.find("-values=")+8);
            valores = valores.substr(0, valores.find("-"));
            c="";
            while (!(valores.empty())){
                 c= valores.substr(0, valores.find(","));
                 dbm->values.push_back(c);
                 valores= valores.substr(campos.find(",")+1);
                 if(!(valores.compare(c)))
                    valores.erase();
            }

            string colS="", sel="";
            if (cmd.length()>cmd.find("-where=")){
                campos = cmd.substr(cmd.find("-where=")+7);
                colS=campos.substr(0, campos.find("="));
                sel=campos.substr(campos.find("=")+1);
            }

            dbm->updateRegister(name, sel, colS);
            continue;
        }

        p= cmd.substr(0,11);
        if (!(p.compare("List tables"))){
            dbm->listTables();
            continue;
        }

        p= cmd.substr(0,11);
        if (!(p.compare("Show table "))){
            string name =cmd.substr(11);
            name =name.substr(0);
            dbm->showTable(name);
            continue;
        }

        p= cmd.substr(0,3);
        if(!(p.compare("cls"))){
            Sleep(2000);
            system("cls");
        }



        //Create table Place -columns=key,name -types=int,char(100) -key=key
        //Create table P1 -columns=key,grade -types=int,double -key=key
        //Create table T4 -columns=key,name -types=int,char(100) -key=key
        //Create table Students -columns=id,grade,name -types=int,double,char(100) -key=id
        //Insert Place -columns=key,name -values=3,Tegus
        //Insert T4 -columns=key,name -values=1,La Ceiba
        //Insert T4 -columns=c1,c3 -values=1,value1
        //Insert Students2 -columns=id,grade,name -values=1,90,Francis
        //Delete Place -where=key=2
        //Delete Persons -where=id=1
        //Select -from=Places -columns=key -where=name =Saonta Rosa de Copan
        //Select -from=Place -columns=* -where=
        //Select -from=Persons -columns=* -where=
        //Update Place -columns=name -values=Choloma -where=key=1

    }
    //Create table Test -columns=codigo,nombre -types=int,char(100) -key=codigo
    //Insert Test -columns=codigo,nombre -values=3,Tegus
    //Select -from=Test -columns=codigo,nombre -where=codigo=3
    //Update Test -columns=nombre -values=Choloma -where=codigo=3
    //Show table Test
    //Select -from=Test -columns=*
    //Delete Test -where=codigo=3

    return 0;
}
