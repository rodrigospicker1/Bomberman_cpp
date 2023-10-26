#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <ctime>
#include <stdlib.h>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

int linhas = 0;
int colunas = 0;
int x = 0;
int y = 0;

int ger_num_aleatorio(int valor_max)
{
    unsigned seed = time(0);
    srand(seed);
    return rand()%valor_max;
}

// Função para construir a matriz inicial do jogo
void constroiMatriz(int **matriz) {
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
}
// Função para mostrar a matriz do jogo na tela
int mostraMatriz(int **matriz)
{
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            if(i==x && j==y){
                //cout<<char(2); //personagem
                cout<<"*"; //personagem
            } else
            {
                switch (matriz[i][j])
                {
                    /*
                    case 0: cout << " "; break; // Caminho
                    case 1: cout << char(219); break; // Parede
                    case 2: cout << char(178); break; // Parede frágil
                    case 3: cout << char(1); break; // Inimigo
                    case 4: cout << char(15); break; // Bomba
                    case 5: cout << char(15); break; // Explosão
                    case 6: cout << char(1); break; // Inimigo (explodindo)
                    case 7: cout << char(15); break; // Bomba (explodindo)
                    case 8: cout << char(15); break; // Explosão (fade out)
                    case 9: cout << char(2); break; // Personagem
                    */
                    case 0: cout << " "; break; // Caminho
                    case 1: cout << "#"; break; // Parede
                    case 2: cout << "="; break; // Parede frágil
                    case 3: cout << "+"; break; // Inimigo
                    case 4: cout << "@"; break; // Bomba
                    case 5: cout << "@"; break; // Explosão
                    case 6: cout << "@"; break; // Inimigo (explodindo)
                    case 7: cout << "@"; break; // Bomba (explodindo)
                    case 8: cout << "@"; break; // Explosão (fade out)
                    case 9: cout << "%"; break; // Coletável aumenta raio de bomba
                }
            }
        }
        cout << "\n";
    }
}

void salva_jogo(int **matriz, int segundos_jogador)
{
    fstream dados_jogo;
    dados_jogo.open("dados_jogo.txt");

    if (dados_jogo.is_open())
    {
        dados_jogo << linhas << "\n";
        dados_jogo << colunas << "\n";
        dados_jogo << segundos_jogador << "\n";

        //Salva array do jogo
        for(int i = 0; i < linhas; i++){
            for(int j = 0; j < colunas; j++){
                dados_jogo << matriz[i][j] << " ";
            }
            dados_jogo << "\n";
        }
        dados_jogo.close();

    }else
    {
        cout<<"Falha em abrir o arquivo";
    }
}

int retorna_posicao(int **matriz, int next_move_x, int next_move_y, int *coletavel_bomba)
{
    if(matriz[next_move_x][next_move_y] != 1 && matriz[next_move_x][next_move_y] != 2){
        x = next_move_x;
        y = next_move_y;
        if(matriz[next_move_x][next_move_y] == 9)
        {
            *coletavel_bomba = 1;
        }
    }
}

void play(int **matriz)
{
    system("CLS");
    salva_jogo(matriz, 0);
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

    //mostraMatriz(matriz);

    char tecla;
    bool tempo = false;
    bool tempo_inimigo = false;
    int vivo = true;
    int venceu = false;
    int inimigos = 0;
    int coletavel_bomba = 0;

    int posicao1_x = 0;
    int posicao1_y = 0;

    do{
        posicao1_x = ger_num_aleatorio(linhas-1);
        posicao1_y = ger_num_aleatorio(colunas-1);
    }while(matriz[posicao1_x][posicao1_y] != 0 && (posicao1_x != x || posicao1_y != y) );

    if(matriz[posicao1_x][posicao1_y] == 0){
        matriz[1][2] = 9;
    }


    while (vivo && !venceu) {
        // Posiciona o cursor no início do console e mostra a matriz
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        mostraMatriz(matriz);

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
        if(_kbhit()){
            tecla = getch();
            switch (tecla)
            {
                case 72: case 'w': ///cima
                    /*if (matriz[x - 1][y] == 0) {
                        x--;
                    }*/
                    retorna_posicao(matriz, x-1, y, &coletavel_bomba);
                    break;
                case 80: case 's': ///baixo
                    /*if (matriz[x + 1][y] == 0) {
                        x++;
                    }*/
                    retorna_posicao(matriz, x+1, y, &coletavel_bomba);
                    break;
                case 75:case 'a': ///esquerda
                    /*if (matriz[x][y - 1] == 0) {
                        y--;
                    }*/

                    retorna_posicao(matriz, x, y-1, &coletavel_bomba);
                    break;
                case 77: case 'd': ///direita
                    /*if (matriz[x][y + 1] == 0) {
                        y++;
                    }*/
                    retorna_posicao(matriz, x, y+1, &coletavel_bomba);
                    break;
                case 32: case 'space': ///bomba
                    if (tempo == false){
                        matriz[x][y] = 4;
                        inicio = clock();
                        tempo = true;
                    }
                    break;
                case 'p':
                    if(tempo == false)
                    {
                        salva_jogo(matriz, 0);
                    }else
                    {
                        cout<<"Não é possível salvar com bomba no mapa";
                    }
                    system("CLS");
                    /*mostraMenu();
                    escolhaMenu();*/
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
}

void mostraMenu()
{
    cout << "DEVELOPED BY RODRIGO & VICTOR\n\n";
    cout << "Bem-vindo ao Bomerman!" << endl;
    cout << "1. Novo jogo" << endl;
    cout << "2. Carregar o jogo salvo" << endl;
    cout << "3. Sobre os desenvolvedores" << endl;
    cout << "4. Exit" << "\n\n";
}

void novo_jogo()
{
    int escolha_dimensao;
    int valido = 0;

    cout << "Digite as dimensoes do jogo:\n";
    cout << "1. 15x15\n";
    cout << "2. 9x9\n\n";
    cout << "Escolha: ";
    cin >> escolha_dimensao;

    if(escolha_dimensao == 1){
        linhas = 15;
        colunas = 15;
    }else if(escolha_dimensao == 2){
        linhas = 9;
        colunas = 9;
    }else{
        cout << "Escolha invalida!\n";
        novo_jogo();
    }

    int** matriz = new int*[linhas];
    for (int i = 0; i < linhas; i++) {
        matriz[i] = new int[colunas];
    }

    constroiMatriz(matriz);
    x = 1;
    y = 1; //DEFINE A POSIÇÃO DO JOGADOR

    matriz[linhas - 2][colunas - 2] = 3; //DEFINE AS POSIÇÕES DOS INIMIGOS
    matriz[1][colunas - 2] = 3; //DEFINE AS POSIÇÕES DOS INIMIGOS

    cout << sizeof matriz;
    cout << "\n";


    play(matriz);
}

void carrega_jogo()
{
    int x, y, segundos_jogados;
    fstream dados_jogo;
    dados_jogo.open("dados_jogo.txt");

    if (dados_jogo.is_open())
    {
        // if(!is_empty(dados_jogo)){
            dados_jogo >> linhas;
            dados_jogo >> colunas;
            dados_jogo >> segundos_jogados;

            int** matriz = new int*[linhas];
            for (int i = 0; i < linhas; i++) {
                matriz[i] = new int[colunas];
            }

            for(int i = 0; i < linhas; i++){
                for(int j = 0; j < colunas; j++){
                    dados_jogo>>matriz[i][j];
                    cout << matriz[i][j] << " | ";
                    if(matriz[i][j] == 9){
                        x = i;
                        y = j;
                    }
                }
            }

            dados_jogo.close();
            play(matriz);
        // }
    }else
    {
        cout<<"Falha em abrir o arquivo";
        exit(1);
    }
}

int escolhaMenu()
{
    int escolha_menu;
    int valido = 0;


    cout << "Digite sua escolha: ";
    while(valido == 0){
        cin >> escolha_menu;
        switch(escolha_menu)
        {
            case 1: // CRIA UM NOVO JOGO
                novo_jogo();
                break;
            case 2: // CARREGA JOGO DE UM ARQUIVO
                carrega_jogo();
            case 3:
            case 4:
            default:
                cout << "Escolha invalida!\n";
                mostraMenu();
        }
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
    int vivo = true;
    int venceu = false;
    int inimigos = 0;

    mostraMenu();
    escolhaMenu();

    /*
    do{

        cin >> escolha_menu;

        if(escolha_menu == 1){

        }elseif(escolha_menu == 2){

        }
    }while (escolha_menu != 4);
    */
    exit(1);

    /*

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
            case 'p':
                salva_jogo();
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
    */
    return 0;
}
