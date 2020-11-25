#include <iostream>

#include <SDL.h>

int main() {
  std::cout << "Hello Raytrace" << std::endl;

  // Settings.
  int width = 640;
  int height = 480;

  // Start SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }

  // Open a window
  SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, width, height,
                                     SDL_WINDOW_SHOWN);
  if (win == nullptr) {
    std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  // Creating a renderer
  SDL_Renderer *ren = SDL_CreateRenderer(
      win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == nullptr) {
    SDL_DestroyWindow(win);
    std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  // Create my own surface.
  SDL_Surface *surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

  // Fiddle with the pixels
  SDL_LockSurface(surface);

  // Set fixed colour (white)
  Uint32 color = 0x00FF00;
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      Uint8 *pixel = (Uint8 *)surface->pixels;
      pixel += (y * surface->pitch) + (x * sizeof(Uint32));
      *((Uint32 *)pixel) = color;
    }
  }

  SDL_UnlockSurface(surface);

  // Create texture from surface
  SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, surface);

  // NOTE - surface freed.
  SDL_FreeSurface(surface);
  if (tex == nullptr) {
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError()
              << std::endl;
    SDL_Quit();
    return 1;
  }

  // Drawing the texture.
  // A sleepy rendering loop, wait for 3 seconds and render and present the
  // screen each time
  for (int i = 0; i < 3; ++i) {
    // First clear the renderer
    SDL_RenderClear(ren);
    // Draw the texture
    SDL_RenderCopy(ren, tex, NULL, NULL);
    // Update the screen
    SDL_RenderPresent(ren);
    // Take a quick break after all that hard work
    SDL_Delay(1000);
  }

  // Clean up
  SDL_DestroyTexture(tex);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();

  return 0;
}
