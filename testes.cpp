#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <ctime>
#include <stdlib.h>
#include <tuple>
#include <fstream>
#include <random>

using namespace std;

/*
struct {
  int linhas_matriz;
  int colunas_matriz;
  int segundos_jogados;
  int matriz[linhas_matriz][colunas_matriz];
} DadosJogo;
*/
int linhas = 0;
int colunas = 0;

void ale1_8()
{
     // Crie um objeto para gerar n�meros aleat�rios
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distribution(1, 8);

    // Gere um n�mero aleat�rio de 1 a 8
    int numero_aleatorio = distribution(gen);

    // Imprima o n�mero aleat�rio gerado
    cout << "N�mero aleat�rio: " << numero_aleatorio << std::endl;
}

void salva_jogo(int **matriz, int segundos_jogador)
{
    fstream dados_jogo;
    dados_jogo.open("dados_jogo2.txt");

    if (dados_jogo.is_open())
    {
        dados_jogo << linhas << "\n";
        dados_jogo << colunas << "\n";
        dados_jogo << segundos_jogador << "\n";

        //Salva array do jogo
        for(int i = 0; i < linhas; i++){
            for(int j = 0; j < colunas; j++){
                dados_jogo << matriz[i][j];
            }
            dados_jogo << "\n";
        }
        dados_jogo.close();

    }else
    {
        cout<<"Falha em abrir o arquivo";
    }
}

int main() {

for(int i = 0; i < 20; i++){

    ale1_8();
}
    exit(1);
    linhas = 15;
    colunas = 15;

    int** matriz = new int*[linhas];
    for (int i = 0; i < linhas; i++) {
        matriz[i] = new int[colunas];
    }

    for(int i = 0; i < linhas; i++){
        for(int j = 0; j < colunas; j++){
            matriz[i][j] = j;
        }
    }

    salva_jogo(matriz, 100);

    return 0;
}
