#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720
#define WINDOW_NAME "SDL2 Template"

int main(int argc, char *argv[])
{
#ifdef NDEBUG
#else
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
    SDL_LogWarn(0, "This is a debug build and should not be used in production.");
#endif
    SDL_LogVerbose(0, "Initializing application '%s'...", WINDOW_NAME);
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    SDL_Event event;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    if (SDL_CreateWindowAndRenderer(DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }
    SDL_SetWindowTitle(window, WINDOW_NAME);

    // Background thingy
    if (TTF_Init())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize TTF: %s", TTF_GetError());
        return 3;
    }
    SDL_Texture *bg_texture = IMG_LoadTexture(renderer, "assets/img/bg.jpeg");
    // Watermark
    SDL_Color text_color = {255, 255, 255, 0};
    TTF_Font *font_shizuru = TTF_OpenFont("assets/fonts/Shizuru-Regular.ttf", 64);
    SDL_Surface *surface_text = TTF_RenderUTF8_Blended(font_shizuru, "SDL2 Template", text_color);
    SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, surface_text);
    TTF_SizeUTF8(font_shizuru, "SDL2 Template", &surface_text->w, &surface_text->h);
    SDL_Rect text_rect;            // create a rect
    text_rect.x = 36;              // controls the rect's x coordinate
    text_rect.w = surface_text->w; // controls the width of the rect
    text_rect.h = surface_text->h; // controls the height of the rect

    SDL_LogVerbose(0, "Begin main loop...");
    while (true)
    {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
        {
            break;
        }
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        // Show background
        SDL_RenderCopy(renderer, bg_texture, NULL, NULL);
        // Show font
        SDL_GetWindowSizeInPixels(window, NULL, &text_rect.y);
        text_rect.y -= 110;
        SDL_RenderCopy(renderer, text, NULL, &text_rect);
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    // Background
    SDL_DestroyTexture(bg_texture);
    // Font
    SDL_FreeSurface(surface_text);
    SDL_DestroyTexture(text);
    TTF_CloseFont(font_shizuru);
    // Everything else
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}