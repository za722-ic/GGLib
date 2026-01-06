#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>

#include <string>
#include <iostream>
#include <optional>
#include <unordered_map>
#include <functional>

#include "TextFile.h"

class AssetManager
{
private:
    std::string baseAssetPath = "resources/";

    std::shared_ptr<std::unordered_map<std::string, TTF_Font*>> fontPtrs;
    std::shared_ptr<std::unordered_map<std::string, Mix_Chunk*>> audioPtrs;
    std::shared_ptr<std::unordered_map<std::string, SDL_Texture*>> texturePtrs;
    std::shared_ptr<std::unordered_map<std::string, TextFile*>> textFilePtrs;

    SDL_Renderer* renderer = nullptr; // used when loading textures

public:
    AssetManager();
    ~AssetManager();

    void setRenderer(SDL_Renderer* renderer);

    void setBaseAssetPath(std::string newBaseAssetPath);

    bool loadFont(std::string fileName, int fontSize);
    std::optional<TTF_Font*> getFont(std::string fileName) const;
    void unloadFont(std::string fileName);
    std::weak_ptr<std::unordered_map<std::string, TTF_Font*>> getFontPtrs() const;

    bool loadAudio(std::string fileName);
    std::optional<Mix_Chunk*> getAudio(std::string fileName) const;
    void unloadAudio(std::string fileName);
    std::weak_ptr<std::unordered_map<std::string, Mix_Chunk*>> getAudioPtrs() const;

    bool loadTexture(std::string fileName);
    std::optional<SDL_Texture*> getTexture(std::string fileName) const;
    void unloadTexture(std::string fileName);
    std::weak_ptr<std::unordered_map<std::string, SDL_Texture*>> getTexturePtrs() const;

    bool loadTextFile(std::string fileName);
    std::optional<TextFile*> getTextFile(std::string fileName) const;
    void unloadTextFile(std::string fileName);
    std::weak_ptr<std::unordered_map<std::string, TextFile*>> getTextFilePtrs() const;

private:
    std::string getFilePath(std::string fileName) const;
    template <typename T> void freeMap(std::shared_ptr<std::unordered_map<std::string, T>> ptrMap, std::function<void(std::string)> freeFunc);

    // This is here cus Rule Of 3: https://stackoverflow.com/questions/9139103/am-i-violating-rule-of-three
    AssetManager(AssetManager const&) = delete;
    AssetManager& operator=(AssetManager const&) = delete;
};
