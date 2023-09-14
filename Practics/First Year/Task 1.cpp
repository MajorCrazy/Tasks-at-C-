#include <iostream>
using namespace std;

class Circle
{
private:
	double X, Y, Radius;

public:
	Circle()
	{

	}

	Circle(double x, double y, double r)
	{
		X = x;
		Y = y;
		Radius = r;
	}

	void SetX(double x)
	{
		X = x;
	}

	void SetY(double y)
	{
		Y = y;
	}

	void SetRadius(double r)
	{
		Radius = r;
	}

	bool operator < (const Circle& other)
	{
		if (this->Radius < other.Radius)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool operator+()
	{
		if ((Radius < abs(Y)) && (Radius < abs(X)))
			return true;
		else
			return false;

	}

	friend bool operator+(double x, Circle circlee); //Смещение центра координат по оси Ох и проверка на принадлежность одной или нескольким новым четвертям

	int operator++()
	{
		if ((Radius < abs(Y)) && (Radius < abs(X)))
		{
			if ((X > 0) && (Y > 0))
				return 1;
			if ((X < 0) && (Y > 0))
				return 2;
			if ((X < 0) && (Y < 0))
				return 3;
			if ((X > 0) && (Y < 0))
				return 4;
		}
		else
			return 0;

	}

	void print()
	{
		cout << "X=" << this->X << " Y=" << this->Y << " Radius=" << this->Radius << endl;
	}

};

bool operator+(double x, Circle circlee)
{
	if (circlee.X > x)
	{
		if ((circlee.Radius < abs(abs(circlee.X) - x)) && (circlee.Radius < abs(circlee.Y)))
			return true;
		else
			return false;
	}

	else if (x == 0)
	{
		cout << "Зачем Вам смещение на 0?" << endl;
		return false;
	}

	else
	{
		if ((circlee.Radius < abs(abs(circlee.X) - abs(x))) && (circlee.Radius < abs(circlee.Y)))
			return true;
		else
			return false;
	}


}






int main()
{
	setlocale(LC_ALL, "ru");
	Circle Circle2(2, 3, 1);
	Circle Circle1;
	Circle1.SetX(3);
	Circle1.SetY(5);
	Circle1.SetRadius(8);
	double move_x = 1.5;
	Circle1.print();
	Circle2.print();

	cout << (Circle1 < Circle2) << endl; //Сравнение радиусов
	cout << (Circle2 < Circle1) << endl; 

	cout << +Circle1 << "(Первая окружность)" << endl; // Принадлежность одной четверти или нескольким
	cout << +Circle2 << "(Вторая окружность)" << endl;
	cout << move_x + Circle1 << endl; // Принадлежность одной или нескольким новым четвертям с учётом сдвига центра координатной плоскости по X
	cout << move_x + Circle2 << endl; 
	

	cout << "Окружность принадлежит " << ++Circle1 << "-ой четверти" << endl;	// Принадлежность какой-то конкретной четверти
	cout << "Окружность принадлежит " << ++Circle2 << "-ой четверти" << endl;

	return 0;
}
