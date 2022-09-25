#include <iostream>
#include <stdio.h>
#include <fstream>
using namespace std;

unsigned int tipoR_3var(int, int, int*);
unsigned int tipoR_2var(int, int, int*, int);
unsigned int tipoI_2var(int, int, int*);

int var = 0, immed = 0;

int main() {

	char arquivo[50];
	char teste;
	int i = 0, j = 0, m = 0, n = 0, contVariaveis = 0, contInstrucoes = 1, contImediatos = 0;

	cout << "Digite o nome do arquivo: ";
	cin >> arquivo;

	ofstream fout;
	ifstream fin;
	fin.open(arquivo);

	if (!fin.is_open()) {
		cout << "A abertura do arquivo falhou!" << endl;
	}

	// Contador de caracteres do código

	fin.get(teste);

	while (fin.good()) {
		cout << teste;
		fin.get(teste);
		i++;
		if (teste == '$') {
			contVariaveis++;
		}
		if (teste == '\n') {
			contInstrucoes++;
		}
	}

	cout << endl;

	fin.close();

	char* codigo = new char[i];
	int* variaveis = new int[contVariaveis];
	unsigned int* instrucoes = new unsigned int[contInstrucoes];

	fin.open(arquivo);

	// Colocar o código num char[]
	while (fin.get(teste)) {
		codigo[j] = teste;
		if (codigo[j] >= '0' && codigo[j] <= '9' && codigo[j - 1] == ' ') {
			contImediatos++;	// REVER ISSO AQUI. SÓ TÁ PEGANDO OS IMEDIATOS ENTRE 0 E 9
		}
		j++;
	}

	/* Identificador de variáveis		$t0 = 8 e etc	*/
	for (int k = 0; k < i; k++) {
		if (codigo[k - 1] == '$' && codigo[k] == 't') {
			variaveis[m] = codigo[k + 1] - '0' + 8;	// Utilizar o variaveis[m] para pegar o código da variável
			m++;
		}
		if (codigo[k - 1] == '$' && codigo[k] == 's') {
			variaveis[m] = codigo[k + 1] - '0' + 16; // -'0' por conta da tabela ASCII (transição char ~ int), "+8" e "+16" pela posição dos registradores
			m++;
		}
		if (codigo[k - 1] == '$' && (codigo[k] != 't' && codigo[k] != 's')) {
			variaveis[m] = codigo[k] - '0';
			m++;
		}
	}

	/* Identificador de imediatos */
	int* imediatos = new int[contImediatos];

	for (int k = 0; k < i; k++) {
		if (codigo[k - 1] == ' ' && codigo[k] >= '0' && codigo[k] <= '9') {
			if (codigo[k + 1] == ' ' || codigo[k + 1] == '\n') {
				imediatos[n] = codigo[k] - '0';
			}
		}
	}

	/* Cadastro de instruções Assembly MIPS */
	j = 0;
	for (int k = 0; k < i; k++) {
		if (codigo[k] == 'a' && codigo[k + 1] == 'd' && codigo[k + 2] == 'd') { // add, addi e addu
			if (codigo[k + 3] == 'u') {	// addu
				instrucoes[j] = tipoR_3var(0, 33, variaveis);
				j++;
			}
			if (codigo[k + 3] == 'i') {	// addi
				instrucoes[j] = tipoI_2var(8, imediatos[immed], variaveis); // Mudar esse "7" pro vetor de imediatos. Só no desenvolvimento dos tipo I 
				j++;
			}
			if (codigo[k + 3] == ' ' || codigo[k + 3] == '$') {	// add
				instrucoes[j] = tipoR_3var(0, 32, variaveis);
				j++;
			}
		}
		if (codigo[k] == 's' && codigo[k + 2] == 'l') { // srl e sll
			if (codigo[k + 1] == 'l') {	// sll
				instrucoes[j] = tipoR_2var(0, 0, variaveis, imediatos[immed]);
				j++;
			}
			if (codigo[k + 1] == 'r') {	// srl
				instrucoes[j] = tipoR_2var(0, 2, variaveis, imediatos[immed]);
				j++;
			}
		}
		if (codigo[k] == 'j' && codigo[k + 1] == 'r') {	// jr
			instrucoes[j];
			j++;
		}
		if (codigo[k] == 'm' && codigo[k + 1] == 'f') {	// mfhi e mflo
			if (codigo[k + 2] == 'h' && codigo[k + 3] == 'i') {	// mfhi
				instrucoes[j];
				j++;
			}
			if (codigo[k + 2] == 'l' && codigo[k + 3] == 'o') {	// mflo
				instrucoes[j];
				j++;
			}
		}
		if (codigo[k] == 'm' && codigo[k + 1] == 'u' && codigo[k + 2] == 'l' && codigo[k + 3] == 't') {	// mult e multu
			if (codigo[k + 4] == 'u') {	// multu
				instrucoes[j] = tipoR_2var(0, 25, variaveis, 0);
				j++;
			}
			else {	// mult
				instrucoes[j] = tipoR_2var(0, 24, variaveis, 0);
				j++;
			}
		}
		if (codigo[k] == 'd' && codigo[k + 1] == 'i' && codigo[k + 2] == 'v') {	// div e divu
			if (codigo[k + 3] == 'u') {	// divu
				instrucoes[j] = tipoR_2var(0, 27, variaveis, 0);
				j++;
			}
			else {	// div
				instrucoes[j] = tipoR_2var(0, 26, variaveis, 0);
				j++;
			}
		}
		if (codigo[k] == 's' && codigo[k + 1] == 'u' && codigo[k + 2] == 'b') {	// sub e subu
			if (codigo[k + 3] == 'u') {	// subu
				instrucoes[j] = tipoR_3var(0, 35, variaveis);
				j++;
			}
			else {	// sub
				instrucoes[j] = tipoR_3var(0, 34, variaveis);
				j++;
			}
		}
		if (codigo[k] == 'a' && codigo[k + 1] == 'n' && codigo[k + 2] == 'd') {	// and
			instrucoes[j] = tipoR_3var(0, 36, variaveis);
			j++;
		}
		if (codigo[k] == 'o' && codigo[k + 1] == 'r') {	// or
			instrucoes[j] = tipoR_3var(0, 37, variaveis);
			j++;
		}
		if (codigo[k] == 's' && codigo[k + 1] == 'l' && codigo[k + 2] == 't') {	// slt e sltu
			if (codigo[k + 3] == 'u') {	// sltu
				instrucoes[j] = tipoR_3var(0, 43, variaveis);
				j++;
			}
			else {	// slt
				instrucoes[j] = tipoR_3var(0, 32, variaveis);
				j++;
			}
		}
		if (codigo[k] == 'm' && codigo[k + 1] == 'u' && codigo[k + 2] == 'l' && (codigo[k + 3] == ' ' || codigo[k + 3] == '$')) {
			instrucoes[j] = tipoR_3var(28, 2, variaveis);
			j++;
		}

	}

	/* Identificador de labels
	for (int k = 0; k <= i; k++) {
		teste = codigo[k];
		if (teste == ':') {
			cout << "Dois pontos" << endl;
			teste2 = 0;
			while (teste2 != '\n') {

			}
		}
	}

	Códigos de checagem, só pra ver se tá armazenando certinho

	cout << codigo;

	cout << endl;
	for (int j = 0; j < contVariaveis; j++) {
		cout << variaveis[j] << " ";
	}

	cout << endl;

	for (int j = 0; j < contImediatos; j++) {
		cout << imediatos[j] << " ";
	}

	cout << '\n' << instrucoes[0];
	cout << "\nValor de m = " << m;
	cout << "\nImediatos: " << contImediatos;		*/

	for (int k = 0; k < contInstrucoes; k++) {
		cout << "\nLinha " << k + 1 << ": " << instrucoes[k];
	}

	fin.close();

	delete[] imediatos;
	delete[] instrucoes;
	delete[] variaveis;
	delete[] codigo;

}

unsigned int tipoR_3var(int opcode, int function, int* variaveis) {

	unsigned int valor;

	valor = (variaveis[var + 1] << 21) + (variaveis[var + 2] << 16) + (variaveis[var] << 11) + (0 << 6) + function;
	var = var + 3;	// +3 porque essa instrução utiliza 3 variáveis

	return valor;

}

unsigned int tipoR_2var(int opcode, int function, int* variaveis, int immediate) {

	unsigned int valor;

	valor = (0 << 21) + (variaveis[var + 1] << 16) + (variaveis[var] << 11) + (immediate << 6) + function;
	var = var + 2;
	immed = immed + 1;

	return valor;

}

// Melhorar essa função aqui, fiz só pra teste

unsigned int tipoI_2var(int opcode, int immediate, int* variaveis) {

	unsigned int valor;
	
	valor = (opcode << 26) + (variaveis[var + 1] << 21) + (variaveis[var] << 16) + immediate;
	var = var + 2;
	immed = immed + 1;

	return valor;

}