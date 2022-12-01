#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <bitset>
using namespace std;

unsigned int tipoR_3var(int, int, int*);
unsigned int tipoR_2var(int, int, int*);
unsigned int tipoR_2varImmed(int, int, int*, int);
unsigned int tipoR_1var(int, int, int*);
unsigned int tipoR_JR(int, int, int*);
unsigned int tipoI_2var(int, int, int*);
unsigned int tipoI_1var(int, int, int*);
unsigned int tipoI_B(int, int, int*);
unsigned int tipoJ(int, int);
string toBinary(unsigned int);

int var = 0, immed = 0, lab = 0;

int main() {

	char arquivo[50];
	char teste, teste2[50];
	int i = 0, j = 0, l = 0, m = 0, n = 0, contVariaveis = 0, contInstrucoes = 1, contImediatos = 0, contLabels = 0, contLinhas = 0, num = 0, dif = 0, num16bits = 0;

	cout << "Digite o nome do arquivo: ";
	cin >> arquivo;

	ofstream fout;
	ifstream fin;
	fin.open(arquivo);

	if (!fin.is_open()) {
		cout << "A abertura do arquivo falhou!" << endl;
		return 0;
	}

	// Contador de caracteres do código

	fin.get(teste);

	cout << "\n\tArquivo" << endl;
	cout << "---------------------" << endl;

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
		if (teste == ':') {
			contLabels++;
		}
	}

	int* labels = new int[contLabels];

	cout << endl;

	fin.close();

	fin.open(arquivo);

	while (fin.getline(teste2, 50)) {
		for (int o = 0; o < i; o++) {
			if (teste2[o] == ':') {
				labels[l] = 1048576 + contLinhas; // 1048576 é o valor decimal do primeiro endereço de memória (0x00100000)
				l++;
			}
		}
		contLinhas++;
	}

	fin.close();

	char* codigo = new char[i];
	int* variaveis = new int[contVariaveis];
	unsigned int* instrucoes = new unsigned int[contInstrucoes];

	fin.open(arquivo);

	// Colocar o código num char[]
	while (fin.get(teste)) {
		codigo[j] = teste;
		if (codigo[j] >= '0' && codigo[j] <= '9' && (codigo[j - 1] == ' ' || codigo[j - 1] == ',')) {
			contImediatos++;
		}
		if (codigo[j] == ':') {
			//	contLabels++;
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
			if (codigo[k + 1] < '0' || codigo[k + 1] > '9') {
				variaveis[m] = codigo[k] - '0';
				m++;
			}
			if (codigo[k + 1] >= '0' && codigo[k + 1] <= '9') {
				variaveis[m] = ((codigo[k] - '0') * 10) + (codigo[k + 1] - '0');
				m++;
			}
		}
	}

	/* Identificador de imediatos */
	int* imediatos = new int[contImediatos];

	for (int k = 0; k < i; k++) {
		if ((codigo[k - 1] == ' ' || codigo[k - 1] == ',') && codigo[k] >= '0' && codigo[k] <= '9') {
			if ((codigo[k + 1] >= '0' && codigo[k + 1] <= '9') && (codigo[k + 2] >= '0' && codigo[k + 2] <= '9')) {
				imediatos[n] = ((codigo[k] - '0') * 100) + ((codigo[k + 1] - '0') * 10) + (codigo[k + 2] - '0');
				n++;
			}
			if ((codigo[k + 1] >= '0' && codigo[k + 1] <= '9') && !(codigo[k + 2] >= '0' && codigo[k + 2] <= '9')) {
				imediatos[n] = ((codigo[k] - '0') * 10) + (codigo[k + 1] - '0');
				n++;
			}
			if(!(codigo[k + 1] >= '0' && codigo[k + 1] <= '9')) { // MUDAR ISSO PRA UM IF
				imediatos[n] = codigo[k] - '0';	// REVER ISSO AQUI. SÓ TÁ PEGANDO OS IMEDIATOS ENTRE 0 E 9
				n++;
			}
		}
	}

	num16bits = 65535;

	/* Cadastro de instruções Assembly MIPS */
	j = 0;
	for (int k = 0; k < i; k++) {
		if (codigo[k] == 'a' && codigo[k + 1] == 'd' && codigo[k + 2] == 'd') { // add, addi e addu
			if (codigo[k + 3] == 'u' && (codigo[k + 4] == ' ' || codigo[k + 4] == '$')) {	// addu
				instrucoes[j] = tipoR_3var(0, 33, variaveis);
				j++;
			}
			if (codigo[k + 3] == 'i' && codigo[k + 4] != 'u' && (codigo[k + 4] == ' ' || codigo[k + 4] == '$')) {	// addi
				instrucoes[j] = tipoI_2var(8, imediatos[immed], variaveis); 
				j++;
			}
			if (codigo[k + 3] == 'i' && codigo[k + 4] == 'u' && (codigo[k + 5] == ' ' || codigo[k + 5] == '$')) {	// addiu
				instrucoes[j] = tipoI_2var(9, imediatos[immed], variaveis);
				j++;
			}
			if (codigo[k + 3] == ' ' || codigo[k + 3] == '$') {	// add
				instrucoes[j] = tipoR_3var(0, 32, variaveis);
				j++;
			}
		}
		if (codigo[k] == 's' && codigo[k + 2] == 'l') { // srl e sll
			if (codigo[k + 1] == 'l' && (codigo[k + 3] == ' ' || codigo[k + 3] == '$')) {	// sll
				instrucoes[j] = tipoR_2varImmed(0, 0, variaveis, imediatos[immed]);
				j++;
			}
			if (codigo[k + 1] == 'r' && (codigo[k + 3] == ' ' || codigo[k + 3] == '$')) {	// srl
				instrucoes[j] = tipoR_2varImmed(0, 2, variaveis, imediatos[immed]);
				j++;
			}
		}
		if (codigo[k] == 'j' && codigo[k + 1] == 'r' && (codigo[k + 2] == ' ' || codigo[k + 4] == '2')) {	// jr
			instrucoes[j] = tipoR_JR(0, 8, variaveis);
			j++;
		}
		if (codigo[k] == 'm' && codigo[k + 1] == 'f') {	// mfhi e mflo
			if (codigo[k + 2] == 'h' && codigo[k + 3] == 'i' && (codigo[k + 4] == ' ' || codigo[k + 4] == '$')) {	// mfhi
				instrucoes[j] = tipoR_1var(0, 16, variaveis);
				j++;
			}
			if (codigo[k + 2] == 'l' && codigo[k + 3] == 'o' && (codigo[k + 4] == ' ' || codigo[k + 4] == '$')) {	// mflo
				instrucoes[j] = tipoR_1var(0, 18, variaveis);
				j++;
			}
		}
		if (codigo[k] == 'm' && codigo[k + 1] == 'u' && codigo[k + 2] == 'l' && codigo[k + 3] == 't') {	// mult e multu
			if (codigo[k + 4] == 'u' && (codigo[k + 5] == ' ' || codigo[k + 5] == '$')) {	// multu
				instrucoes[j] = tipoR_2var(0, 25, variaveis);
				j++;
			}
			if (codigo[k + 4] == ' ' || codigo[k + 4] == '$') {	// mult
				instrucoes[j] = tipoR_2var(0, 24, variaveis);
				j++;
			}
		}
		if (codigo[k] == 'd' && codigo[k + 1] == 'i' && codigo[k + 2] == 'v') {	// div e divu
			if (codigo[k + 3] == 'u' && (codigo[k + 4] == ' ' || codigo[k + 4] == '$')) {	// divu
				instrucoes[j] = tipoR_2var(0, 27, variaveis);
				j++;
			}
			if(codigo[k + 3] == ' ' || codigo[k + 3] == '$') {	// div
				instrucoes[j] = tipoR_2var(0, 26, variaveis);
				j++;
			}
		}
		if (codigo[k] == 's' && codigo[k + 1] == 'u' && codigo[k + 2] == 'b') {	// sub e subu
			if (codigo[k + 3] == 'u' && (codigo[k + 4] == ' ' || codigo[k + 4] == '$')) {	// subu
				instrucoes[j] = tipoR_3var(0, 35, variaveis);
				j++;
			}
			if (codigo[k + 3] == ' ' || codigo[k + 3] == '$') {	// sub
				instrucoes[j] = tipoR_3var(0, 34, variaveis);
				j++;
			}
		}
		if (codigo[k] == 'a' && codigo[k + 1] == 'n' && codigo[k + 2] == 'd') {	// and e andi
			if (codigo[k + 3] == ' ' || codigo[k + 3] == '$') {	// and
				instrucoes[j] = tipoR_3var(0, 36, variaveis);
				j++;
			}
			if (codigo[k + 3] == 'i' && (codigo[k + 4] == ' ' || codigo[k + 4] == '$')) {	// andi
				instrucoes[j] = tipoI_2var(12, imediatos[immed], variaveis);
				j++;
			}
		}
		if (codigo[k] == 'o' && codigo[k + 1] == 'r') {	// or e ori
			if ((codigo[k + 2] == ' ' || codigo[k + 2] == '$') && codigo[k + 11] == '$') {	// or // Esse "&& codigo[k + 11] == '$'" é só pq no doc de teste n tem ori
				instrucoes[j] = tipoR_3var(0, 37, variaveis);
				j++;
			} else {	// dai precisa desse else, que é pra identificar quando o or não é or mesmo (apenas no doc de teste). // ori
				instrucoes[j] = tipoI_2var(13, imediatos[immed], variaveis);
				j++;
			}
			// if (codigo[k + 2] == 'i' && (codigo[k + 3] == ' ' || codigo[k + 3] == '$')){	// ori
			//	instrucoes[j] = tipoI_2var(13, imediatos[immed], variaveis);
			//	j++;
			// }
		}
		if (codigo[k] == 's' && codigo[k + 1] == 'l' && codigo[k + 2] == 't') {	// slt, sltu, slti e sltiu
			if (codigo[k + 3] == 'u' && (codigo[k + 4] == ' ' || codigo[k + 4] == '$')) {	// sltu
				instrucoes[j] = tipoR_3var(0, 43, variaveis);
				j++;
			}
			if (codigo[k + 3] == ' ' || codigo[k + 3] == '$') {	// slt
				instrucoes[j] = tipoR_3var(0, 42, variaveis);
				j++;
			}
			if (codigo[k + 3] == 'i' && codigo[k + 4] != 'u' && (codigo[k + 4] == ' ' || codigo[k + 4] == '$')) {	// slti
				instrucoes[j] = tipoI_2var(10, imediatos[immed], variaveis);
				j++;
			}
			if (codigo[k + 3] == 'i' && codigo[k + 4] == 'u' && (codigo[k + 5] == ' ' || codigo[k + 5] == '$')) {	// sltiu
				instrucoes[j] = tipoI_2var(11, imediatos[immed], variaveis);
				j++;
			}
		}
		if (codigo[k] == 'm' && codigo[k + 1] == 'u' && codigo[k + 2] == 'l' && (codigo[k + 3] == ' ' || codigo[k + 3] == '$')) {	// mul
			instrucoes[j] = tipoR_3var(28, 2, variaveis);
			j++;
		}
		if (codigo[k] == 'l' && codigo[k + 1] == 'u' && codigo[k + 2] == 'i' && (codigo[k + 3] == ' ' || codigo[k + 3] == '$')) {	// lui
			instrucoes[j] = tipoI_1var(15, imediatos[immed], variaveis);
			j++;
		}
		if (codigo[k] == 'l' && codigo[k + 1] == 'w' && (codigo[k + 2] == ' ' || codigo[k + 2] == '$')) {	// lw
			instrucoes[j] = tipoI_2var(35, imediatos[immed], variaveis);
			j++;
		}
		if (codigo[k] == 's' && codigo[k + 1] == 'w' && (codigo[k + 2] == ' ' || codigo[k + 2] == '$')) {	// sw
			instrucoes[j] = tipoI_2var(43, imediatos[immed], variaveis);
			j++;
		}
		if (codigo[k] == 'b' && codigo[k + 1] == 'e' && codigo[k + 2] == 'q' && (codigo[k + 3] == ' ' || codigo[k + 3] == '$')) {	// beq
			for(int r = k; codigo[r] != '\n'; r++) {	// percorre a linha em que a instrução é encontrada
				if(codigo[r] == ',' && (codigo[r + 1] == ' ' || codigo[r + 1] != '$') && (codigo[r + 2] == 'L')){
					num = codigo[r + 3] - '0';
					dif = labels[num - 1] - (labels[2]);
					instrucoes[j] = tipoI_B(4, num16bits + dif, variaveis); // 65533 -> mips.asm | 65514 -> outro
					j++;
				}
			}
		}
		if (codigo[k] == 'b' && codigo[k + 1] == 'n' && codigo[k + 2] == 'e' && (codigo[k + 3] == ' ' || codigo[k + 3] == '$')) {	// bne
			for(int r = k; codigo[r] != '\n'; r++) {	// percorre a linha em que a instrução é encontrada
				if(codigo[r] == ',' && (codigo[r + 1] == ' ' || codigo[r + 1] != '$') && (codigo[r + 2] == 'L')){
					num = codigo[r + 3] - '0';
					dif = labels[num - 1] - (labels[2] + 1);
					instrucoes[j] = tipoI_B(5, num16bits + dif, variaveis); // 65533 -> mips.asm | 65514 -> outro
					j++;
				}
			}
		}
		if (codigo[k] == 'j' && (codigo[k + 1] == ' ' || codigo[k + 1] == '$')) {	// j
			if(codigo[k + 2] == 'L') {
				num = codigo[k + 3] - '0';
			}
			instrucoes[j] = tipoJ(2, labels[num - 1]);
			j++;
		}
		if (codigo[k] == 'j' && codigo[k + 1] == 'a' && codigo[k + 2] == 'l' && (codigo[k + 3] == ' ' || codigo[k + 3] == '$')) {	// jal
			if(codigo[k + 4] == 'L') {
				num = codigo[k + 5] - '0';
			}
			instrucoes[j] = tipoJ(3, labels[num - 1]);
			j++;
		}
	}

	fin.close();
	
	//cout << "\nLinhas em binario" << endl;
	//cout << "---------------------" << endl;

	for (int k = 0; k < contInstrucoes; k++) {
		bitset<32> bst(instrucoes[k]);
		// cout << "Linha " << k + 1 << ": " << bst << endl;
	} 

	fout.open("saida.bin");

	if (!fout.is_open()) {
		cout << "A abertura do arquivo falhou!" << endl;
	}

	for (int j = 0; j < contInstrucoes; j++) {
		bitset<32> bst(instrucoes[j]);
		fout << bst << endl;
	}

	cout << "\nArquivo saida.bin criado" << endl;

	fout.close();

	delete[] labels;
	delete[] imediatos;
	delete[] instrucoes;
	delete[] variaveis;
	delete[] codigo;

}

unsigned int tipoR_3var(int opcode, int function, int* variaveis) {

	unsigned int valor;

	valor = (opcode << 26) + (variaveis[var + 1] << 21) + (variaveis[var + 2] << 16) + (variaveis[var] << 11) + (0 << 6) + function;
	var = var + 3;	// +3 porque essa instrução utiliza 3 variáveis

	return valor;

}

unsigned int tipoR_2var(int opcode, int function, int* variaveis) {

	unsigned int valor;

	valor = (opcode << 26) + (variaveis[var] << 21) + (variaveis[var + 1] << 16) + (0 << 11) + (0 << 6) + function;
	var = var + 2;

	return valor;

}

unsigned int tipoR_2varImmed(int opcode, int function, int* variaveis, int immediate) {

	unsigned int valor;

	valor = (opcode << 26) + (0 << 21) + (variaveis[var + 1] << 16) + (variaveis[var] << 11) + (immediate << 6) + function;
	var = var + 2;
	immed = immed + 1;

	return valor;

}

unsigned int tipoR_1var(int opcode, int function, int* variaveis) {

	unsigned int valor;

	valor = (opcode << 26) + (variaveis[var] << 11) + (0 << 6) + function;
	var = var + 1;

	return valor;

}

unsigned int tipoR_JR(int opcode, int function, int* variaveis) {

	unsigned int valor;

	valor = (opcode << 26) + (variaveis[var] << 21) + (0 << 6) + function;
	var = var + 1;

	return valor;

}

unsigned int tipoI_2var(int opcode, int immediate, int* variaveis) {

	unsigned int valor;
	
	valor = (opcode << 26) + (variaveis[var + 1] << 21) + (variaveis[var] << 16) + immediate;
	var = var + 2;
	immed = immed + 1;

	return valor;

}

unsigned int tipoI_1var(int opcode, int immediate, int* variaveis) {

	unsigned int valor;

	valor = (opcode << 26) + (0 << 21) + (variaveis[var] << 16) + immediate;
	var = var + 1;
	immed = immed + 1;

	return valor;

}

unsigned int tipoI_B(int opcode, int immedend, int* variaveis) {

	unsigned int valor;

	valor = (opcode << 26) + (variaveis[var] << 21) + (variaveis[var + 1] << 16) + immedend;
	var = var + 2;

	return valor;

}

unsigned int tipoJ(int opcode, int endereco) {

	unsigned int valor;

	valor = (opcode << 26) + endereco;

	return valor;

}
