#pragma once

#include "AssetManager.h"

// wrapper around engine's asset manager to avoid dealing with strings and filenames
class DemoAssetManager 
{
private:
	AssetManager* assetManager;

	const std::string baseAssetPath = "demo/resources/";

	const std::string fontFile = "Beef-d.ttf";
	

public:
	
	// TODO: use weak ptr instead?
	DemoAssetManager(AssetManager* assetManager);
	
	bool loadResources();

	TTF_Font*    getFont();
};