#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string.h>

// void read_record(std::string *s)
// {
//     char line[256];
//     std::ifstream infile(*s, std::ios::in);

//     if (infile.peek()!=EOF)
//     {
//         while (infile.getline(line, 256))
//         {
//             std::cout << line << std::endl;
//         }
//     }
// }

// int main()
// {
//     std::cout << "Which File to READ: " << std::endl;
//     std::cout.flush();

//     std::string s;
//     std::cin>>s >> std::ws;

//     read_record(&s);

//     return 0;
// }
using namespace std;
int main()
{
    string fname;
    cout << "Enter the file name: ";
    cin >> fname;

    vector<vector<string>> content;
    vector<string> row;
    string line, word;

    fstream file(fname, ios::in);
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
                row.push_back(word);
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