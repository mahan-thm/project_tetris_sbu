#include <chrono>
#include <thread>
#include <stdio.h>
#include <string>
#include <iostream>
#include <string.h>
#include <cstdlib>
#include <stdlib.h>
#include <ctime>
#include <cmath>
#include <conio.h>
#include <iomanip>
#include <windows.h>
#include <typeinfo>
#include <fstream>
#include <algorithm>
using namespace std;
struct player
{
    string name;
    int time;
    int score;
    double level;
};
int Time = 0;
int **pos = nullptr;
int **block = nullptr;
int blockDiv[2][4];
int size_block;
int number = -1;
int score = -10;
bool end = false;
std::chrono::milliseconds ms;
player players[100];
player loosers[100];
string input;
string color(int pos);
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // for_color

int **Table(int satr, int setoon);
int **Craet_Matrix(int size);
int **Random_Block(int &size_block_next);
int **Rotate(int size);
int **array_copy(int **array, int n, int m);

string head(int satr);
string below(int satr);
string innerLine(int satr);

player player_information();
player looser_information();

bool check_right_end(int setoon, int y_pos);
bool check_left_end(int y_pos);
bool check_overlap_down(int x_pos, int y_pos);
bool check_overlap_right(int x_pos, int y_pos);
bool check_overlap_left(int x_pos, int y_pos);
bool check_overlap_rotate(int x_pos, int y_pos, int satr, int setoon);
bool check_end_game(int x_pos, int y_pos);
bool pouse(int satr, int setoon, int x_pos, int y_pos, double level);
bool check_endl(int satr, int x_pos);

void main_menu(int command);
void game(int satr, int setoon, double level);
void structure(int satr, int setoon);
void table(int x_pos, int y_pos, char M_or_S);
void menu(int choice);
void shadow(int x_pos, int y_pos, int satr, int setoon);
void blockDeliting(int x_pos, int y_pos);
void blockDividing();
void RAM_deleting(int **pointer, int n);
void equal_tow_block(int **next_block);
void last_pos_block(bool &whileBreak, int x_pos, int y_pos, auto start, int satr, int setoon, int **next_block, int size_block_next);
void Player_name_Save(string name, double level);
void Player_game_Save(int satr, int setoon, int &number, string name);
void Load_Save(string name, double level);
void Print_Block(int **block, int size, int setoon);
void get_score(int satr, int setoon, int **next_block, int size_block_next);
void MoveCurser(int x, int y);
void setcursor(bool visible, DWORD size);
void looser_name_Save(string name, double level);
void looser_game_Save(int satr, int setoon, int &number, string name);
void looser_Load_Save(string name, double level);
void sort_players(int count);
void sort_loosers(int count);
void delete_line(int line, int satr, int setoon, int **next_block, int size_block_next, int scoreplus);

int main()
{
    int command = 1;
    while (true)
    {
        main_menu(command);
    }
    return 0;
}

void main_menu(int command)
{
    while (true)
    {
        system("cls");
        menu(command);
        char choice = getch();
        if (choice == 77) // راست
        {
            command++;
            if (command == 6)
            {
                command--;
            }
        }
        else if (choice == 75) // چپ
        {
            command--;
            if (command == 0)
            {
                command++;
            }
        }
        else if (choice == 80) // پایین
        {
            command += 2;
            if (command >= 6)
            {
                command -= 2;
            }
        }
        else if (choice == 72) // بالا
        {
            command -= 2;
            if (command <= 0)
            {
                command += 2;
            }
        }

        else if (choice == 13) // enter
        {
            break;
        }
    }
    score = 0;
    Time = 0;
    setcursor(0, 0);
    system("cls");
    int satr, setoon;
    if (command == 1)
    {
        system("cls");
        cout << "level : " << endl
             << "1 : EASY" << endl
             << "2 : MEDIOM " << endl
             << "3 : HARD" << endl
             << "4 : VERY HARD " << endl;
        double level = getch();
        //  PlaySound(TEXT("clicksound1.wav"), NULL, SND_FILENAME | SND_ASYNC);
        cout << "What area do you want to play?\nGive me a & b, I'll generate an a x b area. \na: ";
        cin >> satr;
        cout << endl
             << "b: ";
        cin >> setoon;
        pos = Table(satr, setoon);
        game(satr, setoon, int(level - '0'));
        RAM_deleting(pos, satr);
    }
    else if (command == 2)
    {
        cout << "To play Tetris, you need to move and rotate falling blocks called Tetriminos to create solid horizontal lines without any gaps." << endl;
        cout << "Use the arrow keys to move the Tetriminos left or right, and press the down arrow to make them fall faster. Rotate the Tetriminos" << endl;
        cout << "using the up arrow. The game ends if the blocks reach the top of the playing area. The goal is to clear as many lines as possible" << endl;
        cout << "to keep the game going." << endl;
        cout << "keys work in this game : " 
        << endl
        << "p : pouse " << endl
        << "space : super speed move"
        << endl
        << "enter : select"
        << endl
        << "choose number : choose intemded work "
        << endl;
        cout << "Press any key to get back... " << endl;
        char key = getch();
    }
    else if (command == 3)
    {
        player gamer;
        gamer = player_information();
        Load_Save(gamer.name, gamer.level);
    }
    else if (command == 4)
    {
        player looser;
        looser = looser_information();
        looser_Load_Save(looser.name, looser.level);
    }
    else if (command == 5)
    {
        return;
    }
}

void game(int satr, int setoon, double level)
{
    double dif = 1000 * (1 - (level * 0.2));
    float spf;
    int size_block_next;
    system("cls");
    int **next_block = Random_Block(size_block_next);
    while (true)
    {
        spf = dif * pow(.9, score / 1500);
        MoveCurser(0, 0);
        score += 10;
        int x_pos = 0;
        int y_pos = setoon / 2;
        get_score(satr, setoon, next_block, size_block_next);
        RAM_deleting(block, size_block);
        // Start the timer
        auto start = std::chrono::high_resolution_clock::now();
        size_block = size_block_next;
        dif *= 0.99;
        MoveCurser(0, 0);
        equal_tow_block(next_block);
        next_block = Random_Block(size_block_next);
        Print_Block(next_block, size_block_next, setoon);
        blockDividing();
        if (check_end_game(x_pos, y_pos))
        {
            system("cls");
            cout << "You Lost The Game" << endl;
            string name;
            cout << "choose a name: " << endl;
            cin >> name;
            looser_name_Save(name, level);
            looser_game_Save(satr, setoon, number, name);
            break;
        }
        shadow(x_pos, y_pos, satr, setoon);
        table(x_pos, y_pos, 'M');
        structure(satr, setoon);
        blockDeliting(x_pos, y_pos);
        bool firstTime = true;
        while (true)
        {
            bool whileBreak = false;
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
            if (int(ms.count()) > dif)
            {
                Time += int(ms.count());
                if (check_endl(satr, x_pos))
                {
                    break;
                }
                else if (check_overlap_down(x_pos, y_pos))
                {
                    break;
                }
                blockDeliting(x_pos, y_pos);
                x_pos++;
                MoveCurser(0, 0);
                shadow(x_pos, y_pos, satr, setoon);
                Print_Block(next_block, size_block_next, setoon);
                table(x_pos, y_pos, 'M');
                structure(satr, setoon);
                cout << endl;
                start = std::chrono::high_resolution_clock::now();
            }
            while (!kbhit())
            {
                // Get the time when you want
                auto end = std::chrono::high_resolution_clock::now();
                // Calculate the duration
                std::chrono::duration<double> duration = end - start;
                // Convert duration to milliseconds
                std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
                if (int(ms.count()) > dif)
                {
                    Time += int(ms.count());
                    if (check_endl(satr, x_pos))
                    {
                        whileBreak = true;
                        break;
                    }
                    else if (check_overlap_down(x_pos, y_pos))
                    {
                        whileBreak = true;
                        break;
                    }
                    if (firstTime == false)
                    {
                        blockDeliting(x_pos, y_pos);
                    }
                    blockDeliting(x_pos, y_pos);
                    x_pos++;
                    MoveCurser(0, 0);
                    shadow(x_pos, y_pos, satr, setoon);
                    Print_Block(next_block, size_block_next, setoon);
                    table(x_pos, y_pos, 'M');
                    structure(satr, setoon);
                    cout << endl;
                    // Start the timer
                    start = std::chrono::high_resolution_clock::now();
                    firstTime = false;
                }
            }

            if (whileBreak == true)
            {
                break;
            }
            char direction = '\0';
            direction = getch();
            MoveCurser(0, 0);
            Print_Block(next_block, size_block_next, setoon);

            if (direction == 72) // چرخاندن با کلید بالا
            {
                if (!check_endl(satr, x_pos))
                {
                    if (!check_overlap_down(x_pos, y_pos))
                    {
                        if (check_overlap_rotate(x_pos, y_pos, satr, setoon))
                        {
                            blockDeliting(x_pos, y_pos);
                            block = Rotate(size_block);
                            blockDividing();
                            shadow(x_pos, y_pos, satr, setoon);
                            table(x_pos, y_pos, 'M');
                            structure(satr, setoon);
                        }
                        MoveCurser(0, 0);
                        Print_Block(next_block, size_block_next, setoon);
                        table(x_pos, y_pos, 'M');
                        structure(satr, setoon);
                    }
                }
            }
            else if (direction == 80) // پایین آوردن با کلید پایین
            {
                if (check_endl(satr, x_pos))
                {
                    break;
                }
                else if (check_overlap_down(x_pos, y_pos))
                {
                    break;
                }
                if (firstTime == false)
                {
                    blockDeliting(x_pos, y_pos);
                }
                blockDeliting(x_pos, y_pos);
                x_pos++;
                shadow(x_pos, y_pos, satr, setoon);
                table(x_pos, y_pos, 'M');
                structure(satr, setoon);
                firstTime = false;
            }
            else if (direction == 77) // به راست بردن با کلید راست
            {
                if (!check_right_end(setoon, y_pos))
                {
                    if (!check_overlap_right(x_pos, y_pos))
                    {
                        blockDeliting(x_pos, y_pos);
                        y_pos++;
                        table(x_pos, y_pos, 'M');
                        shadow(x_pos, y_pos, satr, setoon);
                        structure(satr, setoon);
                    }
                }
                MoveCurser(0, 0);
                Print_Block(next_block, size_block_next, setoon);
                table(x_pos, y_pos, 'M');
                structure(satr, setoon);
            }
            else if (direction == 75) // به چپ بردن با کلید چپ
            {
                if (!check_left_end(y_pos))
                {
                    if (!check_overlap_left(x_pos, y_pos))
                    {
                        blockDeliting(x_pos, y_pos);
                        y_pos--;
                        shadow(x_pos, y_pos, satr, setoon);
                        table(x_pos, y_pos, 'M');
                        structure(satr, setoon);
                    }
                }
                MoveCurser(0, 0);
                Print_Block(next_block, size_block_next, setoon);
                table(x_pos, y_pos, 'M');
                structure(satr, setoon);
            }
            else if (direction == 'p')
            {
                blockDeliting(x_pos, y_pos);
                if (!pouse(satr, setoon, x_pos, y_pos, level))
                {
                    return;
                };
            }
            else if (direction == 32)
            {
                last_pos_block(whileBreak, x_pos, y_pos, start, satr, setoon, next_block, size_block_next);
                break;
            }
        }
    }
}

bool pouse(int satr, int setoon, int x_pos, int y_pos, double level)
{
    char command;
    system("cls");
    cout << "1-resume game \n2-save game \n3-exit" << endl;
    command = getch();
    if (command == '2')
    {
        string name;
        cout << "choose a name: ";
        cin >> name;
        Player_name_Save(name, level);
        Player_game_Save(satr, setoon, number, name);
    }
    else if (command == '3')
    {
        return false;
    }
    return true;
}

int **Table(int satr, int setoon)
{
    pos = new int *[satr]; // ساختن ماتریس 2 بعدی برای اعداد توی خانه های جدول
    for (int i = 0; i < satr; i++)
    {
        pos[i] = new int[setoon];
    }

    for (int i = 0; i < satr; i++) // مقدار اولیه صفر دادن به ماتریس
    {
        for (int j = 0; j < setoon; j++)
        {
            pos[i][j] = 0;
        }
    }
    return (pos);
}

//___________________moon___________________
string color(int pos)
{
    switch (pos)
    {
    case 0:           // خالی
        return "🟤 "; // 🟫
    case 1:
        return "\033[37m|||\033[0m";
    case 2:
        return "\033[31m|||\033[0m";
    case 3:
        return "\033[32m|||\033[0m";
    case 4:
        return "\033[33m|||\033[0m";
    case 5:
        return "\033[34m|||\033[0m";
    case 6:
        return "\033[35m|||\033[0m";
    case 7:
        return "\033[36m|||\033[0m";
    case 8:
        return "\u2B50 ";
    case 11: // سفید
        return "\033[47m   \033[0m";
    case 12: // قرمز
        return "\033[41m   \033[0m";
    case 13: // سبز
        return "\033[42m   \033[0m";
    case 14: // زرد
        return "\033[43m   \033[0m";
    case 15: // آبی
        return "\033[44m   \033[0m";
    case 16: // بنفش
        return "\033[45m   \033[0m";
    case 17: // فیروزه ای
        return "\033[46m   \033[0m";
    default:
        break;
    }
}

string head(int setoon) // بالای جدول
{
    string result = "";
    result += "\u250C";
    bool firstPrint = false;
    for (int i = 0; i < setoon; i++)
    {
        if (firstPrint == true)
        {
            result += "\u252C";
        }
        for (int z = 0; z < 3; z++)
        {
            result += "\u2500";
        }
        firstPrint = true;
    }
    result += "\u2510";
    return result;
}

string below(int setoon) // پایین جدول
{
    string result = "";
    result += "\033[33m▙\033[0m";
    bool firstPrint = false;
    for (int i = 0; i < setoon; i++)
    {
        if (firstPrint == true)
        {
            result += "\033[33m▄\033[0m";
        }
        for (int z = 0; z < 3; z++)
        {
            result += "\033[33m▄\033[0m";
        }
        firstPrint = true;
    }
    result += "\033[33m▟\033[0m";
    return result;
}

string innerLine(int setoon) // خط های وسط جدول
{
    string result = "";
    result += "\033[33m▌\033[0m";
    bool count = false;
    for (int i = 0; i < setoon; i++)
    {
        if (count == true)
        {
            result += "\u253C";
        }
        for (int z = 0; z < 3; z++)
        {
            result += "\u2500";
        }
        count = true;
    }
    result += "\033[33m▐\033[0m";
    return result;
}

void structure(int satr, int setoon) // ساخت جدول
{
    // cout << head(setoon);
    bool firstPrint2 = true;
    for (int i = 0; i < satr; i++)
    {
        if (firstPrint2 == false)
        {
            cout << innerLine(setoon);
        }
        bool firstPrint1 = true;
        cout << endl
             << "\033[33m▌\033[0m";
        for (int j = 0; j < setoon; j++)
        {
            if (firstPrint1 == false)
            {
                cout << "\u2502";
            }
            cout << color(pos[i][j]);
            firstPrint1 = false;
        }
        cout << "\033[33m▐\033[0m" << endl;
        firstPrint2 = false;
    }
    cout << below(setoon);
}

void table(int x_pos, int y_pos, char M_or_S) //***************************************
{
    if (M_or_S == 'M')
    {
        int z = 0;
        for (int i = 0; i < size_block; i++)
        {
            for (int j = 0; j < size_block; j++)
            {
                if (i == blockDiv[0][z] && j == blockDiv[1][z])
                {
                    pos[x_pos + i][y_pos + j - (size_block - 2)] = block[i][j];
                    z++;
                }
            }
        }
    }
    else if (M_or_S == 'S')
    {
        int z = 0;
        for (int i = 0; i < size_block; i++)
        {
            for (int j = 0; j < size_block; j++)
            {
                if (i == blockDiv[0][z] && j == blockDiv[1][z])
                {
                    pos[x_pos + i][y_pos + j - (size_block - 2)] = block[i][j] - 10;
                    z++;
                }
            }
        }
    }
}

void shadow(int x_pos, int y_pos, int satr, int setoon) //********************************
{
    int count = 0;
    bool forBreak = false;
    for (int i = satr - 1; 0 <= i; i--)
    {
        for (int j = setoon - 1; 0 <= j; j--)
        {
            int x = pos[i][j];
            if (x < 10 && x != 0)
            {
                pos[i][j] = 0;
                count++;
                if (count == 4)
                {
                    forBreak = true;
                    break;
                }
            }
        }
        if (forBreak)
        {
            break;
        }
    }

    int **pos_copy = array_copy(pos, satr, setoon);
    for (int i = x_pos; i < satr - 1; i++)
    {
        table(i, y_pos, 'M');
        if (check_endl(satr, i))
        {
            pos = array_copy(pos_copy, satr, setoon);
            RAM_deleting(pos_copy, satr);
            table(i, y_pos, 'S');
            return;
        }
        else if (check_overlap_down(i, y_pos))
        {
            pos = array_copy(pos_copy, satr, setoon);
            RAM_deleting(pos_copy, satr);
            table(i, y_pos, 'S');
            return;
        }
    }
}

void menu(int choice) // "\u2573" ╳         //"\u2572" ╲             // "\u2571"╱
{
    string left1, left2, left3, left4, left5;
    switch (choice)
    {
    case 1:
    {
        left1 = "\033[32;1m";
        break;
    }
    case 2:
    {
        left2 = "\033[32;1m";
        break;
    }
    case 3:
    {
        left3 = "\033[32;1m";
        break;
    }
    case 4:
    {
        left4 = "\033[32;1m";
        break;
    }
    case 5:
    {
        left5 = "\033[32;1m";
        break;
    }
    default:
        break;
    }
    string right = "\033[0m";
    cout << left1 << "┏━━━━━━━━━━━━━━━┓" << right << left3 << "                   " << right << left2 << "┏━━━━━━━━━━━━━━━┓" << right << endl;
    cout << left1 << "┃      ╱ ╲      ┃" << right << left3 << "                   " << right << left2 << "┃      ╱ ╲      ┃" << right << endl;
    cout << left1 << "┃     ╱   ╲     ┃" << right << left3 << "                   " << right << left2 << "┃     ╱   ╲     ┃" << right << endl;
    cout << left1 << "┃    ╱     ╲    ┃" << right << left3 << "                   " << right << left2 << "┃    ╱     ╲    ┃" << right << endl;
    cout << left1 << "┃   ╱       ╲   ┃" << right << left3 << "                   " << right << left2 << "┃   ╱       ╲   ┃" << right << endl;
    cout << left1 << "┃  ╱         ╲  ┃" << right << left3 << "                   " << right << left2 << "┃  ╱         ╲  ┃" << right << endl;
    cout << left1 << "┃ ╱           ╲ ┃" << right << left3 << "                   " << right << left2 << "┃ ╱           ╲ ┃" << right << endl;
    cout << left1 << "┃╱    N e w    ╲┃" << right << left3 << "                   " << right << left2 << "┃╱ H o w   t o ╲┃" << right << endl;
    cout << left1 << "┃╲   G a m e   ╱┃" << right << left3 << " ┏━━━━━━━━━━━━━━━┓ " << right << left2 << "┃╲   P l a y   ╱┃" << right << endl;
    cout << left1 << "┃ ╲           ╱ ┃" << right << left3 << " ┃      ╱ ╲      ┃ " << right << left2 << "┃ ╲           ╱ ┃" << right << endl;
    cout << left1 << "┃  ╲         ╱  ┃" << right << left3 << " ┃     ╱   ╲     ┃ " << right << left2 << "┃  ╲         ╱  ┃" << right << endl;
    cout << left1 << "┃   ╲       ╱   ┃" << right << left3 << " ┃    ╱     ╲    ┃ " << right << left2 << "┃   ╲       ╱   ┃" << right << endl;
    cout << left1 << "┃    ╲     ╱    ┃" << right << left3 << " ┃   ╱       ╲   ┃ " << right << left2 << "┃    ╲     ╱    ┃" << right << endl;
    cout << left1 << "┃     ╲   ╱     ┃" << right << left3 << " ┃  ╱         ╲  ┃ " << right << left2 << "┃     ╲   ╱     ┃" << right << endl;
    cout << left1 << "┃      ╲ ╱      ┃" << right << left3 << " ┃ ╱           ╲ ┃ " << right << left2 << "┃      ╲ ╱      ┃" << right << endl;
    cout << left1 << "┗━━━━━━━━━━━━━━━┛" << right << left3 << " ┃╱   L o a d   ╲┃ " << right << left2 << "┗━━━━━━━━━━━━━━━┛" << right << endl;
    cout << left4 << "┏━━━━━━━━━━━━━━━┓" << right << left3 << " ┃╲   G a m e   ╱┃ " << right << left5 << "┏━━━━━━━━━━━━━━━┓" << right << endl;
    cout << left4 << "┃      ╱ ╲      ┃" << right << left3 << " ┃ ╲           ╱ ┃ " << right << left5 << "┃      ╱ ╲      ┃" << right << endl;
    cout << left4 << "┃     ╱   ╲     ┃" << right << left3 << " ┃  ╲         ╱  ┃ " << right << left5 << "┃     ╱   ╲     ┃" << right << endl;
    cout << left4 << "┃    ╱     ╲    ┃" << right << left3 << " ┃   ╲       ╱   ┃ " << right << left5 << "┃    ╱     ╲    ┃" << right << endl;
    cout << left4 << "┃   ╱       ╲   ┃" << right << left3 << " ┃    ╲     ╱    ┃ " << right << left5 << "┃   ╱       ╲   ┃" << right << endl;
    cout << left4 << "┃  ╱         ╲  ┃" << right << left3 << " ┃     ╲   ╱     ┃ " << right << left5 << "┃  ╱         ╲  ┃" << right << endl;
    cout << left4 << "┃ ╱           ╲ ┃" << right << left3 << " ┃      ╲ ╱      ┃ " << right << left5 << "┃ ╱           ╲ ┃" << right << endl;
    cout << left4 << "┃╱  S c o a r  ╲┃" << right << left3 << " ┗━━━━━━━━━━━━━━━┛ " << right << left5 << "┃╱   Q u i t   ╲┃" << right << endl;
    cout << left4 << "┃╲  B o a r d  ╱┃" << right << left3 << "                   " << right << left5 << "┃╲   G a m e   ╱┃" << right << endl;
    cout << left4 << "┃ ╲           ╱ ┃" << right << left3 << "                   " << right << left5 << "┃ ╲           ╱ ┃" << right << endl;
    cout << left4 << "┃  ╲         ╱  ┃" << right << left3 << "                   " << right << left5 << "┃  ╲         ╱  ┃" << right << endl;
    cout << left4 << "┃   ╲       ╱   ┃" << right << left3 << "                   " << right << left5 << "┃   ╲       ╱   ┃" << right << endl;
    cout << left4 << "┃    ╲     ╱    ┃" << right << left3 << "                   " << right << left5 << "┃    ╲     ╱    ┃" << right << endl;
    cout << left4 << "┃     ╲   ╱     ┃" << right << left3 << "                   " << right << left5 << "┃     ╲   ╱     ┃" << right << endl;
    cout << left4 << "┃      ╲ ╱      ┃" << right << left3 << "                   " << right << left5 << "┃      ╲ ╱      ┃" << right << endl;
    cout << left4 << "┗━━━━━━━━━━━━━━━┛" << right << left3 << "                   " << right << left5 << "┗━━━━━━━━━━━━━━━┛" << right << endl;
}

void blockDeliting(int x_pos, int y_pos)
{
    int z = 0;
    for (int i = 0; i < size_block; i++)
    {
        for (int j = 0; j < size_block; j++)
        {
            if (i == blockDiv[0][z] && j == blockDiv[1][z])
            {
                pos[x_pos + i][y_pos + j - (size_block - 2)] = 0;
                z++;
            }
        }
    }
}

void blockDividing()
{
    int b = 0;
    for (int i = 0; i < size_block; i++)
    {
        for (int j = 0; j < size_block; j++)
        {
            if (block[i][j] > 10)
            {
                blockDiv[0][b] = i;
                blockDiv[1][b] = j;
                b++;
            }
        }
    }
}

bool check_endl(int satr, int x_pos)
{
    for (int j = 0; j < 4; j++)
    {
        if (blockDiv[0][j] + x_pos == satr - 1)
        {
            return true;
        }
    }
    return false;
}
bool check_right_end(int setoon, int y_pos)
{
    for (int j = 0; j < 4; j++)
    {
        if (blockDiv[1][j] + y_pos - (size_block - 2) == setoon - 1)
        {
            return true;
        }
    }
    return false;
}
bool check_left_end(int y_pos)
{

    for (int j = 0; j < 4; j++)
    {
        if (blockDiv[1][j] + y_pos - (size_block - 2) == 0)
        {
            return true;
        }
    }
    return false;
}

bool check_overlap_down(int x_pos, int y_pos)
{

    int j = size_block - 1;
    while (true)
    {
        for (int z = 3; 0 <= z; z--)
        {
            for (int i = size_block - 1; 0 <= i; i--)
            {
                if (i == blockDiv[0][z] && j == blockDiv[1][z])
                {
                    if (pos[x_pos + i + 1][y_pos + j - (size_block - 2)] > 10)
                    {
                        return true;
                    }
                    j--;
                    z = 4;
                    break;
                }
            }
        }
        j--;
        if (j < 0)
        {
            break;
        }
    }
    return false;
}
bool check_overlap_right(int x_pos, int y_pos)
{
    // if (size_block == 3 || size_block == 4)
    // {
    int i = size_block - 1;
    while (true)
    {
        for (int z = 3; 0 <= z; z--)
        {
            for (int j = size_block - 1; 0 <= j; j--)
            {
                if (i == blockDiv[0][z] && j == blockDiv[1][z])
                {
                    if (pos[x_pos + i][y_pos + j - (size_block - 3)] > 10)
                    {
                        return true;
                    }
                    i--;
                    z = 4;
                    break;
                }
            }
        }
        i--;
        if (i < 0)
        {
            break;
        }
    }
    return false;
}
bool check_overlap_left(int x_pos, int y_pos)
{
    int i = size_block - 1;
    while (true)
    {
        for (int z = 0; z < 4; z++)
        {
            for (int j = 0; j < size_block; j++)
            {
                if (i == blockDiv[0][z] && j == blockDiv[1][z])
                {
                    if (pos[x_pos + i][y_pos + j - (size_block - 1)] > 10)
                    {
                        return true;
                    }
                    i--;
                    z = -1;
                    break;
                }
            }
        }
        i--;
        if (i < 0)
        {
            break;
        }
    }
    return false;
}
bool check_overlap_rotate(int x_pos, int y_pos, int satr, int setoon)
{
    int **block_copy = array_copy(block, size_block, size_block);
    blockDeliting(x_pos, y_pos);
    block = Rotate(size_block);
    blockDividing();
    for (int z = 0; z < 4; z++)
    {
        for (int i = 0; i < size_block; i++)
        {
            for (int j = 0; j < size_block; j++)
            {
                if (i == blockDiv[0][z] && j == blockDiv[1][z])
                {
                    if (satr <= x_pos + i || x_pos + i < 0 || setoon <= y_pos + j - (size_block - 2) || y_pos + j - (size_block - 2) < 0 || (pos[x_pos + i][y_pos + j - (size_block - 2)] > 10))
                    {
                        block = array_copy(block_copy, size_block, size_block);
                        blockDividing();
                        RAM_deleting(block_copy, size_block);
                        return false;
                    }
                    i = -1;
                    z++;
                    break;
                }
            }
        }
    }
    block = array_copy(block_copy, size_block, size_block);
    RAM_deleting(block_copy, size_block);
    blockDividing();
    return true;
}

//__________________hamed____________________
int **Craet_Matrix(int size)
{
    int **block = new int *[size];
    for (int i = 0; i < size; i++)
    {
        block[i] = new int[size];
    }
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            block[i][j] = 0;
        }
    }
    return block;
}

int **Random_Block(int &size_block_next)
{
    srand(static_cast<unsigned int>(time(nullptr)));
    int random_shape = rand() % 7 + 1;
    // int random_shape = 7;
    int **block;
    switch (random_shape)
    {
    case 1:
        size_block_next = 4; // یعنی توی چهار در چهار تعریف شده
        block = Craet_Matrix(4);
        block[1][0] = 11;
        block[1][1] = 11;
        block[1][2] = 11;
        block[1][3] = 11;
        return block;
        break;

    case 2:
        size_block_next = 3;
        block = Craet_Matrix(3);
        block[0][1] = 12;
        block[1][0] = 12;
        block[1][1] = 12;
        block[1][2] = 12;
        return block;
        break;

    case 3:
        size_block_next = 3;
        block = Craet_Matrix(3);
        block[0][0] = 13;
        block[1][0] = 13;
        block[1][1] = 13;
        block[1][2] = 13;
        return block;
        break;
    case 4:
        size_block_next = 3;
        block = Craet_Matrix(3);
        block[0][2] = 14;
        block[1][0] = 14;
        block[1][1] = 14;
        block[1][2] = 14;
        return block;
        break;
    case 5:
        size_block_next = 3;
        block = Craet_Matrix(3);
        block[1][0] = 15;
        block[1][1] = 15;
        block[0][1] = 15;
        block[0][2] = 15;
        return block;
        break;
    case 6:
        size_block_next = 3;
        block = Craet_Matrix(3);
        block[1][2] = 16;
        block[1][1] = 16;
        block[0][1] = 16;
        block[0][0] = 16;
        return block;
        break;
    case 7:
        size_block_next = 2;
        block = Craet_Matrix(2);
        block[0][0] = 17;
        block[1][0] = 17;
        block[0][1] = 17;
        block[1][1] = 17;
        return block;
        break;
    }

    return block;
}

int **Rotate(int size)
{
    int **new_block = Craet_Matrix(size); // یه ماتریکس جدید میسازم تا ماتریس حاصل از چرخش اون باشه
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            new_block[i][j] = block[j][i];
        }
    }
    for (int i = 0; i < size / 2; i++)
    {
        swap(new_block[i], new_block[(size - 1) - i]);
    }
    return new_block;
}
void get_score(int satr, int setoon, int **next_block, int size_block_next)
{
    bool tf;
    int scoreplus = 10;
    for (int i = satr - 1; 0 <= i; i--)
    {
        tf = true;
        for (int j = 0; j < setoon; j++)
        {
            if (pos[i][j] == 0)
            {
                tf = false;
                break;
            }
        }
        if (tf == true)
        {
            delete_line(i, satr, setoon, next_block, size_block_next, scoreplus); /////////////////////
            i += 1;
            scoreplus += 10;
        }
    }
}
void delete_line(int line, int satr, int setoon, int **next_block, int size_block_next, int scoreplus)
{
    for (int j = 0; j < setoon; j++) // دلیت لاین
    {
        pos[line][j] = 8;
        score += scoreplus;
        Sleep(10);
        MoveCurser(0, 0);
        Print_Block(next_block, size_block_next, setoon);

        structure(satr, setoon);
    }
    for (int j = 0; j < setoon; j++) // دلیت لاین
    {
        pos[line][j] = 0;
    }
    for (int i = line; 0 < i; i--) // شیفتینگ لاین بالا
    {
        for (int j = 0; j < setoon; j++)
        {
            swap(pos[i][j], pos[i - 1][j]);
        }
    }
    return;
}
void Print_Block(int **block, int size, int setoon)
{
    MoveCurser(4 * setoon + 10, 5);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            cout << " "
                 << " ";
        }
        MoveCurser(4 * setoon + 10, 5 + i + 1);
    }
    MoveCurser(4 * setoon + 10, 5);
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (block[i][j] != 0)
            {
                cout << color(block[i][j]);
            }
            else
            {
                cout << "   ";
            }
        }
        cout << "   ";
        MoveCurser(4 * setoon + 10, 5 + i + 1);
    }
    MoveCurser(4 * setoon + 10, 10);
    cout << " Time : " << Time / 1000;
    MoveCurser(4 * setoon + 10, 11);
    cout << "score : " << score;
    MoveCurser(0, 0);
}

void Player_game_Save(int satr, int setoon, int &number, string name)
{
    system("cls");
    number++;
    name += ".txt";
    ofstream outputFile(name); // write informations of players in file
    if (outputFile.is_open())
    {
        outputFile << satr << "," << setoon << endl;
        for (int i = 0; i < satr; i++)
        {
            for (int j = 0; j < setoon; j++)
            {
                outputFile << pos[i][j] << ",";
            }
            outputFile << endl;
        }
        outputFile.close();
    }
    else
    {
        cout << "Failed to open the file." << endl;
    }
}

void Player_name_Save(string name, double level)
{
    ofstream outputFile("players.txt", ios::app); // write informations of players in file
    if (outputFile.is_open())
    {
        outputFile << name << "," << score << "," << Time << "," << level << endl;
        outputFile.close();
    }
    else
    {
        cout << "Failed to open the file." << endl;
    }
}

player player_information()
{
    int num;
    system("cls");

    ifstream file("players.txt");
    if (file.is_open())
    {
        string line;
        int count = 0;
        while (getline(file, line)) // reading informations aboat players .
        {
            int n;
            stringstream ss(line);
            string token;
            n = 1;
            while (std::getline(ss, token, ','))
            {
                if (n == 1)
                    players[count].name = token;
                else if (n == 2)
                    players[count].score = stoi(token);
                else if (n == 3)
                    players[count].time = stoi(token);
                else if (n == 4)
                    players[count].level = stoi(token);
                n++;
            }
            count++;
        }
        double level;
        int j = 0;
        sort_players(count);
        cout << "                                                             "
             << "what you want to play :" << endl
             << "                                                             1 : easy " << endl
             << "                                                             2 : mediom " << endl
             << "                                                             3 : hard " << endl
             << "                                                             4 : very hard " << endl;
        cin >> level;
        system("cls");
        player intended_players[100];
        for (int i = 0; i < count; i++)
        {
            if (players[i].level == level)
            {
                intended_players[j] = players[i];
                cout << "                                                             "
                     << "(" << j + 1 << ")" << setw(15) << players[i].name << setw(5) << players[i].score << setw(5) << players[i].time / 1000 << endl;

                j++;
            }
        }
        cout << "                                                                     choose a save" << endl
             << endl;
        cin >> num;
        score = intended_players[num - 1].score;
        Time = intended_players[num - 1].time;
        count = 0;
        file.close();
        return intended_players[num - 1];
    }
    else
    {
        cerr << "Unable to open the file." << endl;
    }
    return players[num];
}

void Load_Save(string name, double level)
{
    system("cls");
    name += ".txt";
    ifstream file(name);
    if (file.is_open())
    {
        string line;
        int count = -1, f_satr, f_setoon;
        while (getline(file, line)) // reading informations aboat players .
        {
            if (count == -1)
            {
                f_satr = stoi(line.substr(0, line.find(',')));
                f_setoon = stoi(line.substr(line.find(",") + 1, line.length() - (line.find(",") + 1)));
                pos = Table(f_satr, f_setoon);
            }
            else
            {
                int n;
                stringstream ss(line);
                string token;
                n = 0;
                while (std::getline(ss, token, ','))
                {
                    pos[count][n] = stoi(token);
                    n++;
                }
            }
            count++;
        }
        cout << "__________________________________________________________________________";
        game(f_satr, f_setoon, level);
    }
    else
    {
        cerr << "Unable to open the file." << endl;
    }
}

void sort_players(int count)
{
    for (int i = 0; i < count - 1; i++)
    {
        if (players[i].score < players[i + 1].score)
        {
            swap(players[i].name, players[i + 1].name);
            swap(players[i].score, players[i + 1].score);
            swap(players[i].time, players[i + 1].time);
            i = -1;
        }
        else if (players[i].score == players[i + 1].score and players[i].time > players[i + 1].time)
        {
            swap(players[i], players[i + 1]);
            i = -1;
        }
    }
}

void last_pos_block(bool &whileBreak, int x_pos, int y_pos, auto start, int satr, int setoon, int **next_block, int size_block_next)
{
    while (true)
    {
        if (check_endl(satr, x_pos))
        {
            whileBreak = true;
            break;
        }
        else if (check_overlap_down(x_pos, y_pos))
        {
            whileBreak = true;
            break;
        }
        // Get the time when you want
        auto end = std::chrono::high_resolution_clock::now();
        // Calculate the duration
        std::chrono::duration<double> duration = end - start;
        // Convert duration to milliseconds
        std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
        blockDeliting(x_pos, y_pos);
        x_pos++;
        MoveCurser(0, 0);
        Print_Block(next_block, size_block_next, setoon);

        table(x_pos, y_pos, 'M');
        structure(satr, setoon);
        cout << endl;
        // Start the timer
        start = std::chrono::high_resolution_clock::now();
        score += 5;
    }
}

bool check_end_game(int x_pos, int y_pos)
{
    int z = 0;
    for (int i = 0; i < size_block; i++)
    {
        for (int j = 0; j < size_block; j++)
        {
            if (i == blockDiv[0][z] && j == blockDiv[1][z])
            {
                if (pos[x_pos + i][y_pos + j - (size_block - 2)] > 10)
                {
                    return true;
                }
                z++;
                i = -1;
                break;
            }
        }
    }
    return false;
}

void setcursor(bool visible, DWORD size) // set bool visible = 0 - invisible, bool visible = 1 - visible
{
    if (size == 0)
    {
        size = 20; // default cursor size Changing to numbers from 1 to 20, decreases cursor width
    }
    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = size;
    SetConsoleCursorInfo(console, &lpCursor);
}

void MoveCurser(int x, int y)
{
    COORD position = {x, y};
    SetConsoleCursorPosition(console, position);
}

int **array_copy(int **array, int n, int m) // n * m
{
    int **array_copy = nullptr;
    array_copy = new int *[n];
    for (int i = 0; i < n; i++)
    {
        array_copy[i] = new int[m];
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            array_copy[i][j] = array[i][j];
        }
    }
    return array_copy;
}

void RAM_deleting(int **pointer, int n)

{
    for (int i = 0; i < n; i++)
    {
        delete[] pointer[i];
    }
    delete[] pointer;
}

void equal_tow_block(int **next_block)
{
    block = Craet_Matrix(size_block);
    for (int i = 0; i < size_block; i++)
    {
        for (int j = 0; j < size_block; j++)
        {
            block[i][j] = next_block[i][j];
        }
    }
}

void looser_game_Save(int satr, int setoon, int &number, string name)
{
    system("cls");
    number++;
    name += ".txt";
    ofstream outputFile(name); // write informations of players in file
    if (outputFile.is_open())
    {
        outputFile << satr << "," << setoon << endl;
        for (int i = 0; i < satr; i++)
        {
            for (int j = 0; j < setoon; j++)
            {
                outputFile << pos[i][j] << ",";
            }
            outputFile << endl;
        }
        outputFile.close();
    }
    else
    {
        cout << "Failed to open the file." << endl;
    }
}

void looser_name_Save(string name, double level)
{
    ofstream outputFile("loosers.txt", ios::app); // write informations of players in file
    if (outputFile.is_open())
    {
        outputFile << name << "," << score << "," << Time << "," << level << endl;
        outputFile.close();
    }
    else
    {
        cout << "Failed to open the file." << endl;
    }
}

player looser_information()
{
    int num;
    system("cls");

    ifstream file("players.txt");
    if (file.is_open())
    {
        string line;
        int count = 0;
        while (getline(file, line)) // reading informations aboat players .
        {
            int n;
            stringstream ss(line);
            string token;
            n = 1;
            while (std::getline(ss, token, ','))
            {
                if (n == 1)
                    loosers[count].name = token;
                else if (n == 2)
                    loosers[count].score = stoi(token);
                else if (n == 3)
                    loosers[count].time = stoi(token);
                else if (n == 4)
                    loosers[count].level = stoi(token);
                n++;
            }
            count++;
        }
        double level;
        int j = 0;
        sort_loosers(count);
        cout << "                                                             "
             << "what you want to play :" << endl
             << "                                                             1 : easy " << endl
             << "                                                             2 : mediom " << endl
             << "                                                             3 : hard " << endl
             << "                                                             4 : very hard " << endl;
        cin >> level;
        system("cls");
        player intended_loosers[100];
        for (int i = 0; i < count; i++)
        {
            if (loosers[i].level == level)
            {
                intended_loosers[j] = loosers[i];
                cout << "                                                             "
                     << "(" << j + 1 << ")" << setw(15) << loosers[i].name << setw(5) << loosers[i].score << setw(5) << loosers[i].time / 1000 << endl;

                j++;
            }
        }
        cout << "                                                                     choose a save" << endl
             << endl;
        cin >> num;
        score = intended_loosers[num - 1].score;
        Time = intended_loosers[num - 1].time;
        count = 0;
        file.close();
        return intended_loosers[num - 1];
    }
    else
    {
        cerr << "Unable to open the file." << endl;
    }
    return loosers[num];
}

void looser_Load_Save(string name, double level)
{
    system("cls");
    name += ".txt";
    ifstream file(name);
    if (file.is_open())
    {
        system("cls");
        name += ".txt";
        ifstream file(name);
        if (file.is_open())
        {
            string line;
            int count = -1, f_satr, f_setoon;
            while (getline(file, line)) // reading informations aboat players .
            {
                if (count == -1)
                {
                    f_satr = stoi(line.substr(0, line.find(',')));
                    f_setoon = stoi(line.substr(line.find(",") + 1, line.length() - (line.find(",") + 1)));
                    pos = Table(f_satr, f_setoon);
                }
                else
                {
                    int n;
                    stringstream ss(line);
                    string token;
                    n = 0;
                    while (std::getline(ss, token, ','))
                    {
                        pos[count][n] = stoi(token);
                        n++;
                    }
                }
                count++;
            }
            cout << "__________________________________________________________________________";
            structure(f_satr, f_setoon);
        }
        else
        {
            cerr << "Unable to open the file." << endl;
        }
    }
    else
    {
        cerr << "Unable to open the file." << endl;
    }
}

void sort_loosers(int count)
{
    for (int i = 0; i < count - 1; i++)
    {
        if (loosers[i].score < loosers[i + 1].score)
        {
            swap(loosers[i].name, loosers[i + 1].name);
            swap(loosers[i].score, loosers[i + 1].score);
            swap(loosers[i].time, loosers[i + 1].time);
            i = -1;
        }
        else if (loosers[i].score == loosers[i + 1].score and loosers[i].time > loosers[i + 1].time)
        {
            swap(loosers[i], loosers[i + 1]);
            i = -1;
        }
    }
}
