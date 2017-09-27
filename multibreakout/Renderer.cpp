#include "Renderer.hpp"
#include "Paddle.hpp"
#include "MultiBreakout.hpp"
#include <cmath>

Renderer::Renderer(const Window &window) {
    sdlRenderer = SDL_CreateRenderer(window.sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_assert(sdlRenderer);
}

void Renderer::clear() const {
    SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 0);
    SDL_RenderClear(sdlRenderer);
}

void Renderer::update() const {
    SDL_RenderPresent(sdlRenderer);
}

void Renderer::drawPaddle(const Paddle& paddle) const {
    
    Vec2 flippedCenter = paddle.centerPos;
    flippedCenter.y = SCREEN_HEIGHT - flippedCenter.y;
    
    int w = round(paddle.width);
    int h = round(paddle.height);

    int x = round(paddle.centerPos.x - paddle.width * 0.5f);
    int y = round(flippedCenter.y - paddle.height);
    
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
    SDL_RenderFillRect(sdlRenderer, &rect);
}

void Renderer::drawRectangle(const SDL_Rect& rectangle) const {
    SDL_SetRenderDrawColor(sdlRenderer, 128, 0, 0, 255);
    SDL_Rect flippedRect = rectangle;
    flippedRect.y = SCREEN_HEIGHT - rectangle.y - rectangle.h;
    SDL_RenderFillRect(sdlRenderer, &flippedRect);
}

void Renderer::drawBall(const Ball& ball) const {
    
    Vec2 flippedCenter = ball.center;
    flippedCenter.y = SCREEN_HEIGHT - ball.center.y;
    
    int x0 = round(flippedCenter.x);
    int y0 = round(flippedCenter.y);
    int radius = round(ball.radius);
    
    int x = radius-1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);
    
    while (x >= y)
    {
        SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
        SDL_RenderDrawPoint(sdlRenderer, x0 + x, y0 + y);
        SDL_RenderDrawPoint(sdlRenderer, x0 + y, y0 + x);
        SDL_RenderDrawPoint(sdlRenderer, x0 - y, y0 + x);
        SDL_RenderDrawPoint(sdlRenderer, x0 - x, y0 + y);
        SDL_RenderDrawPoint(sdlRenderer, x0 - x, y0 - y);
        SDL_RenderDrawPoint(sdlRenderer, x0 - y, y0 - x);
        SDL_RenderDrawPoint(sdlRenderer, x0 + y, y0 - x);
        SDL_RenderDrawPoint(sdlRenderer, x0 + x, y0 - y);
        
        if (err <= 0)
        {
            y++;
            err += dy;
            dy +=2;
        }
        if (err > 0)
        {
            x--;
            dx += 2;
            err += (-radius << 1) + dx;
        }
    }
}

void Renderer::drawPoint(const Vec2& vec, SDL_Color color) const {
    SDL_SetRenderDrawColor(sdlRenderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawPoint(sdlRenderer, round(vec.x), round(SCREEN_HEIGHT - vec.y));
}

void Renderer::drawPoint(float x, float y, SDL_Color color) const {
    SDL_SetRenderDrawColor(sdlRenderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawPoint(sdlRenderer, round(x), round(SCREEN_HEIGHT - y));
}


Renderer::~Renderer() {
    SDL_DestroyRenderer(sdlRenderer);
}