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

void SDLFramework::FillBox(int x0, int y0, int x1, int y1)
{
	SDL_Rect rect = { std::min(x0, x1), std::min(y0, y1), abs(x0 - x1), abs(y0 - y1) };
	int retcode = SDL_RenderFillRect(renderer, &rect);
	assert(retcode == 0);
}

void SDLFramework::FillPolygon(int num_points, int *points)
{
	assert(num_points > 2);
	int length = num_points * 2;
	for (int i = 2; i < length; i += 2)
	{
		FillTriangle(points[0], points[1],
					 points[i-2], points[i-1],
					 points[i], points[i+1]);
	}
}

void SDLFramework::FillTrapezoid(int y_top, int y_bottom, int x0_top, int x1_top, int x0_bottom, int x1_bottom)
{
	float x0 = static_cast<float>(x0_top);
	float dx0 = static_cast<float>(x0_bottom-x0_top)/static_cast<float>(y_bottom-y_top);
	float x1 = static_cast<float>(x1_top);
	float dx1 = static_cast<float>(x1_bottom-x1_top)/static_cast<float>(y_bottom-y_top);

	for (int y=y_top; y<=y_bottom; y++)
	{
		DrawLine(static_cast<int>(x0), y, static_cast<int>(x1), y);
		x0 += dx0;
		x1 += dx1;
	}
}

void SDLFramework::FillTriangle(int x0, int y0, int x1, int y1, int x2, int y2)
{
	int y[3];
	int x[3];

	if (y0 <= y1 && y0 <= y2)
	{
		x[0] = x0;
		y[0] = y0;
		
		if (y1 <= y2)
		{
			x[1] = x1; y[1] = y1; x[2] = x2; y[2] = y2;
		}
		else
		{
			x[1] = x2; y[1] = y2; x[2] = x1; y[2] = y1;
		}
	}
	else if (y1 <= y0 && y1 <= y2)
	{
		x[0] = x1;
		y[0] = y1;
		
		if (y0 <= y2)
		{
			x[1] = x0; y[1] = y0; x[2] = x2; y[2] = y2;
		}
		else
		{
			x[1] = x2; y[1] = y2; x[2] = x0; y[2] = y0;
		}
	}
	else
	{
		x[0] = x2;
		y[0] = y2;
		
		if (y0 <= y1)
		{
			x[1] = x0; y[1] = y0; x[2] = x1; y[2] = y1;
		}
		else
		{
			x[1] = x1; y[1] = y1; x[2] = x0; y[2] = y0;
		}
	}

	if (y[0] == y[1])
	{
		FillTrapezoid(y[0], y[2], x[0], x[1], x[2], x[2]);
	}
	else if (y[1] == y[2])
	{
		FillTrapezoid(y[0], y[1], x[0], x[0], x[1], x[2]);
	}
	else
	{
		float t = static_cast<float>(y[1] - y[0]) / static_cast<float>(y[2] - y[0]);
		int new_x = x[0] + static_cast<int>(static_cast<float>(x[2] - x[0]) * t);

		FillTrapezoid(y[0], y[1], x[0], x[0], new_x, x[1]);
		FillTrapezoid(y[1], y[2], new_x, x[1], x[2], x[2]);

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
