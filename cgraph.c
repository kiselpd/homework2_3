#include <stdio.h>
#include <stdlib.h>

#define MAX(x, y) ((x>=y) ? (x) : (y))               //search max
#define DIGIT 10                                     //const of digit

int TREE(int **array, int N);
int SUM_OF_EDGE(int* array, int N);
int LOOP(int x);
int EDGE_BETWEEN_TOPS(int* array, int N);
void FREE_ARRAY(int** array, int N, int*array_name);
int CHECK_TOP(int* array, int N, int top);

int main(void) {
	FILE* file_graph;
	int top = 0, first_top = 0, second_top = 0, check = 0, max_top = 1, error = 0;
	char get_symbol;

	int** arr = (int**)calloc(max_top, sizeof(int*));             //creating a array of pointers to pointers
	arr[0] = (int*)calloc(max_top, sizeof(int));

	int* arr_name = (int*)malloc(sizeof(int));                    //array for names of tops
	int max_name = 1;

	file_graph = fopen("graph.dot", "w");                         //open the file for writing
	fprintf(file_graph, "graph new{\n");                          //declaration the graph

	printf("Enter the edges connected to the graph. Example:\n1--2\n2--3\nPush \"Enter\" for exit. Your graph:\n");

	for (;;) {
		get_symbol = getchar();
		fputc(get_symbol, file_graph);                            //creating the graph

		if ((get_symbol <= '9') && (get_symbol >= '0'))
			top = top * DIGIT + (int)(get_symbol - '0');          //input symbol

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
		}

		if ((get_symbol == '\n') && (first_top + second_top == 0))
			break;                                                                             //finish the program, if you push "Enter" 2 times

		if ((get_symbol == '\n') && (second_top != 0) && (first_top == 0)) {
			if (CHECK_TOP(arr_name, max_name, second_top))                      
				second_top = CHECK_TOP(arr_name, max_name, second_top);
			else {
				arr_name = (int*)realloc(arr_name, (max_name) * sizeof(int));
				arr_name[max_name - 1] = second_top;
				second_top = max_name;
				max_name++;
				arr = (int**)realloc(arr, sizeof(int*) *second_top);
				arr[second_top-1]= (int*)calloc(second_top, sizeof(int));
				max_top = second_top;
			}
		}

		if ((first_top != 0) && (second_top != 0)) {

			if (CHECK_TOP(arr_name, max_name, first_top))                       //check first_top in array of names
				first_top = CHECK_TOP(arr_name, max_name, first_top);
			else {
				arr_name = (int*)realloc(arr_name, (max_name) * sizeof(int));
				arr_name[max_name - 1] = first_top;                                //extension of array with name
				first_top = max_name;
				max_name++;
			}

			if (CHECK_TOP(arr_name, max_name, second_top))                      //check second_top in array of name
				second_top = CHECK_TOP(arr_name, max_name, second_top);
			else {
				arr_name = (int*)realloc(arr_name, (max_name) * sizeof(int));
				arr_name[max_name - 1] = second_top;                                 //extension of array with name
				second_top = max_name;
				max_name++;
			}
			
			if (max_top < MAX(first_top, second_top)) {                                     //extension of array
				arr = (int**)realloc(arr, sizeof(int*) * MAX(first_top, second_top));

				for (int i = 0; i < MAX(first_top, second_top); i++) {
					if (!arr[i])
						arr[i] = (int*)calloc(MAX(first_top, second_top), sizeof(int));
					else {
						arr[i] = (int*)realloc(arr[i], sizeof(int) * MAX(first_top, second_top));
						for (int j = max_top; j < MAX(first_top, second_top); j++) arr[i][j] = 0;
					}
					max_top = MAX(first_top, second_top);
				}
			}
				arr[second_top - 1][first_top - 1] += 1;                
			first_top = 0;
			second_top = 0;
		}
	}

	error = TREE(arr, max_top);                            //error(graph is not a tree)

	if (error) 
		fprintf(file_graph, "label=\"This graph IS NOT a tree!\"}");
	else                                                                             //print result in file
		fprintf(file_graph, "label=\"This graph IS a tree!\"}");

	FREE_ARRAY(arr, max_top, arr_name);                             //free array
	fclose(file_graph);
	system("dot graph.dot -Tpng -o graph.png");           //creating file(png) with graph in terminal

	return 0;
}

                                            //*********FUNCTIONS***********//
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

int SUM_OF_EDGE(int* array, int N) {                    //the function checks a sum of edges
	int sum = 0;
	for (int i = 0; i < N; i++) {          
		if (array[i])
			sum = sum + array[i];
	}
	return sum;
}

int LOOP(int x) {                                       //the function checks a loop
	if (x)
		return 1;                  
	else
		return 0;
}

int EDGE_BETWEEN_TOPS(int* array, int N) {               //the function checks the number of edges between tops
	for (int i = 0; i < N; i++) 
		if (array[i] > 1) return 1;                
	return 0;
}

void FREE_ARRAY(int** array, int N, int *array_name) {                    //the function free array
	for (int i = 0; i < N; i++)
		free(array[i]);           
	free(array);
	free(array_name);
}

int CHECK_TOP(int* array, int N, int top) {              //the function check top in array of name
	for (int i = 0; i < N; i++) {
		if (array[i] == top) {
			top = i+1;
			return top;
		}
	}
	return 0;
}
