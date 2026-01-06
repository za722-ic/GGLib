#pragma once

#include <string>
#include <fstream>

/*
NOTE: This is intended for easy manipulation of small files only. It cannot robustly handle large files. It reads entire files into memory when loadFromFile() is called.
*/
class TextFile
{
private:
    std::string filePath;

public:
    std::string fileText;

public:
    TextFile(std::string filePath) : filePath(filePath)
    {}

    bool loadFromFile();

    bool saveToDisk();

    static TextFile* loadTextFile(std::string filePath);
};
