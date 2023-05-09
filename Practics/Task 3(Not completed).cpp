//Схема наследования
/*   A1
B1  B2  B3
  C1  C2
	D1*/
#include <iostream>
using namespace std;

class A1
{
protected:
	int a1;

public:
	A1()
	{
		a1 = 0;
	}

	A1(int a)
	{
		a1 = a;
	}

	virtual void print()
	{
		cout << "A1" << endl;
	}

	virtual void show()
	{
		cout << "\ta = " << a1 << endl;
	}
};

class B1 : virtual public A1
{
protected:
	int b1;

public:
	B1() : A1(0)
	{
		b1 = 0;
	}

	B1(int a, int b) : A1(a)
	{
		b1 = b;
	}

	virtual void print()
	{
		cout << "B1" << endl;
	}

	virtual void show()
	{
		cout << "\ta = " << a1 << endl << 
			"\tb = " << b1 << endl;
	}
};

class B2 : virtual public A1
{
protected:
	int b1;

public:
	B2() : A1(0)
	{
		b1 = 0;
	}

	B2(int a, int b) : A1(a)
	{
		b1 = b;
	}

	virtual void print() 
	{
		cout << "B2" << endl;
	}

	virtual void show() 
	{
		cout << "\ta = " << a1 << endl <<
			"\tb = " << b1 << endl;
	}
};

class B3 : virtual public A1
{
protected:
	int b1;

public:
	B3() : A1(0)
	{
		b1 = 0;
	}

	B3(int a, int b) : A1(a)
	{
		b1 = b;
	}

	virtual void print() 
	{
		cout << "B3" << endl;
	}

	virtual void show() 
	{
		cout << "\ta = " << a1 << endl <<
			"\tb = " << b1 << endl;
	}
};

class C1 : virtual public B1, virtual public B2, virtual public B3
{
protected:
	int c1;

public:
	C1(int a, int b, int c, int d, int e) : B1(10, b), B2(11, c), B3(12, d)
	{
		c1 = e;
		a1 = a;
	}

	virtual void print() 
	{
		cout << "C1" << endl;
	}

	virtual void show() 
	{
		cout << "\ta = " << a1 << endl <<
			"\tb (B1) = " << B1::b1 << endl <<
			"\tb (B2) = " << B2::b1 << endl <<
			"\tb (B3) = " << B3::b1 << endl <<
			"\tc = " << c1 << endl;
	}
};

class C2 : virtual public B1, virtual public B2, virtual public B3
{
protected:
	int c1;

public:
	C2(int a, int b, int c, int d, int e) : B1(13, b), B2(14, c), B3(15, d)
	{
		c1 = e;
		a1 = a;
	}

	virtual void print() 
	{
		cout << "C2" << endl;
	}

	virtual void show() 
	{
		cout << "\ta = " << a1 << endl <<
			"\tb (B1) = " << B1::b1 << endl <<
			"\tb (B2) = " << B2::b1 << endl <<
			"\tb (B3) = " << B3::b1 << endl <<
			"\tc = " << c1 << endl;
	}
};

class D1 : virtual public C1, virtual public C2
{
protected:
	int d1;

public:
	D1(int a, int b, int c, int d, int e, int f, int g, int h, int q) : C1(16, 17, 18, 19, 20), C2(21, 22, 23, 24, 25)
	{
		d1 = q;
		a1 = a;
		b1 = b, c, d, f, g;
		c1 = e, h;

	}

	virtual void print() 
	{
		cout << "D1" << ::endl;
	}

	virtual void show() 
	{
		cout << "\ta = " << a1 << endl <<
			"\tb (B1) = " << B1::b1 << endl <<
			"\tb (B2) = " << B2::b1 << endl <<
			"\tb (B3) = " << B3::b1 << endl <<
			"\tc (C1) = " << C1::c1 << endl <<
			"\tc (C2) = " << C2::c1 << endl <<
			"\td = " << d1 << endl;
	}
};

int main()
{
	A1 a1(1);
	a1.print();
	a1.show();
	B3 b3(2, 3);
	b3.print();
	b3.show();
	C2 c2(4, 5, 6, 7, 8);
	c2.print();
	c2.show();
	D1 d1(11, 12, 13, 14, 15, 16, 17, 18, 19);
	d1.print();
	d1.show();
	A1* pa1 = &d1;
	pa1->print();
	pa1->show();
	return 0;
}
