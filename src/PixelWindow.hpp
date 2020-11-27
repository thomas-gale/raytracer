#ifndef PIXELWINDOW_H
#define PIXELWINDOW_H

#include <vector>

#include <SDL.h>

#include "Color.hpp"
#include "Pixel.hpp"

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
        win = SDL_CreateWindow("Pixel Window", 1200, 100, width, height,
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

        // Create the texture
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
    }

    ~PixelWindow() {
        SDL_DestroyTexture(tex);
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
    }

    void draw() {
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
            for (int x = 0; x < width; x++) {
                // Compute color
                auto r = double(x) / double(width);
                auto g = double(height - y) / double(height);
                auto b = 0.25;
                setPixelUnlocked(pitch, pixels, x, y, Color<double>(r, g, b));
            }
        }
        SDL_UnlockTexture(tex);
    }

    void setSolidColor(const Color<T>& color) {
        int pitch;
        uint8_t* pixels;
        SDL_LockTexture(tex, NULL, (void**)&pixels, &pitch);
        for (int y = 0; y < height; y++) {
            std::cerr << "\rScanlines remaining: " << height - y - 1 << " "
                      << std::flush;
            for (int x = 0; x < width; x++) {
                setPixelUnlocked(pitch, pixels, x, y, color);
            }
        }
        SDL_UnlockTexture(tex);
    }

    // Using bottom left coordinate system.
    // NOTE - This is slooow - batch and call setPixels with a vector of pixels.
    void setPixel(int x, int y, const Color<T>& color) {
        int pitch;
        uint8_t* pixels;
        SDL_LockTexture(tex, NULL, (void**)&pixels, &pitch);
        Uint32* p = (Uint32*)(pixels + pitch * (height - y));
        p += x;
        *p = convertRGBA(color);
        SDL_UnlockTexture(tex);
    }

    // Using bottom left coordinate system.
    void setPixels(const std::vector<Pixel<T>>& pixels,
                   int samplesPerPixel = 1) {
        int pitch;
        uint8_t* pixelsPtr;
        SDL_LockTexture(tex, NULL, (void**)&pixelsPtr, &pitch);
        for (const Pixel<T>& pixel : pixels) {
            setPixelUnlocked(pitch, pixelsPtr, pixel.location().x(),
                             pixel.location().y(), pixel.color(),
                             samplesPerPixel);
        }
        SDL_UnlockTexture(tex);
    }

    // Spin till quit requested
    void awaitQuit() {
        SDL_Event event;
        bool quit = false;
        while (true) {
            SDL_WaitEvent(&event);
            switch (event.type) {
            case SDL_QUIT:
                std::cerr << "\nQuit Raytrace" << std::endl;
                return;
            }
        }
    }

    PixelWindow(const PixelWindow& other) = delete;
    PixelWindow(PixelWindow&& other) = delete;
    PixelWindow& operator=(const PixelWindow& other) = delete;
    PixelWindow& operator=(PixelWindow&& other) = delete;

  private:
    int width, height;
    SDL_Window* win;
    SDL_Renderer* ren;
    SDL_Texture* tex;

    inline void setPixelUnlocked(int pitch, uint8_t* pixels, int x, int y,
                                 const Color<T>& color,
                                 int samplesPerPixel = 1) {
        Uint32* p = (Uint32*)(pixels + pitch * (height - y - 1));
        p += x;
        *p = convertRGBA(color, samplesPerPixel);
    }
};

} // namespace raytrace

#endif // PIXELWINDOW_H
