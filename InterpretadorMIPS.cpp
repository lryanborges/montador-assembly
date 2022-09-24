#include <iostream>
#include <stdio.h>
#include <fstream>
using namespace std;

unsigned int tipoR_3var(int, int, int*);
unsigned int tipoI_2var(int, int, int*);

int main() {

	char arquivo[50];
	char teste, teste2;
	int i = 0, j = 0, contVariaveis = 0, m = 0, n = 0, contInstrucoes = 1, contImediatos = 0;
	unsigned int mulR = 28; mulR = mulR << 26; // cria o padrão opcode (op) de mul, no padrão R

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
			contImediatos++;
		}
		j++;
	}

	/* Identificador de variáveis		$t0 = 8 e etc	*/
	for (int k = 0; k < i; k++) {
		if (codigo[k] == 't') {
			variaveis[m] = codigo[k + 1] - '0' + 8;	// Utilizar o variaveis[m] para pegar o código da variável
			m++;
		}
		if (codigo[k] == 's') {
			variaveis[m] = codigo[k + 1] - '0' + 16; // -'0' por conta da tabela ASCII (transição char ~ int), "+8" e "+16" pela posição dos registradores
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

	/* Instruções em decimal */
	j = 0;
	for (int k = 0; k < i; k++) {
		if (codigo[k] == 'a' && codigo[k + 1] == 'd' && codigo[k + 2] == 'd') {
			if (codigo[k + 3] == 'u') {
				instrucoes[j] = tipoR_3var(0, 33, variaveis);
				j++;
			}
			if (codigo[k + 3] == 'i') {
				instrucoes[j] = tipoI_2var(8, 7, variaveis); // Mudar esse "7" pro vetor de imediatos
				j++;
			}
			else {
				instrucoes[j] = tipoR_3var(0, 32, variaveis);
				j++;
			}
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

	cout << '\n' << instrucoes[0];
	cout << '\n' << instrucoes[1];

	fin.close();

	delete[] imediatos;
	delete[] instrucoes;
	delete[] variaveis;
	delete[] codigo;

}

unsigned int tipoR_3var(int opcode, int function, int* variaveis) {

	int l = 0;
	unsigned int valor;

	valor = (variaveis[l + 1] << 21) + (variaveis[l + 2] << 16) + (variaveis[l] << 11) + (0 << 6) + function;
	l = l + 3;	// +3 porque essa instrução utiliza 3 variáveis

	return valor;

	variaveis[0] = variaveis[l];
	variaveis[1] = variaveis[l+1];
	variaveis[2] = variaveis[l+2];

}

// Melhorar essa função aqui, fiz só pra teste

unsigned int tipoI_2var(int opcode, int immediate, int* variaveis) {

	int l = 0;
	unsigned int valor;

	valor = (opcode << 26) + (variaveis[l + 4] << 21) + (variaveis[l + 3] << 16) + immediate;

	return valor;

}