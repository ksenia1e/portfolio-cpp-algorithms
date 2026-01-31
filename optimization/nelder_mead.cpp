#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <locale.h>

using namespace std;

void constructionSimplex(double &x1, double &x2,  double &x3, double &y1, double &y2, double &y3)
{
    double l{1.0}; // длина ребра симплекса
    double r1 = l * (sqrt(3) + 1)/(2 * sqrt(2));
    double r2 = l * (sqrt(3) - 1)/(2 * sqrt(2));

    printf("Введите координаты начальной точки: ");
    cin >> x1 >> y1;

    x2 = x1 + r1;
    y2 = y1 + r2;
    
    x3 = x1 + r2;
    y3 = y1 + r1;
}

double Calc_f(double x, double y)
{
    return -(3 * exp(-(x - 2) * (x - 2) - (y - 3) * (y - 3) / 4) + 
    exp(-(x - 1) * (x - 1) / 4 - (y - 1) * (y - 1))); // поиск максимума (минус для максимума)
    //return 100 * pow(y - x, 2) + pow(1 - x, 2); // поиск минимума
    //return 100 * pow(y - pow(x, 2), 2) + pow(1 - x, 2); // поиск минимума
}

struct vershina
{
    double x{0.0}, y{0.0}, f{0.0};
    vershina(double x_, double y_, double f_) : x(x_), y(y_), f(f_) {};
};

bool compare(vershina &e1, vershina &e2) 
{ 
    return e1.f < e2.f; 
}

vershina reflection(vershina &mid, vershina &worst, double &alfa)
{
    double x = mid.x + alfa * (mid.x - worst.x);
    double y = mid.y + alfa * (mid.y - worst.y);

    vershina ref = {x, y, Calc_f(x, y)};

    return ref;
}

vershina expansion(vershina &mid, vershina &xr, double &gamma)
{
    double x = mid.x + gamma * (xr.x - mid.x);
    double y = mid.y + gamma * (xr.y - mid.y);

    vershina ex = {x, y, Calc_f(x, y)};

    return ex;
}

vershina contract(vershina &mid, vershina &worst, double &beta)
{
    double x = mid.x + beta * (worst.x - mid.x);
    double y = mid.y + beta * (worst.y - mid.y);

    vershina con = {x, y, Calc_f(x, y)};

    return con;
}

void shrink(vector<vershina> &k, double &delt)
{
    k[1].x = k[0].x + delt * (k[1].x - k[0].x);
    k[1].y = k[0].y + delt * (k[1].y - k[0].y);
    k[1].f = Calc_f(k[1].x, k[1].y);

    k[2].x = k[0].x + delt * (k[2].x - k[0].x);
    k[2].y = k[0].y + delt * (k[2].y - k[0].y);
    k[2].f = Calc_f(k[2].x, k[2].y);
}

void check(vector<vershina> &k, double &eps, bool &flag, vershina &mid)
{
    double res{0.0};
    for(int i = 0; i < 3; i++)
    {
        res += pow(k[i].f - mid.f, 2);
    }

    if(sqrt(res) <= eps)
    {
        flag = true;
    }
}

int main()
{
    setlocale(LC_ALL, "");
    vector<vershina> k; // треугольник
    double f1{0.0}, f2{0.0}, f3{0.0};
    double x1{0.0}, x2{0.0}, x3{0.0}, y1{0.0}, y2{0.0}, y3{0.0};
    int max_iter{1000};
    double eps{1e-7};
    bool flag{false};
    int i{0};

    constructionSimplex(x1, x2, x3, y1, y2, y3);

    vershina e1 = {x1 , y1 , Calc_f(x1, y1)};
    vershina e2 = {x2 , y2 , Calc_f(x2, y2)};
    vershina e3 = {x3 , y3 , Calc_f(x3, y3)};

    k.push_back(e1);
    k.push_back(e2);
    k.push_back(e3);

    ofstream out;
    out.open("result.txt");

    for (; i < max_iter; i++) // !реализовать выход из цикла
    {
        sort(begin(k), end(k), compare); // i = 0 - best, i = 1 - good, i = 2 - worst

        if(out.is_open())
        {
            // out << "|| " << setprecision(7) << " F(x, y) = "  << k[0].f <<  
            // " x = " << k[0].x << " y = " << k[0].y << " || " << " F(x, y) = "  << k[1].f <<  
            // " x = " << k[1].x << " y = " << k[1].y << " || " << " F(x, y) = "  << k[2].f <<  
            // " x = " << k[2].x << " y = " << k[2].y << " || " <<  endl;
            out << setprecision(7) <<  k[0].x << " " << k[0].y << " " << k[0].f << endl;
        }
// поиск центра тяжести
        double x = (k[0].x + k[1].x) / 2.0;
        double y = (k[0].y + k[1].y) / 2.0;

        vershina mid = {x , y, Calc_f(x, y)};

// проверка на выход
        check(k, eps, flag, mid);
        if (flag == true)
            break;

// операция отражения
        double alfa{1.0};
        vershina xr = reflection(mid, k[2], alfa);

        if (xr.f <= k[0].f)
        {
    // операция растяжения
            alfa = 2.0;
            vershina xe = expansion(mid, xr, alfa);
            
            if (xe.f < k[0].f)
            {
                k.pop_back();
                k.push_back(xe);
                continue;
            }
            k.pop_back();
            k.push_back(xr);
            continue;
        }
        else if (xr.f <= k[1].f)
        {
            k.pop_back();
            k.push_back(xr);
            continue;
        }
        else if (xr.f <= k[2].f)
        {
    // операция сжатия
            alfa = 0.5;
            vershina xc = contract(mid, k[2], alfa);

            k.pop_back();
            k.push_back(xc);
            continue;
        }

// операция сокращения(редукция)
        alfa = 0.5;
        shrink(k, alfa);
    }

    out.close();
    cout << i + 1;
    return 0;
}