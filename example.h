#include <SDL2/SDL.h>
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
			static int c = 0;
			static int z = 0;
			c += static_cast<int>(30.0f * elapsed_time);
			z += static_cast<int>(75.0f * elapsed_time);
			SDL_Renderer *renderer = get_renderer();

			for (int y=0; y<height(); y++)
			{
				for (int x=0; x<width(); x++)
				{
					int r = (x * x) + (y * y) + z;
					float f = (static_cast<float>((r) % 256)) / 256.0f;

					SDL_SetRenderDrawColor(renderer, 255 - (f * 255), c, f * 255, 255);
					SDL_RenderDrawPoint(renderer, x, y);
				}
			}

			SDL_RenderPresent(renderer);
			
			return true;
		}

	virtual void OnDestroy()
	{
	}
};
