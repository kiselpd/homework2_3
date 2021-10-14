#include <stdio.h>
#include <stdlib.h>

#define MAX(x, y) ((x>=y) ? (x) : (y))               //search max
#define SWAP(x, y) {int t; t = x; x = y; y = t; }    //swap two values
#define DIGIT 10                                     //const of digit


int TREE(int **array, int N);
int SUM_OF_EDGE(int* array, int N);
int LOOP(int x);
int EDGE_BETWEEN_TOPS(int* array, int N);
void FREE_ARRAY(int** array, int N);

int main(void) {
	FILE* file_graph;
	int top = 0, first_top = 0, second_top = 0, check = 0, max_top = 1, error = 0;
	char get_symbol;

	int** arr = (int**)calloc(max_top, sizeof(int*));             //creating a array of pointers to pointers
	arr[0] = (int*)calloc(max_top, sizeof(int));


	file_graph = fopen("graph.dot", "w");                         //open the file for writing
	fprintf(file_graph, "graph new{\n");                          //declaration the graph

	printf("Enter the edges connected to the graph. Example:\n1--2\n2--3\nEnter \"e\" for exit. Your graph:\n");

	while ((get_symbol = getchar()) != 'e') {
		fputc(get_symbol, file_graph);                            //creating the graph

		if ((get_symbol <= '9') && (get_symbol >= '0'))
			top = top * DIGIT + (int)(get_symbol - '0');

		if (get_symbol == '-') 
			check++;

		if (check == 2) {
			first_top = top;
			top = 0;
			check = 0;
		}

		if (get_symbol == '\n') {
			second_top = top;
			top = 0;

			if (first_top > second_top) 
				SWAP(first_top, second_top);
		}

		if ((first_top == 0) && (second_top != 0))
			second_top = 0;

		if ((first_top != 0) && (second_top != 0)) {

			if (max_top < MAX(first_top, second_top)) {                                     //extension of array
				arr = (int**)realloc(arr, sizeof(int*) * MAX(first_top, second_top));

				for (int i = 0; i < MAX(first_top, second_top); i++) {

					if (!arr[i]) {
						arr[i] = (int*)calloc(MAX(first_top, second_top), sizeof(int));

					}                                                                             
					else {
						arr[i] = (int*)realloc(arr[i], sizeof(int) * MAX(first_top, second_top));
						for (int j = max_top; j < MAX(first_top, second_top); j++) arr[i][j] = 0;
					}
					max_top = MAX(first_top, second_top);
				}

			}
			arr[second_top - 1][first_top - 1] += 1;                //filling in the array(matrix)
			first_top = 0;
			second_top = 0;
		}
	}
	
	error = TREE(arr, max_top);                            //error(graph is not a tree)

	if (error) fprintf(file_graph, "label=\"This graph IS NOT a tree!\"}");
	else fprintf(file_graph, "label=\"This graph IS a tree!\"}");

	fclose(file_graph);                        //close file 

	FREE_ARRAY(arr, max_top);                  //free array

	system("dot graph.dot -Tpng -o graph.png");           //creating file(png) with graph in terminal

	return 0;
}


int TREE(int **array, int N) {
	int error = 0;
	int check_sum = 0;
	for (int i = 0; i < N; i++) {
		error = LOOP(array[i][i]);
		error = EDGE_BETWEEN_TOPS(array[i], N);                        //the function check graph for the tree
		check_sum = check_sum + SUM_OF_EDGE(array[i], N);
		if (error) break;
	}
	if (check_sum != (N - 1)) 
		error = 1;
	return error;
}

int SUM_OF_EDGE(int* array, int N) {
	int sum = 0;
	for (int i = 0; i < N; i++) {           //the function checks a sum of edges
		if (array[i])
			sum = sum + array[i];
	}
	return sum;
}

int LOOP(int x) {
	if (x)
		return 1;                  //the function checks a loop
	else
		return 0;
}

int EDGE_BETWEEN_TOPS(int* array, int N) {
	for (int i = 0; i < N; i++) 
		if (array[i] > 1) return 1;                //the function checks the number of edges between tops 
	return 0;
}

void FREE_ARRAY(int** array, int N) {
	for (int i = 0; i < N; i++) free(array[i]);           //the function free array
	free(array);
}