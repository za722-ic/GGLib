#pragma once

#include "GG/AssetManagement/AssetManager.h"

// wrapper around engine's asset manager to avoid dealing with strings and filenames
class AssetManagerWrapper
{
private:
	AssetManager* assetManager;

	const std::string baseAssetPath = "resources/";

	const std::string fontFile = "NotoSans-Medium.ttf";
	const unsigned int fontSize = 14;


public:

	// TODO: use weak ptr instead?
	AssetManagerWrapper(AssetManager* assetManager) : assetManager(assetManager)
	{
		assetManager->setBaseAssetPath(baseAssetPath);
	}

	bool loadResources()
	{
		bool loadedSuccessfully = true;

		// fonts
		loadedSuccessfully &= assetManager->loadFont(fontFile, fontSize);

		return loadedSuccessfully;
	}

	TTF_Font* getFont()
	{
		auto fontOptional = assetManager->getFont(fontFile);
		if (!fontOptional) throw std::runtime_error("ERROR: failed to get font");
		return fontOptional.value();
	}
};
