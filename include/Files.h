/*
* Files.h
*
*  Created on: Nov 11, 2017
*      Author: shch
*/
using namespace std;
#ifndef FILES_H_
#define FILES_H_


#include <string>
#include <vector>

//////////////////////////////BaseFile//////////////////////////////

class BaseFile {
private:
    string name;

public:

    // Constructor
    BaseFile(string name);
    string getName() const;
    void setName(string newName);
    // Destructor
    virtual ~BaseFile();
    virtual int getSize() = 0;
    virtual void  printMyType()=0;
    virtual string getClass();
};

//////////////////////////////File//////////////////////////////

class File : public BaseFile {
private:
    int size;

public:
    // Constructor
    File(string name, int size);
    //Destructor
    ~File();
    int getSize(); // Return the size of the file
    void printMyType();
    string getClass();

};

//////////////////////////////Directory//////////////////////////////

class Directory : public BaseFile {
private:
    vector<BaseFile*> children;
    Directory *parent;

public:
    // Constructor
    Directory(string name, Directory *parent);
    // Destructor
    ~Directory();
    // Copy Constructor
    Directory(const Directory &other);
    // Move Constructor
    Directory(Directory &&other);
    // Copy Assignment
    Directory& operator=(const Directory &other);
    // Move Assignment
    Directory& operator=(Directory &&other);

    void clear();
    void copy(const Directory &other);


    Directory *getParent() const; // Return a pointer to the parent of this directory
    void setParent(Directory *newParent); // Change the parent of this directory
    void addFile(BaseFile* file); // Add the file to children
    void removeFile(string name); // Remove the file with the specified name from children
    void removeFile(BaseFile* file); // Remove the file from children
    void sortByName(); // Sort children by name alphabetically (not recursively)
    void sortBySize(); // Sort children by size (not recursively)
    vector<BaseFile*> getChildren(); // Return children
    int getSize(); // Return the size of the directory (recursively)
    string getAbsolutePath();  //Return the path from the root to this
    void printMyType();
    bool checkExistence(string name);
    string getClass();
};



#endif /* FILES_H_ */