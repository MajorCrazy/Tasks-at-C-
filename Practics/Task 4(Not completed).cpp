#include <iostream>
#define s_min_len 256

using namespace std;

class BaseString
{
protected:
	char* p; 
	int len, capacity;
public:
	BaseString(char* cs)
	{
		len = 0;
		while (cs[len] != '\0')
			len++;
		capacity = len + 1 > s_min_len ? len + 1 : s_min_len;
		p = new char[capacity];
		for (int i = 0; i < capacity + 1; i++)
			p[i] = cs[i];
	};

	BaseString(const char* cs)
	{
		len = 0;
		while (cs[len] != '\0')
			len++;
		if (len + 1 > s_min_len)
			capacity = len + 1;
		else
			capacity = s_min_len;
		p = new char[capacity];
		for (int i = 0; i < capacity; i++)
			p[i] = cs[i];
	};

	BaseString(const BaseString& s)
	{
		capacity = s.capacity;
		len = s.len;
		p = new char[s.capacity];

		for (int i = 0; i < s.len; i++)
			p[i] = s.p[i];
	};


	~BaseString()
	{
		if (p != NULL)
			delete[] p;
		len = 0;
	};

	BaseString(int capacity_ = s_min_len)
	{
		if (capacity_ > s_min_len)
			capacity = capacity_;
		else
			capacity = s_min_len;
		len = 0;
		p = new char[capacity] {'\0'};
	};

	BaseString& operator=(BaseString s)
	{
		if (capacity != s.capacity)
		{
			delete[] p;
			p = new char[s.capacity];
			capacity = s.capacity;
		}

		len = s.len;
		for (int i = 0; i < s.capacity; i++)
			p[i] = s.p[i];
		return *this;
	};

	int GetCapacity()
	{
		return capacity;
	};

	int GetLen()
	{
		return len;
	};

	char GetChar(int index)
	{
		if (index < 0 || index >= len)
			return '\0';
		return p[index];
	};

	char* GetPoint()
	{
		return p;
	};

	int IndexOf(char c, int index = -1)
	{
		if (index == -1)
			index = 0;
		for (int i = index; i < len; i++)
			if (p[i] == c)
				return i;
		return -1;
	}

	bool IsPalindrome()
	{
		if (len < 2)
			return true;
		char* p1 = p;
		char* p2 = &p[len - 1];
		while (p1 < p2)
		{
			if (*p1 != *p2)
				return false;
			p1++;
			p2--;
		}
		return true;
	}

	friend void PrintS(BaseString s);
	friend void PrintSS(BaseString s);

};

class String : public BaseString
{
protected:

public:
	String(int capacity_ = s_min_len) : BaseString(capacity_) { };
		String(char* cs) : BaseString(cs) { };
		String(const char* cs) : BaseString(cs) { };
	String(const String& s) : BaseString(s) { };

	int IndexOfLast(char c, int index = -1)
	{
		if (index == -1)
			index = len - 1;

		for (int i = index; i >= 0; i--)
			if (p[i] == c)
				return i;

		return -1;
	}

	String trim()
	{
		int start = len, end = 0;

		for (int i = 0; i < len; i++)
			if (p[i] != ' ')
			{
				start = i;
				break;
			}
		for (int i = len - 1; i >= 0; i--)
			if (p[i] != ' ')
			{
				end = i;
				break;
			}
		if (start > end)
			return String();

		int len_ = end - start + 1;

		String trimmed(len_ + 1);

		for (int i = 0; i < len_; i++)
			trimmed.p[i] = p[i + start];
		trimmed.p[len_ + 1] = '\0';
		trimmed.len = len_;

		return trimmed;
	};

	~String() { };
};

void PrintS(BaseString s)
{
	for (int i = 0; i < s.GetLen(); i++)
		cout << s.GetChar(i);
};

void PrintSS(BaseString s)
{
	for (int i = 0; i < s.GetLen(); i++)
		if (s.GetChar(i) == ' ')
			cout << "[ ]";
		else
			cout << s.GetChar(i);
};

int main()
{
	String s1("  31 CEF ");
	cout << "s1: "; PrintSS(s1); cout << endl;

	String trimmed1 = s1.trim();
	cout << "trimmed1: "; PrintSS(trimmed1); cout << endl;

	return 0;
}
