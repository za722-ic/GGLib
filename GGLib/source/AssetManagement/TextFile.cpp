#include "GG/AssetManagement/TextFile.h"

bool TextFile::loadFromFile()
{
    std::ifstream file(filePath);

    if (!file) return false;

    fileText = "";

    try
    {
        std::string line;
        while (std::getline(file, line))
        {
            fileText += line + "\n";
        }
        fileText = fileText.substr(0, fileText.length() - 1); // remove final '\n'
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool TextFile::saveToDisk()
{
    std::ofstream file(filePath);

    if (!file) return false;

    file << fileText;

    return true;
}

TextFile* TextFile::loadTextFile(std::string filePath)
{
    TextFile* textFile = new TextFile(filePath);

    if (!textFile->loadFromFile()) return nullptr;

    return textFile;
}