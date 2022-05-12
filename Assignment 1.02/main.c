#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "map.h"

#define size_x 399
#define size_y 399
#define center_x 199
#define center_y 199

map_t *world[size_y][size_x];

int main(int argc, char const *argv[])
{
    // int size_x = 399;
    // int size_y = 399;
    // int center_x = 199;
    // int center_y = 199;
    map_t *world[size_y][size_x];

    int i, j, up, down, left, right, x, y, dis;
    char dir;

    for (j = 0; j < size_y; j++)
    {
        for (i = 0; i < size_x; i++)
        {
            world[j][i] = NULL;
        }
    }

    i = center_x, j = center_y;

    do
    {
        printf("\n");
        up = -1, down = -1, left = -1, right = -1;
        if (world[j][i] == NULL)
        {
            if (j - 1 > 0 && world[j - 1][i] != NULL)
                up = world[j - 1][i]->down[0];
            if (j + 1 < size_y && world[j + 1][i] != NULL)
                down = world[j + 1][i]->up[0];
            if (i - 1 > 0 && world[j][i - 1] != NULL)
                left = world[j][i - 1]->right[1];
            if (i + 1 < size_x && world[j][i + 1] != NULL)
                right = world[j][i + 1]->left[1];

            if (j == 0)
                up = -2;
            if (j == size_y - 1)
                down = -2;
            if (i == 0)
                left = -2;
            if (i == size_x - 1)
                right = -2;

            dis = abs(i - center_x) + abs(j - center_y);

            world[j][i] = map_init(up, down, left, right, dis);
        }

        print(world[j][i]->mp, i - center_x, j - center_y);

        do
        {
            printf("Direction (n, s, e, w, f, q to quit): ");
            scanf("%c", &dir);
            // fflush(stdin);
            if (dir == 'n')
            {
                if (j - 1 >= 0)
                    j--;
                else
                {
                    printf("At the edge of map, try another direction.\n");
                    dir = 'z';
                }
            }
            else if (dir == 's')
            {
                if (j + 1 < size_y)
                    j++;
                else
                {
                    printf("At the edge of map, try another direction.\n");
                    dir = 'z';
                }
            }
            else if (dir == 'e')
            {
                if (i + 1 < size_x)
                    i++;
                else
                {
                    printf("At the edge of map, try another direction.\n");
                    dir = 'z';
                }
            }
            else if (dir == 'w')
            {
                if (i - 1 >= 0)
                    i--;
                else
                {
                    printf("At the edge of map, try another direction.\n");
                    dir = 'z';
                }
            }
            else if (dir == 'f')
            {
                printf("Where to fly to (x y): ");
                if (scanf("%d", &x) == 1 && scanf("%d", &y) == 1)
                {
                    // fflush(stdin);

                    if (x + center_x >= 0 && x + center_x < size_x && y + center_y >= 0 && y + center_y < size_y)
                    {
                        i = x + center_x;
                        j = y + center_y;
                        printf("\nFlying to (%d, %d)...\n", x, y);
                        while ((getchar()) != '\n');
                        break;
                    }
                    else
                    {
                        printf("Invalid coordinate. Please try again.\n");
                        dir = 'z';
                    }
                }
                else
                {
                    printf("Input is not a coordinate. Please try again.\n");
                    dir = 'z';
                }
                // if (dir != '\n')
                //     while ((getchar()) != '\n');
            }
            else if (dir == 'q')
            {
                printf("Quitting...\n");
                for (j = 0; j < size_y; j++)
                    for (i = 0; i < size_x; i++)
                        if (world[j][i] != NULL)
                            free(world[j][i]);
            }
            else
            {
                printf("Invalid direction. Please try again.\n");
            }
            if (dir != '\n')
                while ((getchar()) != '\n');

        } while (dir != 'n' && dir != 's' && dir != 'e' && dir != 'w' && dir != 'q' && dir != 'f');
    } while (dir != 'q');

    return 0;
}
