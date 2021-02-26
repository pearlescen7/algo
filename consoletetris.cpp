/*******************************
* Author : Uzay Uysal          *
*                              *            
*******************************/

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <stdio.h>
#include <string>
#include <windows.h>
using namespace std;

wstring tetromino[7];
int FieldWidth = 12;
int FieldHeight = 20;
unsigned char* pField = NULL;

int ScreenWidth = 80;
int ScreenHeight = 30;

//Function that returns index value for the chosen rotation
int Rotate(int px, int py, int r)
{
    switch (abs(r) % 4)
    {
    case 0: return py * 4 + px;         //0 degrees
    case 1: return 12 + py - (px * 4);  //90 degrees
    case 2: return 15 - (py * 4) - px;  //180 degrees
    case 3: return 3 - py + (px * 4);   //270 degrees 
    }

    return 0;
}

//Check if the piece fits for the given rotation and position
bool DoesPieceFit(int id, int Rotation, int PosX, int PosY)
{
    //Check for every cell in a tetromino
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            //Calculate rotated indexes and the position of each cell
            int PieceIndex = Rotate(i, j, Rotation);
            int FieldIndex = (PosY + j) * FieldWidth + (PosX + i);

            //If any cell collides with something other than 0 reject
            if (PosX + i >= 0 && PosX + i < FieldWidth)
            {
                if (PosY + j >= 0 && PosY + j < FieldHeight)
                {
                    if (tetromino[id][PieceIndex] == L'X' && pField[FieldIndex] != 0){ return false; }  
                }
            }   
        }
    }
    
    return true;
}

wchar_t CurrentPieceChar(int id)
{
    switch (id)
    {
    case 0:
        return L'I';
    case 1:
        return L'Z';
    case 2:
        return L'S';
    case 3:
        return L'O';
    case 4:
        return L'T';
    case 5:
        return L'L';
    case 6:
        return L'F';
    }
}

int main(int argc, const char** argv)
{
    //Tetromino assets
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");

    tetromino[1].append(L"..X.");
    tetromino[1].append(L".XX.");
    tetromino[1].append(L".X..");
    tetromino[1].append(L"....");

    tetromino[2].append(L".X..");
    tetromino[2].append(L".XX.");
    tetromino[2].append(L"..X.");
    tetromino[2].append(L"....");

    tetromino[3].append(L"....");
    tetromino[3].append(L".XX.");
    tetromino[3].append(L".XX.");
    tetromino[3].append(L"....");

    tetromino[4].append(L"..X.");
    tetromino[4].append(L".XX.");
    tetromino[4].append(L"..X.");
    tetromino[4].append(L"....");

    tetromino[5].append(L"....");
    tetromino[5].append(L".XX.");
    tetromino[5].append(L"..X.");
    tetromino[5].append(L"..X.");

    tetromino[6].append(L"....");
    tetromino[6].append(L".XX.");
    tetromino[6].append(L".X..");
    tetromino[6].append(L".X..");
    
    const wchar_t GameOverText[] = L"##############################################################################################################========####  GAME  ####  OVER  ####========######################################################################################";
    
    bool Playing = true;
    while(Playing)
    {
        //Creating the playfield buffer
        //Put 9 for the borders, 0 for other cells
        pField = new unsigned char[FieldWidth * FieldHeight];
        for (int i = 0; i < FieldWidth; i++)
        {
            for (int j = 0; j < FieldHeight; j++)
            {
                pField[j * FieldWidth + i] = (i == 0 || i == FieldWidth - 1 || j == FieldHeight - 1) ? 9 : 0;
            }
            
        }


        //Create the screen as a 1d array and fill with blank chars
        wchar_t* screen = new wchar_t[ScreenWidth * ScreenHeight];
        for (int i = 0; i < ScreenWidth * ScreenHeight; i++) { screen[i] = L' '; }

        //Create a new console buffer and get the handle
        HANDLE Console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        
        //Use the handle to set the active buffer
        SetConsoleActiveScreenBuffer(Console);

        //Font settings
        CONSOLE_FONT_INFOEX cfi;
        cfi.cbSize = sizeof(cfi);
        cfi.nFont = 0;
        cfi.dwFontSize.X = 20;                   // Width of each character in the font
        cfi.dwFontSize.Y = 30;                  // Height
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;
        std::wcscpy(cfi.FaceName, L"Consolas");
        SetCurrentConsoleFontEx(Console, TRUE, &cfi);

        //Count the number of bytes written to call write output
        DWORD dwBytesWritten = 0;

        //Game loop
        bool GameOver = false;

        //Screen coordinates
        COORD ScreenCoord;
        ScreenCoord.X = 0;
        ScreenCoord.Y = 0;

        srand(time(NULL));
        int ShuffleCounter = 0;
        int ShuffleArray[14] = {0, 1, 2, 3, 4, 5, 6, 0, 1, 2, 3, 4, 5, 6};
        for (int i = 0; i < 14; i++)
        {
            swap(ShuffleArray[i], ShuffleArray[rand() % 14]);
        }

        int CurrentPiece = ShuffleArray[ShuffleCounter];
        int CurrentRotation = 1;
        int CurrentX = (FieldWidth / 2) - 2;
        int CurrentY = 0;

        bool Keys[5];
        bool RotateHold = false;

        int GameSpeed = 20;
        int SpeedCounter = 0;
        bool ForceDown = false;
        
        int Score = 0;
        int LinesCleared = 0;
        vector<int> Lines;
        
        while (!GameOver)
        {
            //TIMING
            this_thread::sleep_for(chrono::milliseconds(50)); //Ticks
            SpeedCounter++;
            ForceDown = (SpeedCounter == GameSpeed);

            //INPUT
            for (int i = 0; i < 5; i++)
            {                                                         //D  A   S   L    K
                Keys[i] = (0x8000 & GetAsyncKeyState((unsigned char)("\x44\x41\x53\x4C\x4B"[i]))) != 0;
            }

            //LOGIC
            CurrentX += (Keys[0] && DoesPieceFit(CurrentPiece, CurrentRotation, CurrentX + 1, CurrentY)) ? 1 : 0;
            CurrentX -= (Keys[1] && DoesPieceFit(CurrentPiece, CurrentRotation, CurrentX - 1, CurrentY)) ? 1 : 0;
            CurrentY += (Keys[2] && DoesPieceFit(CurrentPiece, CurrentRotation, CurrentX, CurrentY + 1)) ? 1 : 0;
            if(Keys[3])
            {
                //Wall kick
                if(!RotateHold && !DoesPieceFit(CurrentPiece, CurrentRotation, CurrentX - 1, CurrentY))
                {
                    CurrentX += DoesPieceFit(CurrentPiece, CurrentRotation, CurrentX + 2, CurrentY) ? 2 : 0;
                } else if (!RotateHold && !DoesPieceFit(CurrentPiece, CurrentRotation, CurrentX + 1, CurrentY))
                {
                    CurrentX -= DoesPieceFit(CurrentPiece, CurrentRotation, CurrentX - 2, CurrentY) ? 2 : 0;
                } else if (!RotateHold && !DoesPieceFit(CurrentPiece, CurrentRotation, CurrentX, CurrentY + 1))
                {
                    CurrentY -= DoesPieceFit(CurrentPiece, CurrentRotation, CurrentX, CurrentY - 1) ? 1 : 0;
                }
                CurrentRotation += (!RotateHold && DoesPieceFit(CurrentPiece, CurrentRotation + 1, CurrentX, CurrentY)) ? 1 : 0;
                RotateHold = true;
            } else if (!(Keys[3] || Keys[4])){ RotateHold = false; }
            
            if(Keys[4])
            {
                //Wall kick
                if(!RotateHold && !DoesPieceFit(CurrentPiece, CurrentRotation, CurrentX - 1, CurrentY))
                {
                    CurrentX += DoesPieceFit(CurrentPiece, CurrentRotation, CurrentX + 2, CurrentY) ? 2 : 0;
                } else if (!RotateHold && !DoesPieceFit(CurrentPiece, CurrentRotation, CurrentX + 1, CurrentY))
                {
                    CurrentX -= DoesPieceFit(CurrentPiece, CurrentRotation, CurrentX - 2, CurrentY) ? 2 : 0;
                } else if (!RotateHold && !DoesPieceFit(CurrentPiece, CurrentRotation, CurrentX, CurrentY + 1))
                {
                    CurrentY -= DoesPieceFit(CurrentPiece, CurrentRotation, CurrentX, CurrentY - 1) ? 1 : 0;
                }
                CurrentRotation -= (!RotateHold && DoesPieceFit(CurrentPiece, CurrentRotation - 1, CurrentX, CurrentY)) ? 1 : 0;
                RotateHold = true;
            } else if (!(Keys[3] || Keys[4])){ RotateHold = false; }

            if(ForceDown)
            {
                if(DoesPieceFit(CurrentPiece, CurrentRotation, CurrentX, CurrentY + 1))
                {
                    CurrentY += 1;
                } else
                {
                    //Lock the piece
                    for (int i = 0; i < 4; i++)
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            if(tetromino[CurrentPiece][Rotate(i, j, CurrentRotation)] == L'X')

                                pField[(CurrentY + j ) * FieldWidth + (CurrentX + i)] = CurrentPiece + 1;
                        }
                        
                    }
                    
                    //Check for lines
                    for (int i = 0; i < 4; i++)
                    {
                        if (CurrentY + i < FieldHeight - 1)
                        {
                            bool LineExist = true;
                            for (int j = 1; j < FieldWidth - 1; j++)
                            {
                                LineExist &= (pField[(CurrentY + i) * FieldWidth + j]) != 0;   
                            }

                            if(LineExist)
                            {
                                //Cue visual effect
                                for (int j = 1; j < FieldWidth - 1; j++)
                                {
                                    pField[(CurrentY + i) * FieldWidth + j] = 8;
                                }
                                Lines.push_back(CurrentY + i);
                                LinesCleared++; 
                            }
                        }
                    }
                    if(LinesCleared % 5 == 0)
                        if (GameSpeed >= 10) GameSpeed--;

                    Score += 25;
                    if (!Lines.empty()) Score += (1 << Lines.size()) * 100;

                    //Spawn next piece
                    CurrentX = (FieldWidth / 2) - 2;
                    CurrentY = 0;
                    CurrentRotation = 1;
                    // Using two bags to randomize
                    if(ShuffleCounter == 13)
                    {
                        for (int i = 0; i < 14; i++)
                        {
                            swap(ShuffleArray[i], ShuffleArray[rand() % 14]);
                        }
                        ShuffleCounter = 0;  
                    } else { ShuffleCounter++; }
                    CurrentPiece = ShuffleArray[ShuffleCounter];
                    //Game over!
                    GameOver = !DoesPieceFit(CurrentPiece, CurrentRotation, CurrentX, CurrentY);
                }
                SpeedCounter = 0;
            }
            

            //OUTPUT

            //Draw the field
            for (int i = 0; i < FieldWidth; i++)
            {
                for (int j = 0; j < FieldHeight; j++)
                {
                    screen[(j + 2) * ScreenWidth + (i + 2)] = L" IZSOTLF=#"[pField[j * FieldWidth + i]];
                }
                
            }

            //Draw current piece
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    if(tetromino[CurrentPiece][Rotate(i, j, CurrentRotation)] == L'X')
                    { 
                        screen[(CurrentY + j + 2) * ScreenWidth + (CurrentX + i + 2)] = CurrentPieceChar(CurrentPiece);
                    }
                }
                
            }
            
            //Print score
            swprintf(&screen[2 * ScreenWidth + FieldWidth + 6], 16, L"SCORE: %8d", Score);

            if(!Lines.empty())
            {
                WriteConsoleOutputCharacterW(Console, screen, ScreenWidth * ScreenHeight, ScreenCoord, &dwBytesWritten);
                this_thread::sleep_for(chrono::milliseconds(400));

                for (auto &v : Lines)
                {
                    for (int i = 1; i < FieldWidth - 1; i++)
                    {
                        for (int j = v; j > 0; j--)
                        {
                            pField[j * FieldWidth + i] = pField[(j - 1) * FieldWidth + i];
                            
                        }
                        pField[i] = 0;
                    }
                }
                Lines.clear();
            }

            //Display the current frame
            WriteConsoleOutputCharacterW(Console, screen, ScreenWidth * ScreenHeight, ScreenCoord, &dwBytesWritten);

        }

        for (int i = 0; i < FieldWidth; i++)
        {
            for (int j = 0; j < FieldHeight; j++)
            {
                this_thread::sleep_for(chrono::milliseconds(20));
                screen[(j + 2) * ScreenWidth + (i + 2)] = GameOverText[j * FieldWidth + i];
                WriteConsoleOutputCharacterW(Console, screen, ScreenWidth * ScreenHeight, ScreenCoord, &dwBytesWritten);
            }
        }
        this_thread::sleep_for(chrono::milliseconds(1000));
        CloseHandle(Console);

        SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &cfi);
        cout << "Game Over! Score: " << Score << endl;
        cout << "Press the esc key to clear the screen. Would you like to play again?(Y/N)" << endl;

        string NewGame = "N";
        getline(cin, NewGame);
        while((NewGame != "Y") && (NewGame != "y") && (NewGame != "N") && (NewGame != "n"))
        {
            cout << "Ah come on, shouldn't be that hard to write a single character correctly-\nI'll give you another chance." << endl;
            getline(cin, NewGame);
        }
        if((NewGame == "Y") || (NewGame == "y"))
        {
            system("cls");
            GameOver = false;
            Playing = true;
        } else { Playing = false; }
    }
    system("pause");
    return 0;
}