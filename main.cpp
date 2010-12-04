#include <windows.h>
#include <iostream>
#include <fstream>

// Filepath is a cString containing the directory path
// Extension is a cString containing the file type to search for.
// Handle is a reference to a handle which points to the file
// FD is a reference to a struct to store information about the file
// return: returns true of a file of the specified file type is found
// within the directory.
bool FindFileType(char *filepath, char * extension, HANDLE &handle, WIN32_FIND_DATA &fd) {
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
	WIN32_FIND_DATA fd;
	FINDEX_INFO_LEVELS fInfoLevelId;
	FINDEX_SEARCH_OPS fSearchOp;
	HANDLE handle = NULL;
	char *filepath;
	char *ext;
	char *type;

	if(argc == 3) {
		ext = argv[2];
	}
	else if(argc == 2) {
		ext = ".mp3";  // Assuming .mp3 since no filetype was specified
	}
	else {
		std::cout << "\nUsage: pm.exe <directory to music> <file extension>";
		return 1;
	}

	ZeroMemory(&fd, sizeof(WIN32_FIND_DATA));
	filepath = argv[1];

	if(FindFileType(filepath, ext, handle, fd)) {
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