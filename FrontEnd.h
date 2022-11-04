
#ifndef FRONTEND_H   
#define FRONTEND_H
#include <string>

using namespace std;

extern string command;
extern string commandType;
extern string commandName;
extern string commandToSplit;
extern string params[5];

void getCommandType(string command);
void printCommands();
void splitParams(string command, int paramCount);

#endif