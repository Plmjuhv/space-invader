#include <iostream>
#include <thread>
#include <vector>
using namespace std;

#include <stdio.h>
#include <windows.h>

#define UNICODE
#define _UNICODE

wstring ship[1];
int nFieldWidth = 100;
int nFieldHeight = 30;
unsigned char *pField = nullptr;
// unsigned char *pEnemyProjectiles = nullptr;
unsigned char *pProjectiles = nullptr;

int nEnemyPositionY = 1;
int nEnemyPositionLeftX = 24;
int nEnemyPositionRightX = 76;
int nEnemyPositionBottomY = 14;
int nShotPosition = 0;
vector<int> vEnemyShotPosition;
int nKillShot = -1;
int nUfoPosition = -1;

int nScreenWidth = 120;
int nScreenHeight = 30;

bool CanMove(int nPosX) {
    for (int px = 0; px < 3; px++) {
        if (pField[nPosX + px] == 15) {
            return false;
        } 
    }

    return true;
}

int main() {
    ship[0].append(L"xXx");
    
    pProjectiles = new unsigned char[nFieldWidth * nFieldHeight];
    unsigned char *begin = pProjectiles;
    unsigned char *end = begin + nFieldWidth * nFieldHeight;
    fill(begin, end, 0);

    pField = new unsigned char[nFieldWidth * nFieldHeight];
    for (int x = 0; x < nFieldWidth; x++) {
        for (int y = 0; y < nFieldHeight; y++) {
            if (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) {
                pField[y * nFieldWidth + x] = 15;
            } else if ((y == 1 || y == 2) && x >= 24 && x <= 76 && x % 5 != 2 && x % 5 != 3) {
                pField[y * nFieldWidth + x] = 3;
            } else if ((y == 4 || y == 5 || y == 7 || y == 8) && x >= 24 && x <= 76 && x % 5 != 2 && x % 5 != 3) {
                pField[y * nFieldWidth + x] = 2;
            } else if ((y == 10 || y == 11 || y == 13 || y == 14) && x >= 24 && x <= 76 && x % 5 != 2 && x % 5 != 3) {
                pField[y * nFieldWidth + x] = 1;
            } else if ((y == 23 || y == 24) && x % 25 >= 8 && x % 25 <= 16) {
                pField[y * nFieldWidth + x] = 6;
            } else if ((y == 25 || y == 26) && ((x % 25 >= 8 && x % 25 <= 9) || (x % 25 >= 15 && x % 25 <= 16))) {
                pField[y * nFieldWidth + x] = 6;
            } else {
                pField[y * nFieldWidth + x] = 0;
            }
        }
    }

    wchar_t *screen  = new wchar_t[nScreenWidth * nScreenHeight];
    for (int i = 0; i < nScreenWidth * nScreenHeight; i++) {
        screen[i] = L' ';
    }

    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    srand((unsigned int)time(NULL));

    bool bGameOver = false;
    bool bDied = false;

    int nCurrentX = 13;
    int nLives = 3;
    int nScore = 0;
    int nLevel = 1;

    bool bKey[3];

    int nSpeed = 20;
    int nEnemyDirection = 1;
    bool bReverseDirection = false;
    int nTurnCounter = 0;
    int nUfoCounter = 0;
    int nUfoSpawn = 0;
    int nUfoDirection = 0;

    while (!bGameOver) {
        // Game Timing =================
        this_thread::sleep_for(30ms);
        nTurnCounter++;
        nUfoCounter++;

        if (nTurnCounter % nSpeed == 0) {
            for (int py = nEnemyPositionBottomY; py >= nEnemyPositionY; py--) {
                if (nEnemyDirection == 1) {
                    for (int px = nEnemyPositionRightX; px >= nEnemyPositionLeftX; px--) {
                        if (nEnemyPositionRightX < nFieldWidth - 2) {
                            pField[py * nFieldWidth + px + 1] = pField[py * nFieldWidth + px];
                            if (rand() % (600 - (nLevel - 1) * 5) == 0 && pField[py * nFieldWidth + px + 1] > 0) {
                                vEnemyShotPosition.push_back(py * nFieldWidth + px + 1);
                                if (pProjectiles[py * nFieldWidth + px + 1] != 9) {
                                    pProjectiles[py * nFieldWidth + px + 1] = 7;
                                } 
                            } 
                            if (pField[py * nFieldWidth + px + 1] >= 10 && pField[py * nFieldWidth + px + 1] < 15) {
                                pField[py * nFieldWidth + px + 1] = 0;
                            }
                        } else {
                            if (rand() % (600 - (nLevel - 1) * 5) == 0 && pField[py * nFieldWidth + px + 1] > 0) {
                                vEnemyShotPosition.push_back(py * nFieldWidth + px + 1);
                                if (pProjectiles[py * nFieldWidth + px + 1] != 9) {
                                    pProjectiles[py * nFieldWidth + px + 1] = 7;
                                } 
                            } 
                            bReverseDirection = true;
                            pField[(py + 1) * nFieldWidth + px] = pField[py * nFieldWidth + px];

                            if (py == nEnemyPositionY) {
                                pField[py * nFieldWidth + px] = 0;
                            }
                            if (pField[py * nFieldWidth + px - 1] >= 10 && pField[py * nFieldWidth + px - 1] < 15) {
                                pField[py * nFieldWidth + px - 1] = 0;
                            }
                        }
                    } 
                } else {
                    for (int px = nEnemyPositionLeftX; px <= nEnemyPositionRightX; px++)
                        if (nEnemyPositionLeftX > 1) {
                            pField[py * nFieldWidth + px - 1] = pField[py * nFieldWidth + px];
                            if (rand() % (600 - (nLevel - 1) * 5) == 0 && pField[py * nFieldWidth + px + 1] > 0) {
                                vEnemyShotPosition.push_back(py * nFieldWidth + px + 1);
                                if (pProjectiles[py * nFieldWidth + px + 1] != 9) {
                                    pProjectiles[py * nFieldWidth + px + 1] = 7;
                                } 
                            } 
                            if (pField[py * nFieldWidth + px - 1] >= 10 && pField[py * nFieldWidth + px - 1] < 15) {
                                pField[py * nFieldWidth + px - 1] = 0;
                            }
                        } else {
                            if (rand() % (600 - (nLevel - 1) * 5) == 0 && pField[py * nFieldWidth + px + 1] > 0) {
                                vEnemyShotPosition.push_back(py * nFieldWidth + px + 1);
                                if (pProjectiles[py * nFieldWidth + px + 1] != 9) {
                                    pProjectiles[py * nFieldWidth + px + 1] = 7;
                                } 
                            } 
                            bReverseDirection = true;
                            pField[(py + 1) * nFieldWidth + px] = pField[py * nFieldWidth + px];

                            if (py == nEnemyPositionY) {
                                pField[py * nFieldWidth + px] = 0;
                            }
                            if (pField[py * nFieldWidth + px - 1] >= 10 && pField[py * nFieldWidth + px - 1] < 15) {
                                pField[py * nFieldWidth + px - 1] = 0;
                            }
                        }
                    }

                if (nEnemyDirection == 1 && !bReverseDirection) {
                    pField[py * nFieldWidth + nEnemyPositionLeftX] = 0;
                } else if (nEnemyDirection != 1 && !bReverseDirection) {
                    pField[py * nFieldWidth + nEnemyPositionRightX] = 0;
                }
            }

            bool bFoundLeft = false;
            bool bFoundRight = false;
            bool bFoundBottom = false;

            for (int x = 1; x < nFieldWidth - 1 && !bFoundLeft; x++) {
                for (int y = 1; y < nFieldHeight - 1 && !bFoundLeft; y++) {
                    if (pField[y * nFieldWidth + x] > 0 && pField[y * nFieldWidth + x] <= 3) {
                        nEnemyPositionLeftX = x;
                        bFoundLeft = true;
                    }
                }
            }

            for (int x = nFieldWidth - 2; x > 0 && !bFoundRight; x--) {
                for (int y = 1; y < nFieldHeight - 1 && !bFoundRight; y++) {
                    if (pField[y * nFieldWidth + x] > 0 && pField[y * nFieldWidth + x] <= 3) {
                        nEnemyPositionRightX = x;
                        bFoundRight = true;
                    }
                }
            }

            for (int y = nFieldHeight - 1; y > 0 && !bFoundBottom; y--) {
                for (int x = nFieldWidth - 2; x > 0 && !bFoundBottom; x--) {
                    if (pField[y * nFieldWidth + x] > 0 && pField[y * nFieldWidth + x] <= 3) {
                        nEnemyPositionBottomY = y;
                        bFoundBottom = true;
                    }
                }
            }

            if (bFoundBottom && nEnemyPositionBottomY == 27) {
                bGameOver = true;
            }

            if (bReverseDirection) {
                nEnemyDirection = (nEnemyDirection == -1) ? 1 : -1;
                nEnemyPositionY++;
                nSpeed -= (nSpeed >= 3) ? 1 : 0;
                bReverseDirection = false;
            }

            if (!bFoundLeft) {
                nLevel++;
                for (int x = 0; x < nFieldWidth; x++) {
                    for (int y = 0; y < nFieldHeight; y++) {
                        if (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) {
                            pField[y * nFieldWidth + x] = 15;
                        } else if ((y == 1 || y == 2) && x >= 24 && x <= 76 && x % 5 != 2 && x % 5 != 3) {
                            pField[y * nFieldWidth + x] = 3;
                        } else if ((y == 4 || y == 5 || y == 7 || y == 8) && x >= 24 && x <= 76 && x % 5 != 2 && x % 5 != 3) {
                            pField[y * nFieldWidth + x] = 2;
                        } else if ((y == 10 || y == 11 || y == 13 || y == 14) && x >= 24 && x <= 76 && x % 5 != 2 && x % 5 != 3) {
                            pField[y * nFieldWidth + x] = 1;
                        } else if ((y == 23 || y == 24) && x % 25 >= 8 && x % 25 <= 16) {
                            pField[y * nFieldWidth + x] = pField[y * nFieldWidth + x];
                        } else if ((y == 25 || y == 26) && ((x % 25 >= 8 && x % 25 <= 9) || (x % 25 >= 15 && x % 25 <= 16))) {
                            pField[y * nFieldWidth + x] = pField[y * nFieldWidth + x];
                        } else {
                            pField[y * nFieldWidth + x] = 0;
                        }
                    }
                }
                nSpeed = (nSpeed >= 20 - nLevel) ? (((20 - nLevel) >= 3) ? 20 - nLevel : 3) : nSpeed + 1;
                nEnemyPositionY = 1;
                nEnemyPositionLeftX = 24;
                nEnemyPositionRightX = 76;
                nEnemyPositionBottomY = 14;
                nEnemyDirection = 1;
                bReverseDirection = false;
                nTurnCounter = 0;
                nUfoCounter = 1;
                nUfoSpawn = 0;
                nUfoDirection = 0;
                nUfoPosition = -1;
            }
        }

        if (nShotPosition != 0) {
            pProjectiles[nShotPosition] = 0;

            nShotPosition = nShotPosition - nFieldWidth;

            if (pField[nShotPosition] <= 3 && pField[nShotPosition] > 0) {
                nScore += 10 * pField[nShotPosition];
                if (pField[nShotPosition] == 3) {
                    nScore += 10;
                }

                int nEnemy = pField[nShotPosition];
                pField[nShotPosition] = 10;
                if (pField[nShotPosition - nFieldWidth] == nEnemy) {
                    if (pField[nShotPosition - 1] == nEnemy && pField[nShotPosition + 1] == nEnemy) {
                        pField[nShotPosition - 1] = 11;
                        pField[nShotPosition + 1] = 12;
                        pField[nShotPosition - nFieldWidth - 1] = 12;
                        pField[nShotPosition - nFieldWidth + 1] = 11;
                        pField[nShotPosition - nFieldWidth] = 13;
                    } else if (pField[nShotPosition - 1] == nEnemy) {
                        pField[nShotPosition - 1] = 14;
                        pField[nShotPosition - 2] = 11;
                        pField[nShotPosition - nFieldWidth] = 11;
                        pField[nShotPosition - nFieldWidth - 1] = 13;
                        pField[nShotPosition - nFieldWidth - 2] = 12;
                    } else {
                        pField[nShotPosition + 1] = 14;
                        pField[nShotPosition + 2] = 12;
                        pField[nShotPosition - nFieldWidth] = 12;
                        pField[nShotPosition - nFieldWidth + 1] = 13;
                        pField[nShotPosition - nFieldWidth + 2] = 11;
                    }

                } else {
                    if (pField[nShotPosition - 1] == nEnemy) {
                        pField[nShotPosition - 1] = 13;
                        pField[nShotPosition - 2] = 12;
                        pField[nShotPosition + nFieldWidth] = 12;
                        pField[nShotPosition + nFieldWidth - 1] = 14;
                        pField[nShotPosition + nFieldWidth - 2] = 11;
                    } else {
                        pField[nShotPosition + 1] = 13;
                        pField[nShotPosition + 2] = 11;
                        pField[nShotPosition + nFieldWidth] = 11;
                        pField[nShotPosition + nFieldWidth + 1] = 14;
                        pField[nShotPosition + nFieldWidth + 2] = 12;
                    }
                }

                nShotPosition = 0;

            } else if (pField[nShotPosition] == 4) {
                pField[nShotPosition] = 10;
                int nUfoScores[5] = {50, 100, 150, 200, 300};
                nScore += nUfoScores[rand() % 5];
                for (int i = 0; i < 5; i++) {
                    if (pField[nUfoPosition + i] == 4) {
                        pField[nUfoPosition + i] = 10;
                    }
                    if (pField[nUfoPosition - i] == 4) {
                        pField[nUfoPosition - i] = 10;
                    }
                }

                nShotPosition = 0;
            } else if (pField[nShotPosition] == 6) {
                pField[nShotPosition]--;
                nShotPosition = 0;
            } else if (pField[nShotPosition] == 5) {
                pField[nShotPosition] = 0;
                nShotPosition = 0;
            } else if (nShotPosition < 0) {
                nShotPosition = 0;
            } else {
                pProjectiles[nShotPosition] = 9;
            }
        }

        if (!vEnemyShotPosition.empty() && nTurnCounter % 3 == 0) {
            for (auto &v : vEnemyShotPosition) {
                if(pProjectiles[v + nFieldWidth] != 9) {
                    pProjectiles[v + nFieldWidth] = 7;
                }
                if (pProjectiles[v] != 9) {
                    pProjectiles[v] = 0;
                }
            }

            for (long long unsigned int i = 0; i < vEnemyShotPosition.size(); i++) {
                if (vEnemyShotPosition[i] + nFieldWidth >= nFieldWidth * 29) {
                    vEnemyShotPosition.erase(vEnemyShotPosition.begin() + i);
                } else {
                    vEnemyShotPosition[i] += nFieldWidth;
                    
                    if (vEnemyShotPosition[i] >= 28 * nFieldWidth + nCurrentX && vEnemyShotPosition[i] <= 28 * nFieldWidth + nCurrentX + 2) {
                        vEnemyShotPosition.clear();
                        unsigned char *begin = pProjectiles;
                        unsigned char *end = begin + nFieldWidth * nFieldHeight;
                        fill(begin, end, 0);

                        nLives--;
                        bDied = true;
                        pField[vEnemyShotPosition[i] - nFieldWidth] = 10;
                        nKillShot = vEnemyShotPosition[i] - nFieldWidth;

                        if (nLives == 0) {
                            bGameOver = true;
                            break;
                        }
                    } else if (pField[vEnemyShotPosition[i]] == 6) {
                        pField[vEnemyShotPosition[i]]--;
                        pProjectiles[vEnemyShotPosition[i]] = 0;
                        vEnemyShotPosition.erase(vEnemyShotPosition.begin() + i);
                    } else if (pField[vEnemyShotPosition[i]] == 5) {
                        pField[vEnemyShotPosition[i]] = 0;
                        pProjectiles[vEnemyShotPosition[i]] = 0;
                        vEnemyShotPosition.erase(vEnemyShotPosition.begin() + i);
                    }
                }
            }
        }

        if (nUfoCounter % (200 - nUfoSpawn) == 0 && nUfoPosition == -1) {
            nUfoPosition = (rand() % 2 == 0) ? 1 : nFieldWidth - 6;
            nUfoDirection = (nUfoPosition == 1) ? 1 : -1;

            for (int i = 0; i < 5; i++) {
                pField[nUfoPosition + i] = 4;
            }
            nUfoSpawn = rand() % 40;
        }

        if (nUfoPosition != -1 && nTurnCounter % 3 == 0) {
            if (pField[nUfoPosition] == 10) {
                for (int i = 0; i < 5; i++) {
                    pField[nUfoPosition + i] = 0;
                }
                nUfoPosition = -1;
            } else {
                nUfoPosition += nUfoDirection;
                if (nUfoDirection == -1) {
                    pField[nUfoPosition + 5] = 0;
                } else {
                    pField[nUfoPosition - 1] = 0;
                }

                if (nUfoPosition <= 1 || nUfoPosition >= nFieldWidth - 6) {
                    for (int i = 0; i < 5; i++) {
                        pField[nUfoPosition + i] = 0;
                    }

                    nUfoPosition = -1;
                } else {
                    for (int i = 0; i < 5; i++) {
                        pField[nUfoPosition + i] = 4;
                    }
                }
            }
        }

        // Input =======================
        for (int k = 0; k < 3; k++) {
            bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x20"[k]))) != 0;
        }

        // Game Logic ==================
        if (bKey[0] && CanMove(nCurrentX + 1)) {
            nCurrentX++;
        }

        if (bKey[1] && CanMove(nCurrentX - 1)) {
            nCurrentX--;
        }

        if (bKey[2] && nShotPosition == 0) {
            nShotPosition = 27 * nFieldWidth + nCurrentX + 1;
            pProjectiles[nShotPosition] = 9;
        }

        // Render Output ===============

        // Draw Field
        for (int x = 0; x < nFieldWidth; x++) {
            for (int y = 0; y < nFieldHeight; y++) {
                int nObjectIndex = (pField[y * nFieldWidth + x] > pProjectiles[y * nFieldWidth + x]) ? pField[y * nFieldWidth + x] : pProjectiles[y * nFieldWidth + x];
                screen[(y) * nScreenWidth + (x + 5)] = L" UVY~12; |*/\\!¡\u2588"[nObjectIndex];
            }
        }

        // Draw Ship
        for (int px = 0; px < 3; px++) {
            if (ship[0][px] == L'x') {
                screen[28 * nScreenWidth + (nCurrentX + px + 5)] = L"m"[0];
            } else {
                screen[28 * nScreenWidth + (nCurrentX + px + 5)] = L"A"[0];
            }
        }

        // Display Game Info
        swprintf_s(&screen[1 * nScreenWidth + nFieldWidth + 6], 14, L"LEVEL: %6d", nLevel);
        swprintf_s(&screen[3 * nScreenWidth + nFieldWidth + 6], 14, L"SCORE: %6d", nScore);
        swprintf_s(&screen[4 * nScreenWidth + nFieldWidth + 5], 15, L"==================");
        swprintf_s(&screen[5 * nScreenWidth + nFieldWidth + 6], 14, L"LIVES: %6d", nLives);

        if (bDied) {
            WriteConsoleOutputCharacterW(hConsole, screen, nScreenWidth * nScreenHeight, {0, 0}, &dwBytesWritten);
            this_thread::sleep_for(400ms);
            pField[nKillShot] = 0;
            nCurrentX = 13;
            bDied = false;

            if (nLives == 0) {
                bGameOver = true;
                break;
            }
        }

        // Display Frame
        WriteConsoleOutputCharacterW(hConsole, screen, nScreenWidth * nScreenHeight, {0, 0}, &dwBytesWritten);
    }

    CloseHandle(hConsole);
    cout << "Game Over!! Score:" << nScore << endl;
    system("pause");

    return 0;
}