/*
Создайте игру «Память».
Правила игры на английском языке можно прочесть по этой ссылке:
    https://www.memozor.com/memory-games/memory-game-rules.
Главные особенности реализации игры:
    ■ Выбор размера поля.
    ■ Начальная расстановка карт выполняется компьютером.
    ■ Звуковое сопровождение игры:
        • Если карты совпали, звучит один звук;
        • Если карты не совпали, звучит другой звук.
    ■ Статистика:
        • Время игры;
        • Количество переворотов карт.
*/
#include <iostream>
#include <ctime>
#include <Windows.h>
#include <chrono>
using namespace std;

class GameField
{
private:
    int fieldSizeX, fieldSizeY;
    char** GameFieldArr;
    char** ToRender;
    int cardRange;

public:

    GameField(int sizeX, int sizeY)
    {
        fieldSizeX = sizeX;
        fieldSizeY = sizeY;
        cardRange = sizeX * sizeY/2; //number of unique cards

        GameFieldArr = new char* [fieldSizeX];
        for (int i = 0; i < fieldSizeX; i++)
        {
            GameFieldArr[i] = new char[fieldSizeY];
        }
        for (int i = 0; i < fieldSizeX; i++)
        {
            for (int j = 0; j < fieldSizeY; j++)
            {
                GameFieldArr[i][j] = char(254);
            }
        }

        ToRender = new char* [fieldSizeX];
        for (int i = 0; i < fieldSizeX; i++)
        {
            ToRender[i] = new char[fieldSizeY];
        }
        for (int i = 0; i < fieldSizeX; i++)
        {
            for (int j = 0; j < fieldSizeY; j++)
            {
                ToRender[i][j] = char(254);
            }
        }
    }

    ~GameField()
    {
        for (int i = 0; i < fieldSizeX; i++)
        {
            delete[] GameFieldArr[i];
            delete[] ToRender[i];
        }
        delete[] GameFieldArr;
        delete[] ToRender;
    }

    void ShuffleCards()
    {
        int step = 0;
        srand(time(0));
        int startingPoint = rand() % (220-cardRange) + 32;
        for (int i = 0; i < fieldSizeX; i++)
        {
            for (int j = 0; j < fieldSizeY; j+=2)
            {
                char toAdd = char(startingPoint+step);
                GameFieldArr[i][j] = toAdd;
                GameFieldArr[i][j + 1] = toAdd;
                step++;
            }
        }
        //swap random card with another random card
        for (int i = 0; i < 25; i++)
        {
            swap(GameFieldArr[rand() % fieldSizeX][rand() % fieldSizeY], GameFieldArr[rand() % fieldSizeX][rand() % fieldSizeY]);
        }
    }

    bool Check2Cards(int x1, int y1, int x2, int y2)
    {
        return (GameFieldArr[x1][y1]==GameFieldArr[x2][y2]);
    }

    bool CheckWholeField()
    {
        for (int i = 0; i < fieldSizeX; i++)
        {
            for (int j = 0; j < fieldSizeY; j++)
            {
                if (ToRender[i][j] == char(254))
                {
                    return false;
                }
            }
        }

        return true;
    }

    void Reveal2Cards(int x1, int y1, int x2, int y2)
    {
        ToRender[x1][y1] = GameFieldArr[x1][y1];
        ToRender[x2][y2] = GameFieldArr[x2][y2];
    }

    void Hide2Cards(int x1, int y1, int x2, int y2)
    {
        ToRender[x1][y1] = char(254);
        ToRender[x2][y2] = char(254);
    }

    void Render()
    {
        cout << "  ";
        for (int i = 0; i < fieldSizeY; i++)
            cout << " " << i << " ";
        cout << endl;
        for (int i = 0; i < fieldSizeX; i++)
        {
            cout << i << " ";
            for (int j = 0; j < fieldSizeY; j++)
            {
                cout << '[' << ToRender[i][j] << ']';
            }
            cout << endl;
        }
    }

};

int main()
{
    //game setup
    int inputX, inputY, steps = 0;
    do
    {
        cout << "Welcome to Memory Game"
            << endl << "Enter size of the field (2 integers) to start" << endl;
        cin >> inputX >> inputY;
    } while ((inputX%2!=0)||(inputY%2!=0));

    GameField* TheGame = new GameField(inputX, inputY);
    TheGame->ShuffleCards();
    TheGame->Render();
    //gameplay cycle
    auto start_time = chrono::high_resolution_clock::now();
    while (true)
    {
        system("CLS");
        cout << "Memory Game by Lev_Sychev" << endl;
        TheGame->Render();
        if (TheGame->CheckWholeField() == true)
        {
            break;
        }
        int x1, y1, x2, y2;
        do {
            cout << "input coordinates of cards to reveal (4 integers)" << endl;
            cin >> x1 >> y1 >> x2 >> y2;
        } while ((x1 >= inputX) || (y1 >= inputY) || (x2 >= inputX) || (y2 >= inputY));

        TheGame->Reveal2Cards(x1, y1, x2, y2);
        steps++;
        if (TheGame->Check2Cards(x1, y1, x2, y2)==false)
        {
            Beep(217, 200);
            system("CLS");
            cout << "Memory Game by Lev_Sychev" << endl;
            TheGame->Render();
            cout << "input coordinates of cards to reveal (4 integers)" << endl;
            Sleep(1500);
            TheGame->Hide2Cards(x1, y1, x2, y2);
        }
        else
        {
            Beep(417, 200);
        }
    }
    //output results

    auto end_time = chrono::high_resolution_clock::now();
    cout << "Well done! Only took " << steps << " steps and " 
        << chrono::duration_cast<chrono::seconds>(end_time-start_time).count() 
        << " seconds" << endl;

    return 0;
}