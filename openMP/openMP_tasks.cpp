#pragma once
#include <iostream> 
#include <omp.h> 
#include <iostream> 
#include <fstream> 
#include <time.h>
#include <locale.h>

using namespace std;

const int n = 2500;
const int alpha = 2;
const int threads = 16;

void scalar(double x[], double y[]) // Скалярное произведение
{
   double  startTime = omp_get_wtime();
   double sum = 0, resultTime = 0;
#pragma omp parallel num_threads(threads) reduction(+:sum)
   #pragma omp for
      for ( int i = 0; i < n; i++ ) sum += x[i] * y[i];

   resultTime = (omp_get_wtime() - startTime);
   printf_s("(x,y) = %lf\nВремя выполнения вычисления скалярного произведения: %lf с\n", sum, resultTime);
}

void enter_vector()
{
   double *x = NULL;
   double *y = NULL;
   if ( x == NULL ) x = new double[n];
   if ( y == NULL ) y = new double[n];
   for ( int i = 0; i < n; i++ )
   {
      x[i] = y[i] = i;
   }
   scalar(x, y);
   if ( x ) { delete[] x; x = NULL; }
   if ( y ) { delete[] y; y = NULL; }
}

void multiplication_matrix(double x[], double y[]) // Произведение матриц
{
   double *c = NULL, resultTime = 0;
   if ( c == NULL ) c = new double[ n * n ] {0};
   clock_t startTime = clock();
   for ( int i = 0; i < n; i++ )
   {
      for ( int j = 0; j < n; j++ )
      {
         double sum = 0;
         for ( int j0 = 0; j0 < n; j0++ )
         {
            sum += x[i*n + j0] * y[n*j0 + j];
         }
         c[(i * n) + j] = sum;
      }
   }
   resultTime = (clock() - startTime);

   printf_s("Время выполнения умножения матриц: %lf с\n", resultTime);
   double norma = 0.0;
   for ( int i = 0; i < (n * n); i++ )
   {
      norma += c[i] * c[i];
   }
   printf_s("Норма матрицы умножения: %lf\n", sqrt(norma));
   if ( c ) { delete[] c; c = NULL; }
}

void enter_matrix()
{
   double *x = NULL;
   double *y = NULL;
   if ( x == NULL ) x = new double[n*n];
   if ( y == NULL ) y = new double[n*n];
   for ( int j = 0; j < n*n; j++ )
   {
      x[j] = j;//по строкам
      y[j] = j;//по строкам
   }
   multiplication_matrix(x, y);
   if ( x ) { delete[] x; x = NULL; }
   if ( y ) { delete[] y; y = NULL; }
}

void CalcY(double U[], double x[], double b[])
{
   FILE *file = NULL;

   double *y = NULL;
   if ( y == NULL ) y = new double[ n ];

   double resultTime = 0;
   clock_t startTime = clock();

   for ( int i = n - 1; i >= 0; i-- ) // Обратный ход
   {
      double sum = 0;
      for ( int k = i + 1; k < n; k++ )
      {
         sum += y[k] * U[i * n + k];
      }
      y[i] = (b[i] - sum) / U[i * n + i];
   }
   resultTime = (clock() - startTime);
   printf_s("Время выполнения поиска y: %lf с\n", resultTime);

   fopen_s(&file, "XandY.txt", "w");
   fprintf_s(file, "x[i]\ty[i]\n");
   for ( int i = 0; i < n; i++ )
      fprintf_s(file, "%lf\t%lf\n", x[i], y[i]);
   fclose(file);

   double norma_x = 0.0, norma_y = 0.0;
   for ( int i = 0; i < n; i++ )
   {
      norma_x += x[i] * x[i];
      norma_y += y[i] * y[i];
   }
   printf_s("Норма x: %lf\nнорма y: %lf\nразница норм векторов x и y: %lf\n", sqrt(norma_x), sqrt(norma_y), fabs(sqrt(norma_x) - sqrt(norma_y)));
   if ( y ) { delete[] y; y = NULL; }
}

void Calcb(double x[], double u[])
{
   double *b = NULL;
   if ( b == NULL ) b = new double[ n ] {0};
   for ( int k = 0; k < n; k++ )
   {
      for ( int i = k; i < n; i++ )
         b[k] += u[i + k * n] * x[i];
   }
   CalcY(u,x,b);
   if ( b ) { delete[] b; b = NULL; }
}

void enter_matrix_U_and_vector_x()
{
   double *x = NULL;
   double *u = NULL;
   if ( x == NULL ) x = new double[ n ];
   if ( u == NULL ) u = new double[ n * n ] { 0 };
   for ( int i = 0; i < n; i++ )
   {
      x[i] = i;//по строкам
   }
   for ( int i = 0, c = n; i < n * n; i += (n + 1), c-- )
   {
      u[i] = 10;
      for ( int j = 1; j < c; j++)
      {
         u[j + i] = 1;
      }
   }
   Calcb(x, u);
   if ( x ) { delete[] x; x = NULL; }
   if ( u ) { delete[] u; u = NULL; }
}

void combine_vectors(double x[], double y[])
{
   double norma = 0.0;
   for ( int i = 0; i < n; i++ )
   {
      y[i] += y[i] + alpha * x[i];
      norma += y[i] * y[i];
   }
   printf_s("Норма нового вектора y: %lf", sqrt(norma));
}
void enter_vector_dop()
{
   double *x = NULL;
   double *y = NULL;
   if ( x == NULL ) x = new double[n];
   if ( y == NULL ) y = new double[ n ] { 0 };
   for ( int i = 0; i < n; i++ )
   {
      x[i] = y[i] = i;
   }
   combine_vectors(x, y);
   if ( x ) { delete[] x; x = NULL; }
   if ( y ) { delete[] y; y = NULL; }
}


int main()
{
   setlocale(0, "Russian");

   printf_s("----- ЗАДАНИЕ 1 -----\n");
   enter_vector(); // Задание 1

   printf_s("----- ЗАДАНИЕ 2 -----\n");
   enter_matrix(); // Задание 2

   printf_s("----- ЗАДАНИЕ 3 -----\n");
   enter_matrix_U_and_vector_x(); // Задание 3

   printf_s("----- ДОП. ЗАДАНИЕ -----\n");
   enter_vector_dop(); // Доп.задание
}