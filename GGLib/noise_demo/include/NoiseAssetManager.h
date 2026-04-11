#pragma once

#include "AssetManager.h"

// wrapper around engine's asset manager to avoid dealing with strings and filenames
class NoiseAssetManager 
{
private:
	AssetManager* assetManager;

	const std::string baseAssetPath = "noise_demo/resources/";

	const std::string fontFile = "NotoSans-Medium.ttf";
	const unsigned int fontSize = 14;
	

public:
	
	// TODO: use weak ptr instead?
	NoiseAssetManager(AssetManager* assetManager);
	
	bool loadResources();

	TTF_Font*    getFont();
};