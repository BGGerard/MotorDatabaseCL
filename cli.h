#ifndef CLI_H
#define CLI_H
#include <iostream>
#include "dbmanager.h"
#include <utility>
#include "table.h"

using namespace std;

class cli
{
    public:
        cli();
        virtual ~cli();
        void evalStatement();
        DbManager* dbm;
    protected:

    private:
};

#endif // CLI_H
