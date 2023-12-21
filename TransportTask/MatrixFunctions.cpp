#include <iostream>
#include <vector>
#include <random>
#include <ctime>

using namespace std;


int getVectorSum(vector <int> vec) {
	int result = 0;
	for (int i = 0; i < vec.size(); i++) {
		result += vec[i];
	}
	return result;
}

void printMatrix(vector <vector<int>> amount_matrix, vector <vector<int>> cost_matrix, vector <int> stockpiles, vector <int> requirements) {
	int x = cost_matrix.size();
	int y = cost_matrix[0].size();

	cout << "\n" << "Пост.";
	for (int i = 0; i <= y / 2; i++) {
		cout << "\t";
	}
	cout << "Потр.";
	for (int i = 0; i < (y - y / 2); i++) {
		cout << "\t";
	}
	cout << "Запас" << endl;

	for (int i = 0; i < y; i++) {
		cout << "\t" << "B" << (i + 1);
	}
	cout << "\n";

	for (int i = 0; i < x; i++) {
		for (int j = 0; j <= y + 1; j++) {
			if (j == 0) {
				cout << "A" << (i + 1) << "\t";
			}
			else if (j != y + 1) {
				cout << amount_matrix[i][j - 1] << "/" << cost_matrix[i][j - 1] << "\t";
			}
			else {
				cout << stockpiles[i] << "\n";
			}
		}
	}

	cout << "Потр." << "\t";
	for (int i = 0; i < y; i++) {
		cout << requirements[i] << "\t";
	}
	cout << getVectorSum(stockpiles) << "/" << getVectorSum(requirements) << endl << endl;
}

void fillMatrix(vector <vector<int>>& amount_matrix, vector <vector<int>> cost_matrix, vector <int> stockpiles, vector <int> requirements) {
	for (int i = 0; i < amount_matrix.size(); i++) {
		for (int j = 0; j < amount_matrix[i].size(); j++) {
			amount_matrix[i][j] = -1;
		}
	}

	pair<int, int> pos(0, 0);
	while (pos.first < amount_matrix.size() && pos.second < amount_matrix[0].size()) {
		//cout << "pos(" << pos.first << "," << pos.second << ")" << endl;

		int y_filled = 0;
		for (int i = 0; i < pos.first; i++) {
			if (amount_matrix[i][pos.second] >= 0) {
				y_filled += amount_matrix[i][pos.second];
			}
		}

		int x_filled = 0;
		for (int i = 0; i < pos.second; i++) {
			if (amount_matrix[pos.first][i] >= 0) {
				x_filled += amount_matrix[pos.first][i];
			}
		}

		//cout << "x filled: " << x_filled << endl;
		//cout << "y filled: " << y_filled << endl;

		int filler = min(stockpiles[pos.first] - x_filled, requirements[pos.second] - y_filled);
		amount_matrix[pos.first][pos.second] = filler;
		if (stockpiles[pos.first] - x_filled == filler) {
			pos.first++;
		}
		if (requirements[pos.second] - y_filled == filler) {
			pos.second++;
		}
	}
}

bool checkForNotInitiatedVars(vector <pair<int, bool>> potential_u_array, vector <pair<int, bool>> potential_v_array) {
	for (pair<int, bool> i : potential_u_array) {
		if (!i.second) {
			return true;
		}
	}
	for (pair<int, bool> i : potential_v_array) {
		if (!i.second) {
			return true;
		}
	}
	return false;
}

void countPotentials(vector <vector<int>> amount_matrix, vector <vector<int>> cost_matrix, vector <vector<pair<int, bool>>>& delta_array) {
	delta_array = vector <vector<pair<int, bool>>>(cost_matrix.size(), vector<pair<int, bool>>(cost_matrix[0].size(), pair<int, bool>(0, false)));

	vector <pair<int, bool>> potential_u_array(amount_matrix.size(), pair<int, bool>(0, false));
	vector <pair<int, bool>> potential_v_array(amount_matrix[0].size(), pair<int, bool>(0, false));
	potential_u_array[0].second = true;

	while (checkForNotInitiatedVars(potential_u_array, potential_v_array)) {
		for (int i = 0; i < amount_matrix.size(); i++) {
			for (int j = 0; j < amount_matrix[i].size(); j++) {
				if (amount_matrix[i][j] < 0) { continue; }
				if (potential_u_array[i].second == potential_v_array[j].second) { continue; }

				if (!potential_u_array[i].second) {
					potential_u_array[i].first = cost_matrix[i][j] - potential_v_array[j].first;
					potential_u_array[i].second = true;
				}
				else {
					potential_v_array[j].first = cost_matrix[i][j] - potential_u_array[i].first;
					potential_v_array[j].second = true;
				}
			}
		}
	}

	for (int i = 0; i < potential_u_array.size(); i++) {
		cout << "U" << (i + 1) << " = " << potential_u_array[i].first << endl;
	}
	for (int i = 0; i < potential_v_array.size(); i++) {
		cout << "V" << (i + 1) << " = " << potential_v_array[i].first << endl;
	}
	cout << endl;

	for (int i = 0; i < amount_matrix.size(); i++) {
		for (int j = 0; j < amount_matrix[i].size(); j++) {
			if (amount_matrix[i][j] >= 0) { continue; }
			
			delta_array[i][j].first = cost_matrix[i][j] - (potential_u_array[i].first + potential_v_array[j].first);
			delta_array[i][j].second = true;
			cout << "C'" << (i + 1) << "-" << (j + 1) << " = U" << (i + 1) << " + V" << (j + 1) << " = " << (potential_u_array[i].first + potential_v_array[j].first) << endl;
			cout << "D" << (i + 1) << "-" << (j + 1) << " = C" << (i + 1) << "-" << (j + 1) << " - C'" << (i + 1) << "-" << (j + 1) << " = " << delta_array[i][j].first << endl;
		}
	}
	cout << endl;
}

int countFilledElements(vector <vector<int>> amount_matrix) {
	int filled_elements_amount = 0;
	for (vector<int> row : amount_matrix) {
		for (int num : row) {
			if (num >= 0) {
				filled_elements_amount++;
			}
		}
	}
	return filled_elements_amount;
}

pair <int, int> getBestSpotToFill(vector <vector<int>> amount_matrix, vector <vector<int>> cost_matrix) {
	int filled_elements = 0;
	pair <int, int> pos, last_pos(0, 0);

	for (int j = 0; j < amount_matrix[0].size(); j++) {
		if (filled_elements != 1) {
			pos = last_pos;
		}
		filled_elements = 0;
		last_pos = pos;

		for (int i = 0; i < amount_matrix.size(); i++) {
			if (amount_matrix[i][j] >= 0) {
				filled_elements++;
			}
			else if (cost_matrix[i][j] < cost_matrix[pos.first][pos.second]) {
				pos = pair<int, int>(i, j);
			}
		}
	}

	if (amount_matrix[pos.first][pos.second] < 0) { return pos; }

	int minCost = -1;
	for (int i = 0; i < amount_matrix.size(); i++) {
		for (int j = 0; j < amount_matrix[i].size(); j++) {
			if (amount_matrix[i][j] < 0) {
				if (minCost == -1 || cost_matrix[i][j] < minCost) {
					minCost = cost_matrix[i][j];
					pos = pair<int, int>(i, j);
				}
			}
		}
	}
	if (minCost == -1) { throw(exception("Wasn't able to find proper spot to fill.")); }
	return pos;
}

void getRebuildPathFromPos(pair <int, int> initial_pos, vector <vector<int>> amount_matrix, vector<vector<pair<int, int>>>& paths, int current_path = -1, bool vertical = true) {
	pair<int, int> current_pos;
	if (current_path == -1) {
		current_pos = initial_pos;
		paths.push_back(vector <pair<int, int>> {current_pos});
		current_path = paths.size() - 1;
	}
	else {
		current_pos = paths[current_path][paths[current_path].size() - 1];
	}

	vector <pair<int, int>> possible_positions(0);
	if (vertical) {
		for (int i = 0; i < amount_matrix.size(); i++) {
			if (i == current_pos.first) { continue; }

			pair<int, int> possiblePos(i, current_pos.second);
			if (possiblePos == initial_pos) {
				paths[current_path].push_back(initial_pos);
				return;
			}

			if (amount_matrix[possiblePos.first][possiblePos.second] >= 0) {
				bool alreadyWasThere = false;
				for (int k = 1; k < paths[current_path].size(); k++) {
					if (paths[current_path][k] == possiblePos) {
						alreadyWasThere = true;
						break;
					}
				}
				if (alreadyWasThere) { continue; }
				possible_positions.push_back(possiblePos);
			}
		}
	}
	else {
		for (int j = 0; j < amount_matrix[0].size(); j++) {
			if (j == current_pos.second) { continue; }

			pair<int, int> possiblePos(current_pos.first, j);
			if (possiblePos == initial_pos) {
				paths[current_path].push_back(initial_pos);
				return;
			}

			if (amount_matrix[possiblePos.first][possiblePos.second] >= 0) {
				bool alreadyWasThere = false;
				for (int k = 1; k < paths[current_path].size(); k++) {
					if (paths[current_path][k] == possiblePos) {
						alreadyWasThere = true;
						break;
					}
				}
				if (alreadyWasThere) { continue; }
				possible_positions.push_back(possiblePos);
			}
		}
	}

	if (possible_positions.empty()) {
		return;
	}

	vector <pair<int, int>> initial_path = paths[current_path];
	for (int i = 0; i < possible_positions.size(); i++) {
		if (i != 0) {
			paths.push_back(initial_path);
			current_path = paths.size() - 1;
		}
		paths[current_path].push_back(possible_positions[i]);
		getRebuildPathFromPos(initial_pos, amount_matrix, paths, current_path, !vertical);
	}
}

void rebuildMatrixFromPos(pair <int, int> pos, vector <vector<int>>& amount_matrix) {
	srand(time(0));

	vector <vector<pair<int, int>>> paths(0);
	getRebuildPathFromPos(pos, amount_matrix, paths);
	
	vector <vector<pair<int, int>>> possible_paths(0);
	for (vector<pair<int, int>> path : paths) {
		if (path[path.size() - 1] != pos || path.size() % 2 == 0) {
			continue;
		}
		possible_paths.push_back(path);
	}
	if (possible_paths.empty()) {
		throw(exception("No valid rebuild paths."));
	}

	vector <pair<int, int>> chosen_path = possible_paths[rand() % possible_paths.size()];
	cout << "Выбранный путь: ";
	for (pair <int, int> pos : chosen_path) {
		cout << "-> (" << (pos.first + 1) << ", " << (pos.second + 1) << ") ";
	}
	cout << "\n";

	int minAmount = 1000000000;
	pair <int, int> minAmountPos;
	for (int i = 1; i < chosen_path.size() - 1; i += 2) {
		if (amount_matrix[chosen_path[i].first][chosen_path[i].second] < minAmount) {
			minAmount = amount_matrix[chosen_path[i].first][chosen_path[i].second];
			minAmountPos = chosen_path[i];
		}
	}

	amount_matrix[pos.first][pos.second] = minAmount;
	amount_matrix[minAmountPos.first][minAmountPos.second] = -1;
	for (int i = 1; i < chosen_path.size() - 1; i++) {
		if (chosen_path[i] == minAmountPos) { continue; }

		if (i % 2 != 0) {
			amount_matrix[chosen_path[i].first][chosen_path[i].second] -= minAmount;
		}
		else {
			amount_matrix[chosen_path[i].first][chosen_path[i].second] += minAmount;
		}
	}
}