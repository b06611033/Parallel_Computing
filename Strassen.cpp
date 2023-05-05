#include <bits/stdc++.h>
#include <iostream>
#include <iomanip>
#include <string>
#include "time.h"
#include "math.h"
#include "omp.h"
using namespace std;

int num_threads;

void combineMatrix(int c00[], int c01[], int c10[], int c11[], int c[], int split_index)
{
    int n = split_index * 2;
    for (int i = 0; i < split_index; i++)
    {
        for (int j = 0; j < split_index; j++)
        {
            c[i * n + j] = c00[i * split_index + j];
            c[i * n + split_index + j] = c01[i * split_index + j];
            c[(split_index + i) * n + j] = c10[i * split_index + j];
            c[(split_index + i) * n + split_index + j] = c11[i * split_index + j];
        }
    }
}
void addMatrix(int matA[], int matB[], int matC[], int n)
{
    for (int i = 0; i < n * n; i++)
    {
        matC[i] = matA[i] + matB[i];
    }
}

void subMatrix(int matA[], int matB[], int matC[], int n)
{
    for (int i = 0; i < n * n; i++)
    {
        matC[i] = matA[i] - matB[i];
    }
}

void print(int mat[], int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << mat[i * n + j];
            cout << " ";
        }
        cout << endl;
    }
}

void initialize_matrix(int mat[], int n, int max)
{
    for (int i = 0; i < n * n; i++)
    {
        mat[i] = rand() % max;
    }
}

// standard computation
void standard_multiply(int a[], int b[], int answer[], int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int result = 0;
            for (int w = 0; w < n; w++)
            {
                int indexA = i * n + w;
                int indexB = j + n * w;
                result += a[indexA] * b[indexB];
            }
            answer[n * i + j] = result;
        }
    }
}

string identical(int c[], int answer[], int n)
{
    for (int i = 0; i < n * n; i++)
    {
        if (c[i] != answer[i])
            return "incorrect";
    }
    return "correct";
}

void multiply_matrix(int matA[],
                     int matB[], int matC[], int n, int level)
{
    if (level == 0)
    {
        standard_multiply(matA, matB, matC, n);
    }
    else if (n == 1)
    {
        matC[0] = matA[0] * matB[0];
    }
    else
    {
        int split_index = n / 2;

        // create new subarray
        int *a00 = new int[split_index * split_index];
        int *a01 = new int[split_index * split_index];
        int *a10 = new int[split_index * split_index];
        int *a11 = new int[split_index * split_index];
        int *b00 = new int[split_index * split_index];
        int *b01 = new int[split_index * split_index];
        int *b10 = new int[split_index * split_index];
        int *b11 = new int[split_index * split_index];

        // assign value to subarray
        for (int i = 0; i < split_index; i++)
        {
            for (int j = 0; j < split_index; j++)
            {
                a00[i * split_index + j] = matA[i * n + j];
                a01[i * split_index + j] = matA[i * n + split_index + j];
                a10[i * split_index + j] = matA[(split_index + i) * n + j];
                a11[i * split_index + j] = matA[(split_index + i) * n + split_index + j];
                b00[i * split_index + j] = matB[i * n + j];
                b01[i * split_index + j] = matB[i * n + split_index + j];
                b10[i * split_index + j] = matB[(split_index + i) * n + j];
                b11[i * split_index + j] = matB[(split_index + i) * n + split_index + j];
            }
        }

        int *m1 = new int[split_index * split_index];
        int *m2 = new int[split_index * split_index];
        int *m3 = new int[split_index * split_index];
        int *m4 = new int[split_index * split_index];
        int *m5 = new int[split_index * split_index];
        int *m6 = new int[split_index * split_index];
        int *m7 = new int[split_index * split_index];

// Use multiple threads to compute m1 - m7
#pragma omp parallel sections num_threads(num_threads)
        {
// compute m1
#pragma omp section
            {
                int *temp11 = new int[split_index * split_index];
                int *temp12 = new int[split_index * split_index];
                addMatrix(a00, a11, temp11, split_index);
                addMatrix(b00, b11, temp12, split_index);
                multiply_matrix(temp11, temp12, m1, split_index, level - 1);
                delete[] temp11;
                delete[] temp12;
            }

// compute m2
#pragma omp section
            {
                int *temp2 = new int[split_index * split_index];
                addMatrix(a10, a11, temp2, split_index);
                multiply_matrix(temp2, b00, m2, split_index, level - 1);
                delete[] temp2;
            }

// compute m3
#pragma omp section
            {
                int *temp3 = new int[split_index * split_index];
                subMatrix(b01, b11, temp3, split_index);
                multiply_matrix(a00, temp3, m3, split_index, level - 1);
                delete[] temp3;
            }

// compute m4
#pragma omp section
            {
                int *temp4 = new int[split_index * split_index];
                subMatrix(b10, b00, temp4, split_index);
                multiply_matrix(a11, temp4, m4, split_index, level - 1);
                delete[] temp4;
            }
// compute m5
#pragma omp section
            {
                int *temp5 = new int[split_index * split_index];
                addMatrix(a00, a01, temp5, split_index);
                multiply_matrix(temp5, b11, m5, split_index, level - 1);
                delete[] temp5;
            }
// compute m6
#pragma omp section
            {
                int *temp61 = new int[split_index * split_index];
                int *temp62 = new int[split_index * split_index];
                subMatrix(a10, a00, temp61, split_index);
                addMatrix(b00, b01, temp62, split_index);
                multiply_matrix(temp61, temp62, m6, split_index, level - 1);
                delete[] temp61;
                delete[] temp62;
            }
// compute m7
#pragma omp section
            {
                int *temp71 = new int[split_index * split_index];
                int *temp72 = new int[split_index * split_index];
                subMatrix(a01, a11, temp71, split_index);
                addMatrix(b10, b11, temp72, split_index);
                multiply_matrix(temp71, temp72, m7, split_index, level - 1);
                delete[] temp71;
                delete[] temp72;
            }
        }

        int *c00 = new int[split_index * split_index];
        int *c01 = new int[split_index * split_index];
        int *c10 = new int[split_index * split_index];
        int *c11 = new int[split_index * split_index];
        int *tempc1 = new int[split_index * split_index];
        int *tempc2 = new int[split_index * split_index];

        // compute left up
        addMatrix(m1, m7, tempc1, split_index);
        subMatrix(m4, m5, tempc2, split_index);
        addMatrix(tempc1, tempc2, c00, split_index);

        // compute right up
        addMatrix(m3, m5, c01, split_index);

        // compute left down
        addMatrix(m2, m4, c10, split_index);

        // compute left down
        addMatrix(m3, m6, tempc1, split_index);
        subMatrix(m1, m2, tempc2, split_index);
        addMatrix(tempc1, tempc2, c11, split_index);

        // combine result submatrix to result matrix
        combineMatrix(c00, c01, c10, c11, matC, split_index);

        delete[] a00;
        delete[] a01;
        delete[] a10;
        delete[] a11;
        delete[] b00;
        delete[] b01;
        delete[] b10;
        delete[] b11;
        delete[] c00;
        delete[] c01;
        delete[] c10;
        delete[] c11;

        delete[] m1;
        delete[] m2;
        delete[] m3;
        delete[] m4;
        delete[] m5;
        delete[] m6;
        delete[] m7;

        delete[] tempc1;
        delete[] tempc2;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cout << "Invalid input" << endl;
    }

    int k = stoi(argv[1]);
    int level = stoi(argv[2]);
    num_threads = stoi(argv[3]);
    int n = pow(2, k);
    int *matrixA;
    int *matrixB;
    int *matrixC; // Result of Strassen computation
    int *answer;  // Correct answer of standard computation
    double start_time;
    double end_time;

    // use 1d array because passing 2d array is problematic
    matrixA = new int[n * n];
    matrixB = new int[n * n];
    matrixC = new int[n * n];
    answer = new int[n * n];

    srand(time(NULL)); // provide seed to obtain new set of random number everytime
    initialize_matrix(matrixA, n, 10);
    initialize_matrix(matrixB, n, 10);

    standard_multiply(matrixA, matrixB, answer, n);

    start_time = omp_get_wtime();
    multiply_matrix(matrixA, matrixB, matrixC, n, level);
    end_time = omp_get_wtime();

    cout << "n = " << n << ", ";
    cout << "k = " << k << ", ";
    cout << "level = " << level << ", ";
    cout << "num_threads = " << num_threads << ", ";
    cout << "correctness = " << identical(matrixC, answer, n) << ", ";
    printf("time = %f\n", end_time - start_time);

    // Clean up memory
    delete[] matrixA;
    delete[] matrixB;
    delete[] matrixC;
    delete[] answer;
    return 0;
}