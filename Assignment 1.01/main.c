#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "map.h"

#define size_x 399
#define size_y 399

 int main(int argc, char const *argv[])
{
    // int size_x = 399;
    // int size_y = 399;
    int i, j, up, down, left, right;
    char dir;

    map_t *world[size_y][size_x];
    for (j = 0; j < size_y; j++)
    {
        for (i = 0; i < size_x; i++)
        {
            world[j][i] = NULL;
        }
    }

    i = 0, j = 0;

    do
    {
        printf("\n");
        up = -1, down = -1, left = -1, right = -1;
        if (world[j][i] == NULL)
        {
            if (world[j - 1][i] != NULL)
                up = world[j - 1][i]->down[0];
            if (world[j + 1][i] != NULL)
                down = world[j + 1][i]->up[0];
            if (world[j][i - 1] != NULL)
                left = world[j][i - 1]->right[1];
            if (world[j][i + 1] != NULL)
                right = world[j][i + 1]->left[1];

            world[j][i] = map_init(up, down, left, right);
        }

        print(world[j][i]->mp);

        do
        {
            printf("Direction (n, s, e, w, q to quit): ");
            scanf("%c", &dir);

            switch (dir)
            {
            case 'n':
                if (j - 1 >= 0)
                    j--;
                else
                {
                    printf("At the edge of map, try another direction.\n");
                    dir = 'z';
                }
                break;

            case 's':
                if (j + 1 < size_y)
                    j++;
                else
                {
                    printf("At the edge of map, try another direction.\n");
                    dir = 'z';
                }
                break;

            case 'e':
                if (i + 1 < size_x)
                    i++;
                else
                {
                    printf("At the edge of map, try another direction.\n");
                    dir = 'z';
                }
                break;

            case 'w':
                if (i - 1 >= 0)
                    i--;
                else
                {
                    printf("At the edge of map, try another direction.\n");
                    dir = 'z';
                }
                break;

            case 'q':
                printf("Quitting...");
                break;

            default:
                printf("Invalid direction. Please try again.\n");
                break;
            }
            fflush(stdin);
        } while (dir != 'n' && dir != 's' && dir != 'e' && dir != 'w' && dir != 'q');
    } while (dir != 'q');

    return 0;
}
