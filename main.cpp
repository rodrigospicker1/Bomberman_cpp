#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <ctime>
#include <stdlib.h>
#include <tuple>
#include <fstream>

int linhas = 15;
int colunas = 15;

using namespace std;

// Função para construir a matriz inicial do jogo
int constroiMatriz(int matriz[15][15], int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            // Definindo bordas e paredes fixas
            if (i == 0 || j == 0 || i == linhas - 1 || j == colunas - 1) {
                matriz[i][j] = 1;
            }
            else {
                if (i % 2 == 0 && j % 2 == 0) {
                    matriz[i][j] = 1;
                }
                else {
                    if ((i == 1 && (j == 1 || j == 2)) || (j == 1 && i == 2) || (i == linhas - 2 && (j == colunas - 3 || j == colunas - 2)) || (i == linhas - 3 && j == colunas - 2)) {
                        matriz[i][j] = 0;
                    }
                    else {
                        // Preenchimento aleatório com paredes frágeis
                        if (rand() % 2 == 1) {
                            matriz[i][j] = 2;
                        }
                        else {
                            matriz[i][j] = 0;
                        }
                    }
                }
            }
        }
    }

    return matriz[15][15];
}
// Função para mostrar a matriz do jogo na tela
int mostraMatriz(int matriz[15][15], int linhas, int colunas, int x, int y) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (i == x && j == y) {
                cout << char(2); // Personagem
            }
            else {
                switch (matriz[i][j]) {
                case 0: cout << " "; break; // Caminho
                case 1: cout << char(219); break; // Parede
                case 2: cout << char(178); break; // Parede frágil
                case 3: cout << char(1); break; // Inimigo
                case 4: cout << char(15); break; // Bomba
                case 5: cout << char(15); break; // Explosão
                case 6: cout << char(1); break; // Inimigo (explodindo)
                case 7: cout << char(15); break; // Bomba (explodindo)
                case 8: cout << char(15); break; // Explosão (fade out)
                }
            }
        }
        cout << "\n";
    }
}

int main() {
    // Configurações de console para ocultar o cursor e reposicionar o cursor
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);

    short int CX = 0, CY = 0;
    COORD coord;
    coord.X = CX;
    coord.Y = CY;

    // Variáveis para medir o tempo e controlar o jogo
    clock_t inicio, fim;
    clock_t inicio_inimigo, fim_inimigo;
    srand(time(NULL));

    char tecla;
    bool tempo = false;
    bool tempo_inimigo = false;
    int x = 1;
    int y = 1;
    int vivo = true;
    int venceu = false;
    int inimigos = 0;
    ifstream mapa;
    mapa.open("./map.text");
    int matriz[15][15];
    for(int i = 0; i < linhas; i++){
        for(int j = 0; j < colunas; j++){
            mapa>>matriz[i][j];
        }
    }
    mostraMatriz(matriz, 15, 15, x, y);
    mapa.close();

    constroiMatriz(matriz, linhas, colunas);

    matriz[linhas - 2][colunas - 2] = 3;
    matriz[1][colunas - 2] = 3;

    while (vivo && !venceu) {
        // Posiciona o cursor no início do console e mostra a matriz
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        mostraMatriz(matriz, 15, 15, x, y);

        // Movimento dos inimigos
        for (int i = 0; i < linhas; i++) {
            for (int j = 0; j < colunas; j++) {
                if (matriz[i][j] == 3) {
                    if (tempo_inimigo == false) {
                        tempo_inimigo = true;
                        inicio_inimigo = clock();
                        // Movimento dos inimigos em direção ao jogador
                        if (i > x && matriz[i - 1][j] == 0) {
                            matriz[i][j] = 0;
                            matriz[i - 1][j] = 3;
                        }
                        else if (j > y && matriz[i][j - 1] == 0) {
                            matriz[i][j] = 0;
                            matriz[i][j - 1] = 3;
                        }
                        else if (i < x && matriz[i + 1][j] == 0) {
                            matriz[i][j] = 0;
                            matriz[i + 1][j] = 3;
                        }
                        else if (j < y && matriz[i][j + 1] == 0) {
                            matriz[i][j] = 0;
                            matriz[i][j + 1] = 3;
                        }
                        else if (i == x && j == y) {
                            matriz[i][j] = 0;
                            matriz[i + 1][j] = 3;
                            vivo = false;
                        }
                        else if (matriz[i][j + 1] == 0) {
                            matriz[i][j] = 0;
                            matriz[i][j + 1] = 3;
                        }
                        else if (matriz[i - 1][j] == 0) {
                            matriz[i][j] = 0;
                            matriz[i - 1][j] = 3;
                        }
                        else if (matriz[i][j - 1] == 0) {
                            matriz[i][j] = 0;
                            matriz[i][j - 1] = 3;
                        }
                        else if (matriz[i + 1][j] == 0) {
                            matriz[i][j] = 0;
                            matriz[i + 1][j] = 3;
                        }
                    }
                }
            }
        }

        //verifica e controla a explosão de inimigos no jogo
        for (int i = 0; i < linhas; i++) {
            for (int j = 0; j < colunas; j++) {
                if (matriz[i][j] == 6) {
                        if (tempo_inimigo == false) {
                        tempo_inimigo = true;
                        inicio_inimigo = clock();
                        if (i > x && matriz[i - 1][j] == 0) {
                            matriz[i][j] = 0;
                            matriz[i - 1][j] = 3;
                        }
                        else if (j > y && matriz[i][j - 1] == 0) {
                            matriz[i][j] = 0;
                            matriz[i][j - 1] = 3;
                        }
                        else if (i < x && matriz[i + 1][j] == 0) {
                            matriz[i][j] = 0;
                            matriz[i + 1][j] = 3;
                        }
                        else if (j < y && matriz[i][j + 1] == 0) {
                            matriz[i][j] = 0;
                            matriz[i][j + 1] = 3;
                        }
                        else if (i == x && j == y) {
                            matriz[i][j] = 0;
                            matriz[i + 1][j] = 3;
                            vivo = false;
                        }
                        else if (matriz[i][j + 1] == 0) {
                            matriz[i][j] = 0;
                            matriz[i][j + 1] = 3;
                        }
                        else if (matriz[i - 1][j] == 0) {
                            matriz[i][j] = 0;
                            matriz[i - 1][j] = 3;
                        }
                        else if (matriz[i][j - 1] == 0) {
                            matriz[i][j] = 0;
                            matriz[i][j - 1] = 3;
                        }
                        else if (matriz[i + 1][j] == 0) {
                            matriz[i][j] = 0;
                            matriz[i + 1][j] = 3;
                        }
                    }
                }
            }
        }
        //verifica se o tempo do inimigo está ativo
        if (tempo_inimigo == true) {
            fim_inimigo = clock();
            if ((fim_inimigo - inicio_inimigo) / CLOCKS_PER_SEC == 1) {
                // Aperece o amarelo no raio de destruição
                tempo_inimigo = false;
            }
        }

        //executa os movimentos
        if (_kbhit()) {
            tecla = getch();
            switch (tecla) {
            case 72: case 'w': ///cima
                if (matriz[x - 1][y] == 0) {
                    x--;
                }
                break;
            case 80: case 's': ///baixo
                if (matriz[x + 1][y] == 0) {
                    x++;
                }
                break;
            case 75:case 'a': ///esquerda
                if (matriz[x][y - 1] == 0) {
                    y--;
                }
                break;
            case 77: case 'd': ///direita
                if (matriz[x][y + 1] == 0) {
                    y++;
                }
                break;
            case 32: case 'space': ///bomba
                if (tempo == false) {
                    matriz[x][y] = 4;
                    inicio = clock();
                    tempo = true;
                }
                break;
            }
        }

        //verifica se o tempo de uma bomba está ativo
        if (tempo == true) {
            fim = clock();
            if ((fim - inicio) / CLOCKS_PER_SEC == 3) {
                // Aperece o amarelo no raio de destruição
                for (int i = 0; i < linhas; i++) {
                    for (int j = 0; j < colunas; j++) {
                        if (matriz[i][j] == 4) {
                            if (matriz[i - 1][j] != 1) {
                                matriz[i - 1][j] = 5;
                            }
                            if (matriz[i + 1][j] != 1) {
                                matriz[i + 1][j] = 5;
                            }
                            if (matriz[i][j - 1] != 1) {
                                matriz[i][j - 1] = 5;
                            }
                            if (matriz[i][j + 1] != 1) {
                                matriz[i][j + 1] = 5;
                            }
                        }
                    }
                }

            }
            if ((fim - inicio) / CLOCKS_PER_SEC == 4) {
                // Volta para o fundo normal
                for (int i = 0; i < linhas; i++) {
                    for (int j = 0; j < colunas; j++) {
                        if (matriz[i][j] == 4 || matriz[i][j] == 5) {
                            matriz[i][j] = 0;
                            if (i == x && j == y) {
                                vivo = false;
                            }
                        }
                    }
                }
                tempo = false;
            }
        }


    } //fim do jogo

    system("cls");
    if (venceu) {
        cout << "Voce venceu";
    }
    else if (!vivo) {
        cout << "Game over";
    }

    return 0;
}
