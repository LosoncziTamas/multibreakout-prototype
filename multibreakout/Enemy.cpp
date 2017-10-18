#include "Enemy.hpp"
#include <SDL2/SDL_assert.h>

static void initCommon(Enemy& enemy) {
    enemy.paddle.speed = DEFAULT_SPEED;
    enemy.state = none;
    enemy.paddle.textureIndex = INVALID_INDEX;
}

void initLeftEnemy(Enemy& enemy, float leftBoundary) {
    initCommon(enemy);
    enemy.paddle.orientation = left;
    enemy.paddle.height = DEFAULT_WIDTH;
    enemy.paddle.width = DEFAULT_HEIGHT;
    enemy.paddle.newPos = Vec2(leftBoundary + enemy.paddle.width * 0.5f, SCREEN_HEIGHT * 0.5f);
    enemy.paddle.oldPos = enemy.paddle.newPos;
    enemy.steeringPos = enemy.paddle.newPos;
}

void initRightEnemy(Enemy& enemy, float rightBoundary) {
    initCommon(enemy);
    enemy.paddle.orientation = right;
    enemy.paddle.height = DEFAULT_WIDTH;
    enemy.paddle.width = DEFAULT_HEIGHT;
    enemy.paddle.newPos = Vec2(rightBoundary - enemy.paddle.width * 0.5f, SCREEN_HEIGHT * 0.5f);
    enemy.paddle.oldPos = enemy.paddle.newPos;
    enemy.steeringPos = enemy.paddle.newPos;
}

void initUpperEnemy(Enemy& enemy) {
    initCommon(enemy);
    enemy.paddle.orientation = upper;
    enemy.paddle.width = DEFAULT_WIDTH;
    enemy.paddle.height = DEFAULT_HEIGHT;
    enemy.paddle.newPos = Vec2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - enemy.paddle.height * 0.5f);
    enemy.paddle.oldPos = enemy.paddle.newPos;
    enemy.steeringPos = enemy.paddle.newPos;
}

bool danger(std::vector<Ball>& balls) {
    for (auto& ball : balls) {
        if (ball.newPos.y > SCREEN_HEIGHT * 0.5f && ball.assignedPaddle == nullptr) {
            return true;
        }
    }
    return false;
}

Vec2 getTargetPosition(Paddle& enemy, std::vector<Ball>& balls, float leftBoundary, float rightBoundary) {
    Vec2 targetPos(enemy.newPos);
    Ball *target = nullptr;
    
    for (auto& ball : balls) {
        bool betterTarget = target == nullptr || (target->newPos.y < ball.newPos.y && ball.velocity.y > 0);
        if (ball.newPos.y > SCREEN_HEIGHT * 0.5f && betterTarget) {
            targetPos = ball.newPos;
            target = &ball;
            if (target->newPos.x - target->radius < leftBoundary) {
                targetPos.x += enemy.width;
            } else if (target->newPos.x + target->radius > rightBoundary) {
                targetPos.x -= enemy.width;
            }
            SDL_assert(ball.assignedPaddle != &enemy);
        }
    }
    return targetPos;
}

void moveToward(Paddle& enemy, const Vec2& target, float delta, float leftBoundary, float rightBoundary) {
    Vec2 acceleration;
    Vec2 diff = target - enemy.newPos;
    if (diff.x > 0) {
        if (enemy.velocity.x < 0){
            enemy.velocity.x = 0.0f;
        }
        acceleration.x = 1.0;
    } else if (diff.x < 0) {
        if (enemy.velocity.x > 0){
            enemy.velocity.x = 0.0f;
        }
        acceleration.x = -1.0;
    } else {
        enemy.velocity += -0.02 * enemy.velocity;
    }
    
    acceleration *= enemy.speed;
    
    enemy.oldPos = enemy.newPos;
    enemy.movementDelta = (0.5f * acceleration * pow(delta, 2) + enemy.velocity * delta);
    enemy.velocity += acceleration * delta;
    enemy.newPos = enemy.oldPos + enemy.movementDelta;
    
    float offset = enemy.width * 0.5f;
    if (enemy.newPos.x - offset < leftBoundary) {
        Vec2 wallNorm(1, 0);
        enemy.velocity = enemy.velocity - 2 * enemy.velocity.dotProduct(wallNorm) * wallNorm;
        enemy.movementDelta.x += 1.0f;
    } else if (enemy.newPos.x + offset > rightBoundary) {
        Vec2 wallNorm(-1, 0);
        enemy.velocity = enemy.velocity - 2 * enemy.velocity.dotProduct(wallNorm) * wallNorm;
        enemy.movementDelta.x -= 1.0f;
    }
    
}

void updateEnemy(Enemy& enemy, std::vector<Ball>& balls, float delta, float leftBoundary, float rightBoundary) {
    bool isDanger = danger(balls);
    switch (enemy.state) {
        case none: {
            if (enemy.paddle.ballIndex != INVALID_INDEX && !isDanger) {
                enemy.steeringPos = enemy.paddle.newPos;
                int randomX = rand() % static_cast<int>(rightBoundary - leftBoundary) + 1;
                enemy.steeringPos.x = randomX;
                enemy.state = steering;
            } else if (isDanger) {
                enemy.state = defending;
            }
        } break;
        case steering: {
            bool reached = enemy.steeringPos.distance(enemy.paddle.newPos) < enemy.paddle.width;
            if (!isDanger && !reached) {
                moveToward(enemy.paddle, enemy.steeringPos, delta, leftBoundary, rightBoundary);
            } else {
                if (reached) {
                    Ball& assignedBall = balls.at(enemy.paddle.ballIndex);
                    assignedBall.velocity = Vec2(0.0f, -1.0f);
                    assignedBall.assignedPaddle = nullptr;
                    enemy.paddle.ballIndex = INVALID_INDEX;
                }
                enemy.state = isDanger ? defending : none;
            }
        }break;
        case defending: {
            if (!isDanger) {
                enemy.state = none;
            } else {
                Vec2 target = getTargetPosition(enemy.paddle, balls, leftBoundary, rightBoundary);
                moveToward(enemy.paddle, target, delta, leftBoundary, rightBoundary);
            }
        } break;
        default:
            break;
    }
}