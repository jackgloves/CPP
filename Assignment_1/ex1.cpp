#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    // Create a map of key locations on the keyboard
    vector<string> keyboard = {
            "qwertyuiop",
            "asdfghjkl",
            "zxcvbnm"
    };

    // Read the input characters from the user
    string input;
    cout << "Enter characters: ";
    getline(cin, input);

    if (input.empty()) {
        std::cout << "Input is empty." << std::endl;
        return 0;
    } else if (!std::all_of(input.begin(), input.end(), [](char c) { return std::isalpha(c); })) {
        std::cout << "Input contains non-alphabetic characters." << std::endl;
        return 0;
    }

    // Create a vector of key locations for each input character
    vector<vector<pair<int, int>>> locations(input.size());
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < keyboard.size(); j++) {
            for (int k = 0; k < keyboard[j].size(); k++) {
                if (keyboard[j][k] == tolower(input[i])) {
                    locations[i].push_back(make_pair(j, k));
                }
            }
        }
    }


    // Create a matrix to represent the keyboard and mark the locations of the keys that can print the input characters
//    vector<vector<char>> matrix(keyboard.size(), vector<char>(keyboard[0].size(), '.'));
    vector<vector<char>> matrix = {{'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
                                     {' ', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
                                     {' ', ' ', '.', '.', '.', '.', '.', '.', '.'}};
    for (int i = 0; i < locations.size(); i++) {
        for (auto loc : locations[i]) {
            if (loc.first == 0) {
            }
            else if (loc.first == 1) {
               loc.second = loc.second + 1;
            }
            else {
                loc.second = loc.second + 2;
            }
            matrix[loc.first][loc.second] = 'o';
        }
    }

    // Display the keyboard matrix
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
        for (int k = 0; k < i; k++) {
            cout << " ";
        }
    }

    return 0;
}
