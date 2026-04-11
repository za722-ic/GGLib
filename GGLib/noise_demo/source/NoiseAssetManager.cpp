#include "NoiseAssetManager.h"

NoiseAssetManager::NoiseAssetManager(AssetManager *assetManager) : assetManager(assetManager)
{
	assetManager->setBaseAssetPath(baseAssetPath);
}

bool NoiseAssetManager::loadResources()
{
	bool loadedSuccessfully = true;

	// fonts
	loadedSuccessfully &= assetManager->loadFont(fontFile, fontSize);

	return loadedSuccessfully;
}

TTF_Font* NoiseAssetManager::getFont()
{
	auto fontOptional = assetManager->getFont(fontFile);
	if (!fontOptional) throw std::runtime_error("ERROR: failed to get font");
	return fontOptional.value();
}