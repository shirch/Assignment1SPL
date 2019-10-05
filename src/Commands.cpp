/*
* Commasnds.cpp
*
*  Created on: Nov 11, 2017
*      Author: nirkov
*/
#include <algorithm>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>
#include <string.h>
#include <iostream>
#include "../include/Commands.h"
#include "../include/stringFunction.h"
#include "../include/GlobalVariables.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          BASE COMMAND
//////////////////////////////////////////////////////////////////////////////////////////////////////

BaseCommand::BaseCommand(string args_):args(args_) {}

string BaseCommand::getArgs() {
    return args;
}

BaseCommand::~BaseCommand(){}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            COMMANDS
//////////////////////////////////////////////////////////////////////////////////////////////////////


//Pwd Command
PwdCommand::PwdCommand(string args) : BaseCommand(args) {}

void PwdCommand::execute(FileSystem & fs) {
    if (&fs.getWorkingDirectory() != &fs.getRootDirectory()) {
        cout << fs.getWorkingDirectory().getAbsolutePath() << endl;
    }
    else   cout << "/" << endl;

}

string PwdCommand::toString() {
    return"pwd";
}

PwdCommand::~PwdCommand(){}

//Cd Command
CdCommand::CdCommand(string args) : BaseCommand(args) {}

void CdCommand::execute(FileSystem & fs) {
    string path = this->getArgs().substr(this->getArgs().find_first_of(" ") + 1);
    Directory* toUpdate = pathFromUser(path, fs);
    if (toUpdate) {
        fs.setWorkingDirectory(toUpdate);
    }
    else cout << "The system cannot find the path specified" << endl;

}

string CdCommand::toString() {
    return getArgs();
}

CdCommand::~CdCommand(){}

//Ls Command
LsCommand::LsCommand(string args) : BaseCommand(args) {}

void LsCommand::execute(FileSystem & fs) {
    bool print = false;
    string lsCom = this->getArgs();
    if (lsCom.find_first_of(" ") != std::string::npos) {
        lsCom = lsCom.substr(lsCom.find_first_of(" ") + 1);       //ls with path or -s or both
    }
    else {
        printForLs(false, &fs.getWorkingDirectory());        //only "ls"
        print = true;
    }
    if (!print) {
        if (lsCom.find("-s") == std::string::npos) {
            Directory *lsPath = pathFromUser(lsCom, fs);
            if (lsPath) {
                printForLs(false, lsPath);      //ls <path>
            }
            else cout << "The system cannot find the path specified" << endl;
        }
        else {
            if (lsCom.find_first_of(" ") == std::string::npos) {        //ls -s
                printForLs(true, &fs.getWorkingDirectory());
            }
            else {
                lsCom = lsCom.substr(lsCom.find_first_of(" ") + 1);       //ls -s <path>
                Directory *lsPath = pathFromUser(lsCom, fs);
                if (lsPath) {
                    printForLs(true, lsPath);
                }
                else cout << "The system cannot find the path specified" << endl;

            }
        }
    }
}

string LsCommand::toString() {
    if(!(getArgs().find(" ")==string::npos))
    {
        return getArgs();
    }
    else
    {
        return getArgs()+" ";
    }

}

LsCommand::~LsCommand(){}

//Mkdir Command
MkdirCommand::MkdirCommand(string args) : BaseCommand(args) {}

void MkdirCommand::execute(FileSystem & fs) {
    string path = this->getArgs().substr(this->getArgs().find_first_of(" ") + 1);	//path without mkdir command
    string pathCreate = pathToCreate(path, fs); 		//string of path to create
    path = path.substr(0, path.find(pathCreate));		//path to directory which alredy exists
    if ((path == "") & (pathCreate[0]== '/')) {               //in case the path begin in root directory
        path = "/";
    }
    Directory* mkdirPath = pathFromUser(path, fs);
    mkdirCreate(pathCreate, mkdirPath,"mkdir");				//the function create the new path
}

string MkdirCommand::toString() {
    return getArgs();
}

MkdirCommand::~MkdirCommand(){}

//mkfile Command
MkfileCommand::MkfileCommand(string args) : BaseCommand(args) {}

void MkfileCommand::execute(FileSystem & fs) {
    string path = this->getArgs().substr(this->getArgs().find_first_of(" ") + 1);  //path without mkdir command
    string sizdOfFile = path.substr(path.find_last_of(" ") +1);
    path = path.substr(0,path.find_last_of(" "));
    if(path.find_last_of("/") == std::string::npos){
        if(dirOrFileNotExistens(path ,  &fs.getWorkingDirectory(),"mkfile") & checkName(path)){
            BaseFile* file = new File(path , stoi(sizdOfFile));
            fs.getWorkingDirectory().addFile(file);
        }

    }else{
        string nameOfFile = path.substr(path.find_last_of("/")+1);
        path = path.substr(0,path.find_last_of("/"));       //only path
        Directory* mkdirPath = pathFromUser(path, fs);      //parent of the file we create
        if(mkdirPath){
            if(dirOrFileNotExistens(nameOfFile , mkdirPath,"mkfile") ){
                if( checkName(nameOfFile)){
                    BaseFile* file = new File(nameOfFile , stoi(sizdOfFile));
                    mkdirPath->addFile(file);
                }
            }
        }else cout << "The system cannot find the path specified" << endl;

    }

}

string MkfileCommand::toString() {
    return getArgs();
}

MkfileCommand::~MkfileCommand(){}

//cp Command
CpCommand::CpCommand(string args) : BaseCommand(args) {}

void CpCommand::execute(FileSystem & fs) {

    Directory *target;
    Directory *destination;
    bool copied = false;
    bool exist = false;
    string file;//the file/directory name we want to copy
    string path1 = getArgs().substr(getArgs().find(" ")+1,getArgs().find_last_of(" ")-getArgs().find(" ")-1);
    string path2 = getArgs().substr(getArgs().find_last_of(" ")+1,getArgs().length()-getArgs().find_last_of(" ")+1);
    destination = pathFromUser(path2,fs);
    if(!destination)
    {
        cout<<"No such file or directory"<<endl;
        return;
    }
    if(path1.find("/")==string::npos)
    {
        target = &fs.getWorkingDirectory();
        file = path1;

    }
    else
    {
        file = path1.substr(path1.find_last_of("/")+1,path1.length()-path1.find_last_of("/")+1);
        path1 = path1.substr(0,path1.find_last_of("/"));
        target = pathFromUser(path1,fs);
    }

    if(target)
    {
        for(unsigned int i=0;i<target->getChildren().size();i++)
        {
            if(target->getChildren()[i]->getName()==file)
            {
                if(!existence(file,destination)) {
                    if ((target->getChildren()[i] != &fs.getWorkingDirectory()) || (target->getChildren()[i] != &fs.getRootDirectory()))
                    {
                        if (target->getChildren()[i]->getClass() == "Directory")//dir
                        {
                            destination->addFile(new Directory(*(Directory *) target->getChildren()[i]));
                            ((Directory *) destination->getChildren()[destination->getChildren().size() -
                                                                      1])->setParent(destination);
                            copied = true;
                        } else//File
                        {
                            destination->addFile(new File(*(File *) target->getChildren()[i]));
                            copied = true;
                        }
                    }
                }
                else
                {
                    exist = true;
                }
            }
        }
        if((!copied)&& (!exist))
        {
            cout<<"No such file or directory"<<endl;
        }
    }
    else
    {
        cout<<"No such file or directory"<<endl;
    }
}

string CpCommand::toString() {
    return getArgs();
}

CpCommand::~CpCommand(){}

//Mv Command
MvCommand::MvCommand(string args) : BaseCommand(args) {}

void MvCommand::execute(FileSystem & fs) {
    Directory *target;
    Directory *destination;
    bool moved = false;
    string file;//the file/directory name we want to
    string path1 = getArgs().substr(getArgs().find(" ")+1,getArgs().find_last_of(" ")-getArgs().find(" ")-1);
    string path2 = getArgs().substr(getArgs().find_last_of(" ")+1,getArgs().length()-getArgs().find_last_of(" ")+1);
    destination = pathFromUser(path2,fs);
    if((path1.compare(fs.getWorkingDirectory().getAbsolutePath())==0)||(path1.compare(fs.getRootDirectory().getAbsolutePath())==0))
    {
        cout<<"Can’t move directory"<<endl;
        return;
    }
    if(!destination)
    {
        cout<<"No such file or directory"<<endl;
        return;
    }
    if(path1.find("/")==string::npos)
    {
        target = &fs.getWorkingDirectory();
        file = path1;

    }
    else
    {
        file = path1.substr(path1.find_last_of("/")+1,path1.length()-path1.find_last_of("/")+1);
        path1 = path1.substr(0,path1.find_last_of("/"));
        target = pathFromUser(path1,fs);
    }
    if(target)
    {
        int size =  target->getChildren().size();
        for( int i=0;(i < size )& (!moved) ;i++)
        {
            if(target->getChildren()[i]->getName()==file)
            {

                if(!(existence(file,destination))) {
                    if (target->getChildren()[i]->getClass() == "Directory")//dir
                    {

                            Directory *dir = new Directory(*(Directory *) (target->getChildren()[i]));
                            //dir->setParent(destination);
                            destination->addFile(dir);
                            target->removeFile(file);
                            moved = true;

                    } else {
                        File *f = new File(*(File *) (target->getChildren()[i]));
                        destination->addFile(f);
                        target->removeFile(file);
                        moved = true;
                    }
                }
            }
        }
        if(!moved)
        {
            cout<<"Can't move directory"<<endl;
        }
    }
    else
    {
        cout<<"No such file or directory"<<endl;
    }
}

string MvCommand::toString() {
    return getArgs();
}

MvCommand::~MvCommand(){}

//Rename Command
RenameCommand::RenameCommand(string args) : BaseCommand(args) {}
void RenameCommand::execute(FileSystem & fs) {                                    //rename a/b/toRename newName
    string args = getArgs().substr(getArgs().find_first_of(" ")+1);
    string oldName , newName , path;
    Directory *parentOf;
    if(args.find_first_of("/") == std::string::npos){
        oldName = args.substr(0,args.find_last_of(" "));
        newName = args.substr(args.find_last_of(" ")+1);
        Directory* dir = &fs.getWorkingDirectory();
        vector<BaseFile*> vec =  dir->getChildren();
        if(index_of_file(oldName, dir) != -1){
            if(index_of_file(newName, dir)== -1){
                if(checkName(newName)){
                    vec[index_of_file(oldName, dir)]->setName(newName);
                }
            }
        }else cout << "No such file or directory" << endl;

    }else {
        path = args.substr(0, args.find_last_of("/"));
        if (path == "") {
            path = "/";
        }
        oldName = args.substr(args.find_last_of("/") + 1, args.find_last_of(" ") - args.find_last_of("/")-1);
        newName = args.substr(args.find_last_of(" ") + 1);
        if(path == "/"){
            parentOf = &fs.getRootDirectory();
        }else   parentOf = pathFromUser(path, fs);

        if (parentOf) {
            if (index_of_file(oldName, parentOf) != -1) {
                if (index_of_file(newName, parentOf) == -1) {
                    if (parentOf->getChildren()[index_of_file(oldName, parentOf)] != &fs.getWorkingDirectory()) {
                        parentOf->getChildren()[index_of_file(oldName, parentOf)]->setName(newName);
                    } else cout << "Can't rename the working directory" << endl;
                } else cout << "No such file or directory" << endl;
            }
        } else cout << "The system cannot find the path specified" << endl;
    }
}

string RenameCommand::toString() {
    return getArgs();

}

RenameCommand::~RenameCommand(){}

//RmCommand

RmCommand::RmCommand(string args) : BaseCommand(args) {}

void RmCommand::execute(FileSystem & fs)
{
    Directory *dir;
    bool removed = false;
    string file;//the file/directory name we want to remove
    string path = getArgs().substr(getArgs().find(" ")+1, getArgs().length()-getArgs().find(" ")+1);
    if((path.compare(fs.getWorkingDirectory().getAbsolutePath())==0)||(path.compare(fs.getRootDirectory().getAbsolutePath())==0))
    {
        cout<<"Can't remove directory"<<endl;
        return;
    }
    if(path.find("/")==string::npos)
    {
        dir = &fs.getWorkingDirectory();
        file = path;

    }
    else
    {
        file = path.substr(path.find_last_of("/")+1,path.length()-path.find_last_of("/")+1);
        path = path.substr(0,path.find_last_of("/"));
        dir = pathFromUser(path,fs);
    }

    if(dir)
    {
        for(unsigned int i=0;i<dir->getChildren().size();i++)
        {
            if(dir->getChildren()[i]->getName()==file)
            {

                    dir->removeFile(dir->getChildren()[i]);
                    removed = true;
            }
        }
        if(!removed)
        {
            cout<<"No such file or directory"<<endl;
        }
    }
    else
    {
        cout<<"No such file or directory"<<endl;
    }

}

string RmCommand::toString() {
    return getArgs();
}

RmCommand::~RmCommand(){}

//HistoryCommand
HistoryCommand::HistoryCommand(string args, const vector<BaseCommand *> & history) : BaseCommand(args), history(history) {}

void HistoryCommand::execute(FileSystem & fs)
{
    unsigned int size =this->history.size();
    for(unsigned int i=0 ; i<size;i++)
    {
        cout<<std::to_string(i)<<"\t"+history[i]->toString()<<endl;
    }
}

string HistoryCommand::toString() {
    return getArgs()+ " ";
}

HistoryCommand::~HistoryCommand(){}

//ExecCommand
ExecCommand::ExecCommand(string args, const vector<BaseCommand *> & history) : BaseCommand(args), history(history) {}

void ExecCommand::execute(FileSystem & fs) {
    string numOfCom = this->getArgs().substr(this->getArgs().find_first_of(" "));
    unsigned int num = stoi(numOfCom);
    if((num >= 0) & (num <= history.size())){
        history[num]->execute(fs);
    }
    else
    {
        cout<<"Command not found"<<endl;
    }
}

string ExecCommand::toString() {
    return getArgs();
}

ExecCommand::~ExecCommand(){}

//Error Command
ErrorCommand::ErrorCommand(string args) : BaseCommand(args) {}

void ErrorCommand::execute(FileSystem & fs) {
    string errorCom = this->getArgs();
    if (errorCom.find_first_of(" ") != std::string::npos) {
        errorCom = errorCom.substr(0,errorCom.find_first_of(" "));
    }
    cout << errorCom + ": Unknown command" << endl;
}

string ErrorCommand::toString() {
    return getArgs().substr(getArgs().find(" ")+1,getArgs().length()-getArgs().find(" "));
}

ErrorCommand::~ErrorCommand(){}


//Verbose Command
VerboseCommand::VerboseCommand(string args) : BaseCommand(args) {}

void VerboseCommand::execute(FileSystem & fs)
{
    unsigned int num= std::stoi(getArgs().substr(getArgs().find(" ")+1, getArgs().length()-getArgs().find(" ")+1));
    if(num>3||num<0)
    {
        cout<<"Wrong verbose input"<<endl;
    }
    else
    {
        verbose = num;
    }
}

string VerboseCommand::toString() {
    return getArgs();
}

VerboseCommand::~VerboseCommand(){}