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

void escolhaMenu();

void mostraMenu()
{
    cout << "DEVELOPED BY RODRIGO & VICTOR\n\n";
    cout << "Bem-vindo ao Bomerman!" << endl;
    cout << "1. Novo jogo" << endl;
    cout << "2. Carregar o jogo salvo" << endl;
    cout << "3. Sobre os desenvolvedores" << endl;
    cout << "4. Exit" << "\n\n";
}

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
                    case 10: cout << char(2); break; // Personagem
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
                    case 10: cout << "~"; break; // Coletável atravesar parede
                }
            }
        }
        cout << "\n";
    }
}

void salva_jogo(int **matriz, int segundos_jogados, int max_paredes_passadas, int total_bombas)
{
    fstream dados_jogo;
    dados_jogo.open("dados_jogo.txt");

    if (dados_jogo.is_open())
    {
        dados_jogo << linhas << "\n";
        dados_jogo << colunas << "\n";
        dados_jogo << x << "\n";
        dados_jogo << y << "\n";
        dados_jogo << segundos_jogados << "\n";

        //Salva array do jogo
        for(int i = 0; i < linhas; i++){
            for(int j = 0; j < colunas; j++){
                dados_jogo << matriz[i][j] << " ";
            }
            dados_jogo << "\n";
        }


        dados_jogo << max_paredes_passadas << "\n";
        dados_jogo << total_bombas << "\n";

        dados_jogo.close();

    }else
    {
        cout<<"Falha em abrir o arquivo";
    }
}

void retorna_posicao(int **matriz, int next_move_x, int next_move_y, int *coletavel_bomba, int *coletavel_parede, int *max_paredes_passadas)
{
    if( (matriz[next_move_x][next_move_y] != 1 && matriz[next_move_x][next_move_y] != 2) || (*coletavel_parede == 1 && *max_paredes_passadas > 0) ){
        x = next_move_x;
        y = next_move_y;
        if(matriz[next_move_x][next_move_y] == 1 || matriz[next_move_x][next_move_y] == 2)
        {
            *max_paredes_passadas = *max_paredes_passadas-1;
        }
        if(matriz[next_move_x][next_move_y] == 9)
        {
            *coletavel_bomba = 1;
            matriz[next_move_x][next_move_y] = 0;
        }else if(matriz[next_move_x][next_move_y] == 10)
        {
            *coletavel_parede = 1;
            matriz[next_move_x][next_move_y] = 0;
        }
    }
}

void verifica_bomba(int **matriz, int raio_bomba)
{
    int cima = 1;
    int baixo = 1;
    int esq = 1;
    int dir = 1;
    int x_bomba = 0;
    int y_bomba = 0;

    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            if (matriz[i][j] == 4)
            {
                x_bomba = i;
                y_bomba = j;

            }
        }
    }

    for(int i = 1; i <= raio_bomba; i++)
    {
        if(cima == 1){
            if(matriz[x_bomba-i][y_bomba] != 1){
                if(matriz[x_bomba-i][y_bomba] == 2)
                {
                    cima = 0;
                }
                matriz[x_bomba-i][y_bomba] = 5;
            }else
            {
                cima = 0;
            }
        }
        if(baixo == 1){
            if(matriz[x_bomba+i][y_bomba] != 1){
                if(matriz[x_bomba+i][y_bomba] == 2)
                {
                    baixo = 0;
                }
                matriz[x_bomba+i][y_bomba] = 5;
            }else{
                baixo = 0;
            }
        }
        if(esq == 1){
            if(matriz[x_bomba][y_bomba-i] != 1){
                if(matriz[x_bomba][y_bomba-i] == 2)
                {
                    esq = 0;
                }
                matriz[x_bomba][y_bomba-i] = 5;
            }else{
                esq = 0;
            }
        }
        if(dir == 1){
            if(matriz[x_bomba][y_bomba+i] != 1){
                if(matriz[x_bomba][y_bomba+i] == 2)
                {
                    dir = 0;
                }
                matriz[x_bomba][y_bomba+i] = 5;
            }else{
                dir = 0;
            }
        }
    }
}

void game_over(){
    system("CLS");
    char opc;
    while(opc != 's' || opc != 'n')
    {
        cout << "Você perdeu!\n\n";
        cout << "Você deseja continuar [s/n]\n";
        cout << "?: ";
        cin >> opc;
    }
    if(opc == 's'){
        system("CLS");
        mostraMenu();
        escolhaMenu();
    }else{
        exit(1);
    }
}

void play(int **matriz, int segundos_jogados , int max_paredes, int total_bomb)
{
    system("CLS");
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
    clock_t tempo_jogado, total_tempo_jogado;
    clock_t inicio, fim;
    clock_t inicio_inimigo, fim_inimigo;
    clock_t inicio_coletavel_bomba, fim_coletavel_bomba;
    srand(time(NULL));

    //mostraMatriz(matriz);

    char tecla;
    bool tempo = false;
    bool tempo_inimigo = false;
    int vivo = true;
    int venceu = false;
    int inimigos = 0;
    int coletavel_bomba = 0;
    int coletavel_parede = 0;
    bool tempo_coletavel_bomba = false;
    int raio_bomba = 1;
    int max_paredes_passadas = 5;
    int total_bombas = 0;

    int posicao1_x = 0;
    int posicao1_y = 0;

    do{
        posicao1_x = ger_num_aleatorio(linhas-1);
        posicao1_y = ger_num_aleatorio(colunas-1);
    }while(matriz[posicao1_x][posicao1_y] != 0 && (posicao1_x != x || posicao1_y != y) );

    if(matriz[posicao1_x][posicao1_y] == 0){
        matriz[1][2] = 9;
    }

    do{
        posicao1_x = ger_num_aleatorio(linhas-1);
        posicao1_y = ger_num_aleatorio(colunas-1);
    }while(matriz[posicao1_x][posicao1_y] != 0 && (posicao1_x != x || posicao1_y != y) );

    if(matriz[posicao1_x][posicao1_y] == 0){
        matriz[2][1] = 10;
    }

    tempo_jogado = clock();
    if(max_paredes > 0){
        coletavel_parede = 1;
        max_paredes_passadas = max_paredes;
    }
    if(total_bomb > 0){
        coletavel_bomba = 1;
        total_bombas = total_bomb;
    }

    while (vivo && !venceu) {
        // Posiciona o cursor no início do console e mostra a matriz
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        total_tempo_jogado = clock();
        int tempo_corrido = (total_tempo_jogado-tempo_jogado) / CLOCKS_PER_SEC;
        cout << "Tempo jogado: " << tempo_corrido << "\n\n";
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
                    retorna_posicao(matriz, x-1, y, &coletavel_bomba, &coletavel_parede, &max_paredes_passadas);
                    break;
                case 80: case 's': ///baixo
                    /*if (matriz[x + 1][y] == 0) {
                        x++;
                    }*/
                    retorna_posicao(matriz, x+1, y, &coletavel_bomba, &coletavel_parede, &max_paredes_passadas);
                    break;
                case 75:case 'a': ///esquerda
                    /*if (matriz[x][y - 1] == 0) {
                        y--;
                    }*/

                    retorna_posicao(matriz, x, y-1, &coletavel_bomba,  &coletavel_parede, &max_paredes_passadas);
                    break;
                case 77: case 'd': ///direita
                    /*if (matriz[x][y + 1] == 0) {
                        y++;
                    }*/
                    retorna_posicao(matriz, x, y+1, &coletavel_bomba,  &coletavel_parede, &max_paredes_passadas);
                    break;
                case 32: case 'space': ///bomba
                    if (tempo == false){
                        matriz[x][y] = 4;
                        inicio = clock();
                        tempo = true;
                        if(coletavel_bomba == 1)
                        {
                            total_bombas++;
                        }
                    }
                    break;
                case 'p':
                    if(tempo == false)
                    {
                        salva_jogo(matriz, tempo_corrido, max_paredes_passadas, total_bombas);
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


        if(coletavel_bomba == 1)
        {
            cout << "Bombas com raio 3 restante: " << 3 - total_bombas << "\n";
            raio_bomba = 3;
            if (total_bombas == 3)
            {
                raio_bomba = 1;
                coletavel_bomba = 0;
                system("CLS");
            }
        }

        if(coletavel_parede == 1)
        {
            cout << "Paredes restante: " << max_paredes_passadas << "\n";
            if (max_paredes_passadas == 0)
            {
                system("CLS");
                coletavel_parede = 0;
                if(matriz[x][y] != 0){
                    int feito = 0;
                    for(int i = x; i >= 0; i--){
                        if(matriz[i][y] == 0 && feito == 0){
                            feito = 1;
                            x = i;
                        }
                    }
                    if(feito == 0)
                    {
                        for(int i = y; i < colunas; i++){
                            if(matriz[i][y] == 0 && feito == 0){
                                feito = 1;
                                y = i;
                            }
                        }
                    }
                    if(feito == 0)
                    {
                        for(int i = y; i >= 0; i--){
                            if(matriz[x][i] == 0 && feito == 0){
                                feito = 1;
                                y = i;
                            }
                        }
                    }
                    if(feito == 0)
                    {
                        for(int i = x; i < linhas; i++){
                            if(matriz[i][y] == 0 && feito == 0){
                                feito = 1;
                                x = i;
                            }
                        }
                    }
                    if(feito == 0){
                        x = 1;
                        y = 1;
                    }
                }
            }
        }


        //verifica se o tempo de uma bomba está ativo
        if (tempo == true) {
            fim = clock();
            if ((fim - inicio) / CLOCKS_PER_SEC == 3) {
                // Aperece o amarelo no raio de destruição
                verifica_bomba(matriz, raio_bomba);
                /*
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
                */

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
        game_over();
    }
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


    play(matriz, 0 , 0);
}

void carrega_jogo()
{
    int segundos_jogados, max_paredes_passadas, total_bombas;
    fstream dados_jogo;
    dados_jogo.open("dados_jogo.txt");

    if (dados_jogo.is_open())
    {
        // if(!is_empty(dados_jogo)){
            dados_jogo >> linhas;
            dados_jogo >> colunas;
            dados_jogo >> x;
            dados_jogo >> y;
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

            dados_jogo >> max_paredes_passadas;
            dados_jogo >> total_bombas;

            dados_jogo.close();
            play(matriz, max_paredes_passadas, total_bombas);
        // }
    }else
    {
        cout<<"Falha em abrir o arquivo";
        exit(1);
    }
}

void escolhaMenu()
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
