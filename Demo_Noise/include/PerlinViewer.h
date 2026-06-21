#pragma once

#include "GG/UI/Control.h"

#include "Perlin.h"

class PerlinViewer : public GG::Control
{
private:
	float viewWidth = 16;
	float viewHeight = 9;

public:
	float posX = 0.0f;
	float posY = 0.0f;
	float posZ = 0.0f;

	float velX = 0.0f;
	float velY = 0.0f;
	float velZ = 0.0f;

	int resolutionDivision = 8;
	bool roundNoise = false;
	bool isClippedColorDrawn = false;

	int octaves = 1;
	float initialAmplitude = 1.0f;
	float initialFrequency = 1.0f;

	SDL_Texture* texture;

	bool isDirty = true;
	int oldW = -1, oldH = -1;
	int textureW, textureH;
	int oldResolutionDivision;
	
	Perlin3D pn;
public:

	PerlinViewer()
	{
		setColor(0x407848);
		borderColor = { 38, 84, 44, 255 };
		borderThickness = 4;
		shadowThickness = 6;
		//radius = 1;
	}

	~PerlinViewer()
	{
		if (texture != nullptr)
			SDL_DestroyTexture(texture);
	}

	void render(GG::Canvas* canvas) override
	{
		// call super
		Element::render(canvas);

		//return; // TODO for profiling --> add an option to UI to disable rendering

		// regenerate texture if the control's area has changed
		isDirty = (w != oldW) || (h != oldH) || (resolutionDivision != oldResolutionDivision);
		oldW = w;
		oldH = h;
		oldResolutionDivision = resolutionDivision;
		if (isDirty) generateTexture(canvas);

		// start rendering to texture
		SDL_SetRenderTarget(canvas->getSDLRenderer(), texture);

		// render noise
		drawNoise3D(canvas);

		// resume rendering to window 
		SDL_SetRenderTarget(canvas->getSDLRenderer(), NULL);

		// render texture to window
		const int imagePadding = 0;
		canvas->drawImage(texture, screenX + borderThickness + imagePadding, screenY+borderThickness + imagePadding, w - 2*borderThickness - 2*imagePadding, h - 2*borderThickness - 2*imagePadding);

		// render rounded inner frame on top of image to hide its corners
		canvas->drawRoundedRect(screenX, screenY, w, h, radius,5, borderColor, borderColor, 12);
	}

	void updateVelocities(float deltaTime)
	{
		posX += velX * deltaTime;
		posY += velY * deltaTime;
		posZ += velZ * deltaTime;
	}

private:
	void generateTexture(GG::Canvas *canvas)
	{
		if (texture != nullptr)	SDL_DestroyTexture(texture);

		textureW = (w - borderThickness)/resolutionDivision;
		textureH = (h - borderThickness)/resolutionDivision;

		viewHeight = ((float)textureH / textureW) * viewWidth;
		texture = SDL_CreateTexture(canvas->getSDLRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, textureW, textureH);
		SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST); // when enlarging the texture for rendering, keep things "pixelated"
	}

	void drawNoise3D(GG::Canvas *canvas)
	{
		// draw noise plot
		for (int x = 0; x < textureW; x++)
		{
			for (int y = 0; y < textureH; y++)
			{
				float noiseVal = pn.fbm(
					GG::MoreMath::lerp(getViewLeft(), getViewRight(), (float)x / textureW),
					GG::MoreMath::lerp(getViewTop(), getViewBottom(), (float)y / textureH),
					posZ,
					initialFrequency,
					initialAmplitude,
					octaves
				);

				noiseVal = (noiseVal + 1.0f) / 2.0f;

				drawNoisePoint3D(canvas, x, y, noiseVal);
			}
		}
	}

	void drawNoisePoint3D(GG::Canvas *canvas, int x, int y, float noiseVal)
	{
		// round noise to 0 or 1
		if (roundNoise)
			noiseVal = GG::MoreMath::step(noiseVal, 0.5f);

		// draw color onto buffered image
		int grayscaleValue = noiseVal * 255;
		
		if (!isClippedColorDrawn)
		{
			grayscaleValue = GG::MoreMath::clamp(grayscaleValue, 0, 255);
		}

		if (grayscaleValue > 255)
			canvas->setColor(255, 0, 255);
		else if (grayscaleValue < 0)
			canvas->setColor(255, 255, 0);
		else
			canvas->setColor(grayscaleValue);

		canvas->drawRect(x, y, 1, 1);
	}

	float getViewLeft()
	{
		return posX;
	}
	float getViewRight()
	{
		return posX + viewWidth;
	}
	float getViewTop()
	{
		return posY;
	}
	float getViewBottom()
	{
		return posY + viewHeight;
	}

};