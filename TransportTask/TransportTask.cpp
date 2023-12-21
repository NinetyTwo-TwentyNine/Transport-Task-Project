// TransportTask.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>

#include "MatrixFunctions.cpp"

using namespace std;

vector <vector<pair<int, bool>>> delta_array;
vector <vector<int>> amount_matrix, cost_matrix;
vector <int> stockpiles, requirements;

void printMatrix() {
	printMatrix(amount_matrix, cost_matrix, stockpiles, requirements);
}
void fillMatrix() {
	fillMatrix(amount_matrix, cost_matrix, stockpiles, requirements);
}
void countPotentials() {
	countPotentials(amount_matrix, cost_matrix, delta_array);
}
void rebuildMatrixFromPos(pair <int, int> pos) {
	rebuildMatrixFromPos(pos, amount_matrix);
}

int main()
{
	setlocale(LC_ALL, "");

	int x, y;
	cout << "Введите количество строчек матрицы: ";
	cin >> x;
	while (x <= 0) {
		cout << "Данное число было введено неверно. Введите его ещё раз: ";
		cin >> x;
	}
	cout << "Введите количество столбцов матрицы: ";
	cin >> y;
	while (y <= 0) {
		cout << "Данное число было введено неверно. Введите его ещё раз: ";
		cin >> y;
	}

	stockpiles = vector <int>(x);
	for (int i = 0; i < x; i++) {
		cout << "Введите запас №" << (i + 1) << ": ";
		cin >> stockpiles[i];
		while (stockpiles[i] <= 0) {
			cout << "Данное число было введено неверно. Введите его ещё раз: ";
			cin >> stockpiles[i];
		}
	}

	requirements = vector <int>(y);
	for (int i = 0; i < y; i++) {
		cout << "Введите потребность №" << (i + 1) << ": ";
		cin >> requirements[i];
		while (requirements[i] <= 0) {
			cout << "Данное число было введено неверно. Введите его ещё раз: ";
			cin >> requirements[i];
		}
	}

	cost_matrix = vector <vector<int>>(x, vector<int>(y));
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			cout << "Введите стоимость №" << (i + 1) << "-" << (j + 1) << ": ";
			cin >> cost_matrix[i][j];
			while (cost_matrix[i][j] <= 0) {
				cout << "Данное число было введено неверно. Введите его ещё раз: ";
				cin >> cost_matrix[i][j];
			}
		}
	}
	amount_matrix = vector <vector<int>>(x, vector<int>(y, -1));

	printMatrix();

	int stockpiles_sum = getVectorSum(stockpiles);
	int requirements_sum = getVectorSum(requirements);

	if (stockpiles_sum != requirements_sum) {
		if (stockpiles_sum < requirements_sum) {
			cout << "Добавление дополнительной строчки (запаса)" << endl;
			stockpiles.push_back(requirements_sum - stockpiles_sum);
			x++;
			amount_matrix.push_back(vector<int>(y, -1));
			cost_matrix.push_back(vector<int>(y, 0));
		}
		else {
			cout << "Добавление дополнительного столбца (потребности)" << endl;
			requirements.push_back(stockpiles_sum - requirements_sum);
			y++;
			for (int i = 0; i < x; i++) {
				amount_matrix[i].push_back(-1);
				cost_matrix[i].push_back(0);
			}
		}
		printMatrix();
	}

	fillMatrix();
	printMatrix();

	int matrix_rank = amount_matrix.size() + amount_matrix[0].size() - 1;
	cout << "Ранг матрицы = " << matrix_rank << endl;
	int filled_elements_amount = countFilledElements(amount_matrix);
	cout << "Количество заполненных элементов = " << filled_elements_amount << endl << endl;

	while (filled_elements_amount < matrix_rank) {
		cout << "Заполнение ещё одного элемента:" << endl;
		pair <int, int> pos = getBestSpotToFill(amount_matrix, cost_matrix);
		amount_matrix[pos.first][pos.second] = 0;
		filled_elements_amount++;
		printMatrix();
	}

	bool shouldContinue = true;
	while (shouldContinue) {
		countPotentials();
		pair <int, int> pos(0, 0);
		for (int i = 0; i < delta_array.size(); i++) {
			for (int j = 0; j < delta_array[i].size(); j++) {
				if (!delta_array[i][j].second) { continue; }
				if (delta_array[i][j].first < delta_array[pos.first][pos.second].first) {
					pos = pair <int, int>(i, j);
				}
			}
		}
		shouldContinue = (delta_array[pos.first][pos.second].first < 0);
		if (shouldContinue) {
			rebuildMatrixFromPos(pos);
			cout << "Перестройка матрицы: " << endl;
			printMatrix();
		}
	}

	cout << "Отрицательные потенциалы отсутствуют." << endl;

	int s_f = 0;
	for (int i = 0; i < amount_matrix.size(); i++) {
		for (int j = 0; j < amount_matrix[i].size(); j++) {
			if (amount_matrix[i][j] >= 0) {
				s_f += amount_matrix[i][j] * cost_matrix[i][j];
			}
		}
	}
	cout << "S(f) = " << s_f << endl;
	return 0;
}