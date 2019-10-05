//
// Created by shch on 11/18/17.
//

using namespace std;

#include "../include/Files.h"
#include <vector>
#include <iostream>
#include <tuple>
#include <algorithm>
#include "../include/GlobalVariables.h"
//////////////////////////////BaseFile//////////////////////////////
//Constructor
BaseFile::BaseFile(string name):name(name){}

string BaseFile::getName() const
{
    return name;
}

void BaseFile::setName(string newName_)
{
    name = newName_;
}

string BaseFile::getClass()
{
    return "BaseFile";
}

BaseFile::~BaseFile()=default;
//////////////////////////////File//////////////////////////////


//Constructor
File::File(string name, int size) :BaseFile(name), size(size) {}// Constructor

//Destructor
File::~File()=default;

int File::getSize()// Return the size of the file
{
    return size;
}

void File::printMyType() {
    cout << "FILE";
}
string File::getClass()
{
    return "File";
}
//////////////////////////////Directory//////////////////////////////

//Constructor
Directory::Directory(string name, Directory *parent_) :BaseFile(name), children(),parent(parent_)// Constructor
{
    if (parent_)
    {
        parent->addFile(this);
    }

}
// Destructor
Directory::~Directory()
{
    BaseFile *bs = nullptr;
    std::vector<BaseFile*> vec = children;
    unsigned  vec_size = children.size();

    if (!vec.empty()) {
        for (unsigned i = 0; i <vec_size; i++)
        {
            if(!(vec[i]->getClass()=="BaseFile")) {
                bs = vec[i];
                delete bs;
            }
        }
    }
    children.clear();
    parent = nullptr;
    if (verbose==1 || verbose==3)cout << "~Directory()" << endl;
}

// Copy Constructor
Directory::Directory(const Directory &other) :BaseFile(other.getName()), children(),parent(other.parent)
{
    copy(other);
    if (verbose==1 || verbose==3)cout << "Directory(const Directory& other)" << endl;
}

// Move Constructor
Directory::Directory(Directory &&other):BaseFile(other.getName()), children(other.getChildren()),parent(other.parent)//do we need to do it recursively or the changing of the first pointers are enough?
{
    unsigned int vec_size = getSize();
    for (unsigned int i = 0; i < vec_size; i++)
    {
        children[i] = other.children[i];
        other.children[i] = nullptr;
    }
    if (verbose==1 || verbose==3)cout << "Directory(Directory&& rhs)" << endl;
}

// Copy Assignment
Directory& Directory::operator=(const Directory &other)
{
    if (this != &other)
    {
        clear();
        copy(other);
    }
    if (verbose==1 || verbose==3)cout << "Directory& operator=(const Directory& rhs)" << endl;
    return *this;
}

// Move Assignment
Directory& Directory::operator=(Directory &&other)
{
    if (this != &other)
    {
        clear();
        parent = other.parent;
        other.parent = nullptr;

        unsigned int vec_size = getSize();
        for (unsigned int i = 0; i < vec_size; i++)
        {
            children[i] = other.children[i];
            other.children[i] = nullptr;
        }
    }
    if (verbose==1 || verbose==3)cout << "Directory& operator=(Directory&& rhs)" << endl;
    return *this;
}

void Directory::clear() {

    if (!children.empty()) {
        for (unsigned i = 0; i <children.size(); i++)
        {
            delete this->children[i];
            this->children[i]= nullptr;
        }

        this->children.clear();
    }
    this->parent = nullptr;
    if (verbose==1 || verbose==3)cout << "~Directory()" << endl;
}

void Directory::copy(const Directory &other)
{
    std::vector<BaseFile*> vec = other.children;
    unsigned  vec_size = (&other)->children.size();
    setParent(other.getParent());
    if (vec_size != 0) {//has children
        for (unsigned  i = 0; i < vec_size; i++)
        {
            if (other.children[i]->getClass()=="Directory")
            {
                addFile(new Directory(*(Directory *)(vec[i])));
            }
            else if (other.children[i]->getClass()=="File")
            {
                BaseFile* F = new File(vec[i]->getName(), vec[i]->getSize());
                addFile(F);
            }

        }
    }
}

Directory* Directory::getParent() const // Return a pointer to the parent of this directory
{
    return parent;
}

void Directory::setParent(Directory *newParent) // Change the parent of this directory
{
    parent = newParent;
}

void Directory::addFile(BaseFile* file) // Add the file to children
{
    children.push_back(file);
    if(file->getClass()=="Directory")
    {
        ((Directory*)file) -> setParent(this);
    }

}

void Directory::removeFile(string name)// Remove the file with the specified name from children
{
    if ( !(children.empty()) )
    {
        for (unsigned i = 0 ; i< getChildren().size(); i++) {
            if (children[i]->getName() == name)
            {
                delete children[i];
                children.erase(children.begin()+i);


            }
        }
    }

}

void Directory::removeFile(BaseFile* file) // Remove the file from children
{
    removeFile(file->getName());

}
/*
void Directory::sortByName() // Sort children by name alphabetically (not recursively)
{

    sort(children.begin(), children.end(), [](BaseFile* left, BaseFile* right) {
        return (left->getName() < right->getName());
    });
}
void Directory::sortBySize() // Sort children by size (not recursively)
{
    stable_sort(children.begin(), children.end(), [](BaseFile* left, BaseFile* right) {
        string Lname = left->getName();
        string Rname = right->getName();
        int Lsize = left->getSize();
        int Rsize = right->getSize();
        return  tie(Lsize, Lname) < tie(Rsize, Rname);
    });
}
*/
void Directory::sortByName() // Sort children by name alphabetically (not recursively)
{

    sort(children.begin(), children.end(), [](BaseFile* left, BaseFile* right) {
        string Lname = left->getName();
        string Rname = right->getName();
        for(unsigned i = 0; i < Lname.size() ; i++){
            if((Lname[i] >= 65)&&(Lname[i] <=90)) Lname[i] = Lname[i]+32;
        }
        for(unsigned k = 0 ; k < Rname.size() ; k++){
            if((Rname[k] >= 65)&&(Rname[k] <= 90)) Rname[k] = Rname[k]+32;
        }
        return (Lname < Rname);
    });
}
void Directory::sortBySize() // Sort children by size (not recursively)
{
    stable_sort(children.begin(), children.end(), [](BaseFile* left, BaseFile* right) {
        string Lname = left->getName();
        string Rname = right->getName();
        for(unsigned i = 0; i < Lname.size() ; i++){
            if((Lname[i] >= 65)&&(Lname[i] <=90)) Lname[i] = Lname[i]+32;
        }
        for(unsigned k = 0 ; k < Rname.size() ; k++){
            if((Rname[k] >= 65)&&(Rname[k] <= 90)) Rname[k] = Rname[k]+32;
        }
        int Lsize = left->getSize();
        int Rsize = right->getSize();
        return  tie(Lsize, Lname) < tie(Rsize, Rname);
    });
}
vector<BaseFile*> Directory::getChildren()// Return children
{
    return children;
}

int Directory::getSize() // Return the size of the directory (recursively)
{
    unsigned  int sum = 0;
    for (unsigned int i = 0; i< children.size(); i++)
    {
        if((children[i]->getClass()=="File")||(children[i]->getClass()=="Directory"))
        {
            sum = sum + children[i]->getSize();
        }


    }
    return sum;
}

string Directory::getAbsolutePath()  //Return the path from the root to this
{
    Directory* dir = this;
    string path = "";
    while (dir->getName() != "/")
    {
        path = dir->getName() + "/" + path;
        dir = dir->getParent();
    }
    return "/" + path.substr(0,path.length()-1);
}

void Directory::printMyType() {
    cout << "DIR";
}

string Directory::getClass()
{
    return "Directory";
}