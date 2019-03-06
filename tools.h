#ifndef TOOLS_H
#define TOOLS_H
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

//#define int_type 0
//#define double_type 1
//#define char_type 2
#define BLOCK_SIZE 4096

using namespace std;

void printMsg(string msg);
unsigned long int from_MB(unsigned int n);

#endif // TOOLS_H
