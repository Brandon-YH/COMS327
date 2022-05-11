// #include "map.h"
#include "heap.h"
#include "pokemon.h"
#include "item.h"
// #include "csv_parse.h"

#include <ncurses.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <assert.h>
#include <unistd.h>
#include <cmath>

#define heightpair(pair) (m->height[pair[dim_y]][pair[dim_x]])
#define heightxy(x, y) (m->height[y][x])

#define map_size 401
#define center_x 401 / 2
#define center_y 401 / 2
#define xN 80
#define yN 21

#define PLAYER_PAIR 01
#define RIVAL_PAIR 02
#define HIKER_PAIR 03
#define PACER_PAIR 04
#define WANDERER_PAIR 05
#define STATIONARIE_PAIR 06
#define RANDWALKER_PAIR 07

#define CENTER_PAIR 21
#define MART_PAIR 22
#define PATH_PAIR 23
#define GRASS1_PAIR 24
#define GRASS2_PAIR 25
#define GRASS3_PAIR 26
#define GRASS4_PAIR 27
#define MOUNT_PAIR 28
#define TREE_PAIR 29
#define WATER_PAIR 30
#define WALL_PAIR 31
#define EXIT_PAIR 32

#define COLOR_DARK_GREEN 11
#define COLOR_LIGHT_GREEN 12
#define COLOR_CACT_GREEN 13
#define COLOR_DARK_BLUE 14
#define COLOR_LIGHT_BLUE 15
#define COLOR_BROWN 16
#define COLOR_ORANGE 17
#define COLOR_PINK 18
#define COLOR_GREY 19
#define COLOR_LIGHT_GREY 20

typedef enum dim
{
    dim_x,
    dim_y,
    num_dims
} dim_t;

typedef int16_t pair_t[num_dims];

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

static const char *ter_name[] = {
    "Grass_1",
    "Grass_2",
    "Grass_3",
    "Grass_4",

    "Tree",
    "Snowy mount",
    "Lake",

    "blank",
    "Wall",
    "Door",
    "Path",

    "Center",
    "Mart"};

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
    skip,
    no_dir = -1,
} dir_t;

static pair_t all_dirs[8] = {
    {0, -1},
    {0, 1},
    {-1, 0},
    {1, 0},
    {-1, -1},
    {1, -1},
    {-1, 1},
    {1, 1},
};

typedef class path
{
public:
    heap_node_t *hn;
    uint8_t pos[2];
    uint8_t from[2];
    int32_t cost;
} path_t;

// class Pokemon
// {
// public:
//     Pokemon pokemon;
//     int level, IV, maxHP, currHP, atk, def, s_atk, s_def, speed, accuracy, evasion;
//     bool is_shiny, isMale;
//     std::vector<moves> move_set;
// };

typedef class trainerInfo
{
public:
    heap_node_t *hn;
    int pos[num_dims];
    int currCost;
    int placement;
    bool defeated;
    dir_t prev;
    trainer_t trainer;
    Pokemon *captured[6];
    Items *item;
} info_t;

typedef class map
{
public:
    terrain_t mp[yN][xN];
    info_t *npc_mp[yN][xN];
    info_t **trainerList;
    int pc_currCost;

    heap_t turn;
    // coordinate of doors, eg: up = {x, y}, down = {x, y}, ...
    pair_t up, down, left, right;
} map_t;

typedef class world
{
public:
    map_t *world[map_size][map_size];
    map_t *currMap;
    pair_t cur_idx;
    int hiker_dist[yN][xN];
    int rival_dist[yN][xN];
    info_t pc;
} world_t;

world_t Bworld;
uint32_t seed;
char check;
int numTrainer = 8;

void customColor()
{

    init_color(COLOR_DARK_GREEN, 0, 431, 0);
    init_color(COLOR_LIGHT_GREEN, 529, 843, 529);
    init_color(COLOR_CACT_GREEN, 627, 745, 235);
    init_color(COLOR_LIGHT_BLUE, 313, 686, 999);
    init_color(COLOR_DARK_BLUE, 10, 10, 999);
    init_color(COLOR_BROWN, 666, 529, 0);
    init_color(COLOR_ORANGE, 901, 588, 196);
    init_color(COLOR_PINK, 999, 588, 588);
    init_color(COLOR_GREY, 352, 352, 352);
    init_color(COLOR_LIGHT_GREY, 686, 686, 686);

    // #define PLAYER_PAIR       01
    // #define RIVAL_PAIR        02
    // #define HIKER_PAIR        03
    // #define PACER_PAIR        04
    // #define WANDERER_PAIR     05
    // #define STATIONARIE_PAIR  06
    // #define RANDWALKER_PAIR   07

    init_pair(PLAYER_PAIR, COLOR_PINK, COLOR_BLACK);

    init_pair(CENTER_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(MART_PAIR, COLOR_DARK_BLUE, COLOR_BLACK);
    init_pair(PATH_PAIR, COLOR_LIGHT_GREY, COLOR_BLACK);
    init_pair(EXIT_PAIR, COLOR_LIGHT_GREY, COLOR_BLACK);
    init_pair(GRASS1_PAIR, COLOR_LIGHT_GREEN, COLOR_BLACK);
    init_pair(GRASS2_PAIR, COLOR_DARK_GREEN, COLOR_BLACK);
    init_pair(GRASS3_PAIR, COLOR_CACT_GREEN, COLOR_BLACK);
    init_pair(GRASS4_PAIR, COLOR_GREEN, COLOR_BLACK);
    init_pair(MOUNT_PAIR, COLOR_LIGHT_BLUE, COLOR_BLACK);
    init_pair(TREE_PAIR, COLOR_BROWN, COLOR_BLACK);
    init_pair(WATER_PAIR, COLOR_BLUE, COLOR_LIGHT_BLUE);
    init_pair(WALL_PAIR, COLOR_ORANGE, COLOR_GREY);
}

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

    while ((p = (path_t *)heap_remove_min(&h)))
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
    // int x[] = {0, 0, -1, 1, -1, 1, -1, 1};
    // int y[] = {-1, 1, 0, 0, -1, -1, 1, 1};
    int i, dir, temp, lowest = INT_MAX;

    for (i = 0; i < num_dir; i++)
    {
        if (info->trainer == hiker)
            temp = Bworld.hiker_dist[info->pos[dim_y] + all_dirs[i][dim_y]][info->pos[dim_x] + all_dirs[i][dim_x]];
        else
            temp = Bworld.rival_dist[info->pos[dim_y] + all_dirs[i][dim_y]][info->pos[dim_x] + all_dirs[i][dim_x]];
        if (temp < lowest && m->npc_mp[info->pos[dim_y] + all_dirs[i][dim_y]][info->pos[dim_x] + all_dirs[i][dim_x]] == NULL)
        {
            lowest = temp;
            dir = i;
        }
    }

    return (dir_t)dir;
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
void print_map()
{
    int i, j;
    char c, t;
    map_t *m = Bworld.currMap;

    customColor();
    mvprintw(0, 0, "Using seed: %u\n", seed);
    mvprintw(23, 0, "                                                      ");
    for (j = 0; j < yN; j++)
    {
        for (i = 0; i < xN; i++)
        {
            if (i == Bworld.pc.pos[dim_x] && j == Bworld.pc.pos[dim_y])
            {
                attron(COLOR_PAIR(PLAYER_PAIR));
                mvprintw(j + 1, i, "@");
                attroff(COLOR_PAIR(PLAYER_PAIR));
                continue;
            }
            c = trainerCheck(m, i, j);
            if (c != '\0')
            {
                mvprintw(j + 1, i, "%c", c);
                continue;
            }
            switch (m->mp[j][i])
            {
            case grass_1:
                attron(COLOR_PAIR(GRASS1_PAIR));
                t = '\'';
                mvprintw(j + 1, i, "%c", t);
                attroff(COLOR_PAIR(GRASS1_PAIR));
                break;
            case grass_2:
                attron(COLOR_PAIR(GRASS2_PAIR));
                t = ';';
                mvprintw(j + 1, i, "%c", t);
                attroff(COLOR_PAIR(GRASS2_PAIR));
                break;
            case grass_3:
                attron(COLOR_PAIR(GRASS3_PAIR));
                t = '-';
                mvprintw(j + 1, i, "%c", t);
                attroff(COLOR_PAIR(GRASS3_PAIR));
                break;
            case grass_4:
                attron(COLOR_PAIR(GRASS4_PAIR));
                t = '"';
                mvprintw(j + 1, i, "%c", t);
                attroff(COLOR_PAIR(GRASS1_PAIR));
                break;

            case tree:
                attron(COLOR_PAIR(TREE_PAIR));
                t = '^';
                mvprintw(j + 1, i, "%c", t);
                attroff(COLOR_PAIR(TREE_PAIR));
                break;
            case snow_mount:
                attron(COLOR_PAIR(MOUNT_PAIR));
                t = '*';
                mvprintw(j + 1, i, "%c", t);
                attroff(COLOR_PAIR(MOUNT_PAIR));
                break;

            case water:
                attron(COLOR_PAIR(WATER_PAIR));
                t = '~';
                mvprintw(j + 1, i, "%c", t);
                attroff(COLOR_PAIR(WATER_PAIR));
                break;

            case wall:
                attron(COLOR_PAIR(WALL_PAIR));
                t = '%';
                mvprintw(j + 1, i, "%c", t);
                attroff(COLOR_PAIR(MOUNT_PAIR));
                break;
            case door:
                attron(COLOR_PAIR(EXIT_PAIR));
                t = '=';
                mvprintw(j + 1, i, "%c", t);
                attroff(COLOR_PAIR(EXIT_PAIR));
                break;
            case path:
                attron(COLOR_PAIR(PATH_PAIR));
                t = '#';
                mvprintw(j + 1, i, "%c", t);
                attroff(COLOR_PAIR(PATH_PAIR));
                break;

            case mart:
                attron(COLOR_PAIR(MART_PAIR));
                t = 'M';
                mvprintw(j + 1, i, "%c", t);
                attroff(COLOR_PAIR(MART_PAIR));
                break;
            case center:
                attron(COLOR_PAIR(CENTER_PAIR));
                t = 'C';
                mvprintw(j + 1, i, "%c", t);
                attroff(COLOR_PAIR(CENTER_PAIR));
                break;

            default:
                t = '.';
                mvprintw(j + 1, i, "%c", t);
                break;
            }
            // mvprintw(j + 1, i, "%c", t);
        }
        // printw("\n");
    }
    mvprintw(22, 0, "Current Coords: (%d, %d)\n", Bworld.cur_idx[dim_x] - center_x, Bworld.cur_idx[dim_y] - center_y);
    refresh();
}

void print_MoveList()
{
    int i, j;

    mvprintw(0, 0, "Esc to return.                            ");
    refresh();

    // create white space
    for (j = 3; j < yN - 4; j++)
    {
        for (i = xN / 2 - 32; i < xN / 2 + 32; i++)
        {
            mvprintw(j, i, " ");
        }
    }
    j = 4;
    mvprintw(j++, xN / 2 - 28, "7 or y: Attempt to move PC one cell to the upper left.");
    mvprintw(j++, xN / 2 - 28, "8 or k: Attempt to move PC one cell up.");
    mvprintw(j++, xN / 2 - 28, "9 or u: Attempt to move PC one cell to the upper right.");
    mvprintw(j++, xN / 2 - 28, "6 or l: Attempt to move PC one cell to the right.");
    mvprintw(j++, xN / 2 - 28, "3 or n: Attempt to move PC one cell to the lower right.");
    mvprintw(j++, xN / 2 - 28, "2 or j: Attempt to move PC one cell down.");
    mvprintw(j++, xN / 2 - 28, "1 or b: Attempt to move PC one cell to the lower left.");
    mvprintw(j++, xN / 2 - 28, "4 or h: Attempt to move PC one cell to the left");
    mvprintw(j++, xN / 2 - 28, ">     : Attempt to enter structures.");
    mvprintw(j++, xN / 2 - 28, "<     : Attempt to exit structures.");
    mvprintw(j++, xN / 2 - 28, "5 or space or .: Skips a player's turn.");

    while (getch() != 27)
    {
        mvprintw(0, 0, "Invalid input! Please press 'ESC' to return.");
        refresh();
    }
    mvprintw(0, xN / 2, "                     ");
    mvprintw(yN + 2, 0, "                                              ");
    return;
}

void print_trainerList(map_t *m)
{
    int i, j, index = 0, start = 0, counter;
    // char *s_x, *s_y;
    int32_t key;
    info_t *temp;

    mvprintw(0, 0, "Esc to return.                            ");
    refresh();

    do
    {
        // create white space
        for (j = 3; j < yN - 4; j++)
        {
            for (i = xN / 2 - 14; i < xN / 2 + 15; i++)
            {
                mvprintw(j, i, " ");
            }
        }

        for (j = 4, index = start, counter = 0; index < numTrainer && counter < yN - 9; index++, counter++)
        // while (index < numTrainer && counter < yN - 9)
        {
            temp = m->trainerList[index];

            // counter++;
            mvprintw(j++, xN / 2 - 11, "%c, %d %s and %d %s", trainerCheck(m, temp->pos[dim_x], temp->pos[dim_y]),
                     abs(temp->pos[dim_x] - Bworld.pc.pos[dim_x]),
                     (temp->pos[dim_x] < Bworld.pc.pos[dim_x]) ? "West" : "East",
                     abs(temp->pos[dim_y] - Bworld.pc.pos[dim_y]),
                     (temp->pos[dim_y] < Bworld.pc.pos[dim_y]) ? "North" : "South");
        }

        // while (getch() != 27)
        //{
        key = getch();
        if (key == KEY_DOWN && start + yN - 9 < numTrainer)
            start++;
        else if (key == KEY_UP && start > 0)
            start--;
        else if (key != 27 && key != KEY_DOWN && key != KEY_UP)
            mvprintw(0, 0, "Invalid input! Please press 'ESC' to return, arrow Key to scroll.");
        refresh();
    } while (key != 27);

    // mvprintw(0, xN/2, "                     ");
    // mvprintw(yN + 2, 0, "                                                                         ");
    clear();
}

// void initiateBattle(info_t *t)
// {
//     int i, j;
//     // for (j = 2; j < yN; j++)
//     // {
//     //     for (i = 1; i < xN - 1; i++)
//     //     {
//     //         mvprintw(j, i, " ");
//     //     }
//     // }
//     clear();

//     mvprintw(0, 0, "TRAINER BATTLE!     Press 'ESC' to return.");
//     refresh();

//     j = 2;
//     mvprintw(j++, xN / 2 - 25, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
//     mvprintw(j++, xN / 2 - 25, "@@@@@@@@@@@@@@@@@@@&############@@@@@@@@@@@@@@@@@@");
//     mvprintw(j++, xN / 2 - 25, "@@@@@@@@@@@@@@@#####################@@@@@@@@@@@@@@");
//     mvprintw(j++, xN / 2 - 25, "@@@@@@@@@@@@###########################@@@@@@@@@@@");
//     mvprintw(j++, xN / 2 - 25, "@@@@@@@@@@###############################@@@@@@@@@");
//     mvprintw(j++, xN / 2 - 25, "@@@@@@@@@@###############################@@@@@@@@@");
//     mvprintw(j++, xN / 2 - 25, "@@@@@@@@@###############%%@%%###############@@@@@@@@");
//     mvprintw(j++, xN / 2 - 25, "@@@@@@@@############@@@@@@@@@@@############@@@@@@@");
//     mvprintw(j++, xN / 2 - 25, "@@@@@@@@###########@@@       @@@############@@@@@@");
//     mvprintw(j++, xN / 2 - 25, "@@@@@@@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@@@@@@");
//     mvprintw(j++, xN / 2 - 25, "@@@@@@@/           @@@@     @@@@       ....@@@@@@@");
//     mvprintw(j++, xN / 2 - 25, "@@@@@@@@             @@@@@@@@@        .....@@@@@@@");
//     mvprintw(j++, xN / 2 - 25, "@@@@@@@@@                             ....@@@@@@@@");
//     mvprintw(j++, xN / 2 - 25, "@@@@@@@@@@@         PLACEHOLDER     ....@@@@@@@@@@");
//     mvprintw(j++, xN / 2 - 25, "@@@@@@@@@@@@@                      ...@@@@@@@@@@@@");
//     mvprintw(j++, xN / 2 - 25, "@@@@@@@@@@@@@@@@                 ..@@@@@@@@@@@@@@@");
//     mvprintw(j++, xN / 2 - 25, "@@@@@@@@@@@@@@@@@@@@@@@,   #@@@@@@@@@@@@@@@@@@@@@@");
//     mvprintw(j++, xN / 2 - 25, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
//     mvprintw(j++, xN / 2 - 25, "        Pokemon Trainer Battle Placeholder        ");

//     clear();
//     mvprintw(0, 0, "TRAINER BATTLE!     Press 'ESC' to return.");

//     i = 0, j = 2;
//     while (t->captured[i] != NULL && i < 6)
//     {
//         mvprintw(j++, 1, "%d. %s (Level: %d)", i, t->captured[i]->get_pokemon().identifier.data(), t->captured[i]->get_level());
//         i++;
//     }
//     refresh();

//     int32_t key;
//     while (key != 27)
//     {
//         key = getch();
//         mvprintw(0, 0, "Invalid input! Please press 'ESC' to exit.");
//         refresh();
//     }
//     t->defeated = 1;
//     clear();

//     print_map();
//     refresh();
// }

void centerMart_PLACEHOLDER()
{
    int i, j;
    for (j = 2; j < yN; j++)
    {
        for (i = 1; i < xN - 1; i++)
        {
            mvprintw(j, i, " ");
        }
    }

    mvprintw(0, 0, "Press '<' to return.                            ");
    refresh();

    j = 2;
    mvprintw(j++, xN / 2 - 25, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
    mvprintw(j++, xN / 2 - 25, "@@@@@@@@@@@@@@@@@@@&############@@@@@@@@@@@@@@@@@@");
    mvprintw(j++, xN / 2 - 25, "@@@@@@@@@@@@@@@#####################@@@@@@@@@@@@@@");
    mvprintw(j++, xN / 2 - 25, "@@@@@@@@@@@@###########################@@@@@@@@@@@");
    mvprintw(j++, xN / 2 - 25, "@@@@@@@@@@###############################@@@@@@@@@");
    mvprintw(j++, xN / 2 - 25, "@@@@@@@@@@###############################@@@@@@@@@");
    mvprintw(j++, xN / 2 - 25, "@@@@@@@@@###############%%@%%###############@@@@@@@@");
    mvprintw(j++, xN / 2 - 25, "@@@@@@@@############@@@@@@@@@@@############@@@@@@@");
    mvprintw(j++, xN / 2 - 25, "@@@@@@@@###########@@@       @@@############@@@@@@");
    mvprintw(j++, xN / 2 - 25, "@@@@@@@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@@@@@@");
    mvprintw(j++, xN / 2 - 25, "@@@@@@@/           @@@@     @@@@       ....@@@@@@@");
    mvprintw(j++, xN / 2 - 25, "@@@@@@@@             @@@@@@@@@        .....@@@@@@@");
    mvprintw(j++, xN / 2 - 25, "@@@@@@@@@                             ....@@@@@@@@");
    mvprintw(j++, xN / 2 - 25, "@@@@@@@@@@@         PLACEHOLDER     ....@@@@@@@@@@");
    mvprintw(j++, xN / 2 - 25, "@@@@@@@@@@@@@                      ...@@@@@@@@@@@@");
    mvprintw(j++, xN / 2 - 25, "@@@@@@@@@@@@@@@@                 ..@@@@@@@@@@@@@@@");
    mvprintw(j++, xN / 2 - 25, "@@@@@@@@@@@@@@@@@@@@@@@,   #@@@@@@@@@@@@@@@@@@@@@@");
    mvprintw(j++, xN / 2 - 25, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
    mvprintw(j++, xN / 2 - 25, "        Pokemart and Pokecenter Placeholder       ");
    refresh();
    while (getch() != 60)
    {
        mvprintw(0, 0, "Invalid input! Please press '<' to exit.");
        refresh();
    }
    clear();
}

// spawn pokemon
Pokemon *spawn_pokemon()
{
    int dis = abs(Bworld.cur_idx[dim_y] - center_y) + abs(Bworld.cur_idx[dim_x] - center_x);

    if (dis <= 200)
    {
        return new Pokemon((rand() % ((dis < 3 ? 2 : dis) / 2)) + 1);
    }
    else
    {
        return new Pokemon((rand() % 101 - ((dis - 200) / 2) ? ((dis - 200) / 2) : 1) + ((dis - 200) / 2) ? ((dis - 200) / 2) : 1);
    }
}

void init_Pokemon(info_t *trainer)
{
    int i, count, dis;
    for (i = 0; i < 6; i++)
        trainer->captured[i] = NULL;

    if (trainer->trainer == player)
    {
        trainer->captured[0] = spawn_pokemon();
        trainer->captured[1] = spawn_pokemon();
        trainer->captured[2] = spawn_pokemon();
    }
    else
    {
        dis = abs(Bworld.cur_idx[dim_y] - center_y) + abs(Bworld.cur_idx[dim_x] - center_x);
        count = dis / 40 ? ((dis / 40) < 6 ? dis / 40 : 6) : 1;

        for (i = 0; i < count; i++)
            trainer->captured[i] = spawn_pokemon();

        while (i < 6 && rand() % 10 < 6)
            trainer->captured[i++] = spawn_pokemon();
    }
}

void print_pokemon_stats(Pokemon *p)
{
    int i = 0, j = 0;

    mvprintw(1, 0, "===============================================================================");
    mvprintw(21, 0, "===============================================================================");
    for (j = 2; j < yN; j++)
    {
        mvprintw(j, 0, "                                                                               ");
        mvprintw(j, 0, "|");
        mvprintw(j, xN - 1, "|");
    }

    i = 0, j = 2;
    std::string name = p->get_pokemon().identifier;
    toupper(name[0]);

    mvprintw(j++, xN / 2 - 25, "Name:            %s", name.data());
    mvprintw(j++, xN / 2 - 25, "Gender:          %s", p->get_gender_string());
    mvprintw(j++, xN / 2 - 25, "Level:           %d", p->get_level());
    mvprintw(j++, xN / 2 - 25, "Health Point:    %d/%d", p->get_currHP(), p->get_maxHP());
    mvprintw(j++, xN / 2 - 25, "Attack:          %d", p->get_atk());
    mvprintw(j++, xN / 2 - 25, "Special Attack:  %d", p->get_spatk());
    mvprintw(j++, xN / 2 - 25, "Defence:         %d", p->get_def());
    mvprintw(j++, xN / 2 - 25, "Special Defence: %d", p->get_spdef());
    mvprintw(j++, xN / 2 - 25, "Speed:           %d", p->get_speed());
    // mvprintw(j++, xN / 2 - 25, "Accuracy:       %d", p->accuracy);
    // mvprintw(j++, xN / 2 - 25, "Evasion:        %d", p->evasion);
    mvprintw(j++, xN / 2 - 25, "Shiny:           %s", p->is_shiny() ? "SHINY!!!!!!!" : "False");

    mvprintw(j++, xN / 2 - 25, "Move Set:");
    for (i = 0; i < 4 && p->get_move(i) != NULL; i++)
        mvprintw(j++, xN / 2 - 18, "- %s (%d)", p->get_move(i)->identifier.data(), p->get_move(i)->id);

    refresh();
}

void print_pokemon_battle(Pokemon *player, Pokemon *opponent)
{
    int j = 0;

    mvprintw(1, 0, "================================================================================");
    mvprintw(21, 0, "================================================================================");
    for (j = 2; j < yN; j++)
    {
        mvprintw(j, 0, "                                                                               ");
        mvprintw(j, 0, "|");
        mvprintw(j, xN - 1, "|");
        mvprintw(j, xN / 2, "||");
    }

    j = 5;
    mvprintw(j, 3, "Name:   %s", player->get_pokemon().identifier.data());
    mvprintw(j++, xN / 2 + 3, "Name:   %s", opponent->get_pokemon().identifier.data());

    mvprintw(j, 3, "Gender: %s", player->get_gender_string());
    mvprintw(j++, xN / 2 + 3, "Gender: %s", opponent->get_gender_string());

    mvprintw(j, 3, "Level:  %d", player->get_level());
    mvprintw(j++, xN / 2 + 3, "Level:  %d", opponent->get_level());

    mvprintw(j, 3, "HP:     %d/%d", player->get_currHP(), player->get_maxHP());
    mvprintw(j++, xN / 2 + 3, "HP:     %d/%d", opponent->get_currHP(), opponent->get_maxHP());
    refresh();
}

// shows pokemon choices on screen, including stats
void print_pokemon_list()
{
    int j;
    int x[6] = {3, xN / 2 + 3, 3, xN / 2 + 3, 3, xN / 2 + 3};
    int y[6] = {3, 3, yN / 3 + 3, yN / 3 + 3, yN * 2 / 3 + 3, yN * 2 / 3 + 3};

    mvprintw(1, 0, "================================================================================");
    mvprintw(yN, 0, "================================================================================");
    for (j = 2; j < yN; j++)
    {
        mvprintw(j, 0, "                                                                               ");
        mvprintw(j, 0, "|");
        mvprintw(j, xN - 1, "|");
        mvprintw(j, xN / 2, "||");
    }
    mvprintw(yN / 3 + 1, 0, "================================================================================");
    mvprintw(yN * 2 / 3 + 1, 0, "================================================================================");

    for (int temp = 0; temp < 6; temp++)
    {
        mvprintw(y[temp], x[temp], "Name:   %s", (Bworld.pc.captured[temp] != NULL) ? Bworld.pc.captured[temp]->get_pokemon().identifier.data() : "---");
        mvprintw(y[temp] + 1, x[temp], "Gender: %s", (Bworld.pc.captured[temp] != NULL) ? Bworld.pc.captured[temp]->get_gender_string() : "---");
        mvprintw(y[temp] + 2, x[temp], "Level:  %d", (Bworld.pc.captured[temp] != NULL) ? Bworld.pc.captured[temp]->get_level() : 0);
        mvprintw(y[temp] + 3, x[temp], "HP:     %d/%d", (Bworld.pc.captured[temp] != NULL) ? Bworld.pc.captured[temp]->get_currHP() : 0, (Bworld.pc.captured[temp] != NULL) ? Bworld.pc.captured[temp]->get_maxHP() : 0);
    }
    refresh();
}

// items
void print_bag()
{
    int j;

    mvprintw(1, 0, "================================================================================");
    mvprintw(yN, 0, "================================================================================");
    for (j = 2; j < yN; j++)
    {
        mvprintw(j, 0, "                                                                               ");
        mvprintw(j, 0, "|");
        mvprintw(j, xN - 1, "|");
        mvprintw(j, xN / 3, "||");
    }

    j = 3;
    mvprintw(j, 3, "1. Pokeball (x%d)", Bworld.pc.item->get_num_pokeball());
    mvprintw(j + 3, 3, "2. Revives (x%d)", Bworld.pc.item->get_num_revive());
    mvprintw(j + 6, 3, "3. Potions (x%d)", Bworld.pc.item->get_num_potions());

    mvprintw(j, xN / 3 + 3, "- Used to capture pokemon.");
    mvprintw(j + 3, xN / 3 + 3, "- Heals selected Pokemon for 20HP.");
    mvprintw(j + 6, xN / 3 + 3, "- Revives selected Pokemon if it was knocked out.");

    refresh();
}

// checks if trainer whited-out
bool is_white_out(info_t *trainer)
{
    for (int i = 0; i < 6 && trainer->captured[i] != NULL; i++)
    {
        if (trainer->captured[i]->get_currHP() != 0)
            return false;
    }
    return true;
}

void print_pokemon_indicator(info_t *trainer)
{
    mvprintw(3, 3, "%c %c %c %c %c %c",
             (Bworld.pc.captured[0] != NULL) ? (Bworld.pc.captured[0]->get_currHP() != 0 ? 'o' : 'x') : '-',
             (Bworld.pc.captured[1] != NULL) ? (Bworld.pc.captured[1]->get_currHP() != 0 ? 'o' : 'x') : '-',
             (Bworld.pc.captured[2] != NULL) ? (Bworld.pc.captured[2]->get_currHP() != 0 ? 'o' : 'x') : '-',
             (Bworld.pc.captured[3] != NULL) ? (Bworld.pc.captured[3]->get_currHP() != 0 ? 'o' : 'x') : '-',
             (Bworld.pc.captured[4] != NULL) ? (Bworld.pc.captured[4]->get_currHP() != 0 ? 'o' : 'x') : '-',
             (Bworld.pc.captured[5] != NULL) ? (Bworld.pc.captured[5]->get_currHP() != 0 ? 'o' : 'x') : '-');

    if (trainer != NULL)
    {
        mvprintw(3, xN / 2 + 3, "%c %c %c %c %c %c",
                 (trainer->captured[0] != NULL) ? (trainer->captured[0]->get_currHP() != 0 ? 'o' : 'x') : '-',
                 (trainer->captured[1] != NULL) ? (trainer->captured[1]->get_currHP() != 0 ? 'o' : 'x') : '-',
                 (trainer->captured[2] != NULL) ? (trainer->captured[2]->get_currHP() != 0 ? 'o' : 'x') : '-',
                 (trainer->captured[3] != NULL) ? (trainer->captured[3]->get_currHP() != 0 ? 'o' : 'x') : '-',
                 (trainer->captured[4] != NULL) ? (trainer->captured[4]->get_currHP() != 0 ? 'o' : 'x') : '-',
                 (trainer->captured[5] != NULL) ? (trainer->captured[5]->get_currHP() != 0 ? 'o' : 'x') : '-');
    }

    refresh();
}

Pokemon *swap_pokemon(info_t *trainer, Pokemon *sel, bool force_swap)
{
    int i;
    if (trainer->trainer == player)
    {
        do
        {
            mvprintw(22, 0, "Which pokemon to swap to next:");
            if (!force_swap)
                mvprintw(22, xN - 22, "Press '0' to return.");

            mvprintw(23, 0, "1.%s  2.%s   3.%s   4.%s   5.%s   6.%s",
                     (trainer->captured[0] != NULL) ? trainer->captured[0]->get_pokemon().identifier.data() : "---",
                     (trainer->captured[1] != NULL) ? trainer->captured[1]->get_pokemon().identifier.data() : "---",
                     (trainer->captured[2] != NULL) ? trainer->captured[2]->get_pokemon().identifier.data() : "---",
                     (trainer->captured[3] != NULL) ? trainer->captured[3]->get_pokemon().identifier.data() : "---",
                     (trainer->captured[4] != NULL) ? trainer->captured[4]->get_pokemon().identifier.data() : "---",
                     (trainer->captured[5] != NULL) ? trainer->captured[5]->get_pokemon().identifier.data() : "---");
            print_pokemon_list();

            i = getch() - 48;
            if (!force_swap && i == 0)
                return sel;
            mvprintw(0, 0, "                                                                               ");
            if (i < 0 || i > 6)
            {
                mvprintw(0, 0, "Invalid input! Please try again.");
                refresh();
                continue;
            }
            if (trainer->captured[i - 1] == NULL)
            {
                mvprintw(0, 0, "Pokemon does not exist!");
                refresh();
                continue;
            }
            if (trainer->captured[i - 1]->get_currHP() == 0)
            {
                mvprintw(0, 0, "%s is knocked out! Please select a different pokemon.", trainer->captured[i - 1]->get_pokemon().identifier.data());
                refresh();
                continue;
            }
            break;
        } while (true);
        mvprintw(22, 0, "                                                                               ");
        mvprintw(22, 0, "%s! I choose you!", trainer->captured[i - 1]->get_pokemon().identifier.data());
        refresh();
        return trainer->captured[i - 1];
    }
    else
    {
        for (i = 0; i < 6 && trainer->captured[i] != NULL; i++)
            if (trainer->captured[i]->get_currHP() != 0)
                return trainer->captured[i];
    }
    return NULL;
}

void move_selected(Pokemon *attacker, Pokemon *opposer, const moves_db *move_used)
{
    mvprintw(22, 0, "%s used %s!                                           ", attacker->get_pokemon().identifier.data(), move_used->identifier.data());
    mvprintw(23, 0, "                                                                               ");
    refresh();

    // implement missing
    if (rand() % 100 < move_used->accuracy)
    {
        double critical = (rand() % 255 < (stats_list[attacker->get_pokemon_index() * 6 - 1].base_stat / 2)) ? 1.5 : 1.0;
        double dmg = (((2.0 * attacker->get_level() / 5.0 + 2) * (move_used->power ? move_used->power : 2) * attacker->get_atk() / opposer->get_def()) / 50 + 2) * critical * ((rand() % (100 - 85) + 85) / 100.0) * (attacker->is_same_type(move_used) ? 1.5 : 1.0);
        opposer->set_currHP(opposer->get_currHP() - dmg);
        mvprintw(22, xN / 2 - attacker->get_pokemon().identifier.length(), "%s", critical == 1.5 ? "It was a critical hit!" : "");
        mvprintw(23, xN - 28, "Press ANY key to continue.");
        refresh();
    }
    else
    {
        mvprintw(23, 0, "%s missed!", move_used->identifier.data());
        refresh();
    }
}

void initiate_trainer_pokemon_battle(info_t *trainer)
{
    int i, j, action, priority;
    bool battleEnd = false, back = false,consumed_turn = false;
    const moves_db *pc_selected_move, *rand_selected_move;
    Pokemon *selected_pokemon, *p = trainer->captured[0];
    for (i = 0; i < 6; i++)
    {
        if (Bworld.pc.captured[i] != NULL && Bworld.pc.captured[i]->get_currHP() != 0)
        {
            selected_pokemon = Bworld.pc.captured[i];
            break;
        }
        if (i == 5)
        {
            mvprintw(0, 0, "Player has no available pokemon! Please revive and heal! Any key to cont.");
            getch();
            return;
        }
    }

    for (j = 2; j < yN; j++)
    {
        for (i = 1; i < xN - 1; i++)
        {
            mvprintw(j, i, " ");
        }
    }

    std::string name = p->get_pokemon().identifier.data();
    toupper(name[0]);
    mvprintw(0, 0, "                             ");
    mvprintw(0, 0, "Opponent choose %s!", name.data());

    while (!battleEnd)
    {
        // print_pokemon_stats(p);
        print_pokemon_battle(selected_pokemon, p);
        print_pokemon_indicator(trainer);
        mvprintw(22, 0, "Choose your action:                                                            ");
        mvprintw(23, 0, "                                                                               ");

        mvprintw(23, 0, "1. Battle      2. Pokemon      3. Bag      4. Run");
        refresh();
        back = false;
        consumed_turn = false;
        switch (getch())
        {
        // Battle
        case '1':
            while (!back)
            {
                const char *s1 = (selected_pokemon->get_move(0) != NULL) ? selected_pokemon->get_move(0)->identifier.data() : "---";
                const char *s2 = (selected_pokemon->get_move(1) != NULL) ? selected_pokemon->get_move(1)->identifier.data() : "---";
                const char *s3 = (selected_pokemon->get_move(2) != NULL) ? selected_pokemon->get_move(2)->identifier.data() : "---";
                const char *s4 = (selected_pokemon->get_move(3) != NULL) ? selected_pokemon->get_move(3)->identifier.data() : "---";
                s1 = s1;
                s2 = s2;
                s3 = s3;
                s4 = s4;
                mvprintw(22, 0, "You've chosen to battle!                                     ");
                mvprintw(23, 0, "1. %s      2. %s      3. %s      4. %s      ",
                         (selected_pokemon->get_move(0) != NULL) ? selected_pokemon->get_move(0)->identifier.data() : "---",
                         (selected_pokemon->get_move(1) != NULL) ? selected_pokemon->get_move(1)->identifier.data() : "---",
                         (selected_pokemon->get_move(2) != NULL) ? selected_pokemon->get_move(2)->identifier.data() : "---",
                         (selected_pokemon->get_move(3) != NULL) ? selected_pokemon->get_move(3)->identifier.data() : "---");
                mvprintw(22, xN - 22, "Press '0' to return.");
                mvprintw(0, 0, "                                                                               ");
                refresh();

                mvprintw(0, 0, "                                                                               ");
                switch (action = getch())
                {
                case 'q':
                case '0':
                    back = true;
                    break;

                case '1':
                case '2':
                case '3':
                case '4':
                    if (selected_pokemon->get_move(action - 49) != NULL)
                    {
                        pc_selected_move = selected_pokemon->get_move(action - 49);
                        priority = pc_selected_move->priority;
                        back = true;
                        consumed_turn = true;
                    }
                    else
                    {
                        mvprintw(0, 0, "Selection %d does not exist. Please try again.", action - 48);
                        back = false;
                        refresh();
                    }
                    break;
                default:
                    back = false;
                    continue;
                }
            }
            mvprintw(22, 0, "                                                                               ");
            refresh();
            break;

        // Pokemon
        case '2':
            selected_pokemon = swap_pokemon(&Bworld.pc, selected_pokemon, false);
            continue;

        // Bag
        case '3':
            while (!back)
            {
                mvprintw(0, 0, "                                                                               ");

                print_bag();
                mvprintw(22, 0, "                                                                               ");
                mvprintw(23, 0, "                                                                               ");

                mvprintw(22, 0, "You've opened your bag!                                    ");
                mvprintw(23, 0, "1. Pokeball        2. Revive      3. Potion           0. Back");
                refresh();

                switch (action = getch())
                {
                case 'q':
                case '0':
                    back = true;
                    break;

                // Pokeball
                case '1':
                    mvprintw(0, 0, "Cannot capture trainer pokemons. Press ANY key to cont.");
                    refresh();
                    getch();
                    break;

                // Revive
                case '2':
                    if (Bworld.pc.item->get_num_revive() == 0)
                    {
                        mvprintw(0, 0, "You do not own any Revive! Any key to cont.");
                        refresh();
                        getch();
                        break;
                    }

                    print_pokemon_list();
                    do
                    {
                        mvprintw(22, 0, "Which pokemon to revive:");
                        mvprintw(23, 0, "1.%s  2.%s   3.%s   4.%s   5.%s   6.%s",
                                 (Bworld.pc.captured[0] != NULL) ? Bworld.pc.captured[0]->get_pokemon().identifier.data() : "---",
                                 (Bworld.pc.captured[1] != NULL) ? Bworld.pc.captured[1]->get_pokemon().identifier.data() : "---",
                                 (Bworld.pc.captured[2] != NULL) ? Bworld.pc.captured[2]->get_pokemon().identifier.data() : "---",
                                 (Bworld.pc.captured[3] != NULL) ? Bworld.pc.captured[3]->get_pokemon().identifier.data() : "---",
                                 (Bworld.pc.captured[4] != NULL) ? Bworld.pc.captured[4]->get_pokemon().identifier.data() : "---",
                                 (Bworld.pc.captured[5] != NULL) ? Bworld.pc.captured[5]->get_pokemon().identifier.data() : "---");

                        mvprintw(22, xN - 22, "Press '0' to return.");
                        mvprintw(0, 0, "                                                                               ");
                        refresh();

                        i = getch() - 48;

                        mvprintw(22, 0, "                                                                               ");
                        mvprintw(23, 0, "                                                                               ");

                        if (i == 0)
                            break;

                        if (i < 0 || i > 6)
                        {
                            mvprintw(0, 0, "Invalid input! Please try again.");
                            refresh();
                            continue;
                        }
                        if (Bworld.pc.captured[i - 1] == NULL)
                        {
                            mvprintw(0, 0, "Pokemon does not exist!");
                            refresh();
                            continue;
                        }
                        if (Bworld.pc.captured[i - 1]->get_currHP() > 0)
                        {
                            mvprintw(22, 0, "%s is not knocked out! Revive has no effect!", Bworld.pc.captured[i - 1]->get_pokemon().identifier.data());
                            mvprintw(23, xN - 28, "Press ANY key to continue.");
                            refresh();
                            getch();
                            break;
                        }
                        if (Bworld.pc.captured[i - 1]->get_currHP() == 0)
                        {
                            mvprintw(22, 0, "%s is revived!                                        ", Bworld.pc.captured[i - 1]->get_pokemon().identifier.data());
                            mvprintw(23, xN - 28, "Press ANY key to continue.");
                            refresh();

                            Bworld.pc.captured[i - 1]->set_currHP(Bworld.pc.captured[i - 1]->get_maxHP() / 2);
                            Bworld.pc.item->set_num_revive(Bworld.pc.item->get_num_revive() - 1);
                            consumed_turn = true;

                            getch();
                            break;
                        }
                        continue;
                    } while (true);

                    priority = 10;
                    back = true;
                    break;

                // Potion
                case '3':
                    if (Bworld.pc.item->get_num_potions() == 0)
                    {
                        mvprintw(0, 0, "You do not own any Potions! Any key to cont.");
                        refresh();
                        getch();
                        break;
                    }

                    print_pokemon_list();
                    do
                    {
                        mvprintw(22, 0, "Which pokemon to heal:");
                        mvprintw(23, 0, "1.%s  2.%s   3.%s   4.%s   5.%s   6.%s",
                                 (Bworld.pc.captured[0] != NULL) ? Bworld.pc.captured[0]->get_pokemon().identifier.data() : "---",
                                 (Bworld.pc.captured[1] != NULL) ? Bworld.pc.captured[1]->get_pokemon().identifier.data() : "---",
                                 (Bworld.pc.captured[2] != NULL) ? Bworld.pc.captured[2]->get_pokemon().identifier.data() : "---",
                                 (Bworld.pc.captured[3] != NULL) ? Bworld.pc.captured[3]->get_pokemon().identifier.data() : "---",
                                 (Bworld.pc.captured[4] != NULL) ? Bworld.pc.captured[4]->get_pokemon().identifier.data() : "---",
                                 (Bworld.pc.captured[5] != NULL) ? Bworld.pc.captured[5]->get_pokemon().identifier.data() : "---");

                        mvprintw(22, xN - 22, "Press '0' to return.");

                        mvprintw(0, 0, "                                                                               ");
                        refresh();

                        i = getch() - 48;

                        mvprintw(22, 0, "                                                                               ");
                        mvprintw(23, 0, "                                                                               ");

                        if (i == 0)
                            break;

                        mvprintw(22, 0, "                                                                               ");
                        if (i < 0 || i > 6)
                        {
                            mvprintw(0, 0, "Invalid input! Please try again.");
                            refresh();
                            continue;
                        }
                        if (Bworld.pc.captured[i - 1] == NULL)
                        {
                            mvprintw(0, 0, "Pokemon does not exist!");
                            refresh();
                            continue;
                        }
                        if (Bworld.pc.captured[i - 1]->get_currHP() == 0)
                        {
                            mvprintw(22, 0, "%s is knocked out! Potion has no effect!", Bworld.pc.captured[i - 1]->get_pokemon().identifier.data());
                            mvprintw(23, xN - 28, "Press ANY key to continue.");
                            refresh();
                            getch();
                            break;
                        }
                        if (Bworld.pc.captured[i - 1]->get_currHP() > 0)
                        {
                            // mvprintw(22, 0, "                                                                               ");
                            // mvprintw(23, 0, "                                                                               ");

                            mvprintw(22, 0, "%s is healed!                                                      ", Bworld.pc.captured[i - 1]->get_pokemon().identifier.data());
                            mvprintw(23, xN - 28, "Press ANY key to continue.");
                            refresh();

                            Bworld.pc.captured[i - 1]->set_currHP(Bworld.pc.captured[i - 1]->get_currHP() + 20);
                            Bworld.pc.item->set_num_potions(Bworld.pc.item->get_num_potions() - 1);
                            consumed_turn = true;

                            getch();
                            break;
                        }
                        continue;
                    } while (true);

                    priority = 10;
                    back = true;
                    break;
                default:
                    back = false;
                    continue;
                }
            }
            // changed
            break;

        // Run
        case '4':
            mvprintw(22, 0, "Unable to escape trainer battles! Press ANY key to continue.");
            refresh();
            getch();
            continue;

        default:
            continue;
        }

        // ends battle if manage to run
        if (battleEnd)
            break;

        if (!consumed_turn)
        continue;

        // Select move for wild pokemon
        rand_selected_move = p->get_move(rand() % p->get_num_moves());

        if (priority >= 10)
        {
            if (!battleEnd && p->get_currHP() > 0)
            {
                move_selected(p, selected_pokemon, rand_selected_move);
                print_pokemon_battle(selected_pokemon, p);
                print_pokemon_indicator(trainer);
                getch();

                if (selected_pokemon->get_currHP() <= 0)
                {
                    // TODO needs fixing
                    mvprintw(23, 0, "%s %s", selected_pokemon->get_pokemon().identifier.data(), "was knocked out!");
                    mvprintw(23, xN - 28, "Press ANY key to continue.");
                    refresh();
                    getch();

                    mvprintw(22, 0, "                                                                               ");
                    mvprintw(23, 0, "                                                                               ");
                    if (is_white_out(&Bworld.pc))
                    {
                        mvprintw(22, 0, "Player whited-out! Please heal using potions and revives!");
                        mvprintw(23, xN - 28, "Press ANY key to continue.");
                        refresh();
                        Bworld.pc.defeated = true;
                        battleEnd = true;
                        getch();
                    }
                    else
                    {
                        selected_pokemon = swap_pokemon(&Bworld.pc, selected_pokemon, true);
                        continue;
                    }
                }
            }
        }
        else
        {
            // player's pokemon moves
            if (priority > rand_selected_move->priority || (priority == rand_selected_move->priority && selected_pokemon->get_speed() >= p->get_speed()))
            {
                move_selected(selected_pokemon, p, pc_selected_move);
                print_pokemon_battle(selected_pokemon, p);
                print_pokemon_indicator(trainer);
                getch();

                if (p->get_currHP() <= 0)
                {
                    // TODO needs fixing
                    mvprintw(23, 0, "%s %s", p->get_pokemon().identifier.data(), "was knocked out!");
                    mvprintw(23, xN - 28, "Press ANY key to continue.");
                    refresh();
                    getch();

                    mvprintw(22, 0, "                                                                               ");
                    mvprintw(23, 0, "                                                                               ");

                    if (is_white_out(trainer))
                    {
                        battleEnd = true;
                        trainer->defeated = true;
                        mvprintw(22, 0, "Trainer ran out of pokemon. They are defeated!");
                        mvprintw(23, xN - 28, "Press ANY key to continue.");
                        refresh();
                        getch();
                    }
                    else
                    {
                        p = swap_pokemon(trainer, p, true);
                        mvprintw(22, 0, "Trainer choose %s to battle!", p->get_pokemon().identifier.data());
                        mvprintw(23, xN - 28, "Press ANY key to continue.");
                        refresh();
                        getch();
                        continue;
                    }
                }
            }

            // wild moves
            if (!battleEnd && p->get_currHP() > 0)
            {
                move_selected(p, selected_pokemon, rand_selected_move);
                print_pokemon_battle(selected_pokemon, p);
                print_pokemon_indicator(trainer);
                getch();

                if (selected_pokemon->get_currHP() <= 0)
                {
                    // TODO needs fixing
                    mvprintw(23, 0, "%s %s", selected_pokemon->get_pokemon().identifier.data(), "was knocked out!");
                    mvprintw(23, xN - 28, "Press ANY key to continue.");
                    refresh();
                    getch();

                    mvprintw(22, 0, "                                                                               ");
                    mvprintw(23, 0, "                                                                               ");
                    if (is_white_out(&Bworld.pc))
                    {
                        mvprintw(22, 0, "Player whited-out! Please heal using potions and revives!");
                        mvprintw(23, xN - 28, "Press ANY key to continue.");
                        refresh();
                        Bworld.pc.defeated = true;
                        battleEnd = true;
                        getch();
                    }
                    else
                    {
                        selected_pokemon = swap_pokemon(&Bworld.pc, selected_pokemon, true);
                        continue;
                    }
                }

                if (!battleEnd && (priority < rand_selected_move->priority || (priority == rand_selected_move->priority && selected_pokemon->get_speed() < p->get_speed())))
                {
                    move_selected(selected_pokemon, p, pc_selected_move);
                    print_pokemon_battle(selected_pokemon, p);
                    print_pokemon_indicator(trainer);
                    getch();

                    if (p->get_currHP() <= 0)
                    {
                        // TODO needs fixing
                        mvprintw(23, 0, "%s %s", p->get_pokemon().identifier.data(), "was knocked out!");
                        mvprintw(23, xN - 28, "Press ANY key to continue.");
                        refresh();
                        getch();

                        mvprintw(22, 0, "                                                                               ");
                        mvprintw(23, 0, "                                                                               ");

                        if (is_white_out(trainer))
                        {
                            battleEnd = true;
                            trainer->defeated = true;
                            mvprintw(23, 0, "Trainer ran out of pokemon. They are defeated!");
                            mvprintw(23, xN - 28, "Press ANY key to continue.");
                            refresh();
                            getch();
                        }
                        else
                        {
                            p = swap_pokemon(trainer, p, true);
                            mvprintw(23, 0, "Trainer choose %s to battle!", p->get_pokemon().identifier.data());
                            mvprintw(23, xN - 28, "Press ANY key to continue.");
                            refresh();
                            getch();
                            continue;
                        }
                    }
                }
            }
        }
    }
    clear();

    print_map();
    refresh();
}

void initiate_wild_pokemon_battle(Pokemon *p)
{
    int i, j, action, priority, attempts = 0;
    bool battleEnd = false, back = false, consumed_turn = false;
    const moves_db *pc_selected_move, *rand_selected_move;
    Pokemon *selected_pokemon;
    for (i = 0; i < 6; i++)
    {
        if (Bworld.pc.captured[i] != NULL && Bworld.pc.captured[i]->get_currHP() != 0)
        {
            selected_pokemon = Bworld.pc.captured[i];
            break;
        }
        if (i == 5)
        {
            mvprintw(0, 0, "Player has no available pokemon! Please revive and heal! Any key to cont.");
            getch();
            return;
        }
    }

    for (j = 2; j < yN; j++)
    {
        for (i = 1; i < xN - 1; i++)
        {
            mvprintw(j, i, " ");
        }
    }

    std::string name = p->get_pokemon().identifier;
    toupper(name[0]);
    mvprintw(0, 0, "                             ");
    mvprintw(0, 0, "Wild %s Appear!", name.data());

    while (!battleEnd)
    {
        // print_pokemon_stats(p);
        print_pokemon_battle(selected_pokemon, p);
        print_pokemon_indicator(NULL);

        mvprintw(22, 0, "Choose your action:                                                            ");
        mvprintw(23, 0, "                                                                               ");

        mvprintw(23, 0, "1. Battle      2. Pokemon      3. Bag      4. Run");
        refresh();
        back = false;
        consumed_turn = false;
        switch (getch())
        {
        // Battle
        case '1':
            while (!back)
            {
                const char *s1 = (selected_pokemon->get_move(0) != NULL) ? selected_pokemon->get_move(0)->identifier.data() : "---";
                const char *s2 = (selected_pokemon->get_move(1) != NULL) ? selected_pokemon->get_move(1)->identifier.data() : "---";
                const char *s3 = (selected_pokemon->get_move(2) != NULL) ? selected_pokemon->get_move(2)->identifier.data() : "---";
                const char *s4 = (selected_pokemon->get_move(3) != NULL) ? selected_pokemon->get_move(3)->identifier.data() : "---";
                s1 = s1;
                s2 = s2;
                s3 = s3;
                s4 = s4;
                mvprintw(22, 0, "You've chosen to battle!                                     ");
                mvprintw(23, 0, "1. %s      2. %s      3. %s      4. %s      ",
                         (selected_pokemon->get_move(0) != NULL) ? selected_pokemon->get_move(0)->identifier.data() : "---",
                         (selected_pokemon->get_move(1) != NULL) ? selected_pokemon->get_move(1)->identifier.data() : "---",
                         (selected_pokemon->get_move(2) != NULL) ? selected_pokemon->get_move(2)->identifier.data() : "---",
                         (selected_pokemon->get_move(3) != NULL) ? selected_pokemon->get_move(3)->identifier.data() : "---");
                mvprintw(22, xN - 22, "Press '0' to return.");
                mvprintw(0, 0, "                                                                               ");
                refresh();
                mvprintw(0, 0, "                                                                               ");
                switch (action = getch())
                {
                case 'q':
                case '0':
                    back = true;
                    break;

                case '1':
                case '2':
                case '3':
                case '4':
                    if (selected_pokemon->get_move(action - 49) != NULL)
                    {
                        pc_selected_move = selected_pokemon->get_move(action - 49);
                        priority = pc_selected_move->priority;
                        back = true;
                        consumed_turn = true;
                    }
                    else
                    {
                        mvprintw(0, 0, "Selection %d does not exist. Please try again.", action - 48);
                        back = false;
                        refresh();
                    }
                    break;
                default:
                    back = false;
                    continue;
                }
            }
            mvprintw(22, 0, "                                                                               ");
            refresh();
            break;

        // Pokemon
        case '2':
            selected_pokemon = swap_pokemon(&Bworld.pc, selected_pokemon, false);
            continue;

        // Bag
        case '3':
            while (!back)
            {
                mvprintw(0, 0, "                                                                               ");

                print_bag();
                mvprintw(22, 0, "                                                                               ");
                mvprintw(23, 0, "                                                                               ");

                mvprintw(22, 0, "You've opened your bag!                                    ");
                mvprintw(23, 0, "1. Pokeball        2. Revive      3. Potion           0. Back");
                refresh();

                switch (action = getch())
                {
                case 'q':
                case '0':
                    back = true;
                    break;

                // Pokeball
                case '1':
                    if (Bworld.pc.item->get_num_pokeball() == 0)
                    {
                        mvprintw(0, 0, "You do not own any Pokeball! Any key to cont.");
                        refresh();
                        getch();
                        break;
                    }
                    mvprintw(22, 0, "                                                                               ");
                    mvprintw(23, 0, "                                                                               ");

                    mvprintw(22, 0, "You used a Pokeball!");
                    mvprintw(23, xN - 28, "Press ANY key to continue.");
                    getch();

                    print_pokemon_battle(selected_pokemon, p);
                    print_pokemon_indicator(NULL);
                    if (Bworld.pc.captured[5] != NULL)
                    {
                        mvprintw(23, 0, "                                                                               ");
                        mvprintw(23, 0, "%s broke free! Aargh! Almost had it!", p->get_pokemon().identifier.data());
                    }
                    else
                    {
                        mvprintw(23, 0, "                                                                               ");
                        mvprintw(23, 0, "Gotcha! %s was successfully caught!", p->get_pokemon().identifier.data());

                        int temp = 5;
                        while (temp >= 0 && Bworld.pc.captured[temp] == NULL)
                            temp--;
                        Bworld.pc.captured[temp + 1] = p;
                        battleEnd = true;
                    }
                    mvprintw(23, xN - 28, "Press ANY key to continue.");
                    getch();
                    Bworld.pc.item->set_num_pokeball(Bworld.pc.item->get_num_pokeball() - 1);
                    priority = 10;
                    back = true;
                    consumed_turn = true;
                    break;

                // Revive
                case '2':
                    if (Bworld.pc.item->get_num_revive() == 0)
                    {
                        mvprintw(0, 0, "You do not own any Revive! Any key to cont.");
                        refresh();
                        getch();
                        break;
                    }

                    print_pokemon_list();
                    do
                    {
                        mvprintw(22, 0, "Which pokemon to revive:");
                        mvprintw(23, 0, "1.%s  2.%s   3.%s   4.%s   5.%s   6.%s",
                                 (Bworld.pc.captured[0] != NULL) ? Bworld.pc.captured[0]->get_pokemon().identifier.data() : "---",
                                 (Bworld.pc.captured[1] != NULL) ? Bworld.pc.captured[1]->get_pokemon().identifier.data() : "---",
                                 (Bworld.pc.captured[2] != NULL) ? Bworld.pc.captured[2]->get_pokemon().identifier.data() : "---",
                                 (Bworld.pc.captured[3] != NULL) ? Bworld.pc.captured[3]->get_pokemon().identifier.data() : "---",
                                 (Bworld.pc.captured[4] != NULL) ? Bworld.pc.captured[4]->get_pokemon().identifier.data() : "---",
                                 (Bworld.pc.captured[5] != NULL) ? Bworld.pc.captured[5]->get_pokemon().identifier.data() : "---");

                        mvprintw(22, xN - 22, "Press '0' to return.");
                        mvprintw(0, 0, "                                                                               ");
                        refresh();

                        i = getch() - 48;

                        mvprintw(22, 0, "                                                                               ");
                        mvprintw(23, 0, "                                                                               ");

                        if (i == 0)
                            break;

                        if (i < 0 || i > 6)
                        {
                            mvprintw(0, 0, "Invalid input! Please try again.");
                            refresh();
                            continue;
                        }
                        if (Bworld.pc.captured[i - 1] == NULL)
                        {
                            mvprintw(0, 0, "Pokemon does not exist!");
                            refresh();
                            continue;
                        }
                        if (Bworld.pc.captured[i - 1]->get_currHP() > 0)
                        {
                            mvprintw(22, 0, "%s is not knocked out! Revive has no effect!", Bworld.pc.captured[i - 1]->get_pokemon().identifier.data());
                            mvprintw(23, xN - 28, "Press ANY key to continue.");
                            refresh();
                            getch();
                            break;
                        }
                        if (Bworld.pc.captured[i - 1]->get_currHP() == 0)
                        {
                            mvprintw(22, 0, "%s is revived!                                        ", Bworld.pc.captured[i - 1]->get_pokemon().identifier.data());
                            mvprintw(23, xN - 28, "Press ANY key to continue.");
                            refresh();

                            Bworld.pc.captured[i - 1]->set_currHP(Bworld.pc.captured[i - 1]->get_maxHP() / 2);
                            Bworld.pc.item->set_num_revive(Bworld.pc.item->get_num_revive() - 1);
                            
                            consumed_turn = true;
                            getch();
                            break;
                        }
                        continue;
                    } while (true);

                    priority = 10;
                    back = true;
                    break;

                // Potion
                case '3':
                    if (Bworld.pc.item->get_num_potions() == 0)
                    {
                        mvprintw(0, 0, "You do not own any Potions! Any key to cont.");
                        refresh();
                        getch();
                        break;
                    }

                    print_pokemon_list();
                    do
                    {
                        mvprintw(22, 0, "Which pokemon to heal:");
                        mvprintw(23, 0, "1.%s  2.%s   3.%s   4.%s   5.%s   6.%s",
                                 (Bworld.pc.captured[0] != NULL) ? Bworld.pc.captured[0]->get_pokemon().identifier.data() : "---",
                                 (Bworld.pc.captured[1] != NULL) ? Bworld.pc.captured[1]->get_pokemon().identifier.data() : "---",
                                 (Bworld.pc.captured[2] != NULL) ? Bworld.pc.captured[2]->get_pokemon().identifier.data() : "---",
                                 (Bworld.pc.captured[3] != NULL) ? Bworld.pc.captured[3]->get_pokemon().identifier.data() : "---",
                                 (Bworld.pc.captured[4] != NULL) ? Bworld.pc.captured[4]->get_pokemon().identifier.data() : "---",
                                 (Bworld.pc.captured[5] != NULL) ? Bworld.pc.captured[5]->get_pokemon().identifier.data() : "---");

                        mvprintw(22, xN - 22, "Press '0' to return.");

                        mvprintw(0, 0, "                                                                               ");
                        refresh();

                        i = getch() - 48;

                        if (i == 0)
                            break;

                        mvprintw(22, 0, "                                                                               ");
                        if (i < 0 || i > 6)
                        {
                            mvprintw(0, 0, "Invalid input! Please try again.");
                            refresh();
                            continue;
                        }
                        if (Bworld.pc.captured[i - 1] == NULL)
                        {
                            mvprintw(0, 0, "Pokemon does not exist!");
                            refresh();
                            continue;
                        }
                        if (Bworld.pc.captured[i - 1]->get_currHP() == 0)
                        {
                            mvprintw(22, 0, "%s is knocked out! Potion has no effect!", Bworld.pc.captured[i - 1]->get_pokemon().identifier.data());
                            mvprintw(23, xN - 28, "Press ANY key to continue.");
                            refresh();
                            getch();
                            break;
                        }
                        if (Bworld.pc.captured[i - 1]->get_currHP() > 0)
                        {
                            mvprintw(22, 0, "                                                                               ");
                            mvprintw(23, 0, "                                                                               ");

                            mvprintw(22, 0, "%s is healed!                                                      ", Bworld.pc.captured[i - 1]->get_pokemon().identifier.data());
                            mvprintw(23, xN - 28, "Press ANY key to continue.");
                            refresh();

                            Bworld.pc.captured[i - 1]->set_currHP(Bworld.pc.captured[i - 1]->get_currHP() + 20);
                            Bworld.pc.item->set_num_potions(Bworld.pc.item->get_num_potions() - 1);

                            consumed_turn = true;
                            getch();
                            break;
                        }
                        continue;
                    } while (true);

                    priority = 10;
                    back = true;
                    break;
                default:
                    back = false;
                    continue;
                }
            }
            break;

        // Run
        case '4':
            if (rand() % 256 < ((selected_pokemon->get_speed() * 32) / (p->get_speed() % 256)) + 30 * attempts)
            {
                mvprintw(22, 0, "Player Escapes! Press ANY key to continue.");
                mvprintw(23, 0, "                                                                               ");
                battleEnd = true;
                refresh();
                getch();
                print_map();
            }
            else
            {
                mvprintw(22, 0, "Unable to Escape! Press ANY key to continue.");
                mvprintw(23, 0, "                                                                               ");
                attempts++;
                battleEnd = false;
                consumed_turn = true;
                priority = 10;
                refresh();
                getch();
            }
            break;

        default:
            continue;
        }

        // ends battle if manage to run
        if (battleEnd)
            break;

        if (!consumed_turn)
            continue;

        // Select move for wild pokemon
        rand_selected_move = p->get_move(rand() % p->get_num_moves());

        if (priority >= 10)
        {
            if (!battleEnd && p->get_currHP() > 0)
            {
                move_selected(p, selected_pokemon, rand_selected_move);
                print_pokemon_battle(selected_pokemon, p);
                print_pokemon_indicator(NULL);
                getch();

                if (selected_pokemon->get_currHP() <= 0)
                {
                    // TODO needs fixing
                    mvprintw(23, 0, "%s %s", selected_pokemon->get_pokemon().identifier.data(), "was knocked out!");
                    mvprintw(23, xN - 28, "Press ANY key to continue.");
                    refresh();
                    getch();

                    mvprintw(22, 0, "                                                                               ");
                    mvprintw(23, 0, "                                                                               ");

                    if (is_white_out(&Bworld.pc))
                    {
                        mvprintw(22, 0, "Player whited-out! Please heal using potions and revives!");
                        mvprintw(23, xN - 28, "Press ANY key to continue.");
                        refresh();
                        Bworld.pc.defeated = true;
                        battleEnd = true;
                        getch();
                    }
                    else
                    {
                        selected_pokemon = swap_pokemon(&Bworld.pc, selected_pokemon, true);
                        continue;
                    }
                }
            }
        }
        else
        {
            // player's pokemon moves
            if (priority > rand_selected_move->priority || (priority == rand_selected_move->priority && selected_pokemon->get_speed() >= p->get_speed()))
            {
                move_selected(selected_pokemon, p, pc_selected_move);
                print_pokemon_battle(selected_pokemon, p);
                print_pokemon_indicator(NULL);
                getch();

                if (p->get_currHP() <= 0)
                {
                    battleEnd = true;
                    // TODO needs fixing
                    mvprintw(23, 0, "%s %s", p->get_pokemon().identifier.data(), "was knocked out!");
                    mvprintw(23, xN - 28, "Press ANY key to continue.");
                    refresh();
                    getch();
                }
            }

            // wild moves
            if (!battleEnd && p->get_currHP() > 0)
            {
                move_selected(p, selected_pokemon, rand_selected_move);
                print_pokemon_battle(selected_pokemon, p);
                print_pokemon_indicator(NULL);
                getch();

                if (selected_pokemon->get_currHP() <= 0)
                {
                    // TODO needs fixing
                    mvprintw(23, 0, "%s %s", selected_pokemon->get_pokemon().identifier.data(), "was knocked out!");
                    mvprintw(23, xN - 28, "Press ANY key to continue.");
                    refresh();
                    getch();

                    mvprintw(22, 0, "                                                                               ");
                    mvprintw(23, 0, "                                                                               ");
                    if (is_white_out(&Bworld.pc))
                    {
                        mvprintw(22, 0, "Player whited-out! Please heal using potions and revives!");
                        mvprintw(23, xN - 28, "Press ANY key to continue.");
                        refresh();
                        Bworld.pc.defeated = true;
                        battleEnd = true;
                        getch();
                    }
                    else
                    {
                        selected_pokemon = swap_pokemon(&Bworld.pc, selected_pokemon, true);
                        continue;
                    }
                }

                if (!battleEnd && (priority < rand_selected_move->priority || (priority == rand_selected_move->priority && selected_pokemon->get_speed() < p->get_speed())))
                {
                    move_selected(selected_pokemon, p, pc_selected_move);
                    print_pokemon_battle(selected_pokemon, p);
                    print_pokemon_indicator(NULL);
                    getch();

                    if (p->get_currHP() <= 0)
                    {
                        battleEnd = true;
                        // TODO needs fixing
                        mvprintw(23, 0, "%s %s", p->get_pokemon().identifier.data(), "was knocked out!");
                        mvprintw(23, xN - 28, "Press ANY key to continue.");
                        refresh();
                        getch();
                    }
                }
            }
        }
    }
    clear();

    print_map();
    refresh();
}

int checkValidMove(map_t *m, info_t *info, dir_t dir)
{
    // int x[] = {0, 0, -1, 1, -1, 1, -1, 1};
    // int y[] = {-1, 1, 0, 0, -1, -1, 1, 1};

    // if moving into trainer, return -1
    terrain_t ter = m->mp[info->pos[dim_y]][info->pos[dim_x]];
    terrain_t nextTer = m->mp[info->pos[dim_y] + all_dirs[dir][dim_y]][info->pos[dim_x] + all_dirs[dir][dim_x]];
    if (findTileCost(nextTer, info->trainer) != INT_MAX && m->npc_mp[info->pos[dim_y] + all_dirs[dir][dim_y]][info->pos[dim_x] + all_dirs[dir][dim_x]] == NULL)
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

void shiftTrainer(info_t *info, dir_t dir, int counter)
{
    map_t *m = Bworld.currMap;

    if (!info->defeated && !Bworld.pc.defeated && m->trainerList[counter]->pos[dim_x] + all_dirs[dir][dim_x] == Bworld.pc.pos[dim_x] && m->trainerList[counter]->pos[dim_y] + all_dirs[dir][dim_y] == Bworld.pc.pos[dim_y])
    {
        // mvprintw(0, xN/2, "Contacted Player!!!");
        // refresh();
        initiate_trainer_pokemon_battle(info);
        return;
    }
    else if (findTileCost(m->mp[info->pos[dim_y] + all_dirs[dir][dim_y]][info->pos[dim_x] + all_dirs[dir][dim_x]], info->trainer) != INT_MAX && (m->npc_mp[info->pos[dim_y] + all_dirs[dir][dim_y]][info->pos[dim_x] + all_dirs[dir][dim_x]] == NULL))
    {
        int new_x = info->pos[dim_x] + all_dirs[dir][dim_x];
        int new_y = info->pos[dim_y] + all_dirs[dir][dim_y];
        m->npc_mp[new_y][new_x] = (info_t *)malloc(sizeof(info_t));

        m->npc_mp[new_y][new_x] = info;
        m->npc_mp[info->pos[dim_y]][info->pos[dim_x]] = NULL;

        info->pos[dim_x] = new_x;
        info->pos[dim_y] = new_y;
        info->currCost = info->currCost + findTileCost(m->mp[new_y][new_x], info->trainer);

        m->trainerList[counter] = info;
    }
    else
    {
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
        do
        {
            temp_x = sx + (rand() % abs(sx - ex) + 1) * xdir;
        } while (temp_x <= 0 || temp_x >= xN - 1);

    if (sy != ey)
        do
        {
            temp_y = sy + (rand() % abs(sy - ey) + 1) * ydir;
        } while (temp_y <= 0 || temp_y >= yN - 1);

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
    if (Bworld.pc.currCost == 0)
    {
        Bworld.pc.pos[dim_x] = x;
        Bworld.pc.pos[dim_y] = y;
        Bworld.pc.currCost = (findTileCost(m->mp[y][x], player));
    }

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

map_t *map_init(map_t *m, int x, int y)
{
    int up, down, left, right, dis;
    up = -1, down = -1, left = -1, right = -1;

    m = (map_t *)malloc(sizeof(*m));
    m->pc_currCost = 0;

    if (y - 1 > 0 && Bworld.world[y - 1][x] != NULL)
        up = Bworld.world[y - 1][x]->down[0];
    if (y + 1 < map_size && Bworld.world[y + 1][x] != NULL)
        down = Bworld.world[y + 1][x]->up[0];
    if (x - 1 > 0 && Bworld.world[y][x - 1] != NULL)
        left = Bworld.world[y][x - 1]->right[1];
    if (x + 1 < map_size && Bworld.world[y][x + 1] != NULL)
        right = Bworld.world[y][x + 1]->left[1];

    if (y == 0)
        up = -2;
    if (y == map_size - 1)
        down = -2;
    if (x == 0)
        left = -2;
    if (x == map_size - 1)
        right = -2;

    dis = abs(x - center_x) + abs(y - center_y);

    for (int j = 0; j < yN; j++)
    {
        for (int i = 0; i < xN; i++)
        {
            m->npc_mp[j][i] = NULL;
        }
    }

    if (up == -1)
        m->up[0] = rand() % (xN - 6) + 3;
    else
        m->up[0] = up;
    m->up[1] = 0;

    if (down == -1)
        m->down[0] = rand() % (xN - 6) + 3;
    else
        m->down[0] = down;
    m->down[1] = yN - 1;

    m->left[0] = 0;
    if (left == -1)
        m->left[1] = rand() % (yN - 6) + 3;
    else
        m->left[1] = left;

    m->right[0] = xN - 1;
    if (right == -1)
        m->right[1] = rand() % (yN - 6) + 3;
    else
        m->right[1] = right;

    generation(m->mp, m, dis);
    heap_init(&m->turn, info_cmp, NULL);
    heap_insert(&m->turn, &Bworld.pc);
    return m;
}

void init_trainer(map_t *m, int numTrainer)
{
    int x = 0, y = 0;

    // info_t *info = malloc(sizeof(info_t));_t
    m->trainerList = (info_t **)malloc(numTrainer * sizeof(info_t));

    do
    {
        trainer_t npc = (trainer_t)((rand() % (num_trainer_type - 2)) + 2);

        do
        {
            x = rand() % xN;
            y = rand() % yN;
        } while (y >= map_size - 1 || x >= map_size - 1 ||
                 y <= 1 || x <= 1 || (Bworld.pc.pos[dim_x] == x && Bworld.pc.pos[dim_y] == y) || (findTileCost(m->mp[y][x], npc) == INT_MAX) || m->mp[y][x] == snow_mount || m->mp[y][x] == tree || m->mp[y][x] == path);

        if (numTrainer) // if numTrainer = 0, skip
        {
            m->npc_mp[y][x] = (info_t *)malloc(sizeof(info_t));
            info_t *temp = m->npc_mp[y][x];

            // info_t *temp = malloc(sizeof(*temp));

            if (numTrainer > 2)
            {
                temp->trainer = (trainer_t)((rand() % (num_trainer_type - 2)) + 2);
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
            temp->defeated = false;
            --numTrainer;
            temp->placement = numTrainer;
            temp->item = new Items(0, 0, 1);

            // m->npc_mp[y][x] = temp;
            m->trainerList[numTrainer] = m->npc_mp[y][x];

            init_Pokemon(temp);

            // WHEN WE ADD A TRAINER TO LIST, ADD TO QUEUE
            if (temp->trainer != stationarie)
                heap_insert(&m->turn, temp);
        }
    } while (numTrainer != 0);
}

// randomly move player
void randMove(map_t *m)
{
    dir_t dir;
    // int x[] = {0, 0, -1, 1, -1, 1, -1, 1};
    // int y[] = {-1, 1, 0, 0, -1, -1, 1, 1};

    do
    {
        dir = (dir_t)(rand() % num_dir);
    } while (m->npc_mp[Bworld.pc.pos[dim_y] + all_dirs[dir][dim_y]][Bworld.pc.pos[dim_x] + all_dirs[dir][dim_x]] != NULL);

    terrain_t ter = m->mp[Bworld.pc.pos[dim_y] + all_dirs[dir][dim_y]][Bworld.pc.pos[dim_x] + all_dirs[dir][dim_x]];
    if (findTileCost(ter, player) != INT_MAX)
    {
        Bworld.pc.pos[dim_x] += all_dirs[dir][dim_x];
        Bworld.pc.pos[dim_y] += all_dirs[dir][dim_y];
        Bworld.pc.currCost += (findTileCost(ter, player));
    }
}

// move player
int playerMove(map_t *m, dir_t dir)
{
    dir_t world_dir;
    map_t *temp_world;
    pair_t door_pos;

    if (dir == no_dir)
        return -1;
    if (dir == skip)
    {
        Bworld.pc.currCost += 15;
        return 0;
    }

    // if player meet door, do move map similar to main
    int pos_x = Bworld.pc.pos[dim_x];
    int pos_y = Bworld.pc.pos[dim_y];
    int new_pos_x = pos_x + all_dirs[dir][dim_x];
    int new_pos_y = pos_y + all_dirs[dir][dim_y];

    if (m->mp[new_pos_y][new_pos_x] == door)
    {
        if (m->up[dim_x] == new_pos_x && m->up[dim_y] == new_pos_y)
            world_dir = n;
        if (m->down[dim_x] == new_pos_x && m->down[dim_y] == new_pos_y)
            world_dir = s;
        if (m->left[dim_x] == new_pos_x && m->left[dim_y] == new_pos_y)
            world_dir = w;
        if (m->right[dim_x] == new_pos_x && m->right[dim_y] == new_pos_y)
            world_dir = e;

        temp_world = Bworld.world[Bworld.cur_idx[dim_y] + all_dirs[world_dir][dim_y]][Bworld.cur_idx[dim_x] + all_dirs[world_dir][dim_x]];
        if (temp_world == NULL)
        {
            Bworld.world[Bworld.cur_idx[dim_y] + all_dirs[world_dir][dim_y]][Bworld.cur_idx[dim_x] + all_dirs[world_dir][dim_x]] = map_init(Bworld.world[Bworld.cur_idx[dim_y] + all_dirs[world_dir][dim_y]][Bworld.cur_idx[dim_x] + all_dirs[world_dir][dim_x]], Bworld.cur_idx[dim_x] + all_dirs[world_dir][dim_x], Bworld.cur_idx[dim_y] + all_dirs[world_dir][dim_y]);
            temp_world = Bworld.world[Bworld.cur_idx[dim_y] + all_dirs[world_dir][dim_y]][Bworld.cur_idx[dim_x] + all_dirs[world_dir][dim_x]];
            init_trainer(Bworld.world[Bworld.cur_idx[dim_y] + all_dirs[world_dir][dim_y]][Bworld.cur_idx[dim_x] + all_dirs[world_dir][dim_x]], numTrainer);
            Bworld.currMap->pc_currCost = Bworld.pc.currCost;
        }
        else
            Bworld.pc.currCost = temp_world->pc_currCost;

        Bworld.currMap = temp_world;
        Bworld.cur_idx[dim_x] += all_dirs[world_dir][dim_x];
        Bworld.cur_idx[dim_y] += all_dirs[world_dir][dim_y];

        switch (world_dir)
        {
        case n:
            door_pos[dim_x] = Bworld.currMap->down[dim_x];
            door_pos[dim_y] = Bworld.currMap->down[dim_y];
            break;
        case s:
            door_pos[dim_x] = Bworld.currMap->up[dim_x];
            door_pos[dim_y] = Bworld.currMap->up[dim_y];
            break;
        case w:
            door_pos[dim_x] = Bworld.currMap->right[dim_x];
            door_pos[dim_y] = Bworld.currMap->right[dim_y];
            break;
        case e:
            door_pos[dim_x] = Bworld.currMap->left[dim_x];
            door_pos[dim_y] = Bworld.currMap->left[dim_y];
            break;
        default:
            break;
        }
        if (door_pos[dim_x] == 0)
            Bworld.pc.pos[dim_x] = door_pos[dim_x] + 1;
        else if (door_pos[dim_x] == xN - 1)
            Bworld.pc.pos[dim_x] = door_pos[dim_x] - 1;
        else
            Bworld.pc.pos[dim_x] = door_pos[dim_x];

        if (door_pos[dim_y] == 0)
            Bworld.pc.pos[dim_y] = door_pos[dim_y] + 1;
        else if (door_pos[dim_y] == yN - 1)
            Bworld.pc.pos[dim_y] = door_pos[dim_y] - 1;
        else
            Bworld.pc.pos[dim_y] = door_pos[dim_y];

        if (Bworld.currMap->pc_currCost == 0)
        {
            terrain_t ter = Bworld.currMap->mp[Bworld.pc.pos[dim_y] + all_dirs[world_dir][dim_y]][Bworld.pc.pos[dim_x] + all_dirs[world_dir][dim_x]];
            Bworld.pc.currCost = findTileCost(ter, player);
        }
    }
    else if (m->npc_mp[Bworld.pc.pos[dim_y] + all_dirs[dir][dim_y]][Bworld.pc.pos[dim_x] + all_dirs[dir][dim_x]] != NULL)
        if (!(m->npc_mp[Bworld.pc.pos[dim_y] + all_dirs[dir][dim_y]][Bworld.pc.pos[dim_x] + all_dirs[dir][dim_x]]->defeated) && !Bworld.pc.defeated)
            initiate_trainer_pokemon_battle(m->npc_mp[Bworld.pc.pos[dim_y] + all_dirs[dir][dim_y]][Bworld.pc.pos[dim_x] + all_dirs[dir][dim_x]]);
        else
        {
            mvprintw(0, 0, "Defeated this Trainer, please spare their lives. Press any key to continue.");
            refresh();
            getch();
            mvprintw(0, 0, "Using seed: %u                                                      ", seed);
            return -1;
        }
    else
    {
        terrain_t ter = m->mp[Bworld.pc.pos[dim_y] + all_dirs[dir][dim_y]][Bworld.pc.pos[dim_x] + all_dirs[dir][dim_x]];
        if (findTileCost(ter, player) != INT_MAX)
        {
            // pokemon spawn check
            if (ter == grass_1 || ter == grass_2 || ter == grass_3 || ter == grass_4)
            {
                if (rand() % 10 == 0)
                {
                    Pokemon *wild = spawn_pokemon();
                    // spawn_pokemon(&wild_pokemon);
                    initiate_wild_pokemon_battle(wild);
                }
            }
            Bworld.pc.pos[dim_x] += all_dirs[dir][dim_x];
            Bworld.pc.pos[dim_y] += all_dirs[dir][dim_y];
            Bworld.pc.currCost += (findTileCost(ter, player));
        }
        else
        {
            mvprintw(0, 0, "Invalid move! %s is in the way.", ter_name[ter]);
            refresh();
            // mvprintw(0,0, "                                                                ", ter_name[ter]);
            return -1;
        }
    }
    dijkstra_map(Bworld.currMap, hiker);
    dijkstra_map(Bworld.currMap, rival);
    return 0;
}

void move_Hiker_Rival(info_t *info, int counter)
{
    shiftTrainer(info, findNeighbourWeighted(Bworld.currMap, info), counter);
}

void move_RandomWalker_Pacer(info_t *info, int counter)
{
    map_t *m = Bworld.currMap;
    int temp = 0;
    if (info->prev == no_dir)
    {
        while (temp < 100)
        {
            dir_t dir = (dir_t)(rand() % 4);
            if (checkValidMove(m, info, dir) == 0)
            {
                info->prev = dir;
                shiftTrainer(info, dir, counter);
                return;
            }
            temp++;
        }
    }
    else if (info->prev != no_dir && checkValidMove(m, info, info->prev) == 0)
        shiftTrainer(info, info->prev, counter);
    else if (info->prev < 4) // can only have n, s, w, e
    {
        // flip 180
        if (info->prev % 2 == 0)
        {
            info->prev = (dir_t)(info->prev + 1);
        }
        else
        {
            info->prev = (dir_t)(info->prev - 1);
        }
        if (checkValidMove(m, info, info->prev) == 0)
            shiftTrainer(info, (dir_t)(info->prev), counter);
        else
            info->currCost += findTileCost(m->mp[info->pos[dim_y]][info->pos[dim_x]], info->trainer);
    }
}

void moveWanderer(info_t *info, int counter)
{
    map_t *m = Bworld.currMap;
    int temp = 0;
    if (info->prev == no_dir)
    {
        while (temp < 100)
        {
            dir_t dir = (dir_t)(rand() % 4);
            if (checkValidMove(m, info, dir) == 0)
            {
                info->prev = dir;
                shiftTrainer(info, dir, counter);
                return;
            }
            temp++;
        }
    }
    else if (info->prev != no_dir && checkValidMove(m, info, info->prev) == 0)
        shiftTrainer(info, info->prev, counter);
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
                shiftTrainer(info, info->prev, counter);
                return;
            }
            temp++;
        } while (temp < 100);
        info->currCost += findTileCost(m->mp[info->pos[dim_y]][info->pos[dim_x]], info->trainer);
    }
}

void init_world()
{
    int i, j;
    for (j = 0; j < map_size; j++)
    {
        for (i = 0; i < map_size; i++)
        {
            Bworld.world[j][i] = NULL;
        }
    }
    Bworld.cur_idx[dim_x] = Bworld.cur_idx[dim_y] = map_size / 2;
    Bworld.pc.currCost = 0;
    Bworld.pc.item = new Items;
    Bworld.pc.defeated = false;

    Bworld.world[center_y][center_x] = map_init(Bworld.world[center_y][center_x], Bworld.cur_idx[dim_x], Bworld.cur_idx[dim_y]);
    Bworld.currMap = Bworld.world[center_y][center_x];
    dijkstra_map(Bworld.world[center_y][center_x], hiker);
    dijkstra_map(Bworld.world[center_y][center_x], rival);
    init_trainer(Bworld.world[center_y][center_x], numTrainer);
    init_Pokemon(&Bworld.pc);
}

void cycle()
{
    info_t *z;
    // heap_t h;
    // int i = 0;
    //  char c;
    dir_t dir = no_dir;
    int32_t key;

    // heap_init(&h, info_cmp, NULL);

    // temp = &Bworld.pc;
    // temp->hn = heap_insert(&h, &Bworld.pc);
    // while (i < numTrainer)
    // {
    //     temp = m->trainerList[i];
    //     if (temp->trainer != stationarie && temp->defeated == 0)
    //         temp->hn = heap_insert(&h, temp);
    //     i++;
    // }

    // info_t *temp1 = heap_peek_min(&h);
    while (1)
    {
        z = (info_t *)heap_remove_min(&Bworld.currMap->turn);
        if (check == 'q')
            return;
        switch (z->trainer)
        {
        case player:
            print_map();
            do
            {
                key = getch();
                // mvprintw(23, 0, "Key pressed was: %d\n", key);
                //  switch (c)
                switch (key)
                {
                case '7':
                case 'y':
                    dir = nw;
                    break;
                case '8':
                case 'k':
                    dir = n;
                    break;
                case '9':
                case 'u':
                    dir = ne;
                    break;
                case '6':
                case 'l':
                    dir = e;
                    break;
                case '3':
                case 'n':
                    dir = se;
                    break;
                case '2':
                case 'j':
                    dir = s;
                    break;
                case '1':
                case 'b':
                    dir = sw;
                    break;
                case '4':
                case 'h':
                    dir = w;
                    break;
                case '>':
                    // enter building
                    if (Bworld.currMap->mp[Bworld.pc.pos[dim_y]][Bworld.pc.pos[dim_x]] == center || Bworld.currMap->mp[Bworld.pc.pos[dim_y]][Bworld.pc.pos[dim_x]] == mart)
                    {
                        centerMart_PLACEHOLDER();
                        print_map();
                        dir = no_dir;
                    }
                    break;
                case '5':
                case ' ':
                case '.':
                    dir = skip;
                    break;

                case 'q':
                case 'Q':
                    check = 'q';
                    return;

                    // case 'X':
                    // case 'x':
                    //     print_pokemon_list();
                    //     getch();
                    //     print_map();
                    //     dir = no_dir;
                    //     break;

                case 'X':
                case 'x':
                    print_bag();
                    getch();
                    print_map();
                    dir = no_dir;
                    break;

                case 't':
                    print_trainerList(Bworld.currMap);
                    print_map();
                    dir = no_dir;
                    // heap_insert(&Bworld.currMap->turn, &Bworld.pc);
                    continue;

                case 'f':
                    int x, y;
                    mvprintw(23, 0, "Where to FLY to (x y): ");
                    refresh();
                    echo();
                    curs_set(1);
                    scanw("%d %d", &x, &y);
                    refresh();
                    mvprintw(23, 0, "Entered %d %d                                   ", x, y);
                    curs_set(0);
                    noecho();
                    refresh();
                    if (abs(x) + abs(y) > 400)
                    {
                        mvprintw(23, 0, "Invalid coords, (x y) must be within [-200, 200]. Press Any Key to continue.");
                        getch();
                        refresh();
                        mvprintw(23, 0, "                                                                            ");
                        dir = no_dir;
                        break;
                    }
                    Bworld.currMap->pc_currCost = Bworld.pc.currCost;
                    Bworld.cur_idx[dim_y] = y + center_y;
                    Bworld.cur_idx[dim_x] = x + center_x;
                    Bworld.currMap = Bworld.world[y + center_y][x + center_x];

                    if (Bworld.currMap == NULL)
                    {
                        Bworld.currMap = map_init(Bworld.currMap, Bworld.cur_idx[dim_x], Bworld.cur_idx[dim_y]);
                        init_trainer(Bworld.currMap, numTrainer);
                        Bworld.pc.currCost = 0;
                    }
                    else
                    {
                        Bworld.pc.currCost = Bworld.currMap->pc_currCost;
                    }
                    dijkstra_map(Bworld.currMap, hiker);
                    dijkstra_map(Bworld.currMap, rival);

                    mvprintw(23, 0, "                   ");
                    print_map();

                    dir = no_dir;
                    break;

                case '?':
                    print_MoveList();
                    print_map();
                    // heap_insert(&Bworld.currMap->turn, &Bworld.pc);
                    //  if (c != '\n')
                    //      while ((getchar()) != '\n')
                    //          ;
                    continue;

                default:
                    dir = no_dir;
                    continue;
                }
            } while (playerMove(Bworld.currMap, dir) == -1);
            // return;
            break;
        case rival:
        case hiker:
            move_Hiker_Rival(z, z->placement);
            break;
        case random_walker:
        case pacer:
            move_RandomWalker_Pacer(z, z->placement);
            break;
        case wanderer:
            moveWanderer(z, z->placement);
            break;
        default:
            break;
        }
        if (z->trainer == player || (z->defeated == 0 && check != 'q'))
            heap_insert(&Bworld.currMap->turn, z);
    }
    // heap_delete(&h);
}

void deleteWorld()
{
    int x, y, i, j;

    for (y = 0; y < map_size; y++)
    {
        for (x = 0; x < map_size; x++)
        {
            if (Bworld.world[y][x] != NULL)
            {
                for (j = 0; j < yN; j++)
                {
                    for (i = 0; i < xN; i++)
                    {
                        if (Bworld.world[y][x]->npc_mp[j][i] != NULL)
                        {
                            free(Bworld.world[y][x]->npc_mp[j][i]);
                            // Bworld.world[y][x]->npc_mp[j][i] = NULL;
                        }
                    }
                }
                if (Bworld.world[y][x]->trainerList != NULL)
                {
                    free(Bworld.world[y][x]->trainerList);
                    // Bworld.world[y][x]->trainerList = NULL;
                }
                free(Bworld.world[y][x]);
                // Bworld.world[y][x] = NULL;
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    struct timeval tv;
    std::string fname;
    // int i, j;
    // char dir;

    if (argc >= 2)
    {
        for (int i = 1; i < argc; i++)
        {
            if (strcmp(argv[i], "--numtrainers") == 0 && argv[i + 1] != NULL)
                numTrainer = strtol(argv[i + 1], NULL, 10);

            // if (strcmp(argv[i], "pokemon") == 0 || strcmp(argv[i], "pokemon.csv") == 0)
            //     fname = "pokemon.csv";
            // if (strcmp(argv[i], "moves") == 0 || strcmp(argv[i], "moves.csv") == 0)
            //     fname = "moves.csv";
            // if (strcmp(argv[i], "pokemon_moves") == 0 || strcmp(argv[i], "pokemon_moves.csv") == 0)
            //     fname = "pokemon_moves.csv";
            // if (strcmp(argv[i], "pokemon_species") == 0 || strcmp(argv[i], "pokemon_species.csv") == 0)
            //     fname = "pokemon_species.csv";
            // if (strcmp(argv[i], "experience") == 0 || strcmp(argv[i], "experience.csv") == 0)
            //     fname = "experience.csv";
            // if (strcmp(argv[i], "type_names") == 0 || strcmp(argv[i], "type_names.csv") == 0)
            //     fname = "type_names.csv";
            // // fname += ".csv";
            // if (fname.size() != 0)
            //     parse(&fname);

            if (strcmp(argv[i], "--seed") == 0)
                seed = atoi(argv[argc - 1]);
        }
    }
    else
    {
        gettimeofday(&tv, NULL);
        seed = (tv.tv_usec ^ (tv.tv_sec << 20)) & 0xffffffff;
    }

    // std::string t = "pokemon_stats.csv";
    // parse(&data, &t);
    parseAll();
    std::cout << "Parse completed" << std::endl;

    // return 0;

    initscr();
    raw();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    start_color();

    srand(seed);

    init_world();
    print_map();

    cycle();

    deleteWorld();
    // endScreen();

    endwin();
    return 0;
}
