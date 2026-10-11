#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>
#include <time.h>
#define EMPTY '.'
#define X 'X'
#define O 'O'
int rows, cols, winLength;
char** board;
void clearInput(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}
int readInt(const char* message, int min, int max) {
    int value;
    while (1) {
        printf("%s", message);
        if (scanf("%d", &value) != 1) {
            printf("Ошибка: нужно ввести целое число.\n");
            clearInput();
            continue;
        }
        clearInput();
        if (value < min || value > max) {
            printf("Ошибка: ввести число от %d до %d.\n", min, max);
            continue;
        }
        return value;
    }
}
void clearScreen(void) {
    system("cls");
}
void createBoard(void) {
    int i, j;
    board = (char**)malloc(rows * sizeof(char*));
    if (board == NULL) { printf("Неудалось выделить память.\n"); exit(1); }

    for (i = 0; i < rows; i++) {
        board[i] = (char*)malloc(cols * sizeof(char));
        if (board[i] == NULL) { printf("Неудалось выделить память.\n"); exit(1); }
        for (j = 0; j < cols; j++) { board[i][j] = EMPTY; }
    }
}
void deleteBoard(void) {
    int i;
    for (i = 0; i < rows; i++) { free(board[i]); }
    free(board);
}
void printBoard(void) {
    int i, j;
    printf("\n     ");
    for (j = 0; j < cols; j++) { printf("%3d", j + 1); }
    printf("\n");

    for (i = 0; i < rows; i++) {
        printf("%3d  ", i + 1);
        for (j = 0; j < cols; j++) { printf("%c  ", board[i][j]); }
        printf("\n");
    }
    printf("\n");
}
int isInside(int r, int c) {
    return r >= 0 && r < rows && c >= 0 && c < cols;
}
int checkWin(int r, int c, char player) {
    int directions[4][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };
    int d;
    for (d = 0; d < 4; d++) {
        int dr = directions[d][0];
        int dc = directions[d][1];
        int count = 1;
        int step;
        for (step = 1; step < winLength; step++) {
            int nr = r + dr * step;
            int nc = c + dc * step;
            if (!isInside(nr, nc) || board[nr][nc] != player) { break; }
            count++;
        }
        for (step = 1; step < winLength; step++) {
            int nr = r - dr * step;
            int nc = c - dc * step;
            if (!isInside(nr, nc) || board[nr][nc] != player) { break; }
            count++;
        }
        if (count >= winLength) { return 1; }
    }
    return 0;
}
int isBoardFull(void) {
    int i, j;
    for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
            if (board[i][j] == EMPTY) { return 0; }
    return 1;
}
int findWinningMove(char player, int* out_r, int* out_c) {
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (board[i][j] == EMPTY) {
                board[i][j] = player;
                if (checkWin(i, j, player)) {
                    board[i][j] = EMPTY;
                    *out_r = i;
                    *out_c = j;
                    return 1;
                }
                board[i][j] = EMPTY;
            }
        }
    }
    return 0;
}
void aiMove(int* out_r, int* out_c) {
    int r, c;
    printf("Компьютер думает...\n");
    Sleep(1000);
    if (findWinningMove(O, &r, &c)) {
        board[r][c] = O;
        *out_r = r;
        *out_c = c;
        printf("Компьютер позодил в: %d, %d\n", r + 1, c + 1);
        return;
    }
    if (findWinningMove(X, &r, &c)) {
        board[r][c] = O;
        *out_r = r;
        *out_c = c;
        printf("Компьютер заблокировал ваш ход в: %d, %d\n", r + 1, c + 1);
        return;
    }
    do {
        r = rand() % rows;
        c = rand() % cols;
    } while (board[r][c] != EMPTY);
    board[r][c] = O;
    *out_r = r;
    *out_c = c;
    printf("Компьтер походил в: %d, %d\n", r + 1, c + 1);
}
int main(void) {
    setlocale(LC_ALL, "Russian");
    system("chcp 1251 > nul");
    srand((unsigned int)time(NULL));
    char player = X;
    int gameMode;
    printf("Крестики-нолики\n\n");
    printf("Выберите режим игры:\n");
    printf("1 - Игрок против игрока\n");
    printf("2 - Игрок против компьютера\n");
    gameMode = readInt(Выберите (1 или 2): ", 1, 2);
    printf("\n");
    rows = readInt("Введите число строк (îò 3 äî 50): ", 3, 50);
    cols = readInt("Введите число столбцов (îò 3 äî 50): ", 3, 50);
    if (rows < cols) { winLength = rows; }
    else { winLength = cols; }
    clearScreen(); 
    printf("Поле: %d x %d\n", rows, cols);
    printf("Для победы нужно собрать %d символов подряд.\n", winLength);

    createBoard();
    while (1) {
        int r, c;
        clearScreen();
        printBoard();
        if (player == X) {
            printf("Ход игрока X\n");
            r = readInt("Введите номер строки: ", 1, rows) - 1;
            c = readInt("Введите номер столбца: ", 1, cols) - 1;

            if (board[r][c] != EMPTY) {
                printf("Эта клетка уже занята. Сделайте другой ход.\n");
                Sleep(1500); 
                continue;
            }
            board[r][c] = player;
        }
        else if (player == O) {
            
            if (gameMode == 2) {
                
                aiMove(&r, &c);
            }
            else {
                printf("Ход игрока O\n");
                r = readInt("Введите номер сторки: ", 1, rows) - 1;
                c = readInt("Введите номер столбца: ", 1, cols) - 1;

                if (board[r][c] != EMPTY) {
                    printf("Эта клетка уже занята. Сделайте другой ход.\n");
                    Sleep(1500);
                    continue;
                }
                board[r][c] = player;
            }
        }
        if (checkWin(r, c, player)) {
            clearScreen();
            printBoard();
            printf("Победил игрок %c!\n", player);
            Sleep(3000); 
            clearScreen();
            break;
        }
        if (isBoardFull()) {
            clearScreen();
            printBoard();
            printf("Ничья: все клетки заняты.\n");
            Sleep(3000); 
            clearScreen();     
            break;
        }
        if (player == X) {
            player = O;
        }
        else {
            player = X;
        }
    }
    deleteBoard();
    return 0;
}
