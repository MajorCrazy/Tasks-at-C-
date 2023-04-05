#include <fstream>
#include <iostream>

using namespace std;

class BaseString
{
protected:
	char* p;
	int len;
	int capacity;
public:
	BaseString(char* ptr)
	{
		cout << "\nBase Constructor 1\n";
		int i = 0;
		for (; ptr[i] != '\0'; i++);
		len = i;

		capacity = (len > 256) ? len + 1 : 256;
		p = new char[capacity];

		for (i = 0; ptr[i] != '\0'; p[i] = ptr[i], i++);
		p[len] = '\0';
	}

	BaseString(const char* ptr)
	{
		cout << "\nBase Constructor 1\n";
		int i = 0;
		for (; ptr[i] != '\0'; i++);
		len = i;

		capacity = (len > 256) ? len + 1 : 256;
		p = new char[capacity];

		for (i = 0; ptr[i] != '\0'; i++)
			p[i] = ptr[i];
		p[len] = '\0';
	}

	BaseString(int Capacity = 256)
	{
		cout << "\nBase Constructor 0\n";
		capacity = Capacity;
		p = new char[capacity];
		len = 0;
	}

	~BaseString()
	{
		cout << "\nBase Destructor\n";
		if (p != NULL)
			delete[] p;
		len = 0;
		capacity = 0;
		p = NULL;
	}

	int Length() { return len; }
	int Capacity() { return capacity; }
	//char* get() {return p;}
	char& operator[](int i) { return p[i]; }


	BaseString& operator=(BaseString& s)
	{
		cout << "\nBase Operator = \n";

		return *this;
	}

	BaseString(BaseString& s)
	{
		cout << "\nBase Copy Constructor\n";
		capacity = s.capacity;
		len = s.len;
		p = new char[capacity];
		for (int i = 0; i <= len; p[i] = s.p[i], i++);
	}

	virtual void print()
	{
		int i = 0;
		while (p[i] != '\0')
		{
			cout << p[i];
			i++;
		}
	}

	virtual int IndexOf(char c, int start = 0)
	{
		if (start > len || len == 0) return -1;
		/*for (int i = start; p[i] != '\0'; i++)
			if (p[i] == c)
				return i;*/
		for (char* p1 = p; *p1; *p1++)
			if (*p1 == c)
				return p1 - p;
		return -1;
	}
};

class String : public BaseString
{
public:
	String(const char* str) : BaseString(str) {}
	~String() {}

	virtual int IndexOf(char c, int start = 0)
	{
		if (start == 0) start = len - 1;
		if (len == 0 || start >= len) return -1;

		char* p1 = &p[len - 1];
		for (; p1 >= p; p1--)
			if (*p1 == c)
				break;
		return p1 - p;
		//if (*p1 == c)
			//return p1-p;
	//return -1;
	}

	bool IsPalindrome()
	{
		char* p1 = p;
		char* p2 = &p[len - 1];
		while (p1 < p2)
		{
			if (*p1 != *p2) return false;
			*p1++;
			*p2--;
		}
		return true;
	}
};

#include <iostream>

using namespace std;

class Exception : public exception
{
protected:
	//сообщение об ошибке
	char* str;
public:
	Exception(const char* s)
	{
		str = new char[strlen(s) + 1];
		strcpy_s(str, strlen(s) + 1, s);
	}
	Exception(const Exception& e)
	{
		str = new char[strlen(e.str) + 1];
		strcpy_s(str, strlen(e.str) + 1, e.str);
	}
	~Exception()
	{
		delete[] str;
	}

	//функцию вывода можно будет переопределить в производных классах, когда будет ясна конкретика
	virtual void print()
	{
		cout << "Exception: " << str << "; " << what();
	}
};

class InvalidSizeException : public Exception
{
protected:
	int w, h;
public:
	InvalidSizeException(const char* s, int Width, int Height) : Exception(s) { w = Width; h = Height; }
	InvalidSizeException(const InvalidSizeException& e) : Exception(e) { w = e.w; h = e.h; }

	//функцию вывода можно будет переопределить в производных классах, когда будет ясна конкретика
	virtual void print()
	{
		cout << "InvalidSizeException: " << str << "; columns: " << w << "\t" << "rows: " << h << "; " << what();
	}
};

class WrongSizeException : public Exception
{
protected:
	int w, h;
public:
	WrongSizeException(const char* s, int Width, int Height) : Exception(s) { w = Width; h = Height; }
	WrongSizeException(const WrongSizeException& e) : Exception(e) { w = e.w; h = e.h; }

	//функцию вывода можно будет переопределить в производных классах, когда будет ясна конкретика
	virtual void print()
	{
		cout << "WrongSizeException: " << str << "; columns: " << w << "\t" << "rows: " << h << "; " << what();
	}
};

class NonPositiveSizeException : public WrongSizeException
{
public:
	NonPositiveSizeException(const char* s, int Width, int Height) : WrongSizeException(s, Width, Height) { }
	NonPositiveSizeException(const NonPositiveSizeException& e) : WrongSizeException(e) {}
};

class TooLargeSizeException : public WrongSizeException
{
public:
	TooLargeSizeException(const char* s, int Width, int Height) : WrongSizeException(s, Width, Height) { }
	TooLargeSizeException(const TooLargeSizeException& e) : WrongSizeException(e) {}
};

class IndexOutOfBoundsException : public Exception
{
protected:
	int row, col;
public:
	IndexOutOfBoundsException(const char* s, int Row, int Column) : Exception(s) { row = Row; col = Column; }
	IndexOutOfBoundsException(const IndexOutOfBoundsException& e) : Exception(e) { row = e.row; col = e.col; }

	//функцию вывода можно будет переопределить в производных классах, когда будет ясна конкретика
	virtual void print()
	{
		cout << "IndexOutOfBoundsException: " << str << "; row: " << row << "\t" << "column: " << col << "; " << what();
	}
};

class BaseMatrix
{
protected:
	double** ptr;
	int height;
	int width;
public:
	BaseMatrix(int Height = 2, int Width = 2)
	{
		if (Height <= 0 || Width <= 0)
			throw NonPositiveSizeException("NonPositive size of matrix in BaseMatrix(int, int)", Width, Height);
		if (Height > 20 || Width > 20)
			throw TooLargeSizeException("Matrix size is too large in BaseMatrix(int, int)", Width, Height);
		cout << "\nBaseMatrix constructor int, int";
		//конструктор
		height = Height;
		width = Width;
		ptr = new double* [height];
		for (int i = 0; i < height; i++)
			ptr[i] = new double[width];
	}

	BaseMatrix(const BaseMatrix& M)
	{
		//конструктор копий
		cout << "\nBaseMatrix copy constructor";
		height = M.height;
		width = M.width;
		ptr = new double* [height];
		for (int i = 0; i < height; i++)
		{
			ptr[i] = new double[width];
			for (int j = 0; j < width; j++)
				ptr[i][j] = M.ptr[i][j];
		}
	}

	BaseMatrix(double** arr, int w, int h)
	{
		cout << "\nBaseMatrix constructor double**";
		height = h;
		width = w;
		ptr = new double* [height];
		for (int i = 0; i < height; i++)
		{
			ptr[i] = new double[width];
			for (int j = 0; j < width; j++)
				ptr[i][j] = arr[i][j];
		}
	}

	virtual ~BaseMatrix()
	{
		//деструктор
		if (ptr != NULL)
		{
			for (int i = 0; i < height; i++)
				delete[] ptr[i];
			delete[] ptr;
			ptr = NULL;
		}
	}

	void print()
	{
		//вывод
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
				cout << ptr[i][j] << " ";
			cout << "\n";
		}
	}

	double* operator[](int row)
	{
		if (row < 0 || row >= height)
			throw IndexOutOfBoundsException("Index out of bounds in operator[]", row, -1);
		return ptr[row];
	}

	double& operator()(int row, int column)
	{
		if (row < 0 || row >= height || column < 0 || column >= width)
			throw IndexOutOfBoundsException("Index out of bounds in operator()", row, column);
		return ptr[row][column];
	}

	friend ostream& operator <<(ostream& s, BaseMatrix M);
	friend istream& operator >>(istream& s, BaseMatrix& M);
};

ostream& operator <<(ostream& s, BaseMatrix M)
{
	if (typeid(s) == typeid(ofstream))
	{
		s << M.height << " " << M.width << " ";
		for (int i = 0; i < M.height; i++)
			for (int j = 0; j < M.width; j++)
				s << M.ptr[i][j] << " ";
	}
	else
		for (int i = 0; i < M.height; i++)
		{
			for (int j = 0; j < M.width; j++)
				s << M.ptr[i][j] << "";
			s << "\n";
		}
	return s;
}

istream& operator >>(istream& s, BaseMatrix& M)
{
	if (typeid(s) == typeid(ifstream))
	{
		int w, h;
		s >> h >> w;
		if (h != M.height || w != M.width) throw WrongSizeException("Matrix size in file and in code aren't equal", w, h);
	}
	for (int i = 0; i < M.height; i++)
		for (int j = 0; j < M.width; j++)
			s >> M.ptr[i][j];
	return s;
}

class Matrix : public BaseMatrix
{
public:
	Matrix(int Height = 2, int Width = 2) : BaseMatrix(Height, Width) {}
	Matrix(double** arr, int w, int h) : BaseMatrix(arr, w, h) {}
	Matrix(const Matrix& M) : BaseMatrix((BaseMatrix&)M) { cout << "\nMatrix copy constructor\n"; }
	~Matrix() { cout << "\nMatrix Destructor" };
	double operator+()
	{
		if (width != height)
			throw InvalidSizeException("Quantity of rows and columns isn't equal in operator+()", width, height);
		double sum = 0;
		for (int i = 0; i < height; i++)
			sum += ptr[i][i];
		return sum;
	}
	Matrix operator*(double d)
	{
		Matrix s(height, width);
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				s(i, j) = ptr[i][j] * d;

		return s;
	}
};

int main()
{
	try
	{
		/*BaseMatrix* ptr = new Matrix;
		ptr->print();
		delete ptr;*/
		Matrix* arr = new Matrix[10];
		for (int i = 0; i < 10; i++)
		{
			arr[i](0, 0) = i;
			arr[i](0, 1) = i + 1;
			arr[i](1, 0) = i + 2;
			arr[i](1, 1) = i + 3;
			
		}
		Matrix M1(2, 2);
		/*M1(0, 0) = 1; M1(0, 1) = 2; M1(1, 0) = 3; M1(1, 1) = 4; M1(1, 5) = 5;
		Matrix M2 = M1;*/
		cout << "\n";
		cin >> M1;
		//cout << "\n"
		/*cout << +(M2 * 5);*/
		cout << "\n" << M1;
		/*M2.print();*/

		ofstream fout("test.txt");
		if (fout)
		{
			//fout << "10\n";
			fout << M1;
			fout.close();
		}
		ifstream fin("test.txt");
		if (fin)
		{
			//int n; fin >> n;
			//for (int i = 0; i < n; i++)
			while (!fin.eof())
			{
				try
				{
					Matrix M2;
					fin >> M2;
					cout << "\n" << M2;
				}
				catch (exception e) { cout << "\nCaught exception: " << e.what(); }
			}
			fin.close();
		}
		delete[] arr;
	}
	catch (WrongSizeException e)
	{
		cout << "\nCaught WrongSizeException: "; e.print();
	}
	catch (InvalidSizeException e)
	{
		cout << "\nCaught InvalidSizeException: "; e.print();
	}
	catch (IndexOutOfBoundsException e)
	{
		cout << "\nCaught IndexOutOfBoundsException: "; e.print();
	}
	catch (Exception e)
	{
		cout << "\nCaught Exception: "; e.print();
	}
	catch (exception e)
	{
		cout << "\nCaught Exception: "; cout << e.what();
	}
	char c; cin >> c;

	return 0;
}


/*int main()
{
	if (true)
	{
		BaseString* ptr;
		String s("acba"); ptr = &s;
		s.print();
		cout << "\n" << ptr->IndexOf('t');
		cout << "\n" << s.IsPalindrome();
	}
	char c; cin >> c;
	return 0;
}*/
