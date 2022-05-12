#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <time.h>
#include <assert.h>

#include "heap.h"

#define xN 80
#define yN 21

typedef enum terrain_type
{
    grass_1,
    grass_2,
    grass_3,
    grass_4,

    tree,
    snow_mount,
    water,

    blank,
    wall,
    door,
    path,

    center,
    mart
} terrain_t;

typedef enum trainer {
  player,
  hiker,
  rival,
  others,
  num_trainer
} trainer_t;

typedef enum dim {
  dim_x,
  dim_y,
  num_dims
} dim_t;

typedef int16_t pair_t[num_dims];

typedef struct path {
  heap_node_t *hn;
  uint8_t pos[2];
  uint8_t from[2];
  int32_t cost;
} path_t;

typedef struct trainerInfo {
  int pos[2];
  int pathCost[yN][xN];
  trainer_t trainer;
} info_t;

typedef struct map{
    terrain_t mp[yN][xN];
    uint8_t height[yN][xN];
    info_t trainerList[num_trainer];
    //coordinate of doors, eg: up = {x, y}, down = {x, y}, ...
    int up[2], down[2], left[2], right[2];
    // int x, y;
}map_t;

map_t *map_init(int, int, int, int, int);
void dijkstra_map(map_t *m, trainer_t trainer);
void print(map_t *m, int, int);
void generation(terrain_t map[yN][xN], map_t *m, int dis);


#endif
