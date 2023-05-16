#include <iostream>
#include <fstream>
#include <string.h>

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

class InvalidOperationException : public Exception {
public:
	InvalidOperationException(const char* s, int width, int height) : Exception(s) {}
	InvalidOperationException(const InvalidOperationException& e) : Exception(e) {}

	virtual void print() {
		cout << "InvalidOperationException: " << str << what();
	}
};

class InvalidSizeException : public Exception {
	int w, h;
public:
	InvalidSizeException(const char* s, int width, int height) : Exception(s) { w = width; h = height; }
	InvalidSizeException(const InvalidSizeException& e) : Exception(e) { w = e.w; h = e.h; }

	virtual void print() {
		cout << "InvalidSizeException: " << str << "; columns: " << w << " rows: " << h << "; " << what();
	}
};

class IndexOutOfBoundsException : public Exception {
	int row, col;
public:
	IndexOutOfBoundsException(const char* s, int Row, int Column) : Exception(s) { row = Row; col = Column; }
	IndexOutOfBoundsException(const IndexOutOfBoundsException& e) : Exception(e) { row = e.row; col = e.col; }

	virtual void print() {
		cout << "IndexOutOfBoundsException: " << str << "; row: " << row << " column: " << col << "; " << what();
	}
};

class WrongSizeException : public Exception {
	int w, h;
public:
	WrongSizeException(const char* s, int width, int height) : Exception(s) { w = width; h = height; }
	WrongSizeException(const WrongSizeException& e) : Exception(e) { w = e.w; h = e.h; }

	virtual void print() {
		cout << "WrongSizeException:" << str << "; columns: " << h << " rows : " << w << "; " << what();
	}
};

class NonPositiveSizeException : public WrongSizeException {
public:
	NonPositiveSizeException(const char* s, int width, int height) : WrongSizeException(s, width, height) { }
	NonPositiveSizeException(const WrongSizeException& e) : WrongSizeException(e) { }
};

class TooLargeSizeException : public WrongSizeException {
public:
	TooLargeSizeException(const char* s, int width, int height) : WrongSizeException(s, width, height) { }
	TooLargeSizeException(const WrongSizeException& e) : WrongSizeException(e) { }
};

class NonPositiveSizeExceptionArray : public NonPositiveSizeException {
public:
	NonPositiveSizeExceptionArray(const char* s) : NonPositiveSizeException(s, 0, 0) {}
	virtual void print() {
		cout << "NonPositiveSizeExceptionArray: " << str << what();
	}
};

template<class T>
class BaseMatrix
{
protected:
	T** ptr;
	int height;
	int width;
public:

	BaseMatrix(int Height = 2, int Width = 2)
	{
		if (Height <= 0 || Width <= 0)
			throw NonPositiveSizeException("NonPositive size of matrix in BaseMatrix(int, int)", Width, Height);
		if (Height > 20 || Width > 20)
			throw TooLargeSizeException("TooLargeSizeException in BaseMatrix(int, int)", Width, Height);
		height = Height;
		width = Width;
		ptr = new T * [height];
		for (int i = 0; i < height; i++)
			ptr[i] = new T[width];
	}

	BaseMatrix(const BaseMatrix& M)
	{
		height = M.height;
		width = M.width;
		ptr = new T * [height];
		for (int i = 0; i < height; i++) {
			ptr[i] = new T[width];
			for (int j = 0; j < width; j++)
				ptr[i][j] = M.ptr[i][j];
		}
	}

	BaseMatrix operator=(const BaseMatrix& M) {
		for (int i = 0; i < height; i++)
			delete[] ptr[i];
		delete[] ptr;
		height = M.height;
		width = M.width;
		ptr = new T * [height];
		for (int i = 0; i < height; i++) {
			ptr[i] = new T[width];
			for (int j = 0; j < width; j++)
				ptr[i][j] = M.ptr[i][j];
		}
		height = M.height;
		width = M.width;
		ptr = new T * [height];
		for (int i = 0; i < height; i++) {
			ptr[i] = new T[width];
			for (int j = 0; j < width; j++)
				ptr[i][j] = M.ptr[i][j];
		}
		return *this;
	}

	BaseMatrix(T** arr, int w, int h) {
		height = h;
		width = w;
		ptr = new T * [height];
		for (int i = 0; i < height; i++) {
			ptr[i] = new T[width];
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

	friend ostream& operator<<(ostream& s, const BaseMatrix& M) {
		if (typeid(s) == typeid(ofstream)) {
			s << M.height << " " << M.width << " ";
			for (int i = 0; i < M.height; i++)
			{
				for (int j = 0; j < M.width; j++)
					s << M.ptr[i][j] << " ";
			}
		}
		else {
			for (int i = 0; i < M.height; i++)
			{
				for (int j = 0; j < M.width; j++)
					s << M.ptr[i][j] << " ";
				s << "\n";
			}
		}
		return s;
	}

	friend istream& operator>>(istream& s, BaseMatrix& M) {
		if (typeid(s) == typeid(ifstream)) {
			int w, h;
			s >> h >> w;
			if (h != M.height || w != M.width) {
				if (M.ptr != NULL)
				{
					for (int i = 0; i < M.height; i++)
						delete[] M.ptr[i];
					delete[] M.ptr;
				}
				M.ptr = new T * [h];
				for (int i = 0; i < h; i++) {
					M.ptr[i] = new T[w];
				}
				M.width = w;
				M.height = h;
			}

		}
		for (int i = 0; i < M.height; i++)
		{
			for (int j = 0; j < M.width; j++)
				s >> M.ptr[i][j];
		}
		return s;
	}

	T* operator[](int row) {
		if (row < 0 || row >= height)
			throw IndexOutOfBoundsException("Index out of bounds in operator[]", row, -1);
		return ptr[row];
	}

	T& operator()(int row, int column) const {
		if (row < 0 || row >= height || column < 0 || column >= width)
			throw IndexOutOfBoundsException("Index out of bounds in operator[]", row, column);
		return ptr[row][column];
	}

};

template<typename T>
class Matrix : public BaseMatrix<T> 
{
public:
	Matrix(int h = 2, int w = 2) : BaseMatrix<T>::BaseMatrix(h, w) {}
	Matrix(T** arr, int w, int h) : BaseMatrix<T>::BaseMatrix(arr, w, h) {}

	double operator+() {
		if (BaseMatrix<T>::height != BaseMatrix<T>::width) {
			throw InvalidOperationException("Matrix::operator+: Matrix isn't square", BaseMatrix<T>::width, BaseMatrix<T>::height);
		}
		double sum = 0;
		for (int i = 0; i < BaseMatrix<T>::height; i++)
			sum += BaseMatrix<T>::ptr[i][i];
		return sum;
	}

	Matrix operator*(T d) {
		Matrix res(BaseMatrix<T>::height, BaseMatrix<T>::width);
		for (int i = 0; i < BaseMatrix<T>::height; i++)
			for (int j = 0; j < BaseMatrix<T>::width; j++)
				res(i, j) = BaseMatrix<T>::ptr[i][j] * d;
		return res;
	}

	void randomFilling(int n) {
		for (int i = 0; i < BaseMatrix<T>::height; i++)
			for (int j = 0; j < BaseMatrix<T>::width; j++)
				BaseMatrix<T>::ptr[i][j] = rand() % n + 1;
	}

};

template<typename T>
class LocalMinimumMatrix : public Matrix<T> {
public:
	LocalMinimumMatrix(int h = 2, int w = 2) : Matrix<T>(h, w) {}
	LocalMinimumMatrix(T** arr, int w, int h) : Matrix<T>(arr, w, h) {}

	Matrix<bool> LocalMinimum() {
		Matrix<bool> minimum(Matrix<T>::height, Matrix<T>::width);

		for (int i = 1; i < Matrix<T>::height - 1; i++) {
			for (int j = 1; j < Matrix<T>::width - 1; j++) {
				bool isMinimum = true;
				T value = Matrix<T>::ptr[i][j];

				// Check if the current element is smaller than its neighbors
				if (value > Matrix<T>::ptr[i - 1][j] || value > Matrix<T>::ptr[i + 1][j] ||
					value > Matrix<T>::ptr[i][j - 1] || value > Matrix<T>::ptr[i][j + 1]) {
					isMinimum = false;
				}

				minimum(i, j) = isMinimum;
			}
		}

		return minimum;
	}
};


int main()
{
	try {
		
		Matrix<double>* arr = new Matrix<double>[10];
		for (int i = 0; i < 10; i++) {
			arr[i](0, 0) = rand() % 100 + 1;
			arr[i](0, 1) = rand() % 100 + 1;
			arr[i](1, 0) = rand() % 100 + 1;
			arr[i](1, 1) = rand() % 100 + 1;
		}
		ofstream fout("test.txt");
		for (int i = 0; i < 10; i++)
			fout << arr[i] << "\n";
		fout.close();
		ifstream fin("test.txt");
		cout << "Data from file\n";
		for (int i = 0; i < 10; i++) {
			Matrix<double> M1(5, 6);
			fin >> M1;
			cout << M1 << endl;
		}
		fin.close();
		cout << "Initial data\n";
		for (int i = 0; i < 10; i++)
			cout << arr[i] << endl;

		// Create a matrix and fill it with random values
		LocalMinimumMatrix<double>* arr = new LocalMinimumMatrix<double>[10];
		for (int i = 0; i < 10; i++) {
			arr[i].randomFilling(100);
		}

		// Find the local minima in each matrix
		for (int i = 0; i < 10; i++) {
			cout << "Matrix " << i + 1 << ":\n";
			cout << "Original matrix:\n";
			cout << arr[i] << endl;

			Matrix<bool> minimum = arr[i].LocalMinimum();
			cout << "Local minimum:\n";
			cout << minimum << endl;
		}

		return 0;
	}
	catch (InvalidOperationException& e) {
		cout << "\nCaught: "; e.print();
	}
	catch (InvalidSizeException& e) {
		cout << "\nCaught: "; e.print();
	}
	catch (IndexOutOfBoundsException& e) {
		cout << "\nCaught: "; e.print();
	}
	catch (WrongSizeException& e) {
		cout << "\nCaught: "; e.print();
	}
	catch (Exception& e) {
		cout << "\nCaught: "; e.print();
	}
	catch (exception& e) {
		cout << "\nCaught: "; cout << e.what();
	}
}
