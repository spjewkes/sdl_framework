#include <SDL2/SDL.h>
#include <string>
#include <cmath>
#include <iostream>
#include "sdl_framework.h"

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
			SDL_Renderer *renderer = get_renderer();

			SDL_RenderPresent(renderer);
			
			return true;
		}

	virtual void OnDestroy()
	{
	}
};
