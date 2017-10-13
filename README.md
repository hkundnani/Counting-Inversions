# Counting-Inversions
Find the closest permutation of a query from a set of million permutations of 32 dimension vectors

To run the project follow the below mentioned instructions	

Run the make command, which will execute the below mentioned steps one by one considering the input file as input.txt and query file as query.txt

OR to manually run each step follow the below steps

To compile and run the preprocess.cpp file:
	g++ -std=c++11 preprocess.cpp
	time ./a.out < input.txt
Where input.txt can be any file which consist of different permutations against which the query has to be computed.

To compile and run the query.cpp file:
	g++ -std=c++11 query.cpp
	time ./a.out < query.txt
Where query.txt can be any file which consist of permutations for which the nearest neighbor has to be computed.

The final output is stored in output.txt file.
		 
Note: 
1. We consider the starting index of permutations in input.txt as 0. So the first permutation is at index 0, second is at index 1 and so on. 
2. Our approach writes all the indexes of nearest permutations in a file named output.txt after the whole computation is finished. The program will not generate output.txt if terminated before the execution is complete.

Our approach to the assignment:

We first looked at different distance methods that could be used for counting the inversions, methods like Kendall Tau’s distance, which is similar to counting inversions but varied in the way it counts, it counts the number of pairwise disagreements or discordant pairs between two ranking lists. It didn't give us the correct inversion count hence we used the merge sort method to count the inversions between two vectors.

We looked for other possible ways to represent the inversions like looking for any properties in representing them in inversion tables
The inversion table b1b2 . . . bn of the permutation a1a2 . . . an is obtained by letting bj be the number of elements to the left of j that are greater than j. In other words, bj is the number of inversions whose second component is j. It follows, for example, that the permutation
Eg: 
Permutation : 5, 9, 1, 8, 2, 6, 4, 7, 3 
has 
Inversion table: 2, 3, 6, 4, 0, 2, 2, 1, 0

since 5 and 9 are to the left of 1; 5, 9, 8 are to the left of 2; etc. This permutation has 20 inversions in all w.r.t 1, 2, 3, 4, 5, 6, 7, 8, 9.
The other way which we thought of is relating it to the Cartesian coordinate system. Like in cartesian coordinate system we looked if we can determine a permutation is nearer to other permutation w.r.t a given set of certain number of points. These two ways did not give any favourable results.

Finally, we took the approach of clustering the input file to reduce the number of permutations the query file reads through. We followed the following way for clustering the permutations

Preprocessing Steps:
1. Take a random(first) permutation as a center and look for (no:of permutations/no:of clusters) permutations near to the center by computing inversions against other permutations in the input file and add the nearest to the first cluster.
2. The farthest point from the previous center is considered as the new center and permutations are grouped in similar way.
3. This process is repeated until all the permutations are divided among choosen number of clusters.
4. Number of clusters are choosen based on cross-validation technique by comparing output of different number of queryset.

Computation Steps:
1. Inversion count is computed against Query and the center points of each cluster
2. The center with minimum inversion count is visited and then inversion is computed against all the permutations of that cluster.
3. To consider the boundary cases we also visit the next nearest cluster and compute inversion against each permutation of that cluster
4. Store the smallest inversion distance permutation's index.
5. This process is repeated against all queries in queries.txt.
