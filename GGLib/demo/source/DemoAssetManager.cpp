#include "DemoAssetManager.h"

DemoAssetManager::DemoAssetManager(AssetManager *assetManager) : assetManager(assetManager)
{
	assetManager->setBaseAssetPath(baseAssetPath);
}

bool DemoAssetManager::loadResources()
{
	bool loadedSuccessfully = true;

	// fonts
	loadedSuccessfully &= assetManager->loadFont(fontFile, fontSize);

	return loadedSuccessfully;
}

TTF_Font* DemoAssetManager::getFont()
{
	auto fontOptional = assetManager->getFont(fontFile);
	if (!fontOptional) throw std::runtime_error("ERROR: failed to get font");
	return fontOptional.value();
}