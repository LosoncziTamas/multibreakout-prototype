#ifndef MULTIBREAKOUT_H
#define MULTIBREAKOUT_H

#include <SDL2/SDL.h>

#include "sdl_multibreakout.h"

extern "C" void gameUpdate(GameMemory *gameMemory, GameInput *GameInput, SDL_Renderer *renderer);

struct Vec2
{
    float x;
    float y;
};

enum EntityType
{
    ENTITY_TYPE_NULL,
    ENTITY_TYPE_BALL,
    ENTITY_TYPE_PADDLE,
    ENTITY_TYPE_BRICK,
    ENTITY_TYPE_OBSTACLE,
    ENTITY_TYPE_PROJECTILE
};

enum EntityFlags
{
    ENTITY_FLAG_COLLIDES = (1 << 0),
    ENTITY_FLAG_STATIC = (1 << 1)
};

struct Entity
{
    Uint32 storageIndex;
    Vec2 p;
    Vec2 dp;
    float w;
    float h;
    EntityType type;
    Uint32 flags;
};

struct GameState
{
    Entity entities[256];
    Uint32 entityCount;
};


struct MemoryPartition
{
    Uint8 *base;
    size_t size;
    size_t used;
};

/*
void initializeMemoryPartition(MemoryPartition *memory, size_t size, void *base)
{
    memory->size = size;
    memory->base = (Uint8 *)base;
    memory->used = 0;
}

void* pushSize(MemoryPartition *memory, size_t size) 
{
    SDL_assert((memory->used + size) <= memory->size);

    void *result = memory->base + memory->used;
    memory->used += size;
    
    return result;
}

void clearSize(MemoryPartition *memory, size_t size, void *ptr)
{
    memory->used -= size;

    Uint8 *byte = (Uint8 *)ptr;
    while(size--)
    {
        *byte++ = 0;
    }
}

#define pushStruct(memory, type) (type *)pushSize(memory, sizeof(type))
#define pushArray(memory, count, type) (type *)pushSize(memory, (count)*sizeof(type))

#define clearStruct(instance) clearSize(sizeof(instance), &(instance))
#define clearArray(memory, instance, count, type) clearSize(memory, (count) * sizeof(type), (instance))
*/
#endif