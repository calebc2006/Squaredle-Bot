#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <array>
#include <set>
#include <stdlib.h>
using namespace std;

string wordlist[400000];
int numWords;
set<string> words;

void loadWords() {
    ifstream file;
    file.open("wordlist.txt");
    int i = 0;

    while (!file.eof()) {
        file >> wordlist[i];
        i++;
    }
    numWords = i;
}

int checkString(string& str) {
    if (str == "")
        return 1;

    for (int i=0; i<numWords; i++) 
        if (wordlist[i] == str) 
            return 2;
    
    for (int i=0; i<numWords; i++) 
        if (wordlist[i].rfind(str, 0) == 0)
            return 1;
    
    return 0;
}

struct Pos {
    int x, y;
    Pos() { x = y = 0; }
    Pos(int px, int py) { x = px; y = py; }
};

class Board {
private:
    char arr[7][7];
    array<array<int, 2>, 8> dir = {{{1, -1}, {1, 0}, {1, 1}, {0, -1}, {0, 1}, {-1, -1}, {-1, 0}, {-1, 1}}};

public:
    Board() {
        for (int i=0; i<7; i++)
            for (int j=0; j<7; j++) 
                arr[i][j] = '-';
    }

    bool takeInput() {
        string line;
        int i=0;

        while (getline(cin, line)) {
            if (i >= 7)
                return false;

            stringstream s(line);
            char c;
            try {
                for (int j=0; j<7; j++) {
                    if (s >> c)
                        arr[i][j] = toupper(c);
                    else
                        arr[i][j] = '-';
                }
            } catch(...) {
                return false;
            }
            if (s >> c)
                return false;

            i++;
            if (line == "") {
                while (i < 7) {
                    for (int j=0; j<7; j++)
                        arr[i][j] = '-';
                    i++;
                }
                break;
            }
        }

        return true;
    }

    void print() const {
        for (int i=0; i<7; i++) {
            for (int j=0; j<7; j++)
                cout << arr[i][j] << ' ';
            cout << endl;
        }
    }

    char valAtPos(const Pos& pos) const {
        return arr[pos.x][pos.y];
    }

    void dfs(const Pos& curPos, const bool vis[7][7], string& curString) const {
        int check = checkString(curString);
        if (check == 2){
            if (words.find(curString) == words.end()) {
                words.insert(curString);
                cout << curString << endl;
            }
        }
        if (check == 0)
            return;

        for (auto delta : dir) {
            Pos newPos(curPos.x+delta[0], curPos.y+delta[1]);
            if (newPos.x < 0 || newPos.x >= 7 || newPos.y < 0 || newPos.y >= 7 )
                continue;
            if (this->valAtPos(newPos) == '-' || vis[newPos.x][newPos.y])
                continue;
            // Else take the new pos (update vis, string, call dfs)
            bool newVis[7][7];
            for (int i=0; i<7; i++)
                for (int j=0; j<7; j++) 
                    newVis[i][j] = vis[i][j];
            newVis[newPos.x][newPos.y] = true;

            string newString = curString + valAtPos(newPos);
            
            this->dfs(newPos, newVis, newString);
        }
    }
};

int main() {
    cout << "-----------------------------------------------\n";
    cout << "Welcome to Squaredle Solver!\n";
    cout << "Enter your puzzle below (up to 7x7):\n";
    cout << "-----------------------------------------------\n";

    loadWords();
    Board board;

    if (!board.takeInput()) {
        cout << "Error in input!\n";
        return 0;
    }
    cout << "Finding Words...\n";

    bool vis[7][7] = {
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };
    string str = "";
    for (int i=0; i<7; i++) {
        for (int j=0; j<7; j++) {
            board.dfs(Pos(i, j), vis, str);
        }
    }

    system("CLS");
    board.print();
    cout << "-----------------------\n";

    cout << "Total: " << words.size() << " words\n";
    cout << "-----------------------\n";
    for (string word : words) {
        cout << word << endl;
    }
    
    cin.get();
}