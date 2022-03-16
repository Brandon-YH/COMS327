#ifndef MAP_H
#define MAP_H

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
    mart,

    player
} terrain_t;

typedef struct map{
    terrain_t mp[yN][xN];

    //coordinate of doors, eg: up = {x, y}, down = {x, y}, ...
    int up[2];
    int down[2];
    int left[2];
    int right[2];

}map_t;



map_t *map_init(int, int, int, int);
void print(terrain_t map[yN][xN]);
void generation(terrain_t map[yN][xN], map_t *m);


#endif
