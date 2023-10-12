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

	virtual void print() override
	{
		cout << "B1" << endl;
	}

	virtual void show() override
	{
		cout << "\ta = " << a1 << endl << 
			"\tb = " << b1 << endl;
	}
};

class B2 : virtual public A1
{
protected:
	int b2;

public:
	B2() : A1(0)
	{
		b2 = 0;
	}

	B2(int a, int b) : A1(a)
	{
		b2 = b;
	}

	virtual void print() override
	{
		cout << "B2" << endl;
	}

	virtual void show() override
	{
		cout << "\ta = " << a1 << endl <<
			"\tb = " << b2 << endl;
	}
};

class B3 : virtual public A1
{
protected:
	int b3;

public:
	B3() : A1(0)
	{
		b3 = 0;
	}

	B3(int a, int b) : A1(a)
	{
		b3 = b;
	}

	virtual void print() override
	{
		cout << "B3" << endl;
	}

	virtual void show() override
	{
		cout << "\ta = " << a1 << endl <<
			"\tb = " << b3 << endl;
	}
};

class C1 : virtual public B1, virtual public B2, virtual public B3
{
protected:
	int c1;

public:
	C1(int a, int b, int c, int d, int e) : B1(a, b), B2(a, c), B3(a, d)
	{
		c1 = e;
		a1 = a;
	}

	virtual void print() override
	{
		cout << "C1" << endl;
	}

	virtual void show() override
	{
		cout << "\ta = " << a1 << endl <<
			"\tb (B1) = " << B1::b1 << endl <<
			"\tb (B2) = " << B2::b2 << endl <<
			"\tb (B3) = " << B3::b3 << endl <<
			"\tc = " << c1 << endl;
	}
};

class C2 : virtual public B1, virtual public B2, virtual public B3
{
protected:
	int c2;

public:
	C2(int a, int b, int c, int d, int e) : B1(a, b), B2(a, c), B3(a, d)
	{
		c2 = e;
		a1 = a;
	}

	virtual void print() override
	{
		cout << "C2" << endl;
	}

	virtual void show() override
	{
		cout << "\ta = " << a1 << endl <<
			"\tb (B1) = " << B1::b1 << endl <<
			"\tb (B2) = " << B2::b2 << endl <<
			"\tb (B3) = " << B3::b3 << endl <<
			"\tc = " << c2 << endl;
	}
};

class D1 : virtual public C1, virtual public C2
{
protected:
	int d1;

public:
	D1(int a, int b, int c, int d, int e, int f, int q) : C1(a, b, c, d, e), C2(a, b, c, d, f), B1(a, b), B2(a, c), B3(a, d)
	{
		d1 = q;
	}

	void print() override
	{
		cout << "D1" << ::endl;
	}

	void show() override
	{
		cout << "\ta = " << a1 << endl <<
			"\tb (B1) = " << b1 << endl <<
			"\tb (B2) = " << b2 << endl <<
			"\tb (B3) = " << b3 << endl <<
			"\tc (C1) = " << c1 << endl <<
			"\tc (C2) = " << c2 << endl <<
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

	D1 d1(9, 10, 11, 12, 13, 14, 15);

	d1.print();
	d1.show();

	A1* pa1 = &d1;

	pa1->print();
	pa1->show();

	return 0;
}
