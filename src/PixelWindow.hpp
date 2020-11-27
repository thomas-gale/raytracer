#ifndef PIXELWINDOW_H
#define PIXELWINDOW_H

#include <SDL.h>

namespace raytrace {

template <class T> class PixelWindow {
  public:
    PixelWindow(int width, int height) : width(width), height(height) {
        // Start SDL
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return;
        }

        // Open a window
        win = SDL_CreateWindow("Hello World!", 1200, 100, width, height,
                               SDL_WINDOW_SHOWN);
        if (win == nullptr) {
            std::cout << "SDL_CreateWindow Error: " << SDL_GetError()
                      << std::endl;
            SDL_Quit();
            return;
        }

        // Creating a renderer
        ren = SDL_CreateRenderer(
            win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (ren == nullptr) {
            SDL_DestroyWindow(win);
            std::cout << "SDL_CreateRenderer Error: " << SDL_GetError()
                      << std::endl;
            SDL_Quit();
            return;
        }

        // Create my own surface.
        surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

        setPrettyColor();

        // Create texture from surface
        tex = SDL_CreateTextureFromSurface(ren, surface);

        // NOTE - surface freed.
        // SDL_FreeSurface(surfacae);

        if (tex == nullptr) {
            SDL_DestroyRenderer(ren);
            SDL_DestroyWindow(win);
            std::cout << "SDL_CreateTextureFromSurface Error: "
                      << SDL_GetError() << std::endl;
            SDL_Quit();
            return;
        }
    }

    ~PixelWindow() {
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(tex);
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
    }

    void draw() {
        // First clear the renderer
        SDL_RenderClear(ren);
        // Draw the texture
        SDL_RenderCopy(ren, tex, NULL, NULL);
        // Update the screen
        SDL_RenderPresent(ren);
    }

    void setPrettyColor() {
        // Fiddle with the pixels
        SDL_LockSurface(surface);

        // Set fixed colour (white)
        for (int y = 0; y < height; ++y) {
            std::cerr << "\rScanlines remaining: " << height - y - 1 << " "
                      << std::flush;
            for (int x = 0; x < width; ++x) {
                // Compute color
                auto r = double(x) / double(width);
                auto g = double(height - y) / double(height);
                auto b = 0.25;

                // Grab the pixel pointer
                Uint8* pixel = (Uint8*)surface->pixels;
                pixel += (y * surface->pitch) + (x * sizeof(Uint32));

                // Compose and set the color
                Uint32 c = convertColor(Color<double>(r, g, b));
                *((Uint32*)pixel) = c;
            }
        }

        SDL_UnlockSurface(surface);
    }

    void setSolidColor(const Color<T>& color) {
        // Fiddle with the pixels
        SDL_LockSurface(surface);

        // Set fixed colour (white)
        for (int y = 0; y < height; ++y) {
            std::cerr << "\rScanlines remaining: " << height - y - 1 << " "
                      << std::flush;
            for (int x = 0; x < width; ++x) {
                // Grab the pixel pointer
                Uint8* pixel = (Uint8*)surface->pixels;
                pixel += (y * surface->pitch) + (x * sizeof(Uint32));

                Uint32 c = convertColor(color);
                *((Uint32*)pixel) = c;
            }
        }

        SDL_UnlockSurface(surface);
    }

    void setPixel(int x, int y, const Color<T>& color) {
        // Fiddle with the pixels
        SDL_LockSurface(surface);

        // Grab the pixel pointer
        Uint8* pixel = (Uint8*)surface->pixels;
        pixel += (y * surface->pitch) + (x * sizeof(Uint32));

        // Compose and set the color
        Uint32 c = convertColor(color);
        *((Uint32*)pixel) = c;

        SDL_UnlockSurface(surface);
    }

    PixelWindow(const PixelWindow& other) = delete;
    PixelWindow(PixelWindow&& other) = delete;
    PixelWindow& operator=(const PixelWindow& other) = delete;
    PixelWindow& operator=(PixelWindow&& other) = delete;

  private:
    int width, height;
    SDL_Window* win;
    SDL_Renderer* ren;
    SDL_Surface* surface;
    SDL_Texture* tex;
};

} // namespace raytrace

#endif // PIXELWINDOW_H
