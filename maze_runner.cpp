#include <stdio.h>
#include <thread>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <chrono>

using namespace std;

// Matriz de char representnado o labirinto
char** maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Variavel que indica se a saída foi encontrada
bool exit_found = false;

// Representação de uma posição
struct pos_t {
	 int i;
	 int j;
};


// Função que le o labirinto de um arquivo texto, carrega em 
// memória e retorna a posição inicial
pos_t load_maze(const char* file_name) {
	pos_t initial_pos;
	// Abre o arquivo para leitura (fopen)
	FILE *fp;
	fp = fopen(file_name, "r");
	// Le o numero de linhas e colunas (fscanf) 
	// e salva em num_rows e num_cols
	fscanf(fp, "%d %d\n", &num_rows, &num_cols);
	// Aloca a matriz maze (malloc)
	maze = (char**) malloc(num_rows * sizeof(char*));
	for (int i = 0; i < num_rows; ++i) {
		// Aloca cada linha da matriz
		maze[i] = (char *) malloc(num_cols * sizeof(char));
	} 
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			// Le o valor da linha i+1,j do arquivo e salva na posição maze[i][j]
			// Se o valor for 'e' salvar o valor em initial_pos
			fscanf(fp, " %c", &maze[i][j]);
			if(maze[i][j] == 'e') {
			initial_pos.i = i;
			initial_pos.j = j;
			}
		}
	}
	fclose(fp);
	return initial_pos;
}

// Função que imprime o labirinto
void print_maze() {
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}


// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
void walk(pos_t pos) {
	pos_t new_pos;
	pos_t valid;
	while(!exit_found) {
		this_thread::sleep_for(chrono::milliseconds(50));
		//verifica se chegamos na saida
		if(pos.i > 0 && maze[pos.i-1][pos.j] == 's') {
			exit_found = true;
		}
		
		if(pos.j < num_cols - 1 && maze[pos.i][pos.j+1] == 's') {
			exit_found = true;
		}
		
		if(pos.i < num_rows - 1 && maze[pos.i+1][pos.j] == 's') {
			exit_found = true;
		}

		if(pos.j > 0 && maze[pos.i][pos.j-1] == 's') {
			exit_found = true;
		}

		//verifica se a posicao acima eh valida e, se sim, cria uma nova thread que chama a função walk para a nova posicao
		if(pos.i > 0 && maze[pos.i-1][pos.j] == 'x') {
			maze[pos.i-1][pos.j] = '.';
			new_pos.i = pos.i-1;
			new_pos.j = pos.j;
			thread t1(walk, new_pos);
			t1.detach();
		}

		//verifica se a posicao a direita eh valida e, se sim, cria uma nova thread que chama a função walk para a nova posicao
		if(pos.j < num_cols - 1 && maze[pos.i][pos.j+1] == 'x') {
			maze[pos.i][pos.j+1] = '.';
			new_pos.i = pos.i;
			new_pos.j = pos.j+1;
			thread t2(walk, new_pos);
			t2.detach();
		}

		//verifica se a posicao abaixo eh valida e, se sim, cria uma nova thread que chama a função walk para a nova posicao
		if(pos.i < num_rows - 1 && maze[pos.i+1][pos.j] == 'x') {
			maze[pos.i+1][pos.j] = '.';
			new_pos.i = pos.i+1;
			new_pos.j = pos.j;
			thread t3(walk, new_pos);
			t3.detach(); 
		}

		//verifica se a posicao ao lado esquerdo eh valida e, se sim, chama funcao walk
		if(pos.j > 0 && maze[pos.i][pos.j-1] == 'x') {
			maze[pos.i][pos.j-1] = '.';
			new_pos.i = pos.i;
			new_pos.j = pos.j-1;
			thread t4(walk, new_pos);
			t4.detach(); 
		}
	}
} 

int main(int argc, char* argv[]) {
	// carregar o labirinto com o nome do arquivo recebido como argumento
	pos_t initial_pos = load_maze(argv[1]);

	// chamar a função de navegação
	thread t(walk, initial_pos);
	t.detach();

	while (!exit_found) {
		print_maze();
		this_thread::sleep_for(chrono::milliseconds(50));
		system("clear || cls");
	}

	print_maze();
	printf("Saida do labirinto encontrada\n");

	return 0; 
}