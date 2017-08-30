#ifndef MultiBreakout_hpp
#define MultiBreakout_hpp

#include <SDL2/SDL.h>

#include "Vec2.hpp"
#include "Renderer.hpp"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

struct GameInput
{
    union
    {
        SDL_bool buttons[5];
        struct
        {
            SDL_bool pause;
            SDL_bool up;
            SDL_bool down;
            SDL_bool left;
            SDL_bool right;
        };
    };
    SDL_bool mouseLeft;
    SDL_bool mouseRight;
    int32_t mouseX;
    int32_t mouseY;
};

void gameUpdate(const GameInput& input, const Renderer& renderer, float delta);


#endif