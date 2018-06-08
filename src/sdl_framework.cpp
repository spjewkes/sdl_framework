#include "sdl_framework.hpp"

SDLFramework::SDLFramework(int width, int height) : w(width), h(height), m_mousePosX(0), m_mousePosY(0)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw SDLException("Failed to init SDL");
	}

	window = SDL_CreateWindow( GetName(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN );
	if (!window)
	{
		throw SDLException("Failed to create SDL window");
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		throw SDLException("Failed to create SDL renderer");
	}
}

SDLFramework::~SDLFramework()
{
	if (renderer)
	{
		SDL_DestroyRenderer(renderer);
	}
	if (window)
	{
		SDL_DestroyWindow(window);
	}

	SDL_Quit();
}

void SDLFramework::Start()
{
	if (!OnCreate())
	{
		throw SDLException("Failed to create game framework");
	}

	bool quit = false;
	SDL_Event e;

	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();

	while (!quit)
	{
		for (int i=0; i<2; i++)
		{
			m_mouse[i].bPressed = false;
			m_mouse[i].bReleased = false;
			m_mouse[i].bHeld = false;
		}
			
		while( SDL_PollEvent( &e ) != 0 )
		{
			switch(e.type)
			{
			case SDL_QUIT:
			{
				quit = true;
				break;
			}
			case SDL_KEYUP:
			{
				if ( e.key.keysym.sym == SDLK_ESCAPE )
				{
					quit = true;
				}
				break;
			}
			case SDL_MOUSEMOTION:
			{
				m_mousePosX = e.motion.x;
				m_mousePosY = e.motion.y;
				if (e.motion.state & SDL_BUTTON_LMASK)
				{
					m_mouse[0].bPressed = false;
					m_mouse[0].bReleased = false;
					m_mouse[0].bHeld = true;
				}
				if (e.motion.state & SDL_BUTTON_RMASK)
				{
					m_mouse[1].bPressed = false;
					m_mouse[1].bReleased = false;
					m_mouse[1].bHeld = true;
				}
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				switch (e.button.button)
				{
				case SDL_BUTTON_LEFT:
					m_mouse[0].bPressed = true;
					m_mouse[0].bReleased = false;
					m_mouse[0].bHeld = true;
					break;
				case SDL_BUTTON_RIGHT:
					m_mouse[1].bPressed = true;
					m_mouse[1].bReleased = false;
					m_mouse[1].bHeld = true;
					break;
				}
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				switch (e.button.button)
				{
				case SDL_BUTTON_LEFT:
					m_mouse[0].bPressed = false;
					m_mouse[0].bReleased = true;
					m_mouse[0].bHeld = false;
					break;
				case SDL_BUTTON_RIGHT:
					m_mouse[1].bPressed = false;
					m_mouse[1].bReleased = true;
					m_mouse[1].bHeld = false;
					break;
				}
				break;
			}
			}
		}
				
		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsed_time = tp2 - tp1;
		tp1 = tp2;

		char title[256];
		snprintf(title, 256, "%8.2f fps - %s", 1.0 / elapsed_time.count(), GetName());
		SDL_SetWindowTitle(window, title);
				
		if (!OnUpdate(elapsed_time.count()))
		{
			throw SDLException("Failed on game update");
		}

		SDL_RenderPresent(GetRenderer());
	}

	OnDestroy();
}

void SDLFramework::DrawLine(int x0, int y0, int x1, int y1)
{
	SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
}

void SDLFramework::FillCircle(int xc, int yc, int r)
{
	// Taken from wikipedia
	int x = 0;
	int y = r;
	int p = 3 - 2 * r;
	if (!r) return;

	auto drawline = [&](int sx, int ex, int ny)
		{
			for (int i = sx; i < ex; i++)
				SDL_RenderDrawPoint(GetRenderer(), i, ny);
		};

	while (y >= x)
	{
		// Modified to draw scan-lines instead of edges
		drawline(xc - x, xc + x, yc - y);
		drawline(xc - y, xc + y, yc - x);
		drawline(xc - x, xc + x, yc + y);
		drawline(xc - y, xc + y, yc + x);
		if (p < 0) p += 4 * x++ + 6;
		else p += 4 * (x++ - y--) + 10;
	}
}

void SDLFramework::SetDrawColor(int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}
	
void SDLFramework::ClearScreen(int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderClear(renderer);
}
