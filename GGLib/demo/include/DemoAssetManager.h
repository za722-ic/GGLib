#pragma once

#include "AssetManager.h"

// wrapper around engine's asset manager to avoid dealing with strings and filenames
class DemoAssetManager 
{
private:
	AssetManager* assetManager;

	const std::string baseAssetPath = "demo/resources/";

	//const std::string fontFile = "Beef-d.ttf";
	//const std::string fontFile = "lemon_milk/LEMONMILK-Bold.otf";
	const std::string fontFile = "calibri.ttf";
	const unsigned int fontSize = 24;
	

public:
	
	// TODO: use weak ptr instead?
	DemoAssetManager(AssetManager* assetManager);
	
	bool loadResources();

	TTF_Font*    getFont();
};