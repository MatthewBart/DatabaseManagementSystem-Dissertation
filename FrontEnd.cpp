#include "FrontEnd.h" 
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

char* databaseName;
string command;
string commandType;
string commandName;
string commandToSplit;
string params[5];
FILE* fp;

// get command type
void getCommandType(string command) {
	if (command.rfind("insert", 0) == 0) { 
		commandType = "insert";
	}
	else if(command.rfind("remove", 0) == 0) {
		commandType = "remove";
	}
	else if (command.rfind("update", 0) == 0) {
		commandType = "update";
	}
	else if (command.rfind("exit", 0) == 0) {
		commandType = "exit";
	}
	else if (command.rfind("help", 0) == 0) {
		commandType = "help";
	}
	else if (command.rfind("use", 0) == 0) {
		commandType = "use";
	}
	else if (command.rfind("create", 0) == 0) {
		commandType = "create";
	}
	else if (command.rfind("find", 0) == 0) {
		commandType = "find";
	}
	else if (command.rfind("display", 0) == 0) {
		commandType = "display";
	}
}

// splits parameters 
void splitParams(string command, int paramCount) {
	int j = 0;
	int pos = 0;
	for (int i = 0; i < command.length(); i++) {
		if (command[i] == ' ') {
			params[j] = commandToSplit;
			commandToSplit = "";
			j++;
			continue;
		}
		else {
			commandToSplit += putchar(tolower(command[i]));
		}
	}		
}

// print all available commands
void printCommands() {
	cout << endl << "When a database has NOT been selected" << endl << " the following commands are available" << endl;
	cout << "Help: Displays all commands" << endl;
	cout << "Create + NAME: Create a selected database" << endl;
	cout << "Remove + NAME: Remove a selected database"  << endl;
	cout << "Use + NAME: Access a selected database"  << endl;
	cout << "Exit: Exit the program" << endl;
	cout << endl << "When a database HAS been selected" << endl << " the following commands are available" << endl;
	cout << "Help: Displays all commands" << endl;
	cout << "Insert + key [KEY] value [VALUE]: Insert a key and value into the database" << endl;
	cout << "Remove + KEY: Remove a key from the database" << endl;
	cout << "Update + key [KEY] value [VALUE]: Update a selecter key with a new value" << endl;
	cout << "Display: Show all values in a database" << endl;
	cout << "Find key [KEY]: Find a key and it's associated value" << endl;
	cout << "Exit: Exit the program" << endl;
}