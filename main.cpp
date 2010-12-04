#include <windows.h>
#include <iostream>
#include <fstream>

namespace { 
	WIN32_FIND_DATA fd;
	FINDEX_INFO_LEVELS fInfoLevelId;
	FINDEX_SEARCH_OPS fSearchOp;
	HANDLE handle = NULL;
	char filepath[256];
	char ext[256];
	char *type;
}  // Anonymous namespace

// input: extension is a pointer to a cstring that is the type of
// file to make a playlit of.
// return: returns true of a file of the specified file type is found
// within the directory.
bool TestFileType(char * extension) {
	char file[256];
	strcpy(file, filepath);
	strcat(file, "\\*");
	strcat(file, extension);

	handle = FindFirstFile(file, &fd);

	if(handle != INVALID_HANDLE_VALUE)
		return true;
	
	return false;	
}

int main(int argc, char *argv[]) {
	if(argc == 3) {
		strcpy(ext, argv[2]);
	}
	else if(argc == 2) {
		strcpy(ext, ".mp3");  // Assuming .mp3 since no filetype was specified
	}
	else {
		std::cout << "\nUsage: pm.exe <directory to music> <file extension>";
		return 1;
	}

	ZeroMemory(&fd, sizeof(WIN32_FIND_DATA));
	strcpy(filepath, argv[1]);

	if(TestFileType(ext)) {
		int value = 1;
		char saveto[256];
		std::ofstream playlist;

		strcpy(saveto, filepath);
		strcat(saveto, "\\00-Playlist.m3u");

		playlist.open(saveto);

		if(playlist.fail()) {
			std::cout << "failed to open '" << saveto << "' for writing\n";
			return 1;
		}
		else {
			while( value != 0) {
				std::cout << fd.cFileName << std::endl;
				playlist << fd.cFileName << std::endl;
				value = FindNextFile(handle, &fd);
			}
		}
	}
	else {
		std::cout << "Failed to find the file type '" << ext << "'\n";
		return 0;
	}

	return 0;
}