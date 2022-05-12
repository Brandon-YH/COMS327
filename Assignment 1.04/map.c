// #include "map.h"
#include "heap.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/time.h>
#include <assert.h>
#include <unistd.h>

#define heightpair(pair) (m->height[pair[dim_y]][pair[dim_x]])
#define heightxy(x, y) (m->height[y][x])

#define map_size 399
#define center_x 199
#define center_y 199

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

typedef enum trainer
{
    player,
    rival,
    hiker,
    pacer,
    wanderer,
    stationarie,
    random_walker,
    num_trainer_type
} trainer_t;

typedef enum dim
{
    dim_x,
    dim_y,
    num_dims
} dim_t;

typedef enum dir
{
    n,
    s,
    w,
    e,
    nw,
    ne,
    sw,
    se,
    num_dir,
    no_dir = -1,
} dir_t;

typedef int16_t pair_t[num_dims];

typedef struct path
{
    heap_node_t *hn;
    uint8_t pos[2];
    uint8_t from[2];
    int32_t cost;
} path_t;

typedef struct trainerInfo
{
    heap_node_t *hn;
    int pos[num_dims];
    int currCost;
    int placement;
    dir_t prev;
    // int pathCost[yN][xN]; // weighted map
    trainer_t trainer;
} info_t;

typedef struct map
{
    terrain_t mp[yN][xN];
    info_t *npc_mp[yN][xN];
    info_t **trainerList;

    // coordinate of doors, eg: up = {x, y}, down = {x, y}, ...
    int up[2], down[2], left[2], right[2];
    // int x, y;
} map_t;

typedef struct pc
{
    pair_t pos;
    int currCost;
} pc_t;

typedef struct world
{
    map_t *world[map_size][map_size];
    // info_t *trainer_world[map_size][map_size];
    int hiker_dist[yN][xN];
    int rival_dist[yN][xN];
    info_t pc;
    // pc_t pc;
} world_t;

world_t Bworld;
char check;
int numTrainer = 8;

// min & max helper
int max(int num1, int num2)
{
    return (num1 > num2) ? num1 : num2;
}
int min(int num1, int num2)
{
    return (num1 > num2) ? num2 : num1;
}

static int32_t info_cmp(const void *key, const void *with)
{
    return ((info_t *)key)->currCost - ((info_t *)with)->currCost;
}

static int32_t path_cmp(const void *key, const void *with)
{
    return ((path_t *)key)->cost - ((path_t *)with)->cost;
}

int findTileCost(terrain_t tile, trainer_t trainer)
{
    int cost;
    switch (tile)
    {
    case tree:
    case snow_mount:
        if (trainer != hiker)
        {
            cost = INT_MAX;
            break;
        }
    case grass_1:
    case grass_2:
    case grass_3:
    case grass_4:
        if (trainer == hiker)
            cost = 15;
        else
            cost = 20;
        break;

    case center:
    case mart:
        if (trainer != player)
        {
            cost = INT_MAX;
            break;
        }
    case path:
        cost = 10;
        break;

    default:
        cost = INT_MAX;
        break;
    }
    return cost;
}

static void dijkstra_tile(map_t *m, pair_t from, pair_t to, path_t path[yN][xN], path_t *p, trainer_t trainer)
{
    // static path_t path[yN][xN], *p;
    static uint32_t initialized = 0;
    heap_t h;
    uint32_t x, y;

    if (!initialized)
    {
        for (y = 0; y < yN; y++)
        {
            for (x = 0; x < xN; x++)
            {
                path[y][x].pos[dim_y] = y;
                path[y][x].pos[dim_x] = x;
            }
        }
        initialized = 1;
    }

    for (y = 0; y < yN; y++)
    {
        for (x = 0; x < xN; x++)
        {
            path[y][x].cost = INT_MAX;
        }
    }

    path[from[dim_y]][from[dim_x]].cost = 0;

    heap_init(&h, path_cmp, NULL);

    for (y = 1; y < yN - 1; y++)
    {
        for (x = 1; x < xN - 1; x++)
        {
            if (findTileCost(m->mp[y][x], trainer) != INT_MAX)
                path[y][x].hn = heap_insert(&h, &path[y][x]);
        }
    }

    while ((p = heap_remove_min(&h)))
    {
        p->hn = NULL;

        if ((path[p->pos[dim_y] - 1][p->pos[dim_x]].hn) &&
            (path[p->pos[dim_y] - 1][p->pos[dim_x]].cost >
             (p->cost + findTileCost(m->mp[p->pos[dim_y]][p->pos[dim_x]], trainer))) &&
            (p->cost + findTileCost(m->mp[p->pos[dim_y]][p->pos[dim_x]], trainer)) >= 0)
        {
            path[p->pos[dim_y] - 1][p->pos[dim_x]].cost =
                (p->cost + findTileCost(m->mp[p->pos[dim_y]][p->pos[dim_x]], trainer));
            path[p->pos[dim_y] - 1][p->pos[dim_x]].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y] - 1][p->pos[dim_x]].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                                 [p->pos[dim_x]]
                                                     .hn);
        }
        if ((path[p->pos[dim_y]][p->pos[dim_x] - 1].hn) &&
            (path[p->pos[dim_y]][p->pos[dim_x] - 1].cost >
             (p->cost + findTileCost(m->mp[p->pos[dim_y]][p->pos[dim_x]], trainer))) &&
            (p->cost + findTileCost(m->mp[p->pos[dim_y]][p->pos[dim_x]], trainer)) >= 0)
        {
            path[p->pos[dim_y]][p->pos[dim_x] - 1].cost =
                (p->cost + findTileCost(m->mp[p->pos[dim_y]][p->pos[dim_x]], trainer));
            path[p->pos[dim_y]][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y]][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y]]
                                                 [p->pos[dim_x] - 1]
                                                     .hn);
        }
        if ((path[p->pos[dim_y]][p->pos[dim_x] + 1].hn) &&
            (path[p->pos[dim_y]][p->pos[dim_x] + 1].cost >
             (p->cost + findTileCost(m->mp[p->pos[dim_y]][p->pos[dim_x]], trainer))) &&
            (p->cost + findTileCost(m->mp[p->pos[dim_y]][p->pos[dim_x]], trainer)) >= 0)
        {
            path[p->pos[dim_y]][p->pos[dim_x] + 1].cost =
                (p->cost + findTileCost(m->mp[p->pos[dim_y]][p->pos[dim_x]], trainer));
            path[p->pos[dim_y]][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y]][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y]]
                                                 [p->pos[dim_x] + 1]
                                                     .hn);
        }
        if ((path[p->pos[dim_y] + 1][p->pos[dim_x]].hn) &&
            (path[p->pos[dim_y] + 1][p->pos[dim_x]].cost >
             (p->cost + findTileCost(m->mp[p->pos[dim_y]][p->pos[dim_x]], trainer))) &&
            (p->cost + findTileCost(m->mp[p->pos[dim_y]][p->pos[dim_x]], trainer)) >= 0)
        {
            path[p->pos[dim_y] + 1][p->pos[dim_x]].cost =
                (p->cost + findTileCost(m->mp[p->pos[dim_y]][p->pos[dim_x]], trainer));
            path[p->pos[dim_y] + 1][p->pos[dim_x]].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y] + 1][p->pos[dim_x]].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
                                                 [p->pos[dim_x]]
                                                     .hn);
        }

        //=======================================
        if ((path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].hn) &&
            (path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost >
             (p->cost + findTileCost(m->mp[p->pos[dim_y]][p->pos[dim_x]], trainer))) &&
            (p->cost + findTileCost(m->mp[p->pos[dim_y]][p->pos[dim_x]], trainer)) >= 0)
        {
            path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost =
                (p->cost + findTileCost(m->mp[p->pos[dim_y]][p->pos[dim_x]], trainer));
            path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                                 [p->pos[dim_x] - 1]
                                                     .hn);
        }
        if ((path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].hn) &&
            (path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].cost >
             (p->cost + findTileCost(m->mp[p->pos[dim_y]][p->pos[dim_x]], trainer))) &&
            (p->cost + findTileCost(m->mp[p->pos[dim_y]][p->pos[dim_x]], trainer)) >= 0)
        {
            path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].cost =
                (p->cost + findTileCost(m->mp[p->pos[dim_y]][p->pos[dim_x]], trainer));
            path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
                                                 [p->pos[dim_x] - 1]
                                                     .hn);
        }
        if ((path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].hn) &&
            (path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].cost >
             (p->cost + findTileCost(m->mp[p->pos[dim_y]][p->pos[dim_x]], trainer))) &&
            (p->cost + findTileCost(m->mp[p->pos[dim_y]][p->pos[dim_x]], trainer)) >= 0)
        {
            path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].cost =
                (p->cost + findTileCost(m->mp[p->pos[dim_y]][p->pos[dim_x]], trainer));
            path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                                 [p->pos[dim_x] + 1]
                                                     .hn);
        }
        if ((path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].hn) &&
            (path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].cost >
             (p->cost + findTileCost(m->mp[p->pos[dim_y]][p->pos[dim_x]], trainer))) &&
            (p->cost + findTileCost(m->mp[p->pos[dim_y]][p->pos[dim_x]], trainer)) >= 0)
        {
            path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].cost =
                (p->cost + findTileCost(m->mp[p->pos[dim_y]][p->pos[dim_x]], trainer));
            path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
                                                 [p->pos[dim_x] + 1]
                                                     .hn);
        }
    }
}

void dijkstra_map(map_t *m, trainer_t trainer)
{
    // dijkstra_tile(map_t *m, pair_t from, pair_t to)
    int i, j;
    pair_t from, to;
    static path_t path[yN][xN], *p;

    from[dim_x] = Bworld.pc.pos[dim_x];
    from[dim_y] = Bworld.pc.pos[dim_y];

    if (findTileCost(m->mp[Bworld.pc.pos[dim_y]][Bworld.pc.pos[dim_x]], trainer) == INT_MAX)
        return;

    j = 0, i = 0;
    to[dim_x] = i;
    to[dim_y] = j;
    dijkstra_tile(m, from, to, path, p, trainer);

    // printf("Weighted Map for ");
    // switch (trainer)
    // {
    // case hiker:
    //     printf("Hiker\n");
    //     break;
    // case rival:
    //     printf("Rival\n");
    //     break;
    // default:
    //     printf("Others\n");
    //     break;
    // }

    for (j = 0; j < yN; j++)
    {
        for (i = 0; i < xN; i++)
        {
            // if (path[j][i].cost != INT_MAX)
            //     printf("%02d ", path[j][i].cost % 100);
            // else
            //     printf("   ");
            if (trainer == hiker)
                Bworld.hiker_dist[j][i] = path[j][i].cost;
            if (trainer == rival)
                Bworld.rival_dist[j][i] = path[j][i].cost;
        }
        // printf("\n");
    }
    // printf("\n");
}

dir_t findNeighbourWeighted(map_t *m, info_t *info)
{
    int x[] = {0, 0, -1, 1, -1, 1, -1, 1};
    int y[] = {-1, 1, 0, 0, -1, -1, 1, 1};
    int i, dir, temp, lowest = INT_MAX;

    for (i = 0; i < num_dir; i++)
    {
        if (info->trainer == hiker)
            temp = Bworld.hiker_dist[info->pos[dim_y] + y[i]][info->pos[dim_x] + x[i]];
        else
            temp = Bworld.rival_dist[info->pos[dim_y] + y[i]][info->pos[dim_x] + x[i]];
        if (temp < lowest && m->npc_mp[info->pos[dim_y] + y[i]][info->pos[dim_x] + x[i]] == NULL)
        {
            lowest = temp;
            dir = i;
        }
    }

    return (dir_t)dir;
}

int checkValidMove(map_t *m, info_t *info, dir_t dir)
{
    int x[] = {0, 0, -1, 1, -1, 1, -1, 1};
    int y[] = {-1, 1, 0, 0, -1, -1, 1, 1};

    // if moving into trainer, return -1

    terrain_t ter = m->mp[info->pos[dim_y]][info->pos[dim_x]];
    terrain_t nextTer = m->mp[info->pos[dim_y] + y[dir]][info->pos[dim_x] + x[dir]];
    if (findTileCost(nextTer, info->trainer) != INT_MAX && m->npc_mp[info->pos[dim_y] + y[dir]][info->pos[dim_x] + x[dir]] == NULL)
    {
        switch (info->trainer)
        {
        case pacer:
        case wanderer:
            if (ter == nextTer)
                return 0;
            break;
        case random_walker:
            return 0;
            break;
        default:
            break;
        }
    }
    return -1;
}

void shiftTrainer(map_t *m, info_t *info, dir_t dir, int counter)
{
    int x[] = {0, 0, -1, 1, -1, 1, -1, 1};
    int y[] = {-1, 1, 0, 0, -1, -1, 1, 1};
    // int *dist_map;
    // terrain_t ter = m->mp[info->pos[dim_y] + y[dir]][info->pos[dim_x] + x[dir]];

    if (m->trainerList[counter]->pos[dim_x] + x[dir] == Bworld.pc.pos[dim_x] && m->trainerList[counter]->pos[dim_y] + y[dir] == Bworld.pc.pos[dim_y])
    {
        printf("Contacted Player!!!\t");
        check = 'q';
        return;
    }
    if (findTileCost(m->mp[info->pos[dim_y] + y[dir]][info->pos[dim_x] + x[dir]], info->trainer) != INT_MAX && (m->npc_mp[info->pos[dim_y] + y[dir]][info->pos[dim_x] + x[dir]] == NULL))
    {
        int new_x = info->pos[dim_x] + x[dir];
        int new_y = info->pos[dim_y] + y[dir];
        m->npc_mp[new_y][new_x] = malloc(sizeof(info_t));

        m->npc_mp[new_y][new_x]->pos[dim_x] = new_x;
        m->npc_mp[new_y][new_x]->pos[dim_y] = new_y;
        m->npc_mp[new_y][new_x]->prev = info->prev;
        m->npc_mp[new_y][new_x]->trainer = info->trainer;
        m->npc_mp[new_y][new_x]->placement = info->placement;
        m->npc_mp[new_y][new_x]->currCost = info->currCost + findTileCost(m->mp[new_y][new_x], info->trainer);
        m->npc_mp[info->pos[dim_y]][info->pos[dim_x]] = NULL;
        free(m->npc_mp[info->pos[dim_y]][info->pos[dim_x]]);

        m->trainerList[counter] = m->npc_mp[new_y][new_x];
    }
    else
    {
        info->currCost += findTileCost(m->mp[info->pos[dim_y]][info->pos[dim_x]], info->trainer);
    }
}

// randomly move player
void randMove(map_t *m)
{
    dir_t dir;
    int x[] = {0, 0, -1, 1, -1, 1, -1, 1};
    int y[] = {-1, 1, 0, 0, -1, -1, 1, 1};

    do
    {
        dir = (dir_t)(rand() % num_dir);
    } while (m->npc_mp[Bworld.pc.pos[dim_y] + y[dir]][Bworld.pc.pos[dim_x] + x[dir]] != NULL);

    terrain_t ter = m->mp[Bworld.pc.pos[dim_y] + y[dir]][Bworld.pc.pos[dim_x] + x[dir]];
    if (findTileCost(ter, player) != INT_MAX)
    {
        Bworld.pc.pos[dim_x] += x[dir];
        Bworld.pc.pos[dim_y] += y[dir];
        Bworld.pc.currCost += (findTileCost(ter, player));
    }
}

void move_Hiker_Rival(map_t *m, info_t *info, int counter)
{
    shiftTrainer(m, info, findNeighbourWeighted(m, info), counter);
}

void move_RandomWalker_Pacer(map_t *m, info_t *info, int counter)
{
    int temp = 0;
    if (info->prev == no_dir)
    {
        while (temp < 100)
        {
            dir_t dir = (dir_t)(rand() % 4);
            if (checkValidMove(m, info, dir) == 0)
            {
                info->prev = dir;
                shiftTrainer(m, info, dir, counter);
                return;
            }
            temp++;
        }
    }
    else if (info->prev != no_dir && checkValidMove(m, info, info->prev) == 0)
        shiftTrainer(m, info, info->prev, counter);
    else if (info->prev < 4) // can only have n, s, w, e
    {
        // flip 180
        if (info->prev % 2 == 0)
        {
            info->prev = info->prev + 1;
        }
        else
        {
            info->prev = info->prev - 1;
        }
        if (checkValidMove(m, info, info->prev) == 0)
            shiftTrainer(m, info, (dir_t)(info->prev), counter);
        else
            info->currCost += findTileCost(m->mp[info->pos[dim_y]][info->pos[dim_x]], info->trainer);
    }
}

void moveWanderer(map_t *m, info_t *info, int counter)
{
    int temp = 0;
    if (info->prev == no_dir)
    {
        while (temp < 100)
        {
            dir_t dir = (dir_t)(rand() % 4);
            if (checkValidMove(m, info, dir) == 0)
            {
                info->prev = dir;
                shiftTrainer(m, info, dir, counter);
                return;
            }
            temp++;
        }
    }
    else if (info->prev != no_dir && checkValidMove(m, info, info->prev) == 0)
        shiftTrainer(m, info, info->prev, counter);
    else if (info->prev < 4) // can only have n, s, w, e
    {
        do
        {
            switch (info->prev)
            {
            case n:
                info->prev = e;
                break;
            case e:
                info->prev = s;
                break;
            case s:
                info->prev = w;
                break;
            case w:
                info->prev = n;
                break;
            default:
                break;
            }
            if (checkValidMove(m, info, info->prev) == 0)
            {
                shiftTrainer(m, info, info->prev, counter);
                return;
            }
            temp++;
        } while (temp < 100);
        info->currCost += findTileCost(m->mp[info->pos[dim_y]][info->pos[dim_x]], info->trainer);
    }
}

// helper for path builder
//  sx, sy = starting coords, ex, ey = ending coords  18 3 18 1
void connect(terrain_t map[yN][xN], int sx, int sy, int ex, int ey)
{
    int temp_x, temp_y, curr_x, curr_y, checkP;
    checkP = 1;
    curr_x = sx;
    curr_y = sy;
    temp_x = 0;
    temp_y = 0;
    int xdir = 1;
    int ydir = 1;

    // catch error
    if ((ex == sx && ey == sy) || sx < 0 || sy < 0 || ex < 0 || ey < 0)
        return;

    // setting direction
    if (ex < sx)
        xdir = -1;
    if (ey < sy)
        ydir = -1;

    // randomizing meeting point
    if (sx != ex)
        temp_x = sx + (rand() % abs(sx - ex) + 1) * xdir;
    if (sy != ey)
        temp_y = sy + (rand() % abs(sy - ey) + 1) * ydir;

    // while (!(abs(curr_y - temp_y) == 0 && abs(curr_x - temp_x) == 0))
    while ((curr_y != temp_y && temp_y != 0) || (curr_x != temp_x && temp_x != 0))
    {
        if (curr_x == 0 && abs(curr_x - temp_x) > 0)
            curr_x = curr_x + 1 * xdir;
        else if (curr_y == 0 || curr_y == yN - 1 || temp_x == 0 || (curr_y != ey && abs(curr_y - temp_y) >= abs(curr_x - temp_x)))
        {
            if (abs(curr_y - temp_y) > 0)
                curr_y = curr_y + 1 * ydir;
        }
        else if (curr_x == 0 || curr_x == xN - 1 || temp_y == 0 || (curr_x != ex && abs(curr_y - temp_y) < abs(curr_x - temp_x)))
        {
            if (abs(curr_x - temp_x) > 0)
                curr_x = curr_x + 1 * xdir;
        }

        if (map[curr_y][curr_x] == path)
        {
            checkP = 0;
            break;
        }
        else if (map[curr_y][curr_x] != path && map[curr_y][curr_x] != center && map[curr_y][curr_x] != mart && map[curr_y][curr_x] != wall)
            map[curr_y][curr_x] = path;
    }

    if (checkP != 0)
    {
        while (curr_y != ey || curr_x != ex)
        {
            if (curr_y == 0 || curr_y == yN - 1 || abs(curr_x - ex) == 0 || (curr_y != ey && abs(curr_y - ey) >= abs(curr_x - ex)))
            {
                if (abs(curr_y - ey) > 0)
                    curr_y = curr_y + 1 * ydir;
            }
            else if (curr_x == 0 || curr_x == xN - 1 || abs(curr_y - ey) == 0 || (curr_x != ex && abs(curr_y - ey) < abs(curr_x - ex)))
            {
                if (abs(curr_x - ex) > 0)
                    curr_x = curr_x + 1 * xdir;
            }

            if (map[curr_y][curr_x] != path && map[curr_y][curr_x] != center && map[curr_y][curr_x] != mart && map[curr_y][curr_x] != wall)
                map[curr_y][curr_x] = path;
            else if (map[curr_y][curr_x] == path)
            {
                break;
            }
        }
    }
}

// path builder
void bPath(terrain_t map[yN][xN], map_t *m)
{
    int x, y;

    // select center
    do
    {
        y = rand() % (yN / 2) + (yN / 3);
        x = rand() % (xN / 2) + (xN / 3);
    } while (map[y][x] == center || map[y][x] == mart);
    map[y][x] = path;

    // setting pc pos
    Bworld.pc.pos[dim_x] = x;
    Bworld.pc.pos[dim_y] = y;
    Bworld.pc.currCost = (findTileCost(m->mp[y][x], player));
    // FIRST TO ADD TO QUEUE

    // connect left
    connect(map, m->left[dim_x], m->left[dim_y], x, y);
    // connect right
    connect(map, m->right[dim_x], m->right[dim_y], x, y);
    // connect up
    connect(map, m->up[dim_x], m->up[dim_y], x, y);
    // connect down
    connect(map, m->down[dim_x], m->down[dim_y], x, y);
}

void building_paths(terrain_t map[yN][xN], int x, int y)
{
    int xsml, ysml, xcount, ycount, dir, xdir, ydir;
    for (int i = 0; i < 2; i++)
    {
        dir = 1;
        xdir = 1;
        ydir = 1;
        xcount = 0;
        ycount = 0;
        if (i == 1)
        {
            dir = -1;
        }
        if (map[y + 2][x] == path || map[y][x + 2] == path || map[y - 1][x] == path || map[y][x - 1] == path)
            break;

        while ((y + ycount * dir) < yN - 1 && (y + ycount * dir) > 0)
        {
            ycount++;
            if (map[y + ycount * dir][x] == path)
                break;
            if (map[y + ycount * dir][x] == wall)
            {
                ycount = 1000;
                break;
            }
        }

        while ((x + xcount * dir) < (xN - 1) && (x + xcount * dir) > 0)
        {
            xcount++;
            if (map[y][x + xcount * dir] == path)
                break;
            if (map[y][x + xcount * dir] == wall)
            {
                xcount = 1000;
                break;
            }
        }

        if (i == 0)
        {
            xsml = xcount;
            ysml = ycount;
        }
        else
        {
            if (xsml > xcount)
            {
                xsml = xcount;
                xdir = dir;
            }

            if (ysml > ycount)
            {
                ysml = ycount;
                ydir = dir;
            }
        }
    }
    if (xcount == 0 || ycount == 0)
        return;
    if (ysml >= 1000 && xsml >= 1000)
        return;
    if (xsml < ysml)
        connect(map, x, y, x + xsml * xdir, y);
    else
        connect(map, x, y, x, y + ysml * ydir);
}

// build PokeMart and PokeCenter
void pokeMC(terrain_t map[yN][xN])
{
    int x1, y1, x2, y2;

    do
    {
        x1 = rand() % (xN - 16) + 8;
        y1 = rand() % (yN - 8) + 4;
    } while (map[y1][x1] == path || map[y1 + 1][x1] == path || map[y1][x1 + 1] == path || map[y1 + 1][x1 + 1] == path);

    map[y1][x1] = center;
    map[y1 + 1][x1] = center;
    map[y1][x1 + 1] = center;
    map[y1 + 1][x1 + 1] = center;

    do
    { // might have to change to complete random
        x2 = max(min(abs(x1 + (rand() % 3 - 1) * max((rand() % (xN / 3)), 3)), xN - 3), 1);
        y2 = max(min(abs(y1 + (rand() % 3 - 1) * max((rand() % (yN / 3)), 3)), yN - 3), 1);
    } while (map[y2][x2] == path || map[y2 + 1][x2] == path || map[y2][x2 + 1] == path || map[y2 + 1][x2 + 1] == path || x1 == x2 || y1 == y2 || x1 + 1 == x2 || y1 + 1 == y2);
    map[y2][x2] = mart;
    map[y2 + 1][x2] = mart;
    map[y2][x2 + 1] = mart;
    map[y2 + 1][x2 + 1] = mart;

    // check closest path
    building_paths(map, x1, y1);
    building_paths(map, x2, y2);
}

// growing grass
void grow(int x, int y, int t, int size, terrain_t map[yN][xN])
{
    for (int j = y; j < y + size && j < yN - 1; j++)
    {
        if (map[j][x] == path || map[j][x] == wall)
            break;
        for (int i = x; i < x + size * 2 && i < xN - 1; i++)
        {
            if (map[j][i] == path || map[j][i] == wall)
                break;
            else if (map[j][i] != blank)
                continue;
            else
                map[j][i] = (terrain_t)(t);
        }
    }
    for (int j = y; j < y + size && j < yN - 1; j++)
    {
        if (map[j][x] == path || map[j][x] == wall)
            break;
        for (int i = x; i > x - size * 2 && i > 0; i--)
        {
            if (map[j][i] == path || map[j][i] == wall)
                break;
            else if (map[j][i] != blank)
                continue;
            else
                map[j][i] = (terrain_t)(t);
        }
    }

    for (int j = y; j > y - size && j > 0; j--)
    {
        if (map[j][x] == path || map[j][x] == wall)
            break;
        for (int i = x; i < x + size * 2 && i < xN - 1; i++)
        {
            if (map[j][i] == path || map[j][i] == wall)
                break;
            else if (map[j][i] != blank)
                continue;
            else
                map[j][i] = (terrain_t)(t);
        }
    }
    for (int j = y; j > y - size && j > 0; j--)
    {
        if (map[j][x] == path || map[j][x] == wall)
            break;
        for (int i = x; i > x - size * 2 && i > 0; i--)
        {
            if (map[j][i] == path || map[j][i] == wall)
                break;
            else if (map[j][i] != blank)
                continue;
            else
                map[j][i] = (terrain_t)(t);
        }
    }
}

// map generation
void generation(terrain_t map[yN][xN], map_t *m, int dis)
{
    int i, j, x, y, attemps;
    float value;

    // initialize map with blanks
    for (j = 0; j < yN; j++)
    {
        for (i = 0; i < xN; i++)
        {
            map[j][i] = blank;
        }
    }

    // fill walls
    for (i = 0; i < xN; i++)
    {
        map[0][i] = wall;
        map[yN - 1][i] = wall;
    }
    for (j = 0; j < yN; j++)
    {
        map[j][0] = wall;
        map[j][xN - 1] = wall;
    }

    // fill doors
    if (m->up[0] != -2)
        map[m->up[1]][m->up[0]] = door;
    if (m->down[0] != -2)
        map[m->down[1]][m->down[0]] = door;
    if (m->left[1] != -2)
        map[m->left[1]][m->left[0]] = door;
    if (m->right[1] != -2)
        map[m->right[1]][m->right[0]] = door;

    // build path
    bPath(map, m);

    // fill empty with grass
    attemps = 0;
    for (i = 0; i < 4; i++)
    {
        do
        {
            x = rand() % (xN - 1) + 1;
            y = rand() % (yN - 1) + 1;
        } while (map[y][x] != blank && attemps++ < 200);

        grow(x, y, i, max((rand() % (yN / 2)) + yN / 3, 3), map);
    }

    // build pokeCenter & pokeMart
    value = ((float)(-45 * dis / 200.0) + 50) / 100;

    if (dis == 0)
        pokeMC(map);
    else if (dis < 200)
    {
        if ((float)rand() / RAND_MAX <= value)
            pokeMC(map);
    }
    else
    {
        if ((float)rand() / RAND_MAX <= 0.05)
            pokeMC(map);
    }

    // attempts to fill remaining blanks with randomized terrain type
    attemps = 0;
    while (attemps < 200)
    {
        x = rand() % (xN - 1) + 1;
        y = rand() % (yN - 1) + 1;
        if (map[y][x] == blank)
        {
            i = rand() % 10;
            // 10% do nothing
            // 30% chance for water, tree, snow_mount
            if (i > 6)
            {
                i = 4 + rand() % 3;
            }
            else
            { // 60% chance for regular grass
                i = rand() % 4;
            }

            grow(x, y, i, max((rand() % (yN / 2) + (y / 3)), 4), map);
        }
        attemps++;
    }

    for (j = 0; j < yN; j++)
    {
        for (i = 0; i < xN; i++)
        {
            if (map[j][i] == blank)
            {
                map[j][i] = grass_1;
            }
        }
    }
}

char trainerCheck(map_t *m, int x, int y)
{
    char c = '\0';
    if (m->npc_mp[y][x] != NULL)
    {
        switch (m->npc_mp[y][x]->trainer)
        {
        case player:
            c = '@';
            break;
        case rival:
            c = 'r';
            break;
        case hiker:
            c = 'h';
            break;
        case pacer:
            c = 'p';
            break;
        case wanderer:
            c = 'w';
            break;
        case stationarie:
            c = 's';
            break;
        case random_walker:
            c = 'n';
            break;
        default:
            break;
        }
    }
    return c;
}

// map printing
void print(map_t *m, int x, int y)
{
    int i, j;
    char c;

    for (j = 0; j < yN; j++)
    {
        for (i = 0; i < xN; i++)
        {
            if (i == Bworld.pc.pos[dim_x] && j == Bworld.pc.pos[dim_y])
            {
                putchar('@');
                continue;
            }
            c = trainerCheck(m, i, j);
            if (c != '\0')
            {
                printf("%c", c);
                continue;
            }
            switch (m->mp[j][i])
            {
            case grass_1:
                putchar(',');
                break;
            case grass_2:
                putchar(';');
                break;
            case grass_3:
                putchar('-');
                break;
            case grass_4:
                putchar('^');
                break;

            case tree:
                putchar('\"');
                break;
            case snow_mount:
                putchar('*');
                break;

            case water:
                putchar('~');
                break;

            case blank:
                putchar(' ');
                break;
            case wall:
                putchar('%');
                break;
            case door:
                putchar('=');
                break;
            case path:
                putchar('#');
                break;

            case mart:
                putchar('M');
                break;
            case center:
                putchar('C');
                break;

            default:
                putchar(' ');
                break;
            }
        }
        putchar('\n');
    }
    printf("Current Coords: (%d, %d)\n", x, y);
}

void cycle(map_t *m)
{
    // info_t **info = m->trainerList;
    info_t *z, *temp;
    heap_t h;
    int i = 0;

    heap_init(&h, info_cmp, NULL);

    temp = &Bworld.pc;
    temp->hn = heap_insert(&h, &Bworld.pc);
    while (i < numTrainer)
    {
        temp = m->trainerList[i];
        if (temp->trainer != stationarie)
            temp->hn = heap_insert(&h, temp);
        i++;
    }

    // info_t *temp1 = heap_peek_min(&h);
    while ((z = heap_remove_min(&h)))
    {
        if (check == 'q')
            return;
        switch (z->trainer)
        {
        case player:
            randMove(m);
            return;
            break;
        case rival:
        case hiker:
            move_Hiker_Rival(m, m->trainerList[z->placement], z->placement);
            break;
        case random_walker:
        case pacer:
            move_RandomWalker_Pacer(m, m->trainerList[z->placement], z->placement);
            break;
        case wanderer:
            moveWanderer(m, m->trainerList[z->placement], z->placement);
            break;
        default:
            break;
        }
        m->trainerList[z->placement]->hn = heap_insert(&h, m->trainerList[z->placement]);
    }
}

void trainer_init(map_t *m, int numTrainer)
{
    int x = 0, y = 0
        // , i, j
        ;

    // info_t *info = malloc(sizeof(info_t));_t
    m->trainerList = (info_t **)malloc(numTrainer * sizeof(info_t));

    do
    {
        trainer_t npc = (trainer_t)(rand() % (num_trainer_type - 2)) + 2;

        do
        {
            x = rand() % xN;
            y = rand() % yN;
        } while (y >= map_size - 1 || x >= map_size - 1 ||
                 y <= 1 || x <= 1 || (Bworld.pc.pos[dim_x] == x && Bworld.pc.pos[dim_y] == y) || (findTileCost(m->mp[y][x], npc) == INT_MAX) || m->mp[y][x] == snow_mount || m->mp[y][x] == path);

        if (numTrainer) // if numTrainer = 0, skip
        {
            m->npc_mp[y][x] = malloc(sizeof(info_t));
            info_t *temp = m->npc_mp[y][x];

            // info_t *temp = malloc(sizeof(*temp));

            if (numTrainer > 2)
            {
                temp->trainer = (trainer_t)(rand() % (num_trainer_type - 2)) + 2;
                // temp->trainer = random_walker;
            }
            if (numTrainer == 2)
            {
                temp->trainer = hiker;
                // temp->trainer = random_walker;
            }
            if (numTrainer == 1)
            {
                temp->trainer = rival;
                // temp->trainer = random_walker;
            }
            temp->currCost = (findTileCost(m->mp[y][x], npc));
            temp->pos[dim_x] = x;
            temp->pos[dim_y] = y;
            temp->prev = no_dir;
            --numTrainer;
            temp->placement = numTrainer;

            // m->npc_mp[y][x] = temp;
            m->trainerList[numTrainer] = m->npc_mp[y][x];

            // WHEN WE ADD A TRAINER TO LIST, ADD TO QUEUE
        }
    } while (numTrainer != 0);
}

map_t *map_init(int u, int d, int l, int r, int dis)
{
    map_t *m = malloc(sizeof(*m));

    for (int j = 0; j < yN; j++)
    {
        for (int i = 0; i < xN; i++)
        {
            m->npc_mp[j][i] = NULL;
        }
    }

    if (u == -1)
        m->up[0] = rand() % (xN - 6) + 3;
    else
        m->up[0] = u;
    m->up[1] = 0;

    if (d == -1)
        m->down[0] = rand() % (xN - 6) + 3;
    else
        m->down[0] = d;
    m->down[1] = yN - 1;

    m->left[0] = 0;
    if (l == -1)
        m->left[1] = rand() % (yN - 6) + 3;
    else
        m->left[1] = l;

    m->right[0] = xN - 1;
    if (r == -1)
        m->right[1] = rand() % (yN - 6) + 3;
    else
        m->right[1] = r;
    generation(m->mp, m, dis);
    return m;
}

int main(int argc, char const *argv[])
{
    struct timeval tv;
    uint32_t seed;

    int i, j, up, down, left, right
        // , x, ys
        ,
        dis;
    char dir;
    if (argc > 2)
    {
        if (strcmp(argv[argc - 2], "--numtrainers") == 0)
            numTrainer = strtol(argv[argc - 1], NULL, 10);
        if (strcmp(argv[argc - 2], "--seed") == 0)
            seed = atoi(argv[argc - 1]);
    }
    else
    {
        gettimeofday(&tv, NULL);
        seed = (tv.tv_usec ^ (tv.tv_sec << 20)) & 0xffffffff;
    }

    srand(seed);

    for (j = 0; j < map_size; j++)
    {
        for (i = 0; i < map_size; i++)
        {
            Bworld.world[j][i] = NULL;
        }
    }

    i = center_x, j = center_y;

    do
    {
        do
        {
            printf("\n");
            up = -1, down = -1, left = -1, right = -1;
            if (Bworld.world[j][i] == NULL)
            {
                if (j - 1 > 0 && Bworld.world[j - 1][i] != NULL)
                    up = Bworld.world[j - 1][i]->down[0];
                if (j + 1 < map_size && Bworld.world[j + 1][i] != NULL)
                    down = Bworld.world[j + 1][i]->up[0];
                if (i - 1 > 0 && Bworld.world[j][i - 1] != NULL)
                    left = Bworld.world[j][i - 1]->right[1];
                if (i + 1 < map_size && Bworld.world[j][i + 1] != NULL)
                    right = Bworld.world[j][i + 1]->left[1];

                if (j == 0)
                    up = -2;
                if (j == map_size - 1)
                    down = -2;
                if (i == 0)
                    left = -2;
                if (i == map_size - 1)
                    right = -2;

                dis = abs(i - center_x) + abs(j - center_y);

                Bworld.world[j][i] = map_init(up, down, left, right, dis);
                dijkstra_map(Bworld.world[j][i], hiker);
                dijkstra_map(Bworld.world[j][i], rival);
                // heap_init(&Bworld.world[j][i]->h, info_cmp, NULL);
                trainer_init(Bworld.world[j][i], numTrainer);
            }
            else
            {
                // randMove(Bworld.world[j][i]);
                cycle(Bworld.world[j][i]);
            }
            dijkstra_map(Bworld.world[j][i], hiker);
            dijkstra_map(Bworld.world[j][i], rival);
            
            printf("Using seed: %u\n", seed);
            print(Bworld.world[j][i], i - center_x, j - center_y);
            usleep(250000);
        } while (check != 'q'); // force cycle

        // do
        // {
        //     printf("Direction (n, s, e, w, f, q to quit): ");
        //     scanf("%c", &dir);
        //     // fflush(stdin);
        //     if (dir == 'n')
        //     {
        //         if (j - 1 >= 0)
        //             j--;
        //         else
        //         {
        //             printf("At the edge of map, try another direction.\n");
        //             dir = 'z';
        //         }
        //     }
        //     else if (dir == 's')
        //     {
        //         if (j + 1 < map_size)
        //             j++;
        //         else
        //         {
        //             printf("At the edge of map, try another direction.\n");
        //             dir = 'z';
        //         }
        //     }
        //     else if (dir == 'e')
        //     {
        //         if (i + 1 < map_size)
        //             i++;
        //         else
        //         {
        //             printf("At the edge of map, try another direction.\n");
        //             dir = 'z';
        //         }
        //     }
        //     else if (dir == 'w')
        //     {
        //         if (i - 1 >= 0)
        //             i--;
        //         else
        //         {
        //             printf("At the edge of map, try another direction.\n");
        //             dir = 'z';
        //         }
        //     }
        //     else if (dir == 'f')
        //     {
        //         printf("Where to fly to (x y): ");
        //         if (scanf("%d", &x) == 1 && scanf("%d", &y) == 1)
        //         {
        //             // fflush(stdin);

        //             if (x + center_x >= 0 && x + center_x < map_size && y + center_y >= 0 && y + center_y < map_size)
        //             {
        //                 i = x + center_x;
        //                 j = y + center_y;
        //                 printf("\nFlying to (%d, %d)...\n", x, y);
        //                 while ((getchar()) != '\n')
        //                     ;
        //                 break;
        //             }
        //             else
        //             {
        //                 printf("Invalid coordinate. Please try again.\n");
        //                 dir = 'z';
        //             }
        //         }
        //         else
        //         {
        //             printf("Input is not a coordinate. Please try again.\n");
        //             dir = 'z';
        //         }
        //         // if (dir != '\n')
        //         //     while ((getchar()) != '\n');
        //     }
        //     else if (dir == 'q')
        //     {
        //         printf("Quitting...\n");
        //         for (j = 0; j < map_size; j++)
        //             for (i = 0; i < map_size; i++)
        //                 if (world[j][i] != NULL)
        //                     free(world[j][i]);
        //     }
        //     else
        //     {
        //         printf("Invalid direction. Please try again.\n");
        //     }
        //     if (dir != '\n')
        //         while ((getchar()) != '\n')
        //             ;

        // } while (dir != 'n' && dir != 's' && dir != 'e' && dir != 'w' && dir != 'q' && dir != 'f');

        dir = 'q';
    } while (dir != 'q');

    return 0;
}
