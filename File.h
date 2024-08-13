#ifndef FILE_H
#define FILE_H
#include <fstream>
#include <string>
#include <iostream>
#include <streambuf>
using namespace std;

struct File_Error {
	string what;
	File_Error(string what_) : what{ what_ } {}
	File_Error() : what{ "File error!" } {}
};

struct File {
	string buffer;

	void load(int argc, char* argv[]) {
		if (argc != 2)
			throw File_Error("Invalid input.");
		ifstream file(argv[1]);
		buffer = string(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
	}
};

struct Object {
	void load(string file_string_, int argc, char* argv[]) {
		if (argc != 2)
			throw File_Error("Invalid output.");
		string object_name;
		size_t size = sizeof(argv[1]) / sizeof(argv[1][0]);
		for (size_t i = 0; i < size; i++) {
			if (argv[1][i] == '.')
				break;
			object_name += argv[1][i];
		}

		ofstream output_file(object_name + ".cpp");
		if (output_file.is_open()) { 
			output_file << file_string_;
			output_file.close(); 
		}
		else {
			throw File_Error("File did not save properly.");
		}
	}
};
#endif