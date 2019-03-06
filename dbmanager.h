#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <windows.h>
#include "masterblock.h"
#include "tools.h"
#include <vector>
#include "datafile.h"
#define BLOCK_SIZE 4096
#define MAX_STR_SIZE 20
#define PER_FOR_TABLES 0.05
#include <fstream>
#include <utility>
#include "table.h"
#include "datafile.h"
#include "masterblock.h"
#include "block.h"
#include "recordblock.h"
#include <sstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
const string PATH= "Databases_DBCLI/";
class DbManager
{
    public:
        DbManager();
        virtual ~DbManager();
        void createDatabase(string name, unsigned int db_size);
        void dropDatabase(string name);
        void setDatabase(string name);
        void createTable( string name, string pk);
        void dropTable(string t_name);
        string currentDB;
        DataFile* myFile;
        MasterBlock * MB;
        vector<pair<string, int> > campos;
        vector<string> values;
        vector<string> proyeccion;
        void insertRegister(string tname, char* registro);
        void selectStmt(string tname, string sel, string colS);
        void deleteRegister(string tname, string sel, string colS);
        void updateRegister(string tname, string sel, string colS);
        void listTables();
        void showTable(string tname);
        char* createRecord(string tname);
    protected:

    private:

};

#endif // DBMANAGER_H
