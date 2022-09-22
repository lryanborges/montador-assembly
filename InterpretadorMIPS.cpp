#include <iostream>
#include <stdio.h>
#include <fstream>
using namespace std;

int main() {

	char arquivo[50];
	char teste, teste2;
	int i = 0, j = 0;

	cout << "Digite o nome do arquivo: ";
	cin >> arquivo;

	ofstream fout;
	ifstream fin;
	fin.open(arquivo);

	if (!fin.is_open()) {
		cout << "A abertura do arquivo falhou!" << endl;
	}

	fin.get(teste);

	while (fin.good()) {
		cout << teste;
		fin.get(teste);
		i++;
	}

	char* codigo = new char[i];

	cout << "\nNumero total de cacteres: " << i << endl;

	fin.close();

	fin.open(arquivo);

	while (fin.get(teste)) {
		codigo[j] = teste;
		j++;
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
	} */

	cout << codigo;

	fin.close();

}