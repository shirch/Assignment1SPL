/*
* stringFunction.cpp
*
*  Created on: Nov 11, 2017
*      Author: nirkov
*/

#include "../include/stringFunction.h"
#include "../include/Commands.h"
#include "../include/FileSystem.h"
#include "../include/Files.h"
#include <vector>
#include <string.h>
#include <iostream>

using namespace std;

string getComString(string args)
{
    if(args.find(" "))
    {
        return args.substr(0, args.find(" "));
    }else
    {
        return args;
    }

}

Directory* pathFromUser(string s, FileSystem & fs) {

    int slesh, size;
    bool found;
    bool validPath = true;
    string temp;
    Directory* updateDir = &fs.getWorkingDirectory();
    if ((s[0] =='/' )& (s.size() > 1)) {				        //if begin with "/" push "back" to the vector as a flag we need to start in root directory
        updateDir = &fs.getRootDirectory();
        s = s.substr(1);
    }
    else if ((s[0] == '/') & (s.size() == 1)) {
        updateDir = &fs.getRootDirectory();
        s = "";
    }
    while ((s != "") & (validPath)) {					//now updateDir is or root dir or work dir according user's command
        found = false;
        slesh = s.find_first_of("/");		//find tindex of first slesh
        if (slesh == (int)(std::string::npos)) {
            temp = s;
            s = "";
        }
        else {
            temp = s.substr(0, slesh); //temp is the first sub directory in the path we received from user
            s = s.substr(slesh + 1);	//now s = s without the first sub directory in the path we received from user
        }
        if ((temp == "..") & (updateDir->getParent() != nullptr)) {      //in case the user insert path like - a/b/../t --> we need to return to a after b
            updateDir = updateDir->getParent();
            found = true;
        }
        else {
            size = updateDir->getChildren().size();
            for (int i = 0; i < size && !found; i++) {
                if (updateDir->getChildren()[i]->getName() == temp && dynamic_cast<Directory *>(updateDir->getChildren()[i])) {
                    updateDir = (Directory*)updateDir->getChildren()[i];
                    found = true;
                }
            }
        }
        //size the children vector


        if (!found) {
            validPath = false;
            updateDir = nullptr;
        }
    }

    return updateDir;
}           //return Directory* to the path the function received



string pathToCreate(string s, FileSystem & fs) {
    int foundSlesh = 0;
    string pathToCreate = "";
    bool foundPathToCreate = false;
    if (s.find_last_of("/") != std::string::npos) {
        if (!pathFromUser(s, fs)) {		//if the path  null that mean the path not exists and we need create it
            while (s != "" && !foundPathToCreate) {
                if (s.find_last_of("/") != std::string::npos) {
                    foundSlesh = s.find_last_of("/");
                    pathToCreate = s.substr(foundSlesh) + pathToCreate;		//if s = a/b/c so pathForCreate = /c
                    s = s.substr(0, foundSlesh);								// and s = a/b
                }
                else {
                    pathToCreate = s + pathToCreate;
                    s = "";
                }
                if (pathFromUser(s, fs)) {
                    foundPathToCreate = true;
                }
            }
        }
        else cout << "The directory already exists" << endl;
    }
    else pathToCreate = s;

    return pathToCreate;
}             //return string of path we need to create



void mkdirCreate(string path, Directory* mkdirPath, string command) {				//*The function receive the input user insert to command line and
    string nameDirToCreate = "";									//* returns a string with the path That we need to create
    bool valid = true;
    while (path != "" && valid) {												// path = /a/b/c
        if (path[0] == '/') {

            path = path.substr(path.find_first_of("/") + 1); 				// path = /a/b/c -- > a/b/c --> b/c --> c
        }
        if (path.find_first_of("/") != std::string::npos) {
            nameDirToCreate = path.substr(0, path.find_first_of("/"));// nameDirToCreate = a
            path = path.substr(path.find_first_of("/"));
            if (dirOrFileNotExistens(nameDirToCreate, mkdirPath,command)) {
                Directory *dir = new Directory(nameDirToCreate, mkdirPath);
                mkdirPath = dir;
            }
            else {
                valid = false;
            }
        }
        else {
            nameDirToCreate = path;
            if (dirOrFileNotExistens(nameDirToCreate, mkdirPath, command)) {
                mkdirPath = new Directory(path, mkdirPath);
                path = "";
            }
            else {
                valid = false;
            }
        }
    }
}

void eraseSpaces(string command) {
    unsigned int rstr = 1;
    unsigned int lstr = 0;
    while (rstr < command.size()) {
        if ((command[lstr] == command[rstr]) && (command[lstr] == ' ')) {
            command.erase(rstr);
        }
        else {
            lstr++;
            rstr++;
        }

    }
}

int index_of_file(string nameToFind, Directory* dir) {              //return index of file in children or -1 if the file not exists in the vector
    int index_of_file = -1;
    bool found = false;
    int size = dir->getChildren().size();
    vector<BaseFile*> vec = dir->getChildren();
    for (int i = 0; i < size && !found; i++) {
        if (vec[i]->getName() == nameToFind) {
            index_of_file = i;
            found = true;
        }
    }
    return index_of_file;
}

bool checkName(string name) {
    bool nameValid = true;
    unsigned int i, k;
    for (i = 0; i < name.size(); i++) {
        k = name[i];
        if (!(((48 <= k) & (k <= 57)) | ((65 <= k) & (k <= 90)) | ((97 <= k) & (k <= 122)))) {
            nameValid = false;
        }
    }
    return nameValid;
}

bool dirOrFileNotExistens(string dirToCreate, Directory* mkdirPath,string command) {
    unsigned size = mkdirPath->getChildren().size();
    bool notExistens = true;
    for (unsigned i = 0; i< size && notExistens; i++) {
        if (mkdirPath->getChildren()[i]->getName() == dirToCreate){
            notExistens = false;
            if(command=="mkfile"){
                cout << "File already exists" << endl;
            }else if(command=="mkdir"){
                cout << "The directory already exists" << endl;
            }
        }
    }
    return notExistens;
}

bool existence(string name, Directory* parent)
{
    for(unsigned int i=0; i < parent->getChildren().size();i++)
    {
        if(parent->getChildren()[i]->getName()== name)
        {
            return true;
        }
    }
    return false;
}

bool contains(Directory* dir,FileSystem fs)
{
    for(unsigned i=0; i < dir->getChildren().size(); i++)
    {

        if(dir->getChildren()[i]->getClass() == "Directory")
        {
            string path = (*(Directory*)(dir->getChildren()[i])).getAbsolutePath();
            if((path.compare(fs.getWorkingDirectory().getAbsolutePath())==0)||(path.compare(fs.getRootDirectory().getAbsolutePath())==0))
            {
                return true;
            }
            else
            {
                return contains((Directory*)dir->getChildren()[i],fs);
            }
        }

    }
    return false;
}
void printForLs(bool sortBySize, Directory* mkdirPath) {
    if (sortBySize) { //if -s in the command
        mkdirPath->sortBySize();  //we sort stable sort
    }
    else mkdirPath->sortByName(); //else we need to sort only by name
    vector<BaseFile*> list = mkdirPath->getChildren();
    unsigned  size = list.size();
    for (unsigned  i = 0; i < size; i++) {
        list[i]->printMyType();
        cout << "\t" << list[i]->getName() << "\t"  << list[i]->getSize() << endl;
    }
}