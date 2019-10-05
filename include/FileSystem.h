
#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "Files.h"


class FileSystem {
private:
	Directory* rootDirectory;
	Directory* workingDirectory;
public:
	//Constructor
	FileSystem();

	// Destructor
	virtual ~FileSystem();

	// Copy Constructor
	FileSystem(const FileSystem &other);

	// Move Constructor
	FileSystem(FileSystem &&other);

	// Copy Assignment
	virtual FileSystem& operator=(const FileSystem &other);

	// Move Assignment
	virtual FileSystem& operator=(FileSystem &&other);

	Directory& getRootDirectory() ; // Return reference to the root directory
	Directory& getWorkingDirectory() ; // Return reference to the working directory
	void setWorkingDirectory(Directory *newWorkingDirectory); // Change the working directory of the file system


};




#endif /* FILESYSTEM_H_ */
#pragma once
