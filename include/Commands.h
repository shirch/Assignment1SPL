/*
* Commands.h
*
*  Created on: Nov 11, 2017
*      Author: nirkov
*/

#ifndef COMMANDS_H_
#define COMMANDS_H_



#include <string>
#include <iostream>
#include "FileSystem.h"



class BaseCommand {
private:
    string args;

public:
    BaseCommand(string args);
    virtual ~BaseCommand();
    string getArgs();
    virtual void execute(FileSystem & fs) = 0;
    virtual string toString() = 0;
};

class PwdCommand : public BaseCommand {
private:
public:
    PwdCommand(string args);
    virtual ~PwdCommand();
    void execute(FileSystem & fs); // Every derived class should implement this function according to the document (pdf)
    virtual string toString();
};

class CdCommand : public BaseCommand {
private:
public:
    CdCommand(string args);
    virtual ~CdCommand();
    void execute(FileSystem & fs);
    string toString();
};

class LsCommand : public BaseCommand {
private:
public:
    LsCommand(string args);
    virtual ~LsCommand();
    void execute(FileSystem & fs);
    string toString();
};

class MkdirCommand : public BaseCommand {
private:
public:
    MkdirCommand(string args);
    virtual ~MkdirCommand();
    void execute(FileSystem & fs);
    string toString();
};

class MkfileCommand : public BaseCommand {
private:
public:
    MkfileCommand(string args);
    virtual ~MkfileCommand();
    void execute(FileSystem & fs);
    string toString();
};

class CpCommand : public BaseCommand {
private:
public:
    CpCommand(string args);
    virtual ~CpCommand();
    void execute(FileSystem & fs);
    string toString();
};

class MvCommand : public BaseCommand {
private:
public:
    MvCommand(string args);
    virtual ~MvCommand();
    void execute(FileSystem & fs);
    string toString();
};

class RenameCommand : public BaseCommand {
private:
public:
    RenameCommand(string args);
    virtual ~RenameCommand();
    void execute(FileSystem & fs);
    string toString();
};

class RmCommand : public BaseCommand {
private:
public:
    RmCommand(string args);
    virtual ~RmCommand();
    void execute(FileSystem & fs);
    string toString();
};

class HistoryCommand : public BaseCommand {
private:
    const vector<BaseCommand *> & history;
public:
    HistoryCommand(string args, const vector<BaseCommand *> & history);
    virtual ~HistoryCommand();
    void execute(FileSystem & fs);
    string toString();
};


class VerboseCommand : public BaseCommand {
private:
public:
    VerboseCommand(string args);
    virtual ~VerboseCommand();
    void execute(FileSystem & fs);
    string toString();
};

class ErrorCommand : public BaseCommand {
private:
public:
    ErrorCommand(string args);
    virtual ~ErrorCommand();
    void execute(FileSystem & fs);
    string toString();
};

class ExecCommand : public BaseCommand {
private:
    const vector<BaseCommand *> & history;
public:
    ExecCommand(string args, const vector<BaseCommand *> & history);
    virtual ~ExecCommand();
    void execute(FileSystem & fs);
    string toString();
};


#endif /* COMMANDS_H_ */