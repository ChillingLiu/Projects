#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

//Global Initial Grid from Textbook.
int grid[9][9] = {
	{1, 2, 4, 5, 3, 9, 1, 8, 7},
	{5, 1, 9, 7, 2, 8, 6, 3, 4},
	{8, 3, 7, 6, 1, 4, 2, 9, 5},
	{6, 4, 3, 8, 6, 5, 7, 2, 9},
	{9, 5, 8, 2, 4, 7, 3, 6, 1},
	{7, 6, 2, 3, 9, 1, 4, 5, 8},
	{3, 7, 1, 9, 5, 6, 8, 4, 2},
	{4, 9, 6, 1, 8, 2, 5, 7, 3},
	{2, 8, 5, 4, 7, 3, 9, 1, 6},
};

//Global Results of Checking.
//Indexes 0 for rows, 1 for columns, 2 to 11 for subgrids
int results[11];

void print_grid() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++)
            printf("%d ", grid[i][j]);
        printf("\n");
    }
}

void print_row(int* row) {
	for (int i = 0; i < 9; i++)
		printf("%d ", *(row + i));
	printf("\n\n");
}

void print_column(int column_index) {
	for (int i = 0; i < 9; i++)
		printf("%d\n", *(*(grid + i) + column_index));
	printf("\n");
}

void print_subgrid(int row_start, int column_start) {
	//Columns
	for (int i = 0; i < 3; i++) {
		//Rows
		for (int j = 0; j < 3; j++)
			printf("%d ", *(*(grid + row_start + i) + column_start + j));
		printf("\n");
	}
}

void* check_rows (void* arg) {
	bool existence;
	//ith row.
	for (int i = 0; i < 9; i++) {
		//Intergers from 1 to 9.
		for (int k = 1; k <= 9; k++) {
			//Elements of each row//
			existence = false;
			for (int j = 0; j < 9; j++) {
				//"grid + i" is ith row of grid, "+ j" is jth element.
				//If k exists, break and go to next row, otherwise fail.
				if (*(*(grid + i) + j) == k) {
					existence = true;
					break;
				}
			}
			if (!existence) {
				printf("Row check failed since %d does not exist in %dth row!\n", k, i+1);
				print_row(*(grid + i));
				results[0] = 0;
				pthread_exit(0);
			}
		}
	}
	printf("Rows check succeeded!\n\n");
	results[0] = 1;
	pthread_exit(0);
}

void *check_columns (void* arg) {
	bool existence;
	//ith column.
	for (int i = 0; i < 9; i++) {
		//Intergers from 1 to 9.
		for (int k = 1; k <= 9; k++) {
			existence = false;
			//Elements of each column//
			for (int j = 0; j < 9; j++) {
				//"grid + j" is jth row of grid, "+ i" is jth column.
				//For j = 0 to 9, forms the ith column of the grid.
				//If k exists, break and go to next column, otherwise fail.
				if (*(*(grid + j) + i) == k) {
					existence = true;
					break;
				}
			}
			if (!existence) {
				printf("Column check failed since %d does not exist in %dth column!\n", k, i+1);
				print_column(i);
				results[1] = 0;
				pthread_exit(0);
			}
		}
	}
	printf("Columns check succeeded!\n\n");
	results[1] = 1;
	pthread_exit(0);

}

void *check_subgrid (void* arg) {
	//Casting from void* to int.
	int subgrid_number = *((int*)arg);
	int row_start, column_start;
	//For int i = 0 to 2
	//Subgrid_i's elements of row will be *(grid + row_start + i)
	//Subgrid_i's elements of column will be *(grid + (column_start + i) * 9)
	switch (subgrid_number) {
		case 0:
			row_start = 0;
			column_start = 0;
			break;
		case 1:
			row_start = 0;
			column_start = 3;
			break;
		case 2:
			row_start = 0;
			column_start = 6;
			break;
		case 3:
			row_start = 3;
			column_start = 0;
			break;
		case 4:
			row_start = 3;
			column_start = 3;
			break;
		case 5:
			row_start = 3;
			column_start = 6;
			break;
		case 6:
			row_start = 6;
			column_start = 0;
			break;
		case 7:
			row_start = 6;
			column_start = 3;
			break;
		case 8:
			row_start = 6;
			column_start = 6;
			break;
		default:
			printf("Error on the subgrid_number! Program terminating!\n");
			pthread_exit(0);
	}

	bool existence;
	//Intergers from 1 to 9.
	for (int k = 1; k <= 9; k++) {
		existence = false;
		//Elements of each column.
		for (int i = 0; i < 3; i++) {
			//Elements of each row.
			for (int j = 0; j < 3; j++) {
				//"grid + row_start + i" is ith row of grid.
				//"+ (column_start + j) is jth elements of the row.
				if (*(*(grid + row_start + i) + column_start + j) == k) {
					existence = true;
					break;
				}
			}
		}
		if (!existence) {
			printf("Subgrid %d check failed since %d does not exist!\n", subgrid_number + 1, k);
			print_subgrid(row_start, column_start);
			results[subgrid_number + 2] = 0;
			pthread_exit(0);
		}

	}
	printf("Subgrid %d check succeeded!\n", subgrid_number + 1);
	results[subgrid_number + 2] = 1;
	pthread_exit(0);
}

int main() {
	printf("The default board is:\n");
	print_grid();

	int user_input;
	printf("Enter 0 to proceed with the default board, or 1 to enter your own board: ");
	scanf("%d", &user_input);
	if (user_input == 1) {
		printf("Enter Your Board here:\n");
		for (int i = 0; i < 9; i++) {
			printf("Row %d:\n", i);
			for (int j = 0; j < 9; j++)
				scanf("%d", &grid[i][j]);
		}
	print_grid();
	}
	printf("\n");

	pthread_t rows_thread;
	pthread_t columns_thread;
	pthread_t subgrid_thread[9];
	pthread_attr_t attr; //Set of attributes for the thread
	pthread_attr_init(&attr);

	//Check for rows
	pthread_create(&rows_thread, &attr, check_rows, NULL);
	pthread_join(rows_thread, NULL);

	//Check for columns
	pthread_create(&columns_thread, &attr, check_columns, NULL);
	pthread_join(columns_thread, NULL);

	//Check for subgrids
	for (int i = 0; i < 9; i++) {
		pthread_create(&subgrid_thread[i], &attr, check_subgrid, &i);
		pthread_join(subgrid_thread[i], NULL);
	}

	//Check for validity in the result array
	bool validity = true;
	for (int i = 0; i < 11; i++) {
		if (results[i] == 0) {
			printf("The solution is invalid!\n");
			validity = false;
			break;
		}
	}
	if (validity)
		printf("The solution is valid!\n");
	return 0;
}
