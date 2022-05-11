#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string.h>

using namespace std;

bool invalidChar(char c)
{
    return !(c >= 0 && c < 128);
}

int main(int argc, char const *argv[])
{
    string fname;

    bool isValidLine = true;

    vector<vector<string>> content;
    vector<string> row;
    string line, word;

    fstream file(fname, ios::in);

    getline(file, line);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            row.clear();

            stringstream str(line);

            while (getline(str, word, ','))
            {
                if (word.empty())
                    word = -1;
                for (int i = 0; i < (int)word.length(); i++)
                {
                    if (invalidChar(word[i]))
                    {
                        isValidLine = false;
                        break;
                    }
                }
                if (!isValidLine)
                {
                    break;
                }

                row.push_back(word);
            }
            if (!isValidLine)
            {
                row.clear();
                isValidLine = true;
                continue;
            }
            

            content.push_back(row);
        }
    }
    else
        cout << "Could not open the file\n";

    for (int i = 0; i < (int)content.size(); i++)
    {
        for (int j = 0; j < (int)content[i].size(); j++)
        {
            cout << content[i][j] << ", ";
        }
        cout << "\n";
    }

    return 0;
}