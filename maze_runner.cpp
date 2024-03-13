#include <stdio.h>
#include <stack>
#include <iostream>
#include <cstdlib>

// Matriz de char representnado o labirinto
char** maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Representação de uma posição
struct pos_t {
	 int i;
	 int j;
};

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
std::stack<pos_t> valid_positions;
/* Inserir elemento: 

     pos_t pos;
     pos.i = 1;
     pos.j = 3;
     valid_positions.push(pos)
 */
// Retornar o numero de elementos: 
//    valid_positions.size();
// 
// Retornar o elemento no topo: 
//  valid_positions.top(); 
// 
// Remover o primeiro elemento do vetor: 
//    valid_positions.pop();


// Função que le o labirinto de um arquivo texto, carrega em 
// memória e retorna a posição inicial
pos_t load_maze(const char* file_name) {
	pos_t initial_pos;
	// Abre o arquivo para leitura (fopen)
	FILE *fp;
	fp = fopen(file_name, "r");
	// Le o numero de linhas e colunas (fscanf) 
	// e salva em num_rows e num_cols
	fscanf(fp, "%d%d", &num_rows, &num_cols);
	// Aloca a matriz maze (malloc)
	maze = (char**) malloc(num_cols * sizeof(char*));
	for (int i = 0; i < num_rows; ++i) {
		// Aloca cada linha da matriz
		maze[i] = (char *) malloc(num_rows * sizeof(char));
	} 
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			// Le o valor da linha i+1,j do arquivo e salva na posição maze[i][j]
			// Se o valor for 'e' salvar o valor em initial_pos
			fscanf(fp, "%c", &maze[i][j]);
			if(maze[i][j] == 'e') {
			initial_pos.i = i;
			initial_pos.j = j;
			}
		}
	}
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
bool walk(pos_t pos) {
	pos_t new_pos;
	pos_t valid;
	system("cls"); // Limpar a tela
    print_maze(); // Imprimir o labirinto

	//verifica se chegamos na saida
	if(pos.j < num_cols - 1 && maze[pos.i][pos.j+1] == 's') return true;
	if(pos.j > 0 && maze[pos.i][pos.j-1] == 's') return true;
	if(pos.i < num_rows - 1 && maze[pos.i+1][pos.j] == 's') return true;
	if(pos.i > 0 && maze[pos.i-1][pos.j+1] == 's') return true;

	//verifica se a posicao acima eh valida e, se sim, chama funcao walk e adiciona as outras posicoes validas na pilha
	if(pos.i > 0 && maze[pos.i-1][pos.j] == 'x') {
		maze[pos.i-1][pos.j] = '.';
		new_pos.i = pos.i-1;
		new_pos.j = pos.j;
		if(pos.j < num_cols - 1 && maze[pos.i][pos.j+1] == 'x') {
			valid.i = pos.i;
			valid.j = pos.j+1;
			valid_positions.push(valid);
		}
		if(pos.i < num_rows - 1 && maze[pos.i+1][pos.j] == 'x') {
			valid.i = pos.i+1;
			valid.j = pos.j;
			valid_positions.push(valid);
		}
		if(pos.j > 0 && maze[pos.i][pos.j-1] == 'x') {
			valid.i = pos.i;
			valid.j = pos.j-1;
			valid_positions.push(valid);
		}
		walk(new_pos); 
	}

	//verifica se a posicao ao lado direito eh valida e, se sim, chama funcao walk e adiciona as outras posicoes validas na pilha
	if(pos.j < num_cols - 1 && maze[pos.i][pos.j+1] == 'x') {
		maze[pos.i][pos.j+1] = '.';
		new_pos.i = pos.i;
		new_pos.j = pos.j+1;
		if(pos.i < num_rows - 1 && maze[pos.i+1][pos.j] == 'x') {
			valid.i = pos.i+1;
			valid.j = pos.j;
			valid_positions.push(valid);
		}
		if(pos.j > 0 && maze[pos.i][pos.j-1] == 'x') {
			valid.i = pos.i;
			valid.j = pos.j+1;
			valid_positions.push(valid);
		}
		walk(new_pos); 
	}

	//verifica se a posicao abaixo eh valida e, se sim, chama funcao walk e adiciona as outras posicoes validas na pilha
	if(pos.j < num_rows - 1 && maze[pos.i+1][pos.j] == 'x') {
		maze[pos.i+1][pos.j] = '.';
		new_pos.i = pos.i+1;
		new_pos.j = pos.j;
		if(pos.j > 0 && maze[pos.i][pos.j-1] == 'x') {
			valid.i = pos.i;
			valid.j = pos.j+1;
			valid_positions.push(valid);
		}
		walk(new_pos); 
	}

	//verifica se a posicao ao lado esquerdo eh valida e, se sim, chama funcao walk
	if(pos.j > 0 && maze[pos.i][pos.j-1] == 'x') {
		maze[pos.i][pos.j-1] = '.';
		new_pos.i = pos.i;
		new_pos.j = pos.j-1;
		walk(new_pos); 
	}

	//caso nao tenha nenhuma posicao valida nos vizinhos, verifica se a pilha tem posicoes salvas
	if(!valid_positions.empty()) {
		new_pos = valid_positions.top();
		valid_positions.pop();
		maze[new_pos.i][new_pos.j] = '.';
		walk(new_pos);
	}

	return false; //saida nao encontrada
}

int main() {
	// carregar o labirinto com o nome do arquivo recebido como argumento
	pos_t initial_pos = load_maze("/maze_runner/data/maze5.txt");
	// chamar a função de navegação
	bool exit_found = walk(initial_pos);
	// Tratar o retorno (imprimir mensagem)
	 if (exit_found) {
        printf("Saída encontrada!\n");
    } else {
        printf("Saída não encontrada.\n");
    }
	return 0;
}