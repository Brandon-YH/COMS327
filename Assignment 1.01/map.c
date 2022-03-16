#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "map.h"

// min & max helper
int max(int num1, int num2)
{
    return (num1 > num2) ? num1 : num2;
}
int min(int num1, int num2)
{
    return (num1 > num2) ? num2 : num1;
}

map_t *map_init(int u, int d, int l, int r)
{
    map_t *m = malloc(sizeof(*m));
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

    generation(m->mp, m);
    return m;
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
    if (ex == sx && ey == sy)
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

    // connect left
    connect(map, m->left[0], m->left[1], x, y);
    // connect right
    connect(map, m->right[0], m->right[1], x, y);
    // connect up
    connect(map, m->up[0], m->up[1], x, y);
    // connect down
    connect(map, m->down[0], m->down[1], x, y);
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
        x1 = rand() % (xN - 6) + 2;
        y1 = rand() % (yN - 6) + 2;
    } while (map[y1][x1] != blank || map[y1 + 1][x1] != blank || map[y1][x1 + 1] != blank || map[y1 + 1][x1 + 1] != blank);

    map[y1][x1] = center;
    map[y1 + 1][x1] = center;
    map[y1][x1 + 1] = center;
    map[y1 + 1][x1 + 1] = center;

    do
    { // might have to change to complete random
        x2 = max(min(abs(x1 + (rand() % 3 - 1) * max((rand() % (xN / 3)), 3)), xN - 2), 0);
        y2 = max(min(abs(y1 + (rand() % 3 - 1) * max((rand() % (yN / 2)), 3)), yN - 2), 0);
    } while (map[y2][x2] != blank || map[y2 + 1][x2] != blank || map[y2][x2 + 1] != blank || map[y2 + 1][x2 + 1] != blank || x1 == x2 || y1 == y2 || x1 + 1 == x2 || y1 + 1 == y2);
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
void generation(terrain_t map[yN][xN], map_t *m)
{
    int i, j, x, y, attemps;
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
    map[m->up[1]][m->up[0]] = door;
    map[m->down[1]][m->down[0]] = door;
    map[m->left[1]][m->left[0]] = door;
    map[m->right[1]][m->right[0]] = door;

    // build path
    bPath(map, m);

    // fill empty with grass
    for (i = 0; i < 4; i++)
    {
        do
        {
            x = rand() % (xN - 1) + 1;
            y = rand() % (yN - 1) + 1;
        } while (map[y][x] != blank);

        grow(x, y, i, max((rand() % (yN / 2)) + yN / 3, 3), map);
    }

    // build pokeCenter & pokeMart
    pokeMC(map);

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
void print(terrain_t map[yN][xN])
{
    for (int i = 0; i < yN; i++)
    {
        for (int j = 0; j < xN; j++)
        {
            terrain_t type = map[i][j];
            switch (type)
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

            case player:
                printf("@");
                break;

            default:
                printf(" ");
                break;
            }
        }
        printf("\n");
    }
}

//  int main(int argc, char const *argv[])
// {
//     int size_x = 399;
//     int size_y = 399;
//     int i, j, up, down, left, right;
//     char dir;

//     map_t *world[size_y][size_x];
//     for (j = 0; j < size_y; j++)
//     {
//         for (i = 0; i < size_x; i++)
//         {
//             world[j][i] = NULL;
//         }
//     }

//     i = 0, j = 0;

//     do
//     {
//         printf("\n");
//         up = -1, down = -1, left = -1, right = -1;
//         if (world[j][i] == NULL)
//         {
//             if (world[j - 1][i] != NULL)
//                 up = world[j - 1][i]->down[0];
//             if (world[j + 1][i] != NULL)
//                 down = world[j + 1][i]->up[0];
//             if (world[j][i - 1] != NULL)
//                 left = world[j][i - 1]->right[1];
//             if (world[j][i + 1] != NULL)
//                 right = world[j][i + 1]->left[1];

//             world[j][i] = map_init(up, down, left, right);
//         }

//         print(world[j][i]->mp);

//         do
//         {
//             printf("Direction (n, s, e, w, q to quit): ");
//             scanf("%c", &dir);

//             switch (dir)
//             {
//             case 'n':
//                 if (j - 1 >= 0)
//                     j--;
//                 else
//                 {
//                     printf("At the edge of map, try another direction.\n");
//                     dir = 'z';
//                 }
//                 break;

//             case 's':
//                 if (j + 1 < size_y)
//                     j++;
//                 else
//                 {
//                     printf("At the edge of map, try another direction.\n");
//                     dir = 'z';
//                 }
//                 break;

//             case 'e':
//                 if (i + 1 < size_x)
//                     i++;
//                 else
//                 {
//                     printf("At the edge of map, try another direction.\n");
//                     dir = 'z';
//                 }
//                 break;

//             case 'w':
//                 if (i - 1 >= 0)
//                     i--;
//                 else
//                 {
//                     printf("At the edge of map, try another direction.\n");
//                     dir = 'z';
//                 }
//                 break;

//             case 'q':
//                 printf("Quitting...");
//                 break;

//             default:
//                 printf("Invalid direction. Please try again.\n");
//                 break;
//             }
//             fflush(stdin);
//         } while (dir != 'n' && dir != 's' && dir != 'e' && dir != 'w' && dir != 'q');
//     } while (dir != 'q');

//     return 0;
// }