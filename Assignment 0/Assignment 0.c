#include <stdio.h>
#include <string.h>
#define N 5

int total = 0;
int xMove[] = {2, 1, -1, -2, -2, -1, 1, 2};
int yMove[] = {1, 2, 2, 1, -1, -2, -2, -1};
int arr2[N][N];

//helper methods
void solve(int arr[N][N], int x, int y, int pos);
void print(int arr[N][N]);

int main(int argc, char const *argv[])
{
        int arr[N][N];
        int c = 1;

        for (int j = 0; j < N; j++)
        {
            for (int i = 0; i < N; i++)
            {
                arr[j][i] = 0;
                arr2[j][i] = c++;
            }
        }

        for (int j = 0; j < N; j++)
        {
            for (int i = 0; i < N; i++)
            {
                solve(arr, j, i, 1);
            }
        }
        printf("%d\n", total);
}
    void solve(int arr[N][N], int x, int y, int pos)
    {
        arr[x][y] = pos;

        if (pos >= N * N)
        {
            print(arr);
            arr[x][y] = 0;
            total++;
            return;
        }

        for (int k = 0; k < 8; k++)
        {
            int newX = x + xMove[k];
            int newY = y + yMove[k];

            if (newX >= 0 && newY >= 0 && newX < N && newY < N && arr[newX][newY] == 0)
            {
                solve(arr, newX, newY, pos + 1);
            }
        }

        arr[x][y] = 0;
    }

    void print(int arr[N][N])
    {
        int count = 1;
        while (count < 26)
        {
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    if (count == arr[i][j])
                    {
                        if (count == N * N)
                        {
                            printf("%d\n", arr2[i][j]);
                        }
                        else
                        {
                            printf("%d, ", arr2[i][j]);
                        }
                        count++;
                    }
                }
            }
        }
    }

