#ifndef PIXELWINDOW_H
#define PIXELWINDOW_H

#include <SDL.h>

#include "Color.hpp"

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
        // surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

        // setPrettyColor();

        // Create texture from surface
        // tex = SDL_CreateTextureFromSurface(ren, surface);

        // NOTE - surface freed.
        // SDL_FreeSurface(surfacae);

        tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_STREAMING, width, height);
        if (tex == nullptr) {
            SDL_DestroyRenderer(ren);
            SDL_DestroyWindow(win);
            std::cout << "SDL_CreateTexture Error: " << SDL_GetError()
                      << std::endl;
            SDL_Quit();
            return;
        }

        pixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);

        // Grissly :D
        pixels = (uint32_t*)malloc(sizeof(uint32_t) * width * height);

        SDL_SetRenderDrawColor(ren, 225, 240, 200, 255);
    }

    ~PixelWindow() {
        // SDL_FreeSurface(surface);

        free(pixelFormat);
        free(pixels);

        SDL_DestroyTexture(tex);
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
    }

    void draw() {

        // Uint32 format;
        // int w;
        // int h;
        // SDL_QueryTexture(tex, &format, NULL, &w, &h);

        /*int pitch;
        uint8_t* pixels;
        SDL_LockTexture(tex, NULL, (void**)&pixels, &pitch);
        for (int y = 0; y < height; y++) {
            Uint32* p =
                (Uint32*)(pixels + pitch * y); // cast for a pointer increments
                                               // by 4 bytes.(RGBA)
            for (int x = 0; x < width; x++) {
                // *p = 0x00FF0000;
                if (x > 20)
                    *p = SDL_MapRGBA(pixelFormat, 255, 0, 0, 255);
                else if (x > 10)
                    *p = SDL_MapRGBA(pixelFormat, 0, 255, 0, 255);
                else
                    *p = SDL_MapRGBA(pixelFormat, 0, 0, 255, 255);

                p++;
            }
        }
        SDL_UnlockTexture(tex);*/

        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, tex, NULL, NULL);
        SDL_RenderPresent(ren);
    }

    void setPrettyColor() {
        int pitch;
        uint8_t* pixels;
        SDL_LockTexture(tex, NULL, (void**)&pixels, &pitch);
        for (int y = 0; y < height; y++) {
            std::cerr << "\rScanlines remaining: " << height - y - 1 << " "
                      << std::flush;
            Uint32* p =
                (Uint32*)(pixels + pitch * y); // cast for a pointer increments
                                               // by 4 bytes.(RGBA)
            for (int x = 0; x < width; x++) {
                // Compute color
                auto r = double(x) / double(width);
                auto g = double(height - y) / double(height);
                auto b = 0.25;

                *p = convertRGBA(Color<double>(r, g, b));
                ++p;
            }
        }
        SDL_UnlockTexture(tex);
    }

    /*void setPrettyColor() {
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
    }*/

    /*void setSolidColor(const Color<T>& color) {
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
    }*/

    /*void setPixel(int x, int y, const Color<T>& color) {
        // Fiddle with the pixels
        SDL_LockSurface(surface);

        // Grab the pixel pointer
        Uint8* pixel = (Uint8*)surface->pixels;
        pixel += (y * surface->pitch) + (x * sizeof(Uint32));

        // Compose and set the color
        Uint32 c = convertColor(color);
        *((Uint32*)pixel) = c;

        SDL_UnlockSurface(surface);
    }*/

    PixelWindow(const PixelWindow& other) = delete;
    PixelWindow(PixelWindow&& other) = delete;
    PixelWindow& operator=(const PixelWindow& other) = delete;
    PixelWindow& operator=(PixelWindow&& other) = delete;

  private:
    int width, height;
    SDL_Window* win;
    SDL_Renderer* ren;
    SDL_Texture* tex;
    uint32_t* pixels;
    SDL_PixelFormat* pixelFormat;

    // To remove
    // SDL_Surface* surface;
};

} // namespace raytrace

#endif // PIXELWINDOW_H
