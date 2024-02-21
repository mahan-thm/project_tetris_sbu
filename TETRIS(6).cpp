#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <conio.h>
// #include <windows.h>
using namespace std;

int **pos = nullptr;
int **block = nullptr;
int blockDiv[2][4];
int size_block;
string input;
string color(int pos); // رنگ دهی به خانه ها  moon
//___________ساختن جدول____________moon
string head(int satr);
string below(int satr);
string innerLine(int satr);
void structure(int satr, int setoon);
void table(int x_pos, int y_pos);
void blockDeliting(int x_pos, int y_pos);
void blockDividing();
bool check_right_end(int setoon, int y_pos);
bool check_left_end(int setoon, int y_pos);

//__________________________________hamed
int **Craet_Matrix(int size);
int **Random_Block();
int **Rotate(int **block, int size);
void Print_Block(int **block, int size);
bool check_endl(int satr, int x_pos);
void table_back_up(int x_pos, int y_pos, bool firstTime);
bool check_overlap_down(int x_pos, int y_pos);
bool check_overlap_right(int x_pos, int y_pos);
bool check_overlap_left(int x_pos, int y_pos);
void **get_score(int satr, int setoon);
void **delete_line(int line, int satr, int setoon);

int main()
{
    int command;
    int satr, setoon;
    cout << "This is tetris!" << endl;
    cout << "1. New game\n2. How to play\n3. Leaderboard\n4. Exit\n";

    while (true)
    {
        cin >> command;
        if (command == 1)
        {
            cout << "What area do you want ti play?\nGive me a & b, I'll generate an a × b area." << endl;
            cout << "a: ";
            cin >> satr;
            cout << endl
                 << "b: ";
            cin >> setoon;

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

            while (true)
            {
                int y_pos = setoon / 2;
                int x_pos = 0;
                system("cls");
                structure(satr, setoon);
                // Start the timer
                auto start = std::chrono::high_resolution_clock::now();
                block = Random_Block();
                blockDividing();
                while (true)
                {
                    bool whileBreak = false;
                    while (!kbhit())
                    {
                        // Get the time when you want
                        auto end = std::chrono::high_resolution_clock::now();
                        // Calculate the duration
                        std::chrono::duration<double> duration = end - start;

                        // Convert duration to milliseconds
                        std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
                        if (int(ms.count()) > 1000)
                        {
                            blockDeliting(x_pos, y_pos);
                            x_pos++;
                            system("cls");
                            table(x_pos, y_pos);
                            structure(satr, setoon);
                            get_score(satr, setoon);
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
                            cout << endl;
                            // Start the timer
                            start = std::chrono::high_resolution_clock::now();
                        }
                    }
                    
                    if (whileBreak == true)
                    {
                        break;
                    }

                    int direction = getch();
                    system("cls");

                    if (direction == 72) // چرخاندن با کلید بالا
                    {
                        if (!check_endl(satr, x_pos))
                        {
                            if (!check_overlap_down(x_pos, y_pos))
                            {
                                blockDeliting(x_pos, y_pos);
                                block = Rotate(block, size_block);
                                blockDividing();
                                table(x_pos, y_pos);
                                structure(satr, setoon);
                            }
                        }
                        // if (check_endl(satr, x_pos))
                        // {
                        //     break;
                        // }
                        // else if (check_overlap_down(x_pos, y_pos))
                        // {
                        //     break;
                        // }
                    }
                    else if (direction == 80) // پایین آوردن با کلید پایین
                    {
                        blockDeliting(x_pos, y_pos);
                        x_pos++;
                        get_score(satr, setoon);
                        table(x_pos, y_pos);
                        structure(satr, setoon);
                        if (check_endl(satr, x_pos))
                        {
                            break;
                        }
                        else if (check_overlap_down(x_pos, y_pos))
                        {
                            break;
                        }
                    }
                    else if (direction == 77) // به راست بردن با کلید راست
                    {
                        if (!check_right_end(setoon, y_pos))
                        {
                            if (!check_overlap_right(x_pos, y_pos))
                            {
                                blockDeliting(x_pos, y_pos);
                                y_pos++;
                                table(x_pos, y_pos);
                                structure(satr, setoon);
                                continue;
                            }
                        }
                        system("cls");
                        table(x_pos, y_pos);
                        structure(satr, setoon);
                    }
                    else if (direction == 75) // به چپ بردن با کلید چپ
                    {
                        if (!check_left_end(setoon, y_pos))
                        {
                            if (!check_overlap_left(x_pos, y_pos))
                            {
                                blockDeliting(x_pos, y_pos);
                                y_pos--;
                                table(x_pos, y_pos);
                                structure(satr, setoon);
                                continue;
                            }
                        }
                        system("cls");
                        table(x_pos, y_pos);
                        structure(satr, setoon);
                    }
                }
            }
        }
        //        else if (command == 2)
        //        {
        //        }
        //        else if (command == 3)
        //        {
        //        }
        //        else if (command == 4)
        //        {
        //            break;
        //        }
    }
    return 0;
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
    if (size_block == 3)
    {
        for (int j = 0; j < 4; j++)
        {
            if (blockDiv[1][j] + y_pos == setoon)
            {
                return true;
            }
        }
    }
    else if (size_block == 2)
    {
        for (int j = 0; j < 4; j++)
        {
            if (blockDiv[1][j] + y_pos + 1 == setoon)
            {
                return true;
            }
        }
    }
    else if (size_block == 4)
    {
        for (int j = 0; j < 4; j++)
        {
            if (blockDiv[1][j] + y_pos - 1 == setoon)
            {
                return true;
            }
        }
    }
    return false;
}

bool check_left_end(int setoon, int y_pos)
{
    if (size_block == 3)
    {
        for (int j = 0; j < 4; j++)
        {
            if (blockDiv[1][j] + y_pos == 1)
            {
                return true;
            }
        }
    }
    else if (size_block == 2)
    {
        for (int j = 0; j < 4; j++)
        {
            if (blockDiv[1][j] + y_pos + 1 == 1)
            {
                return true;
            }
        }
    }
    else if (size_block == 4)
    {
        for (int j = 0; j < 4; j++)
        {
            if (blockDiv[1][j] + y_pos - 1 == 1)
            {
                return true;
            }
        }
    }
    return false;
}

//___________________moon___________________
string color(int pos)
{
    switch (pos)
    {
    case 0: // خالی
        return "   ";
    case 1: // سفید
        return "\033[47m   \033[0m";
    case 2: // قرمز
        return "\033[41m   \033[0m";
    case 3: // سبز
        return "\033[42m   \033[0m";
    case 4: // زرد
        return "\033[43m   \033[0m";
    case 5: // آبی
        return "\033[44m   \033[0m";
    case 6: // بنفش
        return "\033[45m   \033[0m";
    case 7: // فیروزه ای
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
    result += "\u2514";
    bool firstPrint = false;
    for (int i = 0; i < setoon; i++)
    {
        if (firstPrint == true)
        {
            result += "\u2534";
        }
        for (int z = 0; z < 3; z++)
        {
            result += "\u2500";
        }
        firstPrint = true;
    }
    result += "\u2518";
    return result;
}

string innerLine(int setoon) // خط های وسط جدول
{
    string result = "";
    result += "\u251C";
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
    result += "\u2524";
    return result;
}

void structure(int satr, int setoon) // ساخت جدول
{
    cout << head(setoon);
    bool firstPrint2 = true;
    for (int i = 0; i < satr; i++)
    {
        if (firstPrint2 == false)
        {
            cout << innerLine(setoon);
        }
        bool firstPrint1 = true;
        cout << endl
             << "\u2502";
        for (int j = 0; j < setoon; j++)
        {
            if (firstPrint1 == false)
            {
                cout << "\u2502";
            }
            cout << color(pos[i][j]);
            firstPrint1 = false;
        }
        cout << "\u2502" << endl;
        firstPrint2 = false;
    }
    cout << below(setoon);
}

void table(int x_pos, int y_pos)
{
    if (size_block == 2)
    {
        int z = 0;
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                if (i == blockDiv[0][z] && j == blockDiv[1][z])
                {
                    pos[x_pos + i][y_pos + j] = block[i][j];
                    z++;
                }
            }
        }
    }
    else if (size_block == 3)
    {
        int z = 0;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (i == blockDiv[0][z] && j == blockDiv[1][z])
                {
                    pos[x_pos + i][y_pos + j - 1] = block[i][j];
                    z++;
                }
            }
        }
    }
    else if (size_block == 4)
    {
        int z = 0;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (i == blockDiv[0][z] && j == blockDiv[1][z])
                {
                    pos[x_pos + i][y_pos + j - 2] = block[i][j];
                    z++;
                }
            }
        }
    }
}

void blockDeliting(int x_pos, int y_pos)
{
    if (size_block == 2)
    {
        int z = 0;
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                if (i == blockDiv[0][z] && j == blockDiv[1][z])
                {
                    pos[x_pos + i][y_pos + j] = 0;
                    z++;
                }
            }
        }
    }
    else if (size_block == 3)
    {
        int z = 0;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (i == blockDiv[0][z] && j == blockDiv[1][z])
                {
                    pos[x_pos + i][y_pos + j - 1] = 0;
                    z++;
                }
            }
        }
    }
    else if (size_block == 4)
    {

        int z = 0;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (i == blockDiv[0][z] && j == blockDiv[1][z])
                {
                    pos[x_pos + i][y_pos + j - 2] = 0;
                    z++;
                }
            }
        }
    }
}

void blockDividing()
{
    int b = 0;
    if (size_block == 2)
    {
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                if (block[i][j] != 0)
                {
                    blockDiv[0][b] = i;
                    blockDiv[1][b] = j;
                    b++;
                }
            }
        }
    }
    else if (size_block == 3)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (block[i][j] != 0)
                {
                    blockDiv[0][b] = i;
                    blockDiv[1][b] = j;
                    b++;
                }
            }
        }
    }
    else if (size_block == 4)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (block[i][j] != 0)
                {
                    blockDiv[0][b] = i;
                    blockDiv[1][b] = j;
                    b++;
                }
            }
        }
    }
}

bool check_overlap_down(int x_pos, int y_pos)
{
    if (size_block == 3)
    {
        int j = 2;
        while (true)
        {
            for (int z = 3; 0 <= z; z--)
            {
                for (int i = 2; 0 <= i; i--)
                {
                    if (i == blockDiv[0][z] && j == blockDiv[1][z])
                    {
                        if (pos[x_pos + i + 1][y_pos + j - 1] != 0)
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
    }
    else if (size_block == 2)
    {
        int j = 1;
        while (true)
        {
            for (int z = 3; 0 <= z; z--)
            {
                for (int i = 1; 0 <= i; i--)
                {
                    if (i == blockDiv[0][z] && j == blockDiv[1][z])
                    {
                        if (pos[x_pos + i + 1][y_pos + j] != 0)
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
    }
    else if (size_block == 4)
    {
        int j = 3;
        while (true)
        {
            for (int z = 3; 0 <= z; z--)
            {
                for (int i = 3; 0 <= i; i--)
                {
                    if (i == blockDiv[0][z] && j == blockDiv[1][z])
                    {
                        if (pos[x_pos + i + 1][y_pos + j - 2] != 0)
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
    }
    return false;
}

bool check_overlap_right(int x_pos, int y_pos)
{
    if (size_block == 3)
    {
        int i = 2;
        while (true)
        {
            for (int z = 3; 0 <= z; z--)
            {
                for (int j = 2; 0 <= j; j--)
                {
                    if (i == blockDiv[0][z] && j == blockDiv[1][z])
                    {
                        if (pos[x_pos + i][y_pos + j] != 0)
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
    }
    else if (size_block == 2)
    {
        int i = 1;
        while (true)
        {
            for (int z = 3; 0 <= z; z--)
            {
                for (int j = 2; 0 <= j; j--)
                {
                    if (i == blockDiv[0][z] && j == blockDiv[1][z])
                    {
                        if (pos[x_pos + i][y_pos + j + 1] != 0)
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
    }
    else if (size_block == 4)
    {
        int i = 3;
        while (true)
        {
            for (int z = 3; 0 <= z; z--)
            {
                for (int j = 2; 0 <= j; j--)
                {
                    if (i == blockDiv[0][z] && j == blockDiv[1][z])
                    {
                        if (pos[x_pos + i][y_pos + j - 1] != 0)
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
    }
    return false;
}
bool check_overlap_left(int x_pos, int y_pos)
{
    if (size_block == 3)
    {
        int i = 2;
        while (true)
        {
            for (int z = 3; 0 <= z; z--)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (i == blockDiv[0][z] && j == blockDiv[1][z])
                    {
                        if (pos[x_pos + i][y_pos + j - 1] != 0)
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
    }
    else if (size_block == 2)
    {
        int i = 1;
        while (true)
        {
            for (int z = 3; 0 <= z; z--)
            {
                for (int j = 0; j < 2; j++)
                {
                    if (i == blockDiv[0][z] && j == blockDiv[1][z])
                    {
                        if (pos[x_pos + i][y_pos + j - 2] != 0)
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
    }
    else if (size_block == 4)
    {
        int i = 3;
        while (true)
        {
            for (int z = 3; 0 <= z; z--)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (i == blockDiv[0][z] && j == blockDiv[1][z])
                    {
                        if (pos[x_pos + i][y_pos + j - 3] != 0)
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
    }
    return false;
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

int **Random_Block()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    int random_shape = rand() % 7 + 1;
    // int random_shape = 4;
    int **block;
    switch (random_shape)
    {
    case 1:
        size_block = 4; // یعنی توی چهار در چهار تعریف شده
        block = Craet_Matrix(4);
        block[1][0] = 1;
        block[1][1] = 1;
        block[1][2] = 1;
        block[1][3] = 1;
        return block;
        break;

    case 2:
        size_block = 3;
        block = Craet_Matrix(3);
        block[0][1] = 2;
        block[1][0] = 2;
        block[1][1] = 2;
        block[1][2] = 2;
        return block;
        break;

    case 3:
        size_block = 3;
        block = Craet_Matrix(3);
        block[0][0] = 3;
        block[1][0] = 3;
        block[1][1] = 3;
        block[1][2] = 3;
        return block;
        break;
    case 4:
        size_block = 3;
        block = Craet_Matrix(3);
        block[0][2] = 4;
        block[1][0] = 4;
        block[1][1] = 4;
        block[1][2] = 4;
        return block;
        break;
    case 5:
        size_block = 3;
        block = Craet_Matrix(3);
        block[1][0] = 5;
        block[1][1] = 5;
        block[0][1] = 5;
        block[0][2] = 5;
        return block;
        break;
    case 6:
        size_block = 3;
        block = Craet_Matrix(3);
        block[1][2] = 6;
        block[1][1] = 6;
        block[0][1] = 6;
        block[0][0] = 6;
        return block;
        break;
    case 7:
        size_block = 2;
        block = Craet_Matrix(2);
        block[0][0] = 7;
        block[1][0] = 7;
        block[0][1] = 7;
        block[1][1] = 7;
        return block;
        break;
    }

    return block;
}

int **Rotate(int **block, int size)
{
    switch (size)
    {
    case 2:
    {
        return block;
        break;
    }
    case 3:
    {
        int **new_block = Craet_Matrix(3); // یه ماتریکس جدید میسازم تا ماتریس حاصل از چرخش اون باشه
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (i == 1 and j == 1 and block[i][j] != 0)
                {
                    new_block[i][j] = block[i][j];
                    continue;
                }
                else if ((i + j) % 2 == 0)
                {
                    if (i == 0 and j == 0 and block[i][j] != 0)
                    {
                        new_block[2][0] = block[i][j];
                    }
                    else if (i == 2 and j == 0 and block[i][j] != 0)
                    {
                        new_block[2][2] = block[i][j];
                    }
                    else if (i == 2 and j == 2 and block[i][j] != 0)
                    {
                        new_block[0][2] = block[i][j];
                    }
                    else if (i == 0 and j == 2 and block[i][j] != 0)
                    {
                        new_block[0][0] = block[i][j];
                    }
                }
                else
                {
                    if (i == 0 and j == 1 and block[i][j] != 0)
                    {
                        new_block[1][0] = block[i][j];
                    }
                    else if (i == 1 and j == 0 and block[i][j] != 0)
                    {
                        new_block[2][1] = block[i][j];
                    }
                    else if (i == 2 and j == 1 and block[i][j] != 0)
                    {
                        new_block[1][2] = block[i][j];
                    }
                    else if (i == 1 and j == 2 and block[i][j] != 0)
                    {
                        new_block[0][1] = block[i][j];
                    }
                }
            }
        }
        return new_block;
        break;
    }
    case 4:
    {
        int **new_block = Craet_Matrix(4); // یه ماتریکس جدید میسازم تا ماتریس حاصل از چرخش اون باشه
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (block[0][1] == 1)
                {
                    for (int k = 0; k < size; k++)
                    {
                        new_block[k][1] = 1;
                    }
                }
                else if (block[0][2] == 1)
                {
                    for (int k = 0; k < size; k++)
                    {
                        new_block[k][2] = 1;
                    }
                }
                else if (block[1][0] == 1)
                {
                    for (int k = 0; k < size; k++)
                    {
                        new_block[1][k] = 1;
                    }
                }
                else if (block[2][0] == 1)
                {
                    for (int k = 0; k < size; k++)
                    {
                        new_block[2][k] = 1;
                    }
                }
            }
        }
        return new_block;
        break;
    }
        return block;
    }
}

void **get_score(int satr, int setoon)
{
    bool tf;
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
            delete_line(i, satr, setoon);
        }
    }
}
void **delete_line(int line, int satr, int setoon)
{
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
}

void Print_Block(int **block, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            cout << block[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}