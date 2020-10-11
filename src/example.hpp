#ifndef __EXAMPLE_H__
#define __EXAMPLE_H__

#include <SDL2/SDL.h>
#include "sdl_framework.hpp"

using namespace std;

class Example : public SDLFramework
{
public:
	using SDLFramework::SDLFramework;

	virtual bool OnCreate()
		{
			return true;
		}

	virtual bool OnUpdate(float elapsed_time)
		{
			static int c = 0;
			static int z = 0;
			c += static_cast<int>(16.0f * elapsed_time);
			z += static_cast<int>(123.0f * elapsed_time);
			SDL_Renderer *renderer = GetRenderer();

			for (int y=0; y<ScreenHeight(); y++)
			{
				int r = ((y * y) >> 2) + z;
				float f = (static_cast<float>((r) % 256)) / 256.0f;

				SDL_SetRenderDrawColor(renderer, 255 - (f * 255), c, f * 255, 255);
				DrawLine(0, y, ScreenWidth() - 1, y);
			}

			return true;
		}

	virtual void OnDestroy()
	{
	}
};

#endif
