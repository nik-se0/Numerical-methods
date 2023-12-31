#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <windows.h>
#include <iomanip>
#include <locale>
#include <cmath>

using namespace std;

float f(float x, float y) { return x * x*x + y * y * y + 2; }
float del_f(float x, float y) { return -(-6*x - 6 * y); }

int main()
{
	setlocale(LC_ALL, "Russian");

	while (true)
	{
		system("cls");
		float a = 0, b = 1, c = 0, d = 0.2;	//границы сетки
		long int n = 4, m = 4, Nmax = 1000;	//кол-во шагов
		float h, k, e = 1e-6; //шаг

		cout<<"\n\tU(x,y) = x^3+y^3+2\n\tf(x,y) = -(-6y-2)\n\tГраничные условия: x-[0; 1] y-[0; 0.2]\n\tСетка: 4x4\n\tМаксимальное количество шагов: 1000\n\tТочноcть решения: "<<e<<"\n\n\t1.Решить задачу с параметрами по умолчанию\n\t2.Ввести параметры\n\tВвод: ";

		int q = 0;
		cin >> q;
		if (q == 2)
		{
			cout << "\n\tВведите граничные значения XY: ";
			cin >> a >> b >> c >> d;
			cout << "\tВведите размер сетки NxM, разделенный пробелами: ";
			cin >> n >> m;        //n- по вертикале, m-по горизонтале
			cout << "\tВведите максимальное количество шагов: ";
			cin >> Nmax;
			cout << "\tВведите точность решения E: ";
			cin >> e;	//e=0.0000001
		}

		float** B = new float* [n + 1];
		for (int i = 0; i < n + 1; i++) {
			B[i] = new float[m + 1];
			for (int j = 0; j < m + 1; j++)
				B[i][j] = 0;
		}
		
		//Вычисления
		h = (b - a) / m;
		k = (d - c) / n;

		//граничные значения
		for (int i = 0; i < m + 1; i++)
		{
			B[0][i] = f(a + h * i, c);
			B[n][i] = f(a + h * i, d);

		}
		for (int i = 0; i < n + 1; i++)
		{
			B[i][0] = f(a, c + k * i);
			B[i][m] = f(b, c + k * i);
		}
		
		//значение функции
		for (int i = 1; i < n; i++)
		{
			for (int j = 1; j < m; j++)
			{
				B[j][i] = del_f(a + h * i, c + k * j);
			}
		}

		//for(int i=0;i<n+1;i++)
		{
			//for (int j = 0; j < m + 1; j++)
			{
				//cout << B[i][j]<< " ";
			}
			//cout << endl;
		}

		//Матрица для метода
		long int n2 = (n - 1) * (m - 1);
		float** A = new float* [n2];
		float* F1 = new float[n2];
		for (int i = 0; i < n2; i++)
		{
			A[i] = new float[n2];
			F1[i] = 0;
			for (int j = 0; j < n2; j++)
			{
				A[i][j] = 0;
			}
		}
		
		for (int i = 0; i < n2; i++)
		{
			int J1 = i % (n - 1) + 1; int I1 = i / (m - 1) + 1;
			F1[i] = B[I1][J1];
			A[i][i] = -2 * (1 / (h * h) + 1 / (k * k));
			if (I1 > 1)
				A[i][i - n + 1] = 1 / (k * k);
			else
			{
				F1[i] -= (1 / (k * k) * B[I1 - 1][J1]);
			}
			if (I1 < n - 1)
				A[i][i + n - 1] = 1 / (k * k);
			else
			{
				F1[i] -= (1 / (k * k) * B[I1 + 1][J1]);
			}
			if (J1 > 1)
				A[i][i - 1] = 1 / (h * h);
			else
			{
				F1[i] -= (1 / (h * h) * B[I1][J1 - 1]);
			}
			if (J1 < n - 1)
				A[i][i + 1] = 1 / (h * h);
			else
			{
				F1[i] -= (1 / (h * h) * B[I1][J1 + 1]);
			}
		}
		

		for(int i=0;i<n2;i++)
		{
			for (int j = 0; j < n2; j++)
			{
				//cout << A[i][j] << " ";
			}
			cout << F1[i] << endl;
		}


		//Метод
		float* V = new float[n2]; int s = 0;
		for (int i = 0; i < n2; i++)
		{
			V[i] = 0;
		}
		
		float v_old, v_new, E, e_curr, e_mm2;
		bool flag = false;
		while (!flag)
		{
			E = 0; e_mm2 = 0;
			for (int i = 0; i < n2; i++)
			{
				v_old = V[i];
				v_new = F1[i];
				for (int j = 0; j < n2; j++)
					if (j != i) { v_new = v_new - A[i][j] * V[j]; }
				v_new = v_new / A[i][i];
				e_curr = fabs(v_old - v_new);
				E=max(E,e_curr);
				V[i] = v_new;
			}

			if (E <= e) { flag = true; }
			if (s >= Nmax-1) { flag = true; }
			s++;
		}
		
	    //Вычисление невязки
		float* res = new float[n2];
		float r = 0;
		for (int i = 0; i < n2; i++)
		{
			res[i] = 0;
			for (int j = 0; j < n2; j++)
			{
				res[i] += V[j] * A[i][j];
			}
			r = max(r, fabs(res[i] - F1[i]));
		}

		double m_e=0;
		//Вычисление погрешности
		for (int i = 0; i < n2; i++)
		{
			int J1 = i / (n - 1) + 1; int I1 = i % (m - 1) + 1;
			m_e = max(m_e, fabs(V[i] - f(a + h * I1, c + k * J1)));
			//cout << V[i] << " " << f(a + h * I1, c + k * J1) << "   " << fabs(V[i] - f(a + h * I1, c + k * J1)) <<" "<< a + h * I1<<" "<< c + k * J1<< "\n";
		}
		
		//Вывод справка
		cout << "\n\tПри решении СЛАУ AV = F итерационным методом Зайделя начальное приближение равно 0, были установлены следующие критерии остановки: \n\tМаксимальное количество шагов: " << Nmax << "\n\tТочность решения: " << e << "\n\n\tВыполненно шагов N = " << s << "\n\tПри N точность решения e =||V(N)-V(N-1)||= " << E << "\n\tНевязка, т.е. R(N) = A * V(N) - F, имеет норму ||R(N)|| = " << r;
		cout << "\n\tПогрешность решения ||z(N)||=||U-V(N)||= " << m_e; 
		double lam = 4/(h*h)*sin(M_PI/(2*n))*sin(M_PI/(2 * n)) + 4/(k*k)*sin(M_PI/(2*m))*sin(M_PI/(2*m));
		cout << " < " << sqrt((n-1)*(m-1)) * r / lam;
		cout<<"\n\n\n\n\n\n\n\tВведите любой символ, чтобы продолжить работу : ";

		char aq;
		cin >> aq;
		if (aq == '0')
		{
			std::ofstream f;
			f.open("C:\\tmp\\File.txt");
			f << "\n\tU(x,y) = x^3+y^3+2\n\tf(x,y) = -(-6y-2)\n\t";
			f << "\n\tПри решении СЛАУ AV = F итерационным методом Зайделя начальное приближение равно 0, были установлены следующие критерии остановки : \n\tМаксимальное количество шагов :" << Nmax << "\n\tТочность решения : " << e << "\n\n\tВыполненно шагов N = " << s << "\n\tТочность решения при N e =||V(N)-V(N-1)||= " << E << "\n\tНевязка, т.е. R(N)=A*V(N)-F, имеет норму ||R(N)|| = " << r;
			f << "\n\tПогрешность решения ||z(N)||=||U-V(N)||= " << m_e << " < " << sqrt(3) * r / lam;
			f << "\n\ty\\x";
			for (int j = 0; j < m + 1; j++)
				f << "\t\t" << (float)(a + h * j);

			f << endl << "\t" << c;
			for (int j = 0; j < m + 1; j++)
				f << "\t\t" << (float)B[0][j];
			f << endl;
			long int y = 1;
			for (int i = 0; i < n2; i++)
			{
				if (i % (m - 1) == 0) { f << "\t" << c + k * y << "\t\t" << (float)B[y][0]; y++; }
				f << "\t\t" << (float)V[i];
				if (i % (m - 1) == m - 2) f << "\t\t" << (float)B[y - 1][m] << "\n";
			}
			f << "\t" << d;
			for (int j = 0; j < m + 1; j++)
				f << "\t\t" << (float)B[n][j];
			f.close();
			system("C:\\tmp\\File.txt");
		}

}

}