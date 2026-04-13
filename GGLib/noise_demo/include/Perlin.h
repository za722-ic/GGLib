#pragma once

#define _USE_MATH_DEFINES // TODO: microsoft only i think --> i did this to get M_PI, but maybe just have a bunch of constants defined in MoreMath? For portability. See: https://learn.microsoft.com/en-us/cpp/c-runtime-library/math-constants?view=msvc-170
#include <vector>
#include <numeric>
#include <cmath>

#include "Vec2D.h"
#include "Vec3D.h"
#include "MoreMath.h"

class PermutationTable
{
private:
	const std::vector<int> P = generatePermutationTable();

	std::vector<int> generatePermutationTable()
	{
		const int tableSize = 256;

		// create array with values 0, 1, 2, ..., 254, 255
		std::vector<int> permutationTable(tableSize);
		std::iota(std::begin(permutationTable), std::end(permutationTable), 0);

		// pseudo-randomly shuffle array
		shuffle(permutationTable);

		// create a copy of permutation table, and then append another copy to it
		std::vector<int> cPermutationTable = permutationTable;
		cPermutationTable.insert(cPermutationTable.end(), permutationTable.begin(), permutationTable.end());

		return cPermutationTable;
	}

	void shuffle(std::vector<int>& arr)
	{
		// https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
		for (int i = arr.size() - 1; i >= 1; i--)
		{
			// generate random num from 0 to i
			int j = rand() % (i + 1);

			// swap arr[i] and arr[j]
			int tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
		}
	}

public:
	int get(int i)
	{
		return P.at(i);
	}
};

class Perlin
{
protected:
	PermutationTable P;

    virtual float noiseGen(const std::vector<float>& coords) = 0;

    float nNoiseGen(const std::vector<float>& coords)
    {
        return (noiseGen(coords) + 1.0f) / 2.0f;
    }

    float fbmGen(int octaves, float initialFrequency, float initialAmplitude, const std::vector<float>& coords)
    {
        float noiseVal = 0.0f;

        float amplitude = initialAmplitude;
        float frequency = initialFrequency;

        // See here for definitions: https://libnoise.sourceforge.net/glossary/#persistence
        const float persistence = 0.5f;
        const float lacunarity = 2.0f;

        for (int octave = 0; octave < octaves; octave++)
        {
            // multiply coords by frequency before inputting to noiseGen (we are performing noise(fc))
			std::vector<float> c = coords;
            for (int i = 0; i < c.size(); i++) c[i] *= frequency;

            noiseVal += amplitude * noiseGen(c);

            amplitude *= persistence;
            frequency *= lacunarity;
        }

        return noiseVal;
    }

    float fade(float t)
    {
        return ((6 * t - 15) * t + 10) * t * t * t;
    }
};

class Perlin2D : Perlin
{
    float noiseGen(const std::vector<float>& coords) override
    {
        /*
           -- numbering scheme --
           first number is x, second number is y:

           01    11
            +----+
            |    |
            +----+
           00    10
        */

        float x = coords[0];
        float y = coords[1];

        // coords of square containing {x, y}
        int X0 = ((int)std::floor(x)) & 255; // Use Math.floor to account for negative numbers. Then cast to int as Math.floor returns float
        int X1 = ((int)std::floor(x + 1)) & 255;

        int Y0 = ((int)std::floor(y)) & 255;
        int Y1 = ((int)std::floor(y + 1)) & 255;

        // {x, y} := position of {x, y} in square
        x = x - (int)std::floor(x);
        y = y - (int)std::floor(y);

        // get pseudorandom gradients at each of the 4 square corners
        Vec2D grad00 = grad(hash(X0, Y0));
        Vec2D grad01 = grad(hash(X0, Y1));
        Vec2D grad10 = grad(hash(X1, Y0));
        Vec2D grad11 = grad(hash(X1, Y1));

        // get offsets (points from corner to {x, y})
        Vec2D offset00(x, y);       // X coordinate of cell 0, 0
        Vec2D offset01(x, y - 1);
        Vec2D offset10(x - 1, y);
        Vec2D offset11(x - 1, y - 1);

        // grad DOT offset
        float dot00 = offset00.dot(grad00);
        float dot01 = offset01.dot(grad01);
        float dot10 = offset10.dot(grad10);
        float dot11 = offset11.dot(grad11);

        // fade vals
        float u = fade(x);
        float v = fade(y);

        // bilinear interpolation
        float noiseVal = MoreMath::biLerp(dot00, dot01, dot10, dot11, u, v);

        return noiseVal;
    }

    int hash(int x, int y)
    {
        return P.get(P.get(x) + y); // we need 512 rather than 256 elements in the permutation table due to this line (P.get returns max 255, y is max 255, sum is max 510)
    }

    Vec2D grad(int hash)
    {
        double angle = (2 * M_PI) * (hash / 256.0f);
        float vecX = (float)cos(angle);
        float vecY = (float)sin(angle);
        return Vec2D(vecX, vecY);
    }

public:
    float noise(float x, float y)
    {
        return noiseGen({ x, y });
    }
    float nNoise(float x, float y) { return nNoiseGen({ x, y }); }
    float fbm(float x, float y, float initialFrequency, float initialAmplitude, int octaves)
    {
        return fbmGen(octaves, initialFrequency, initialAmplitude, { x, y });
    }
};

class Perlin3D : public Perlin
{
protected:
    float noiseGen(const std::vector<float>& coords) override
    {
        float x = coords[0];
        float y = coords[1];
        float z = coords[2];

        // coords of cube containing {x, y, z}
        int X0 = ((int)std::floor(x)) & 255;
        int X1 = ((int)std::floor(x + 1)) & 255;

        int Y0 = ((int)std::floor(y)) & 255;
        int Y1 = ((int)std::floor(y + 1)) & 255;

        int Z0 = ((int)std::floor(z)) & 255;
        int Z1 = ((int)std::floor(z + 1)) & 255;

        // {x, y, z} := position of {x, y, z} in cube
        x = x - (int)std::floor(x);
        y = y - (int)std::floor(y);
        z = z - (int)std::floor(z);

        // get pseudorandom gradients at each of the 8 cube corners
        Vec3D grad000 = grad(hash(X0, Y0, Z0));
        Vec3D grad001 = grad(hash(X0, Y0, Z1));
        Vec3D grad010 = grad(hash(X0, Y1, Z0));
        Vec3D grad011 = grad(hash(X0, Y1, Z1));
        Vec3D grad100 = grad(hash(X1, Y0, Z0));
        Vec3D grad101 = grad(hash(X1, Y0, Z1));
        Vec3D grad110 = grad(hash(X1, Y1, Z0));
        Vec3D grad111 = grad(hash(X1, Y1, Z1));

        // get offsets (points from corner to {x, y, z})
        Vec3D offset000(x, y, z);
        Vec3D offset001(x, y, z - 1);
        Vec3D offset010(x, y - 1, z);
        Vec3D offset011(x, y - 1, z - 1);
        Vec3D offset100(x - 1, y, z);
        Vec3D offset101(x - 1, y, z - 1);
        Vec3D offset110(x - 1, y - 1, z);
        Vec3D offset111(x - 1, y - 1, z - 1);

        // grad DOT offset
        float dot000 = offset000.dot(grad000);
        float dot001 = offset001.dot(grad001);
        float dot010 = offset010.dot(grad010);
        float dot011 = offset011.dot(grad011);
        float dot100 = offset100.dot(grad100);
        float dot101 = offset101.dot(grad101);
        float dot110 = offset110.dot(grad110);
        float dot111 = offset111.dot(grad111);

        // fade vals
        float u = fade(x);
        float v = fade(y);
        float w = fade(z);

        // trilinear interpolation
        float noiseVal = MoreMath::triLerp(dot000,
            dot001,
            dot010,
            dot011,
            dot100,
            dot101,
            dot110,
            dot111,
            u,
            v,
            w);

        return noiseVal;
    }

private:
    int hash(int x, int y, int z)
    {
        return P.get(P.get(P.get(x) + y) + z);
    }

    Vec3D grad(int hash)
    {
        /*
        Where do these 16 vectors come from?

        From Ken Perlin's improved noise paper (https://mrl.cs.nyu.edu/~perlin/paper445.pdf):
            "...the 12 vectors defined by the directions from the center of a cube to its edges..."
        and
            "To avoid the cost of dividing by 12, we pad to 16 gradient
             directions, adding an extra (1,1,0),(-1,1,0),(0,-1,1) and (0,-1,-1).
             These form a regular tetrahedron, so adding them redundantly
             introduces no visual bias in the texture."

        Visualisation (from this article: https://www.scratchapixel.com/lessons/procedural-generation-virtual-worlds/perlin-noise-part-2/improved-perlin-noise.html):
             https://www.scratchapixel.com/images/noise-part-2/noise-12directions.png?
        */

        switch (hash & 0xF)
        {
		    // 4 vectors from origin to the corners of a unit quad in the x-y plane (z=0)
            case 0x0: return Vec3D(1.0f, 1.0f, 0.0f);
            case 0x1: return Vec3D(-1.0f, 1.0f, 0.0f);
            case 0x2: return Vec3D(1.0f, -1.0f, 0.0f);
		    case 0x3: return Vec3D(-1.0f, -1.0f, 0.0f);

	        // 4 vectors from origin to the corners of a unit quad in the x-z plane (y=0)
		    case 0x4: return Vec3D(1.0f, 0.0f, 1.0f);
		    case 0x5: return Vec3D(-1.0f, 0.0f, 1.0f);
		    case 0x6: return Vec3D(1.0f, 0.0f, -1.0f);
		    case 0x7: return Vec3D(-1.0f, 0.0f, -1.0f);

	        // 4 vectors from origin to the corners of a unit quad in the y-z plane (x=0)
		    case 0x8: return Vec3D(0.0f, 1.0f, 1.0f);
		    case 0x9: return Vec3D(0.0f, -1.0f, 1.0f);
		    case 0xA: return Vec3D(0.0f, 1.0f, -1.0f);
		    case 0xB: return Vec3D(0.0f, -1.0f, -1.0f);

	        // padding so that we have 16 vectors (allows us to use bitwise-and instead of modulo)
		    case 0xC: return Vec3D(1.0f, 1.0f, 0.0f);
		    case 0xD: return Vec3D(0.0f, -1.0f, 1.0f);
		    case 0xE: return Vec3D(-1.0f, 1.0f, 0.0f);
		    case 0xF: return Vec3D(0.0f, -1.0f, -1.0f);

	        // expected value is 0...15
		    default: std::cout << "Unexpected value: " + (hash & 0xF);
        };
    }

public:
    float noise(float x, float y, float z)
    {
        return noiseGen({ x, y, z });
    }
    float nNoise(float x, float y, float z)
    {
        return nNoiseGen({ x, y, z });
    }
    float fbm(float x, float y, float z, float initialFrequency, float initialAmplitude, int octaves)
    {
        return fbmGen(octaves, initialFrequency, initialAmplitude, { x, y, z });
    }
};
