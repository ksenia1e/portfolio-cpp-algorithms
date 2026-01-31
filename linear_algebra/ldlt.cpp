#include <iostream>
#include <stdio.h>
#include <cmath>

//typedef float real;
//typedef float realscal;
//#define STR_R "%f"
//#define STR_W "%f\n"
typedef double real;
typedef double realscal;
#define STR_R "%lf"
#define STR_W "%.14lf\n"

int max(double x, double y)
{
   if ( x < y ) return y;
   return x;
}

int sign(double x)
{
   if ( x < 0 ) return -1;
   if ( x > 0 ) return 1;
   return 0;
}

void Gilbert()
{
   FILE *myfile;
   int n = 0;


   scanf_s("%d", &n);

   fopen_s(&myfile, "n.txt", "w");
   fprintf_s(myfile, "%d", n);
   fclose(myfile);

   double *di = new double[n];
   int *ia = new int[n + 1];

   fopen_s(&myfile, "ia.txt", "w");
   ia[0] = 0;
   ia[1] = 0;
   for ( int i = 2; i < n + 1; i++ )
      ia[i] = ia[i - 1] + (i - 1);
   for ( int i = 0; i < n + 1; i++ )
      fprintf_s(myfile, "%d ", ia[i]);
   fclose(myfile);

   int m = ia[n];
   double *al = new double[m];

   fopen_s(&myfile, "di.txt", "w");
   for ( int i = 0, ne = 1; i < n; i++, ne++ )
   {
      di[i] = 1.0 / (2 * ne - 1);
   }
   for ( int i = 0; i < n; i++ )
      fprintf_s(myfile, "%.14lf ", di[i]);
   fclose(myfile);

   for ( ; n > 1; )
   {
      fopen_s(&myfile, "al.txt", "w");
      int il = 0;
      for ( int i = 1; i < n; i++ )
      {
         int p = ia[i + 1] - ia[i];
         for ( int k = 1; k <= p; k++ )
         {
            al[il] = 1.0 / (p + k);
            il += 1;
         }
      }
      for ( int i = 0; i < m; i++ )
         fprintf_s(myfile, "%.14lf ", al[i]);
      fclose(myfile);
      break;
   }
}

void ReadMatrix_Vector(int &n, int &m, real al[], real di[], real b[])
{
   FILE *myfile;

   fopen_s(&myfile, "al.txt", "r");
   for ( int i = 0; i < m; i++ )
      fscanf_s(myfile, STR_R, &al[i]);
   fclose(myfile);

   fopen_s(&myfile, "di.txt", "r");
   for ( int i = 0; i < n; i++ )
      fscanf_s(myfile, STR_R, &di[i]);
   fclose(myfile);

   fopen_s(&myfile, "b.txt", "r");
   for ( int i = 0; i < n; i++ )
      fscanf_s(myfile, STR_R, &b[i]);
   fclose(myfile);
}



void CalcLDLt(int &n, real al[], real di[], int ia[], int di_signed[])
{
   for ( int i = 0; i < n; i++ )
   {
      int i0 = ia[i];
      int i1 = ia[i + 1];
      int j = i - (i1 - i0); // Номер столбца перового элемента профиля i-ой строки
      int i_col = j;
      realscal sum_d = 0;
      for ( int k = i0; k < i1; j++, k++ )
      {
         int j0 = ia[j];
         int j1 = ia[j + 1];

         int count_i = k - i0; // Кол-во пройденных элементов в профиле i-ой строки
         int count_j = j1 - j0; // Длина профиля j-ой строки
         int count_ij = count_i - count_j;

         if ( count_ij > 0 )
         {
            count_i = count_ij;
            count_j = 0;
         }
         else
         {
            count_j = -count_ij;
            count_i = 0;
         }
         realscal sum_l = 0;
         for ( int ki = i0 + count_i, kj = j0 + count_j, kd = i_col + count_i; ki < k; ki++, kj++, kd++ )
         {
            sum_l += di_signed[kd] * al[ki] * al[kj]; // dk * lik * ljk
         }
         al[k] = (di_signed[j] / di[j]) * (al[k] - sum_l); // lij, i!=j
         sum_d += al[k] * al[k] * di_signed[j];
      }

      di_signed[i] = sign(di[i] - sum_d);  // знаковая диагональ dii
      di[i] = sqrt(fabs(di[i] - sum_d)); //диагональ lii
   }
}

// Ly = b
void CalcY(int &n, real al[], real di[], int ia[], real b[], real y[])
{
   for ( int i = 0; i < n; i++ )
   {
      int p = ia[i + 1] - ia[i];
      realscal sum = 0;
      for ( int j0 = i - p, i1 = ia[i]; j0 < i; j0++, i1++ )
      {
         sum += al[i1] * b[j0];
      }
      y[i] = (b[i] - sum) / di[i];
   }
}

// Dz = y
void CalcZ(int &n, int di_signed[], real y[], real z[])
{
   for ( int i = 0; i < n; i++ )
   {
      z[i] = y[i] * di_signed[i];
   }
}

// Ltx = z
void CalcX(int &n, real al[], real di[], int ia[], real z[], real x[])
{
   for ( int j = n - 1; j >= 0; j-- )
   {
      realscal z1 = 0;
      realscal sum = 0;
      for ( int i0 = j + 1; i0 < n; i0++ )
      {
         int p = ia[i0 + 1] - ia[i0];
         int j0 = i0 - p;

         if ( j0 > j )
            continue;

         int count_i0 = j - j0;
         sum += al[ia[i0] + count_i0] * x[i0];
      }
      z1 = z[j] - sum;
      x[j] = z1 / di[j];
   }
}

void Out(int &n, real al[], real di[], int ia[], int di_signed[], real x[])
{
   printf_s("\nal[i]:\n");
   for ( int i = 0; i < ia[n]; i++ )
      printf_s(STR_W, al[i]);
   printf_s("\ndi[i]:\n");
   for ( int i = 0; i < n; i++ )
      printf_s(STR_W, di[i]);
   printf_s("\nia[i]:\n");
   for ( int i = 0; i < n + 1; i++ )
      printf_s("%d\n", ia[i]);
   printf_s("\ndi_signed[i]:\n");
   for ( int i = 0; i < n; i++ )
      printf_s("%d\n", di_signed[i]);
   printf_s("\nx[i]:\n");
   for ( int i = 0; i < n; i++ )
      printf_s(STR_W, x[i]);
}



int main()
{
   int n = 0, m = 0;

   Gilbert();
   FILE *myfile;
   fopen_s(&myfile, "n.txt", "r");
   fscanf_s(myfile, "%d", &n);
   fclose(myfile);

   int *ia = new int[n + 1];

   fopen_s(&myfile, "ia.txt", "r");
   for ( int i = 0; i < n + 1; i++ )
      fscanf_s(myfile, "%d", &ia[i]);
   fclose(myfile);

   m = ia[n];
   real *di = new real[n];
   int *di_signed = new int[n];
   real *al = new real[m];



   real *v = new real[n];
   real *b = v;
   real *x = v;
   real *y = v;
   real *z = v;

   ReadMatrix_Vector(n, m, al, di, b);

   CalcLDLt(n, al, di, ia, di_signed);

   CalcY(n, al, di, ia, b, y);
   CalcZ(n, di_signed, y, z);
   CalcX(n, al, di, ia, z, x);

   Out(n, al, di, ia, di_signed, x);
}