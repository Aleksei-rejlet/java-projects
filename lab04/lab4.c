#include <stdio.h>
#include <stdlib.h>

int **readMatrix(const char *file_name, int *rows, int *cols)
{
    FILE *file = fopen(file_name, "r");
    if (file == NULL)
    {
        printf("Error: Unable to open file '%s'\n", file_name);
        return NULL;
    }
    fscanf(file, "%d %d", rows, cols);

    int **matrix = (int **)malloc(*rows * sizeof(int *));
    for (int i = 0; i < *rows; i++)
    {
        matrix[i] = (int *)malloc(*cols * sizeof(int));
    }

    // Read the matrix from the file
    for (int i = 0; i < *rows; i++)
    {
        for (int j = 0; j < *cols; j++)
        {
            if (fscanf(file, "%d", &matrix[i][j]) != 1)
            {
                printf("Error: Unable to read matrix element at (%d, %d)\n", i, j);
                fclose(file);
                return NULL;
            }
        }
    }

    fclose(file);
    return matrix;
}

int main(int argc, char **argv)
{
    const char *matrix_file = "m55.txt"; // Specify the file name
    int rows, cols;
    int **matrix = readMatrix(matrix_file, &rows, &cols);
    if (matrix != NULL)
    {
        printf("Matrix read successfully:\n");
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }

        // Free memory
        for (int i = 0; i < rows; i++)
        {
            free(matrix[i]);
        }
        free(matrix);
    }
    return 0;
}