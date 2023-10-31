#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <ctime>
#include <stdlib.h>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int linhas = 0;
int colunas = 0;

void escolhaMenu();
void sobre_dev();

int ger_num_aleatorio(int valor_max)
{
    srand(time(NULL));
    unsigned seed = time(0);
    srand(seed);
    return rand()%valor_max;
}

struct Jogador {
    int x = 0, y = 0;
    bool vivo = true;

    void start(int linha, int coluna)
    {
        this->x = linha;
        this->y = coluna;
    }

    int retorna_posicao(int **matriz, int next_move_x, int next_move_y, int *coletavel_bomba, int *coletavel_parede, int *max_paredes_passadas)
    {
        int vivo = 1;
        if( (matriz[next_move_x][next_move_y] != 1 && matriz[next_move_x][next_move_y] != 2) || (*coletavel_parede == 1 && *max_paredes_passadas > 0) ){
            this->x = next_move_x;
            this->y = next_move_y;
            if(matriz[next_move_x][next_move_y] == 1 || matriz[next_move_x][next_move_y] == 2)
            {
                *max_paredes_passadas = *max_paredes_passadas-1;
            }
            if(matriz[next_move_x][next_move_y] == 3)
            {
                vivo = 0;
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
        return vivo;
    }

    void morreu()
    {
        this->vivo = false;
    }
};

struct Inimigo {
    int x, y;
    bool alive = true;
    bool tempo_inimigo;
    clock_t inicio_inimigo, fim_inimigo;

    void inserir_no_mapa(int **matriz, int linhas, int colunas)
    {
        int posicao_x = 0;
        int posicao_y = 0;
        while(matriz[posicao_x][posicao_y] != 0 || (posicao_x < 2 && posicao_y < 2) )
        {
            posicao_x = ger_num_aleatorio(linhas-1);
            posicao_y = ger_num_aleatorio(colunas-1);
        }
        // posicao_x < 2 && posicao_y < 2

        matriz[posicao_x][posicao_y] = 3;

        this->x = posicao_x;
        this->y = posicao_y;
    }

    int mover(int **matriz, Jogador& jogador)
    {
        int vivo;
        if (this->tempo_inimigo == false)
        {
            this->tempo_inimigo = true;
            this->inicio_inimigo = clock();
            if (this->x > jogador.x && matriz[this->x - 1][this->y] == 0) {
                matriz[this->x][this->y] = 0;
                matriz[this->x - 1][this->y] = 3;
                x--;
            }
            else if (this->y > jogador.y && matriz[this->x][this->y - 1] == 0) {
                matriz[this->x][this->y] = 0;
                matriz[this->x][this->y - 1] = 3;
                y--;
            }
            else if (this->x < jogador.x && matriz[this->x + 1][this->y] == 0) {
                matriz[this->x][this->y] = 0;
                matriz[this->x + 1][this->y] = 3;
                x++;
            }
            else if (this->y < jogador.y && matriz[this->x][this->y + 1] == 0) {
                matriz[this->x][this->y] = 0;
                matriz[this->x][this->y + 1] = 3;
                y++;
            }
            else if (this->x == jogador.x && this->y == jogador.y) {
                matriz[this->x][this->y] = 0;
                matriz[this->x + 1][this->y] = 3;
                vivo = false;
            }
            else if (matriz[this->x][this->y + 1] == 0) {
                matriz[this->x][this->y] = 0;
                matriz[this->x][this->y + 1] = 3;
                y++;
            }
            else if (matriz[this->x - 1][this->y] == 0) {
                matriz[this->x][this->y] = 0;
                matriz[this->x - 1][this->y] = 3;
                x--;
            }
            else if (matriz[this->x][this->y - 1] == 0) {
                matriz[this->x][this->y] = 0;
                matriz[this->x][this->y - 1] = 3;
                y--;
            }
            else if (matriz[this->x + 1][this->y] == 0) {
                matriz[this->x][this->y] = 0;
                matriz[this->x + 1][this->y] = 3;
                x++;
            }
        }
        //verifica se o tempo do inimigo está ativo
        if (this->tempo_inimigo == true) {
            this->fim_inimigo = clock();
            if ((fim_inimigo - inicio_inimigo) / CLOCKS_PER_SEC == 1) {
                tempo_inimigo = false;
            }
        }
        return vivo;
    }
};

void mostraMenu()
{
    cout << "DEVELOPED BY RODRIGO & VICTOR\n\n";
    cout << "Bem-vindo ao Bomerman!" << endl;
    cout << "1. Novo jogo" << endl;
    cout << "2. Carregar o jogo salvo" << endl;
    cout << "3. Sobre os desenvolvedores" << endl;
    cout << "4. Exit" << "\n\n";
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
int mostraMatriz(int **matriz, int x, int y)
{
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            if(i==x && j==y){
                SetConsoleTextAttribute(hConsole, 112);
                cout<<char(2); //personagem
                //cout<<"*"; //personagem
            } else
            {
                switch (matriz[i][j])
                {


                    case 0:
                         SetConsoleTextAttribute(hConsole, 112);
                         cout << " "; break; // Caminho

                    case 1: SetConsoleTextAttribute(hConsole, 118);
                            cout << char(219); break; // Parede

                    case 2:
                        SetConsoleTextAttribute(hConsole, 118);
                        cout << char(178); break; // Parede frágil

                    case 3:
                        SetConsoleTextAttribute(hConsole, 124);
                        cout << char(1); break; // Inimigo
                    case 4:
                        SetConsoleTextAttribute(hConsole, 126);
                        cout << char(15); break; // Bomba

                    case 5:
                         SetConsoleTextAttribute(hConsole, 124);
                         cout << char(15); break; // Explosão
                    case 6: cout << char(1); break; // Inimigo (explodindo)
                    case 7: cout << char(15); break; // Bomba (explodindo)
                    case 8: cout << char(15); break; // Explosão (fade out)
                    case 9: cout << char(3); break; // Coletável aumenta raio de bomba
                    case 10: cout << char(5); break; // Coletável atravesar parede
                    /*
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
                    */
                }
            }
        }
        cout << "\n";
    }
}

void salva_jogo(int **matriz, Jogador jogador, int segundos_jogados, int max_paredes_passadas, int total_bombas, string nome_arquivo)
{
    int count_inimigo = 0;
    fstream dados_jogo;
    dados_jogo.open(nome_arquivo);

    for(int i = 0; i < linhas; i++){
        for(int j = 0; j < colunas; j++){
            if(matriz[i][j] == 3)
            {
                count_inimigo++;
            }
        }
    }

    if (dados_jogo.is_open())
    {
        dados_jogo << linhas << "\n";
        dados_jogo << colunas << "\n";
        dados_jogo << jogador.x << "\n";
        dados_jogo << jogador.y << "\n";
        dados_jogo << segundos_jogados << "\n";
        dados_jogo << count_inimigo << "\n";

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

void jogo_finalizado(string mensagem, char nome_arquivo[]){
    system("CLS");
    char opc;

    remove(nome_arquivo);

    while(opc != 's' && opc != 'n')
    {
        cout << mensagem << "\n\n";
        cout << "Voce deseja continuar [s/n]\n";
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

int tem_inimigos(int **matriz)
{
    int inimigos = 1;
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            if(matriz[i][j] == 3){
                inimigos = 0;
            }
        }
    }
    return inimigos;
}

void play(int **matriz, Jogador& jogador, Inimigo *inimigos, int num_inimigos , int segundos_jogados , int max_paredes, int total_bomb, char nome_arquivo[])
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
    clock_t inicio_coletavel_bomba, fim_coletavel_bomba;
    srand(time(NULL));

    //mostraMatriz(matriz);

    char tecla;
    bool tempo = false;
    int vivo = true;
    int venceu = false;
    int coletavel_bomba = 0;
    int coletavel_parede = 0;
    bool tempo_coletavel_bomba = false;
    int raio_bomba = 1;
    int max_paredes_passadas = 5;
    int total_bombas = 0;
    int pause = 0;
    int *x = &jogador.x;
    int *y = &jogador.y;
    int minuto = 0;

    int posicao1_x = 0;
    int posicao1_y = 0;

    if(segundos_jogados == 0){
        do{
            posicao1_x = ger_num_aleatorio(linhas-1);
            posicao1_y = ger_num_aleatorio(colunas-1);
        }while(matriz[posicao1_x][posicao1_y] != 0 && (posicao1_x != jogador.x || posicao1_y != jogador.y) );

        if(matriz[posicao1_x][posicao1_y] == 0){
            //matriz[1][2] = 9;
            matriz[posicao1_x][posicao1_y] = 9;
        }

        do{
            posicao1_x = ger_num_aleatorio(linhas-1);
            posicao1_y = ger_num_aleatorio(colunas-1);
        }while(matriz[posicao1_x][posicao1_y] != 0 && (posicao1_x != jogador.x || posicao1_y != jogador.y) );

        if(matriz[posicao1_x][posicao1_y] == 0){
            //matriz[2][1] = 10;
            matriz[posicao1_x][posicao1_y] = 10;
        }
    }else
    {
        while(segundos_jogados > 59){
            minuto++;
            segundos_jogados = segundos_jogados-60;
        }
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

    while (vivo && !venceu && !pause) {
        // Posiciona o cursor no início do console e mostra a matriz
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        total_tempo_jogado = clock();
        int tempo_corrido = (segundos_jogados) + (total_tempo_jogado-tempo_jogado) / CLOCKS_PER_SEC;
        if(tempo_corrido > 59)
        {
            tempo_jogado = clock();
            minuto++;
        }
        cout << "Tempo jogado: " << minuto << "m " << tempo_corrido << "s\n\n";
        cout << "P -> pausar\n";
        cout << char(5) << " -> coletavel usado para poder atravessar 5 paredes\n";
        cout << char(3) << " -> coletavel usado para bombas ficarem com raio de 3\n\n";
        venceu = tem_inimigos(matriz);
        mostraMatriz(matriz, jogador.x, jogador.y);

        for(int i = 0; i < num_inimigos; i++)
        {
            vivo = inimigos[i].mover(matriz, jogador);
        }

        //executa os movimentos
        if(_kbhit()){
            tecla = getch();
            switch (tecla)
            {
                case 72: case 'w': ///cima
                    vivo = jogador.retorna_posicao(matriz, *x-1, *y, &coletavel_bomba, &coletavel_parede, &max_paredes_passadas);
                    break;
                case 80: case 's': ///baixo
                    vivo = jogador.retorna_posicao(matriz, *x+1, *y, &coletavel_bomba, &coletavel_parede, &max_paredes_passadas);
                    break;
                case 75:case 'a': ///esquerda
                    vivo = jogador.retorna_posicao(matriz, *x, *y-1, &coletavel_bomba,  &coletavel_parede, &max_paredes_passadas);
                    break;
                case 77: case 'd': ///direita
                    vivo = jogador.retorna_posicao(matriz, *x, *y+1, &coletavel_bomba,  &coletavel_parede, &max_paredes_passadas);
                    break;
                case 32: case 'space': ///bomba
                    if (tempo == false){
                        matriz[*x][*y] = 4;
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
                        minuto = minuto * 60;
                        tempo_corrido = tempo_corrido + minuto;
                        pause = 1;
                        salva_jogo(matriz, jogador, tempo_corrido, max_paredes_passadas, total_bombas, nome_arquivo);
                    }else
                    {
                        system("CLS");
                        cout<<"\nNao e possivel salvar com bomba no mapa\n";
                    }
                    break;
            }
        }


        if(coletavel_bomba == 1)
        {
            cout << "Bombas com raio 3 restante: " << 3 - total_bombas << "\n";
            raio_bomba = 3;
            if (total_bombas == 4)
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
                if(matriz[*x][*y] != 0){
                    int feito = 0;
                    for(int i = *x; i >= 0; i--){
                        if(matriz[i][*y] == 0 && feito == 0){
                            feito = 1;
                            *x = i;
                        }
                    }
                    if(feito == 0)
                    {
                        for(int i = *y; i < colunas; i++){
                            if(matriz[i][*y] == 0 && feito == 0){
                                feito = 1;
                                *y = i;
                            }
                        }
                    }
                    if(feito == 0)
                    {
                        for(int i = *y; i >= 0; i--){
                            if(matriz[*x][i] == 0 && feito == 0){
                                feito = 1;
                                *y = i;
                            }
                        }
                    }
                    if(feito == 0)
                    {
                        for(int i = *x; i < linhas; i++){
                            if(matriz[i][*y] == 0 && feito == 0){
                                feito = 1;
                                *x = i;
                            }
                        }
                    }
                    if(feito == 0){
                        *x = 1;
                        *y = 1;
                    }
                }
            }
        }


        //verifica se o tempo de uma bomba está ativo
        if (tempo == true) {
            fim = clock();
            if ((fim - inicio) / CLOCKS_PER_SEC == 3) {
                verifica_bomba(matriz, raio_bomba);

            }
            if ((fim - inicio) / CLOCKS_PER_SEC == 4) {
                // Volta para o fundo normal
                for (int i = 0; i < linhas; i++) {
                    for (int j = 0; j < colunas; j++) {
                        if (matriz[i][j] == 4 || matriz[i][j] == 5) {
                            matriz[i][j] = 0;
                            if (i == *x && j == *y) {
                                vivo = false;
                            }
                        }
                    }
                }
                tempo = false;
                system("CLS");
            }
        }


    } //fim do jogo

    system("CLS");
    if(pause){
        mostraMenu();
        escolhaMenu();
    }else if (venceu) {
        jogo_finalizado("Voce venceu!", nome_arquivo);
    }
    else if (!vivo) {
        jogo_finalizado("Voce perdeu!", nome_arquivo);
    }
}

void novo_jogo()
{
    int escolha_dimensao;
    int valido = 0;
    int total_inimigo = 0;
    char nome_arquivo[50];

    system("CLS");
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

    system("CLS");
    while(total_inimigo == 0 || total_inimigo > 5)
    {
        cout << "Quantos inimigos? (MAX: 5):\n";
        cout << "Escolha: ";
        cin >> total_inimigo;
    }

    system("CLS");
    cout << "Qual nome do arquivo (OBS.: digite com a extensao)\n";
    cout << "Nome: ";
    cin >> nome_arquivo;

    int** matriz = new int*[linhas];
    for (int i = 0; i < linhas; i++) {
        matriz[i] = new int[colunas];
    }

    constroiMatriz(matriz);
    Jogador jogador;
    jogador.start(1, 1); //DEFINE A POSIÇÃO DO JOGADOR

    Inimigo inimigos[total_inimigo];
    for(int a = 0; a < total_inimigo; a++)
    {
        //cout << a << " ";
        inimigos[a].inserir_no_mapa(matriz, linhas, colunas);
        //cout << inimigos[a].x << " ";
        //cout << inimigos[a].y << " \n\n";
        //inimigos[a].x = 14;
        //inimigos[a].y = 14;
    }

    cout << sizeof matriz;
    cout << "\n";

    ofstream { nome_arquivo };
    salva_jogo(matriz, jogador, 0, 0, 0, nome_arquivo);
    play(matriz, jogador, inimigos, total_inimigo, 0, 0 , 0, nome_arquivo);
}

void carrega_jogo(char nome_arquivo[])
{
    int segundos_jogados, max_paredes_passadas, total_bombas;
    int total_inimigos = 0;
    int x , y;
    int count_inimigo = 0;
    fstream dados_jogo;
    dados_jogo.open(nome_arquivo);

    if (dados_jogo.is_open())
    {
        //if(!is_empty(dados_jogo)){
            dados_jogo >> linhas;
            dados_jogo >> colunas;
            dados_jogo >> x;
            dados_jogo >> y;
            dados_jogo >> segundos_jogados;
            dados_jogo >> total_inimigos;

            total_inimigos;
            Inimigo inimigos[total_inimigos];

            int** matriz = new int*[linhas];
            for (int i = 0; i < linhas; i++) {
                matriz[i] = new int[colunas];
            }

            for(int i = 0; i < linhas; i++){
                for(int j = 0; j < colunas; j++){
                    dados_jogo>>matriz[i][j];
                    if(matriz[i][j] == 3)
                    {
                        inimigos[count_inimigo].x = i;
                        inimigos[count_inimigo].y = j;
                        count_inimigo++;
                    }
                }
            }

            dados_jogo >> max_paredes_passadas;
            dados_jogo >> total_bombas;

            Jogador jogador;
            jogador.start(x, y);

            dados_jogo.close();
            play(matriz, jogador, inimigos, total_inimigos, segundos_jogados, max_paredes_passadas, total_bombas, nome_arquivo);
        //}
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
    char nome_arquivo[50] = {'d','a','d','o','s','_','j','o','g','o','.','t','x','t'};


    cout << "Digite sua escolha: ";
    while(valido == 0){
        cin >> escolha_menu;
        switch(escolha_menu)
        {
            case 1: // CRIA UM NOVO JOGO
                novo_jogo();
                break;
            case 2: // CARREGA JOGO DE UM ARQUIVO
                system("CLS");
                cout << "Digite o nome do arquivo que voce quer carregar (OBS.: digite com a extensao)\n";
                cout << "Nome: ";
                cin >> nome_arquivo;
                carrega_jogo(nome_arquivo);
            case 3:
                //SOBRE OS DESENVOLVEDORES
                sobre_dev();
            case 4:
                //SAIR
                exit(1);
            default:
                cout << "Escolha invalida!\n";
                mostraMenu();
        }
    }
}

void sobre_dev()
{
    char opc;
    while(opc != 's')
    {
        system("CLS");
        cout << "Ola, nos somos estudantes de ciencias da computacaoo na UNIVALI e desenvolvemos um bomberman em c++.\n\n";
        cout << "Meu nome eh Victor e tenho 19 anos.";
        cout << "Sou desenvolvedor Fullstack, ";
        cout << "tenho curso tecnico de Desenvolvimento de Sistemas, e atualmente curso Ciencias da computacao.\n";
        cout << "E eu sou o Rodrigo tenho 18 anos, sou desenvolvedor front-end e back-end e voce pode me encontrar em \n https://www.linkedin.com/in/rodrigospicker/.\n\n";
        cout << "Digite 's' para sair :";
        cin >> opc;
    }
    system("CLS");
    mostraMenu();
    escolhaMenu();
}

int main() {

    mostraMenu();
    escolhaMenu();

    return 0;
}
