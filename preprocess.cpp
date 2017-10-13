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

map<int, vector<int> > gen_list_from_file() {
	string line;
	vector< vector<int> > data;
	map<int, vector<int> > data_map;
	int index = 0;
	while (getline(cin, line)) {
		vector<int> numbers;
		istringstream split(line);
		string value;
		while (getline(split, value, ',')) {
			numbers.push_back(stoi(value));
		}
		data_map.insert(pair<int, vector<int> > (index, numbers));
		index++;
	}
	return data_map;
}

void write_to_file(string file_name, map<int, vector<int> > data) {
	ofstream outFile(file_name);
	for (const auto& index : data) {
		outFile << index.first << ',';
		for (int i = 0; i < index.second.size(); i++) {
			outFile << index.second[i];
			if (i < index.second.size() - 1) {
				outFile << ',';
			}
		}
		outFile << endl;
	}
	outFile.close();
}

void write_multimap_to_file(string file_name, map<int, map<int, vector<int> > > data) {
	ofstream outFile(file_name);
	for (const auto& file : data) {
		outFile << file.first << ',';
		for (const auto& index : file.second) {
			outFile << index.first << ',';
			for (int i = 0; i < index.second.size(); i++) {
				outFile << index.second[i];
				if (i < index.second.size() - 1) {
					outFile << ',';
				}
			}
		}
		outFile << endl;
	}
	outFile.close();
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

bool ordered(const pair<int, int> &a, const pair<int, int> &b) {
    return (a.second < b.second);
}

int main() {
	int cluster_count = 20;
	int latest_center_index = 0;
	int file = 1;
	vector< map<int, vector<int> > > final_data;
	map<int, map<int, vector<int> > > final_cluster_center;
	
	// Reading the input file
	map<int, vector<int> > data_map = gen_list_from_file();
	
	// Calculating the total number of elements that should be in one cluster
	int total_elements = (data_map.size() - cluster_count) / cluster_count;
	while (cluster_count > 0) {
		int count = 0;
		vector< pair <int, int> > inv_distance;
		map<int, vector<int> > cluster_data_map;
		map<int, vector<int> > cluster_center_map;
		
		// Storing the cluster center points
		cluster_center_map[latest_center_index] = data_map[latest_center_index];
		
		// Storing the cluster center among data points also
		cluster_data_map[latest_center_index] = data_map[latest_center_index];
		
		// Removing the cluster center from the initial map of input permutations
		data_map.erase(latest_center_index);
		final_cluster_center[file] = cluster_center_map;
		
		// Step for transformation before actually calculating the inversions 
		vector<int> indexes = create_indices(cluster_center_map[latest_center_index]); 
		for (auto& data : data_map) {
			vector<int> substitute = create_substitution(indexes, data.second);
			// Calculating the inversions
			inv_distance.push_back(make_pair(data.first, sort_and_count(substitute, 0, (substitute.size() - 1))));
		}
		
		// Sorting the inversions
		sort(inv_distance.begin(), inv_distance.end(), ordered);
		
		// Storing the index of next center points
		latest_center_index = inv_distance[inv_distance.size()-1].first;
		
		// Storing the set of elements in the cluster based on center point
		while (count < total_elements) {
			int data_index = inv_distance[count].first;
			cluster_data_map[data_index] = data_map[data_index];
			data_map.erase(data_index);
			count++;
		}
		final_data.push_back(cluster_data_map);
		cluster_count--;
		file++;
	}
	// Writing to file both the center points of cluster and the data points.
	write_multimap_to_file("centers.txt", final_cluster_center);
	for (auto& cluster : final_cluster_center) {
		write_to_file(("file" + to_string(cluster.first) + ".txt"), final_data[cluster.first - 1]);
	}
}