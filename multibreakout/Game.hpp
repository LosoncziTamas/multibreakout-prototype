#ifndef Game_hpp
#define Game_hpp

#include <vector>

#include "Ball.hpp"
#include "Paddle.hpp"
#include "Obstacle.hpp"
#include "Enemy.hpp"
#include "Brick.hpp"
#include "TextureButton.hpp"
#include "NinePatch.hpp"

enum GameType {
    oneVsOne, twoVsTwo
};

const Uint32 MAX_BALLS = 4;
const Uint32 MAX_BRICKS = 256;

struct World {
    GameType type;
    bool initialized;
    
    int leftBoundary;
    int rightBoundary;
    
    Uint32 brickCount;
    Brick bricks[MAX_BRICKS];
    
    Uint32 ballCount;
    Ball balls[MAX_BALLS];
    
    Paddle paddle;
    Enemy enemyUpper;
    
    Obstacles obstacles;
    Enemy enemyLeft;
    Enemy enemyRight;
};

struct GameUi {
    bool initialized;
    NinePatch leftPanel;
    NinePatch rightPanel;
    TextureButton leftButton;
    TextureButton rightButton;
};

void gamePlayUpdate(GameState& gameState);
void initalizeGameWorld(World& world);
void initializeUi(GameUi& gameUi);

#endif