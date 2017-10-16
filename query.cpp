#include<stdio.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<iterator>
#include<sstream>
#include<string>
#include<algorithm>
#include<map>
#include<numeric>
using namespace std;

vector< vector<int> > gen_list_from_file() {
	string line;
	vector< vector<int> > data;
	while (getline(cin, line)) {
		vector<int> numbers;
		istringstream split(line);
		string value;
		while (getline(split, value, ',')) {
			numbers.push_back(stoi(value));
		}
		data.push_back(numbers);
	}
	return data;
}

map<int, vector<int> > gen_map_from_file(string file_name, map<int, int> &file_map) {
	string line;
	vector< vector<int> > data;
	map<int, vector<int> > data_map;
	ifstream inputFile(file_name);
	while (getline(inputFile, line)) {
		vector<int> numbers;
		istringstream split(line);
		string value;
		while (getline(split, value, ',')) {
			numbers.push_back(stoi(value));
		}
		int index = numbers[1];
		file_map[index] = numbers[0];
		numbers.erase(numbers.begin(), numbers.begin() + 2);
		data_map.insert(pair<int, vector<int> > (index, numbers));
	}
	return data_map;
}

map<int, vector<int> > gen_map(string file_name) {
	string line;
	vector< vector<int> > data;
	map<int, vector<int> > data_map;
	ifstream inputFile(file_name);
	while (getline(inputFile, line)) {
		vector<int> numbers;
		istringstream split(line);
		string value;
		while (getline(split, value, ',')) {
			numbers.push_back(stoi(value));
		}
		int index = numbers[0];
		numbers.erase(numbers.begin());
		data_map.insert(pair<int, vector<int> > (index, numbers));
	}
	return data_map;
}

vector<int> create_indices(vector<int> query) {
	vector<int> indices(query.size());
	for (int i = 0; i < query.size(); i++) {
		indices[query[i]] = i;
	}
	return indices;
}

vector<int> create_substitution(vector<int> indices, vector<int> data) {
	vector<int> substitute;
	for (int i = 0; i < data.size(); i++) {
		substitute.push_back(indices[data[i]]);
	}
	return substitute;
}

int merge_and_count(vector<int> &num, int leftIndex, int middle, int rightIndex) {
	int temp[num.size()];
	int i = leftIndex;
	int j = middle;
	int k = leftIndex;
	int inv_count = 0;
	while ((i <= middle - 1) && (j <= rightIndex)) {
		if (num[i] <= num[j]) {
			temp[k++] = num[i++];
		} else {
			temp[k++] = num[j++];
			inv_count += middle - i;
		}
	}
	while(i <= middle - 1) {
		temp[k++] = num[i++];
	}
	while(j <= rightIndex) {
		temp[k++] = num[j++];
	}
	for(i = leftIndex; i <= rightIndex; i++) {
		num[i] = temp[i];
	}
	return inv_count;
}

int sort_and_count(vector<int> &num, int leftIndex, int rightIndex) {
	int inv_count = 0;
	int middle;
	if (rightIndex > leftIndex) {
		middle = (leftIndex + rightIndex) / 2;
		inv_count = sort_and_count(num, leftIndex, middle);
		inv_count += sort_and_count(num, middle + 1, rightIndex);
		inv_count += merge_and_count(num, leftIndex, middle + 1, rightIndex);
	}
	return inv_count;
}

int count_inversion(vector<int> query, vector<int> data) {
	int count = 0;
	for (int i = 0; i < data.size(); i++) {
		for (int j = i + 1; j < data.size(); j++) {
			if (query[data[i]] > query[data[j]]) {
				count++;
			}
		}
	}
	return count;
}

bool ordered(const pair<int, int> &a, const pair<int, int> &b) {
    return (a.second < b.second);
}

void write_to_file(string file_name, vector< vector<int> > data) {
	ofstream outFile(file_name);
	for (int i = 0; i < data.size(); i++) {
		for (int j = 0; j < data[i].size(); j++) {
			outFile << data[i][j] << ' ';
		}
		outFile << '\n';
	}
	outFile.close();
}

int main() {
	map<int, int> file_map;
	vector< vector<int> > final_index;
	// Reading the query file
	vector< vector<int> > query = gen_list_from_file();
	// Reading the center file
	map<int, vector<int> > center_points = gen_map_from_file("centers.txt", file_map);
	for (int i = 0; i < query.size(); i++) {
		int min_inv_distance = 999;
		vector< pair <int, int> > inv_distance;
		vector <int> min_center_index;
		// Step for transformation before actually calculating the inversions
		vector<int> indexes = create_indices(query[i]);
		for (auto& data : center_points) {
			vector<int> substitute = create_substitution(indexes, data.second);
			inv_distance.push_back(make_pair(data.first, sort_and_count(substitute, 0, (substitute.size() - 1))));
		}
		// Sorting the inversions
		sort(inv_distance.begin(), inv_distance.end(), ordered);
		// No. of clusters to visit
		for (int index = 0; index < 10; index++) {
			min_center_index.push_back(inv_distance[index].first);
		}
		vector <int> min_final_index; 
		// Calculate the inversion inside the cluster
		for (int j = 0; j < min_center_index.size(); j++) {
			map<int, vector<int> > data_points = gen_map("file" + to_string(file_map[min_center_index[j]]) + ".txt");
			for (auto& data_point : data_points) {
				vector<int> subs = create_substitution(indexes, data_point.second);
				int inv_count = sort_and_count(subs, 0, (subs.size() - 1));
				if (min_inv_distance > inv_count) {
					min_inv_distance = inv_count;
					min_final_index.clear();
					min_final_index.push_back(data_point.first);
				}
				else if (min_inv_distance == inv_count) {
					min_final_index.push_back(data_point.first);	
				}
			}
		}
		final_index.push_back(min_final_index);
	}
	write_to_file("output.txt", final_index);
}