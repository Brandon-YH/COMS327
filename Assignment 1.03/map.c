#include "map.h"
#include "heap.h"

#define heightpair(pair) (m->height[pair[dim_y]][pair[dim_x]])
#define heightxy(x, y) (m->height[y][x])

// min & max helper
int max(int num1, int num2)
{
    return (num1 > num2) ? num1 : num2;
}
int min(int num1, int num2)
{
    return (num1 > num2) ? num2 : num1;
}

static int32_t path_cmp(const void *key, const void *with)
{
    return ((path_t *)key)->cost - ((path_t *)with)->cost;
}

map_t *map_init(int u, int d, int l, int r, int dis)
{
    map_t *m = malloc(sizeof(*m));

    // m->x = x;
    // m->y = y;

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

int findTileCost(terrain_t tile, trainer_t trainer)
{
    int cost;
    switch (tile)
    {
    case tree:
    case snow_mount:
        if (trainer == rival)
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
        else if (trainer == rival)
            cost = 20;
        break;

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

        // if ((p->pos[dim_y] == to[dim_y]) && p->pos[dim_x] == to[dim_x])
        // {
        //     for (x = to[dim_x], y = to[dim_y];
        //          (x != from[dim_x]) || (y != from[dim_y]);
        //          p = &path[y][x], x = p->from[dim_x], y = p->from[dim_y])
        //     {
        //         mapxy(x, y) = ter_path;
        //         heightxy(x, y) = 0;
        //     }
        //     heap_delete(&h);
        //     return;
        // }

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

    from[dim_x] = m->trainerList[player].pos[dim_x];
    from[dim_y] = m->trainerList[player].pos[dim_y];

    j = 0, i = 0;
    to[dim_x] = i;
    to[dim_y] = j;
    dijkstra_tile(m, from, to, path, p, trainer);

    printf("Weighted Map for ");
    switch (trainer)
    {
    case hiker:
        printf("Hiker\n");
        break;
    case rival:
        printf("Rival\n");
        break;
    default:
        printf("Others\n");
        break;
    }

    for (j = 0; j < yN; j++)
    {
        for (i = 0; i < xN; i++)
        {
            if (path[j][i].cost != INT_MAX)
                printf("%02d ", path[j][i].cost % 100);
            else
                printf("   ");
            m->trainerList[trainer].pathCost[j][i] = path[j][i].cost;
        }
        printf("\n");
    }
    printf("\n");
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
        // while (!(abs(curr_y - ey) == 0 && abs(curr_x - ex) == 0))
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
    m->trainerList[player].pos[dim_x] = x;
    m->trainerList[player].pos[dim_y] = y;

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
    srand(time(NULL));

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

// map printing
void print(map_t *m, int x, int y)
{
    int i, j;
    pair_t player_pos;
    player_pos[dim_y] = m->trainerList[player].pos[dim_y];
    player_pos[dim_x] = m->trainerList[player].pos[dim_x];

    for (j = 0; j < yN; j++)
    {
        for (i = 0; i < xN; i++)
        {
            if (i == player_pos[dim_x] && j == player_pos[dim_y])
            {
                printf("@");
                continue;
            }
            switch (m->mp[j][i])
            {
            case grass_1:
                printf(",");
                break;
            case grass_2:
                printf(";");
                break;
            case grass_3:
                printf("-");
                break;
            case grass_4:
                printf("^");
                break;

            case tree:
                printf("\"");
                break;
            case snow_mount:
                printf("*");
                break;

            case water:
                printf("~");
                break;

            case blank:
                printf(" ");
                break;
            case wall:
                printf("%%");
                break;
            case door:
                printf("=");
                break;
            case path:
                printf("#");
                break;

            case mart:
                printf("M");
                break;
            case center:
                printf("C");
                break;

            default:
                printf(" ");
                break;
            }
        }
        printf("\n");
    }
    printf("Current Coords: (%d, %d)\n", x, y);
}
// new
