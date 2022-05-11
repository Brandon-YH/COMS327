#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string.h>

using namespace std;
int main(int argc, char const *argv[])
{
    string fname;
    cout << "Enter the file name: ";
    cin >> fname;

    vector<string> row;
    string line, word;

    fstream file(fname, ios::in);

    getline(file, line);
    stringstream str(line);
    stringstream str1(line);
    stringstream str2(line);
    stringstream str3(line);
    stringstream str4(line);
    stringstream str5(line);

    while (getline(str, word, ','))
    {
        // (this->id == -1 ? " " : std::to_string(this->id))
        cout << "std::cout << \"" << word << ":\\t\\t\" << (this->" << word << " == -1 ?  \" \" : std::to_string(this->" << word << "))"
             << " << std::endl;" << endl;
    }
    cout << "\n\n\n\n"
         << endl;
    while (getline(str1, word, ','))
    {
        cout << "this->" << word << " = " << word << ";" << endl;
    }

    cout << "\n\n\n\n"
         << endl;
    cout << "int ";
    while (getline(str2, word, ','))
    {
        cout << word << ", ";
    }
    cout << ";";

    cout << "\n\n\n\n"
         << endl;
    while (getline(str3, word, ','))
    {
        cout << "this->" << word << " = "
             << "-1;" << endl;
    }

    cout << "\n\n\n\n"
         << endl;
    while (getline(str4, word, ','))
    {
        cout << word << ",";
    }


    // getline(str, word, ',');
    // if (word.empty())
    //     word = -1;
    // id = stoi(word);
    // row.push_back(word);
    cout << "\n\n\n\n"
         << endl;
    while (getline(str5, word, ','))
    {
        cout << "getline(str, word, ',');\nif (word.empty())\n\tword = -1;\n" << word << " = stoi(word);\n\n";
    }
    return 0;
}
