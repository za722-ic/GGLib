#include "AssetManager.h"

AssetManager::AssetManager()
{
    fontPtrs = std::make_shared<std::unordered_map<std::string, TTF_Font*>>();
    audioPtrs = std::make_shared<std::unordered_map<std::string, MIX_Audio*>>();
    texturePtrs = std::make_shared<std::unordered_map<std::string, SDL_Texture*>>();
    textFilePtrs = std::make_shared<std::unordered_map<std::string, TextFile*>>();
}

AssetManager::~AssetManager()
{
    freeMap<TTF_Font*   >(fontPtrs,     [this](std::string fileName){  unloadFont(fileName);     });
    freeMap<MIX_Audio*  >(audioPtrs,    [this](std::string fileName){  unloadAudio(fileName);    });
    freeMap<SDL_Texture*>(texturePtrs,  [this](std::string fileName){  unloadTexture(fileName);  });
    freeMap<TextFile*   >(textFilePtrs, [this](std::string fileName){  unloadTextFile(fileName); });
}

void AssetManager::init(SDL_Renderer* newRenderer, MIX_Mixer* mixer)
{
    renderer = newRenderer;
}

void AssetManager::setBaseAssetPath(std::string newBaseAssetPath)
{
    baseAssetPath = newBaseAssetPath;
}

bool AssetManager::loadFont(std::string fileName, int fontSize)
{
    // unload font if we already have one of the same name
    if (fontPtrs->contains(fileName)) unloadFont(fileName);

    std::string filePath = getFilePath(fileName);

    TTF_Font* fontPtr = TTF_OpenFont((filePath).c_str(), fontSize);

    if (fontPtr == nullptr)
    {
        std::cout << "Failed to load font file: " << filePath << std::endl << SDL_GetError() << std::endl;
        return false;
    }

    (*fontPtrs)[fileName] = fontPtr;

    return true;
}
std::optional<TTF_Font*> AssetManager::getFont(std::string fileName) const
{
    if (!fontPtrs->contains(fileName)) return std::nullopt;
    return fontPtrs->at(fileName);
}
void AssetManager::unloadFont(std::string fileName)
{ 
    return; // TODO: this function is broken --> undefined pointer error

    if (!fontPtrs->contains(fileName)) return;

    TTF_CloseFont((*fontPtrs)[fileName]);

    fontPtrs->erase(fileName);
}
std::weak_ptr<std::unordered_map<std::string, TTF_Font*>> AssetManager::getFontPtrs() const
{
    return fontPtrs;
}


bool AssetManager::loadAudio(std::string fileName)
{
    // unload audio chunk if we already have one of the same name
    if (audioPtrs->contains(fileName)) unloadAudio(fileName);

    std::string filePath = getFilePath(fileName);

    MIX_Audio* audioPtr = MIX_LoadAudio(mixer, (filePath).c_str(), true); // setting predecode to true - more memory usage as we decode to uncompressed audio immediately on load, but no need to decode during runtime which decreases CPU usage (I THINK)

    if (audioPtr == nullptr)
    {
        std::cout << "Failed to load audio file: " << filePath << std::endl << SDL_GetError() << std::endl;
        return false;
    }

    (*audioPtrs)[fileName] = audioPtr;

    return true;
}
std::optional<MIX_Audio*> AssetManager::getAudio(std::string fileName) const
{
    if (!audioPtrs->contains(fileName)) return std::nullopt;
    return audioPtrs->at(fileName);
}
void AssetManager::unloadAudio(std::string fileName)
{
    if (!audioPtrs->contains(fileName)) return;

    MIX_DestroyAudio((*audioPtrs)[fileName]);

    audioPtrs->erase(fileName);
}
std::weak_ptr<std::unordered_map<std::string, MIX_Audio*>> AssetManager::getAudioPtrs() const
{
    return audioPtrs;
}


bool AssetManager::loadTexture(std::string fileName)
{
    // unload texture if we already have one of the same name
    if (texturePtrs->contains(fileName)) unloadTexture(fileName);

    std::string filePath = getFilePath(fileName);

    SDL_Texture* texturePtr = IMG_LoadTexture(this->renderer, filePath.c_str());
    if (texturePtr == nullptr)
    {
        std::cout << "Failed to load image file: " << filePath << std::endl << SDL_GetError() << std::endl;
        return false;
    }

    (*texturePtrs)[fileName] = texturePtr;

    return true;
}
std::optional<SDL_Texture*> AssetManager::getTexture(std::string fileName) const
{
    if (!texturePtrs->contains(fileName)) return std::nullopt;
    return texturePtrs->at(fileName);
}
void AssetManager::unloadTexture(std::string fileName)
{
    if (!texturePtrs->contains(fileName)) return;

    SDL_DestroyTexture((*texturePtrs)[fileName]);

    texturePtrs->erase(fileName);
}
std::weak_ptr<std::unordered_map<std::string, SDL_Texture*>> AssetManager::getTexturePtrs() const
{
    return texturePtrs;
}


bool AssetManager::loadTextFile(std::string fileName)
{
    // unload text file if we already have one of the same name
    if (textFilePtrs->contains(fileName)) unloadTextFile(fileName);

    std::string filePath = getFilePath(fileName);

    TextFile* textFilePtr = TextFile::loadTextFile(filePath.c_str());

    if (textFilePtr == nullptr)
    {
        std::cout << "Failed to load text file file: " << filePath << std::endl << SDL_GetError() << std::endl;
        return false;
    }

    (*textFilePtrs)[fileName] = textFilePtr;

    return true;
}
std::optional<TextFile*> AssetManager::getTextFile(std::string fileName) const
{
    if (!textFilePtrs->contains(fileName)) return std::nullopt;
    return textFilePtrs->at(fileName);
}
void AssetManager::unloadTextFile(std::string fileName)
{
    if (!textFilePtrs->contains(fileName)) return;

    delete textFilePtrs->at(fileName); // we do NOT write out on delete

    textFilePtrs->erase(fileName);
}
std::weak_ptr<std::unordered_map<std::string, TextFile*>> AssetManager::getTextFilePtrs() const
{
    return textFilePtrs;
}


std::string AssetManager::getFilePath(std::string fileName) const
{
    return baseAssetPath + fileName;
}

template <typename T> void AssetManager::freeMap(std::shared_ptr<std::unordered_map<std::string, T>> ptrMap, std::function<void(std::string)> freeFunc)
{
    for (auto it = ptrMap->begin(); it != ptrMap->end();)
    {
        auto tmp = it; 
        ++tmp; 
        freeFunc(it->first); 
        it = tmp; 
    }
}