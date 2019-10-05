#include "../include/FileSystem.h"
#include "../include/GlobalVariables.h"
#include <iostream>

//Constructor
FileSystem::FileSystem():rootDirectory(new Directory("/", nullptr)),workingDirectory(nullptr)
{
    workingDirectory = rootDirectory;
}

// Destructor
FileSystem::~FileSystem()
{
    delete rootDirectory;
    rootDirectory = nullptr;
    workingDirectory = nullptr;
    if (verbose==1 || verbose==3)cout << "~FileSystem()" << endl;
}

// Copy Constructor
FileSystem::FileSystem(const FileSystem &other):rootDirectory(new Directory(*other.rootDirectory)),workingDirectory(new Directory(*other.workingDirectory))
{
    if (verbose==1 || verbose==3)cout << "FileSystem(const FileSystem& other)" << endl;
}

// Move Constructor
FileSystem::FileSystem(FileSystem &&other):rootDirectory(other.workingDirectory),workingDirectory(other.workingDirectory)
{
    other.workingDirectory = nullptr;
    other.rootDirectory = nullptr;
    if (verbose==1 || verbose==3)cout << "FileSystem(FileSystem&& rhs)" << endl;
}

// Copy Assignment
FileSystem& FileSystem::operator=(const FileSystem &other)
{
    if (this != &other)
    {

        delete rootDirectory;
        FileSystem(other);

    }
    if (verbose==1 || verbose==3)cout << "FileSystem& operator=(const FileSystem& rhs)" << endl;
    return *this;

}

// Move Assignment
FileSystem& FileSystem::operator=(FileSystem &&other)
{
    if (this != &other)
    {
        delete rootDirectory;
        rootDirectory = other.rootDirectory;
        workingDirectory = other.workingDirectory;
        other.rootDirectory = nullptr;
        other.workingDirectory = nullptr;
    }
    if (verbose==1 || verbose==3)cout << "FileSystem& operator=(FileSystem&& rhs)" << endl;
    return *this;
}

Directory& FileSystem::getRootDirectory() // Return reference to the root directory
{
    return *rootDirectory;
}
Directory& FileSystem::getWorkingDirectory() // Return reference to the working directory
{
    return *workingDirectory;
}
void FileSystem::setWorkingDirectory(Directory *newWorkingDirectory) // Change the working directory of the file system
{
    workingDirectory = newWorkingDirectory;
}