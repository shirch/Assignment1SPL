/*
* stringFunction.h
*
*  Created on: Nov 15, 2017
*      Author: nir
*/

#ifndef INCLUDE_STRINGFUNCTION_H_
#define INCLUDE_STRINGFUNCTION_H_
#include "stringFunction.h"
#include "FileSystem.h"
#include "Files.h"
#include <string.h>
#include <iostream>
using namespace std;

Directory* pathFromUser(string s, FileSystem & fs);

string withoutCom(string s, string toFind);
string pathToCreate(string s, FileSystem & fs);
void mkdirCreate(string path, Directory* mkdirPath,string command);
void eraseSpaces(string command);
int index_of_file(string nameToFind, Directory* dir);
bool checkName(string name);
bool dirOrFileNotExistens(string dirToCreate, Directory* mkdirPath, string command);
bool existence(string name, Directory* parent);
//bool fileNotExistens(string fileToCreate, Directory* mkdirPath);
void printForLs(bool sortBySize, Directory* mkdirPath);
string getComString(string args);
bool contains(Directory* dir,FileSystem fs);

#endif /* INCLUDE_STRINGFUNCTION_H_ */