/**
    Estrutura inicial para um jogo de labirinto
    versão: 0.1 (Felski)
*/
#include <iostream>
#include<windows.h>
#include<conio.h>
#include <time.h>
#include <ctime>
#include <stdlib.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
int linhas = 15;
int colunas = 15;

using namespace std;

/*int constroiMatriz(int &matriz){

    for(int i = 0; i < linhas; i++){
        for(int j = 0; j < colunas; j++){
            if(i == 0 || j == 0 || i == linhas-1 || j == colunas-1){ //BORDAS
                matriz[i][j] = 1;
            }else
            {
                if(i % 2 == 0 || j% 2 == 0){ //Tijolos do meio fixo
                    matriz[i][j] = 1;
                }else
                {
                    if((i == 1 && (j == 1 || j ==2)) || (j == 1 && i == 2) || (i == linhas-2 && (j == colunas-3 || j == colunas-2)) || (i == linhas-3 && j == colunas-2)){ //Parte início jogador
                        matriz[i][j] = 0;
                    }else{
                        &matriz[i][j] = (rand()% 2) + 2;
                    }
                }
            }
        }
    }

    return matriz;

} */


int main()
{
    ///ALERTA: N�O MODIFICAR O TRECHO DE C�DIGO, A SEGUIR.
        //INICIO: COMANDOS PARA QUE O CURSOR N�O FIQUE PISCANDO NA TELA
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO     cursorInfo;
        GetConsoleCursorInfo(out, &cursorInfo);
        cursorInfo.bVisible = false; // set the cursor visibility
        SetConsoleCursorInfo(out, &cursorInfo);
        //FIM: COMANDOS PARA QUE O CURSOR N�O FIQUE PISCANDO NA TELA
        //IN�CIO: COMANDOS PARA REPOSICIONAR O CURSOR NO IN�CIO DA TELA
        short int CX=0, CY=0;
        COORD coord;
        coord.X = CX;
        coord.Y = CY;
        //FIM: COMANDOS PARA REPOSICIONAR O CURSOR NO IN�CIO DA TELA
    ///ALERTA: N�O MODIFICAR O TRECHO DE C�DIGO, ACIMA.

    clock_t inicio, fim;
    srand(time(NULL));

    //Vari�vel para tecla precionada
    char tecla;
    bool tempo = false;
    int x = 1;
    int y = 1;


    int matriz[linhas][colunas]=
    {
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,0,0,2,2,0,0,0,0,0,0,0,0,0,1,
        1,0,1,2,1,0,1,0,1,0,1,0,1,0,1,
        1,2,2,2,2,0,0,0,0,0,0,0,0,0,1,
        1,2,1,2,1,0,1,0,1,0,1,0,1,0,1,
        1,2,2,2,0,0,0,0,0,0,0,0,0,0,1,
        1,2,1,0,1,0,1,0,1,0,1,0,1,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    };

    while(true){
        ///Posiciona a escrita no início do console
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        ///Imprime o jogo: mapa e personagem.
        for(int i=0;i<linhas;i++){
            for(int j=0;j<colunas;j++){
                if(i==x && j==y){
                    cout<<char(2); //personagem
                } else {
                    switch (matriz[i][j]){
                        case 0: cout<< GREEN <<char(219)<<RESET; break; //caminho
                        case 1: cout<<char(219); break; //parede
                        case 2: cout<< BLUE <<char(219)<<RESET; break; //parede frágil
                        case 4: cout<< RED <<char(219)<<RESET; break; //parede frágil
                        //default: cout<<"-"; //erro
                    } //fim switch
                }
            }
            cout<<"\n";
        }

        ///executa os movimentos
         if ( _kbhit() ){
            tecla = getch();
            switch(tecla)
            {
                case 72: case 'w': ///cima
                    if(matriz[x-1][y] == 0){
                        x--;
                    }
                break;
                case 80: case 's': ///baixo
                    if(matriz[x+1][y] == 0){
                        x++;
                    }
                break;
                case 75:case 'a': ///esquerda
                    if(matriz[x][y-1] == 0){
                        y--;
                    }
                break;
                case 77: case 'd': ///direita
                    if(matriz[x][y+1] == 0){
                        y++;
                    }
                break;
                case 32: case 'space': ///bomba
                    matriz[x][y] = 4;
                break;
            }
         }


    } //fim do laço do jogo

    return 0;
} //fim main
