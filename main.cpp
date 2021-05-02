#include <stdio.h>

struct program{
	char* dados = nullptr;
	int tamanho = 0;
	~program(){
		delete[] dados;
	}
};

void lerPrograma(const char*, program&);
int checarSemantica(program&);
void capturarChaves(int*, program&);

int main(){
	/* cria a estrutura que armazena o codigo e o tamanho dele */
	program codigo;
	lerPrograma("program.bf", codigo);
	
	/* array principal */
	char* memoria = new char[1024]{0};
	for(int i = 0; i < 1024;i++) memoria[i]=0;
	
	/* ponteiro comeca apontado para o primeiro elemento */
	char* ponteiro = &memoria[0];
	printf("Executando programa com %i bytes!\n", codigo.tamanho);
	
	/* checa se o programa nao possui chaves sem seus respectivos pares */
	int e = checarSemantica(codigo);
	if(e == -1) return -1;
	int* chaves = new int[e];
	capturarChaves(chaves, codigo);
	/* printa o programa */
	//for(int i = 0; i < codigo.tamanho; i++) printf("%c", *((char*)(codigo.dados)+i));
	printf("\n   -  ");
	char comando, c;
	int i = 0;
	while(true){
		comando = *((char*)(codigo.dados)+i);
		//printf("%c\n", comando);
		switch(comando){
			case '>':
				ponteiro++;
				i++;
				break;
			case '<':
				ponteiro--;
				i++;
				break;
			case '+':
				++(*ponteiro);
				i++;
				break;
			case '-':
				--(*ponteiro);
				i++;
				break;
			case '.':
				c = (char)(*ponteiro);
				printf("%c", c);
				i++;
				break;
			case ',':
				scanf(" %c", ponteiro);
				i++;
				break;
			case '[':
				for(int b = 0; b< e; b++){
					if(chaves[b] == i){
						if(*ponteiro == 0)
							i = (chaves[e - 1 - b])+1;
						else
							i++;
						break;
					}
				}
				break;
			case ']':
				for(int b = 0; b< e; b++){
					if(chaves[b] == i){
						if(*ponteiro != 0)
							i = (chaves[e - 1 - b])+1;
						else
							i++;
						break;
					}
				}
				break;
			default:
				//printf("Chamou o default\n");
				break;
		}
		if(i >= codigo.tamanho) break;
	}
	
	
	/* libera a memoria */
	delete[] memoria;
	delete[] chaves;
	return 0;
}
void capturarChaves(int* chaves, program& codigo){
	int t = 0;
	for(int i = 0; i < codigo.tamanho; i++){
		if(codigo.dados[i] == '['){
			chaves[t] = i;
			t++;
		}
		else if(codigo.dados[i] == ']'){
			chaves[t] = i;
			t++;
		}
		
	}
}

int checarSemantica(program& pg){
	int equilibrio = 0, q_chaves = 0;
	for(int i = 0; i<pg.tamanho; i++){
		if(pg.dados[i] == '['){
			q_chaves++;
			equilibrio++;
		}
		else if(pg.dados[i] == ']'){
			q_chaves++;
			equilibrio--;
		}
	}
	if(equilibrio!=0){
		if(equilibrio<0) printf("Encontrada ] sem seu respectivo [");
		else printf("Encontrada [ sem seu respectivo ]");
		return -1;
	}
	return q_chaves;
	
}

void lerPrograma(const char* nome, program& pg){
	/* carrega o arquivo do programa */
	FILE *programa = fopen(nome, "rb");
	/* captura o tamanho em bytes do programa */
	fseek(programa, 0, SEEK_END);
	int tamanho = ftell(programa);
	rewind(programa);
	/* armazena todo o programa em um array temporario */
	char *temp = new char[tamanho];
	fread(temp, sizeof(char), tamanho, programa);
	/* calcula o tamanho real do programa considerando apenas os comandos */
	int tamanho_real = 0;
	for(int i = 0; i < tamanho; i++){
		if(temp[i] == '<' or temp[i] == '>' or temp[i] == '+' or temp[i] == '-' or temp[i] == '[' or temp[i] == ']' or temp[i] == ',' or temp[i] == '.'){
			tamanho_real++;
		}
	}
	pg.dados = new char[tamanho_real];
	pg.tamanho = tamanho_real;
	int v = 0;
	for(int i = 0; i < tamanho; i++){
		if(temp[i] == '<' or temp[i] == '>' or temp[i] == '+' or temp[i] == '-' or temp[i] == '[' or temp[i] == ']' or temp[i] == ',' or temp[i] == '.'){
			pg.dados[v] = temp[i];
			v++;
		}
	}
	delete[] temp;
	/* fechar o arquivo */
	fclose(programa);
}