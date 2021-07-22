//
//  main.cpp
//  Germ_War
//
//  Created by 세광 on 2021/07/21.
//

#include <iostream>
#include <termios.h>
#include <unistd.h>
#define none 0
#define player1 1
#define player2 2

using namespace std;

class Data {
private:
    unsigned int score_player1;
    unsigned int score_player2;
    unsigned int cursor_player1[2];
    unsigned int cursor_player2[2];
    unsigned int cordinate[2];
    
    friend class Germ_War;
};

class Germ_War {
public:
    Germ_War();
    ~Germ_War();
    
    void Print_Screen();
    void Cursor_Move();
    void Get_Area_and_Score();
    void Final_Score();
    
private:
    int getch();
    int End_Condition(int);
    
    unsigned int **matrix;
    unsigned int row, col;
    unsigned int order;
    bool space;
    Data *data;
};

int main() {
    char y_n;
    while (y_n != 'n') {
        Germ_War *game = new Germ_War;
        game->Print_Screen();
        game->Cursor_Move();
        cout << "Continue? (y/n) "; cin >> y_n;
        if (y_n == 'y') system("clear");
        delete game;
    }
}

Germ_War::Germ_War() : order(player1), space(false) {
    while (!((4 <= row && row <= 20) && (4 <= col && col <= 20))) {
        cout << "[ATAXX]" << endl;
        cout << "row: "; cin >> row;
        cout << "col: "; cin >> col;
        getchar();
        system("clear");
    }
    
    data = new Data;
    matrix = new unsigned int*[row];
    for (size_t i = 0; i < row; i++) {
        matrix[i] = new unsigned int[col];
        memset(matrix[i], none, sizeof(int)*col);
    }
    
    data->score_player1 = 2;
    data->score_player2 = 2;
    data->cursor_player1[0] = 0;
    data->cursor_player1[1] = 0;
    data->cursor_player2[0] = row - 1;
    data->cursor_player2[1] = col - 1;
    matrix[0][0] = player1;
    matrix[0][col - 1] = player1;
    matrix[row - 1][0] = player2;
    matrix[row - 1][col - 1] = player2;
}

Germ_War::~Germ_War() {
    for (size_t i = 0; i < row; i++) delete matrix[i];
    delete [] matrix;
    delete data;
}

void Germ_War::Print_Screen() {
    cout << "[Player 1] " << data->score_player1 << " : " << data->score_player2 << " [Player 2]" << endl;
    if (order == player1) cout << '\n' << "[Player 1's Turn]" << endl;
    else cout << '\n' << "[Player 2's Turn]" << endl;
    for (size_t i = 0; i < row; i++) {
        cout << "+";
        for (size_t j = 0; j < col; j++) cout << "---+";
        cout << endl;
        cout << "| ";
        for (size_t k = 0; k < col; k++) {
            if (matrix[i][k] == player1) {
                if (i == data->cursor_player1[0] && k == data->cursor_player1[1] && order == player1) {
                    if (i == data->cordinate[0] && k == data->cordinate[1] && space == true) cout << "√";
                    else if (End_Condition(none) == true || End_Condition(player1) == true || End_Condition(player2) == true) cout << "○";
                    else cout << "◇";
                }
                else if (i == data->cordinate[0] && k == data->cordinate[1] && space == true) cout << "√";
                else cout << "○";
            }
            else if (matrix[i][k] == player2) {
                if (i == data->cursor_player2[0] && k == data->cursor_player2[1] && order == player2) {
                    if (i == data->cordinate[0] && k == data->cordinate[1] && space == true) cout << "√";
                    else if (End_Condition(none) == true || End_Condition(player1) == true || End_Condition(player2) == true) cout << "●";
                    else cout << "◆";
                }
                else if (i == data->cordinate[0] && k == data->cordinate[1] && space == true) cout << "√";
                else cout << "●";
            }
            else {
                if (i == data->cursor_player1[0] && k == data->cursor_player1[1] && order == player1) cout << "_";
                else if (i == data->cursor_player2[0] && k == data->cursor_player2[1] && order == player2) cout << "_";
                else cout << " ";
            }
            cout << " | ";
        }
        cout << endl;
    }
    cout << "+";
    for (size_t i = 0; i < col; i++) cout << "---+";
    cout << endl;
    
    if (End_Condition(none) == true || End_Condition(player1) == true || End_Condition(player2) == true) Final_Score();
    else Cursor_Move();
}

void Germ_War::Cursor_Move() {
    char move = getch();
    system("clear");
    switch (move) {
        case 'i':
            if (order == player1) { if (data->cursor_player1[0] != 0) data->cursor_player1[0] -= 1; }
            else { if (data->cursor_player2[0] != 0) data->cursor_player2[0] -= 1; }
            break;
            
        case 'j':
            if (order == player1) { if (data->cursor_player1[1] != 0) data->cursor_player1[1] -= 1; }
            else { if (data->cursor_player2[1] != 0) data->cursor_player2[1] -= 1; }
            break;
            
        case 'k':
            if (order == player1) { if (data->cursor_player1[0] != row - 1) data->cursor_player1[0] += 1; }
            else { if (data->cursor_player2[0] != row - 1) data->cursor_player2[0] += 1; }
            break;
            
        case 'l':
            if (order == player1) { if (data->cursor_player1[1] != col - 1) data->cursor_player1[1] += 1; }
            else { if (data->cursor_player2[1] != col - 1) data->cursor_player2[1] += 1; }
            break;
            
        case ' ':
            if (space == false) {
                if (order == player1) {
                    if (matrix[data->cursor_player1[0]][data->cursor_player1[1]] == player1) {
                        space = true;
                        data->cordinate[0] = data->cursor_player1[0];
                        data->cordinate[1] = data->cursor_player1[1];
                    }
                }
                else {
                    if (matrix[data->cursor_player2[0]][data->cursor_player2[1]] == player2) {
                        space = true;
                        data->cordinate[0] = data->cursor_player2[0];
                        data->cordinate[1] = data->cursor_player2[1];
                    }
                }
            }
            else {
                if (order == player1) {
                    if (matrix[data->cursor_player1[0]][data->cursor_player1[1]] == player1) {
                        data->cordinate[0] = data->cursor_player1[0];
                        data->cordinate[1] = data->cursor_player1[1];
                    }
                    else if (!(matrix[data->cursor_player1[0]][data->cursor_player1[1]] == player2)) {
                        if (abs(int(data->cordinate[0] - data->cursor_player1[0])) <= 1 && abs(int(data->cordinate[1] - data->cursor_player1[1])) <= 1) {
                            matrix[data->cursor_player1[0]][data->cursor_player1[1]] = player1;
                            data->score_player1++;
                        }
                        else {
                            matrix[data->cordinate[0]][data->cordinate[1]] = none;
                            matrix[data->cursor_player1[0]][data->cursor_player1[1]] = player1;
                        }
                        Get_Area_and_Score();
                        order = player2;
                        space = false;
                    }
                }
                else {
                    if (matrix[data->cursor_player2[0]][data->cursor_player2[1]] == player2) {
                        data->cordinate[0] = data->cursor_player2[0];
                        data->cordinate[1] = data->cursor_player2[1];
                    }
                    else if (!(matrix[data->cursor_player2[0]][data->cursor_player2[1]] == player1)) {
                        if (abs(int(data->cordinate[0] - data->cursor_player2[0])) <= 1 && abs(int(data->cordinate[1] - data->cursor_player2[1])) <= 1) {
                            matrix[data->cursor_player2[0]][data->cursor_player2[1]] = player2;
                            data->score_player2++;
                        }
                        else {
                            matrix[data->cordinate[0]][data->cordinate[1]] = none;
                            matrix[data->cursor_player2[0]][data->cursor_player2[1]] = player2;
                        }
                        Get_Area_and_Score();
                        order = player1;
                        space = false;
                    }
                }
            }
            break;
            
        default:
            break;
    }
    Print_Screen();
}

void Germ_War::Get_Area_and_Score() {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (order == player1) {
                if ((0 <= data->cursor_player1[0] + i && data->cursor_player1[0] + i <= row - 1) && (0 <= data->cursor_player1[1] + j && data->cursor_player1[1] + j <= col - 1)) {
                    if (matrix[int(data->cursor_player1[0] + i)][int(data->cursor_player1[1] + j)] == player2) {
                        matrix[int(data->cursor_player1[0] + i)][int(data->cursor_player1[1] + j)] = player1;
                        data->score_player1++;
                        data->score_player2--;
                    }
                }
            }
            else {
                if ((0 <= data->cursor_player2[0] + i && data->cursor_player2[0] + i <= row - 1) && (0 <= data->cursor_player2[1] + j && data->cursor_player2[1] + j <= col - 1)) {
                    if (matrix[int(data->cursor_player2[0] + i)][int(data->cursor_player2[1] + j)] == player1) {
                        matrix[int(data->cursor_player2[0] + i)][int(data->cursor_player2[1] + j)] = player2;
                        data->score_player2++;
                        data->score_player1--;
                    }
                }
            }
        }
    }
}

void Germ_War::Final_Score() {
    if (data->score_player1 > data->score_player2) cout << '\n' << "Player 1 wins!" << endl;
    else if (data->score_player1 < data->score_player2) cout << '\n' << "Player 2 wins!" << endl;
    else cout << '\n' << "Draw!" << endl;
    cout << "Press any key.." << endl;
}

int Germ_War::getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int Germ_War::End_Condition(int n) {
    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            if (matrix[i][j] == n) return false;
        }
    }
    return true;
}
