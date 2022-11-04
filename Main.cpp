#include <iostream>
#include <stdio.h>
#include "FrontEnd.h"
#include "Backend.h"
#include <fstream>

int main()
{
	bool exit = false;
	cout << "Please enter a command or type help for a list of commands" << endl;
	// define a new tree
	Tree tree(3);
	

	do {
		// reset command,command type and path back to blank every loop
		command = "";
		commandType = "";
		path = "";
		// ask for input
		// we use getline since cin just takes in the first word and not spaces
		getline(cin, command, '\n');
		cout << endl;
		// add a ' ' so the splitParams() function returns every parametercorrectly
		command += ' ';
		// get command type
		getCommandType(command);

		// if command is "create" then split parameters and if the next input isn't  blank
		// define a path then check if database exists
		// print if it does otherwise create new file of that path
		// else print invalid command
		if (commandType == "create") {
			splitParams(command, 2);
			if(params[1].empty() != true)
			{
				path = params[1];
				if ((dbExists(path)) == true) {
					cout << endl << "Database already exists" << endl;
				}
				else {
					createFile(path);
					file.close();
				}
			}
		}
		// if command is "use" then split the parameters and if the second parameter isn't empty
		// check if the database exists and if it does then read in it's value(if it's blank it won't read any in)
		// and exit the do while loop
		// else output that the database doesnt exist
		else if (commandType  == "use") {
			splitParams(command, 2);
			if (params[1].empty() != true)
			{
				path = params[1];
				if ((dbExists(path)) == true) {
						
						tree.populateTree(path);
						exit = true;
						break;					
				}
				else {
					cout << endl << "Database doesn't exists" ;
				}
			}
		}
		// if command is "remove"
		// check if the second parameter is valid then check if it exists
		// if it exists then attempt to delete the associated file and output if successful or not
		// else the parameters are invalid
		else if (commandType == "remove") {
			splitParams(command, 2);
			if (params[1].empty() != true)
			{
				path = params[1];
				if ((dbExists(path)) == true) {
					path = path + ".txt";
					// convert file path to char*
					// output if file is deleted or not
					if (remove(path.c_str()) != 0)
						cout << endl << "Error deleting file";
					else
						cout << endl << "File successfully deleted";
				}
			}
			else {
				cout << endl << "Invalid command parameters" << endl;
			}
		}
		// if the command is "exit"
		// exit the program through the label exit
		else if (commandType == "exit") {
			goto exit;
		}
		// if the command is "help"
		// print all available commands
		else if (commandType == "help") {
			printCommands();
		}
		// repeat until a valid database is being used
	} while (exit != true);
	
	exit = false;
	cout << endl << "Using database " << path << endl;
	while (exit != true)
	{
		// reset command, command type
		command = "";
		commandType = "";
		// get command
		getline(cin, command, '\n');
		command += ' ';
		getCommandType(command);
		// if command is "insert"
		// check if the second parameter is key and the third parameter is valid
		// check if the fourth parameter is value and the fourth parameter is valid
		// if it exists then insert the key and it's associated value
		// update tree in file then
		if (commandType == "insert") {
			splitParams(command, 5);
			if (params[1] == "key" && params[2] != " " && params[3] == "value" && params[4] != " ") {
				tree.insert(stoi(params[2]), params[4]);
				file.open(path + ".txt", fstream::out);
				tree.storeTree();
				file.close();
			}
		}
		// if command is "remove"
		// check if the second parameter is key and the third parameter is valid then check if it exists
		// if it exists then delete the key and it's associated value
		// else the parameters are invalid
		// once removed, store the current tree to file
		// we do this in order to prevent data loss in case of a crash
		else if (commandType == "remove") {
			splitParams(command, 3);
			if (params[1] == "key" && params[2] != " ") {
				tree.remove(stoi(params[2]));
				file.open(path + ".txt", fstream::out);
				tree.storeTree();
				file.close();
			}
		}
		// if command is "update"
		// check if the second parameter is key and the third parameter is valid
		// check if the fourth parameter is value and the fourth parameter is valid
		// if it exists then update the key and it's associated value
		// update tree in file then
		else if (commandType == "update") {
			splitParams(command, 5);
			if (params[1] == "key" && params[2] != " " && params[3] == "value" && params[4] != " ") {
				tree.remove(stoi(params[2]));
				tree.insert(stoi(params[2]), params[4]);
				file.open(path + ".txt", fstream::out);
				tree.storeTree();
				file.close();
			}
		}
		// if command is "exit"
		// store the current tree and then go to the exit label
		else if (commandType == "exit") {
			file.open(path + ".txt", fstream::out);
			tree.storeTree();
			file.close();
			goto exit;
		}
		// if command is "help"
		// print all commands
		else if (commandType == "help") {
			printCommands();
		}
		// if command is "display"
		// print all values in a tree
		else if (commandType == "display") {
			tree.navigateTree();
		}
		// if command is "find"
		// split the parameters and check if the second parameter is key and the third is a valid input
		// then find and print the key and it's value
		else if (commandType == "find") {
			splitParams(command, 3);
			if (params[1] == "key" && params[2] != " ") {
				tree.search(stoi(params[2]));
			}
		}
	}

	// exit application
	exit:
	return(0);
}