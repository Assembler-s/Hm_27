#include <iostream>
#include <windows.h>
#include <cstring>

using namespace std;

class MyString
{
private:
	char* data;
	int length;

	void assign(const char* text)
	{
		int size_text = strlen(text) + 1;
		char* temp = new char[size_text];
		strcpy_s(temp, size_text, text);
		delete[] data;
		data = temp;
		length = size_text - 1;
	}

public:
	MyString(const char* text) : data(nullptr), length(0)
	{
		if (text == nullptr)
			text = "";

		assign(text);
	}

	MyString() : MyString("") {};

	MyString& operator = (const MyString& other)
	{
		if (this == &other)
		{
			return *this;
		}

		assign(other.data);
		return *this;
	}

	MyString(const MyString& other) : data(nullptr), length(0)
	{
		*this = other;
	}

	MyString(MyString&& other)
	{
		data = other.data;
		length = other.length;
		other.data = nullptr;
		other.length = 0;
	}

	MyString& operator = (MyString&& other)
	{
		if (this == &other)
		{
			return *this;
		}

		delete[] data;
		data = other.data;
		length = other.length;
		other.data = nullptr;
		other.length = 0;

		return *this;
	}

	bool operator < (const MyString& other) const
	{
		return strcmp(data, other.data) < 0;
	}

	int getLength() const
	{
		return length;
	}

	char& operator [] (int index)
	{
		return data[index];
	}

	char operator [] (int index) const
	{
		return data[index];
	}

	MyString& operator += (const MyString& other)
	{
		size_t newLength = length + other.length + 1;
		char* temp = new char[newLength];
		strcpy_s(temp, newLength, data);
		strcat_s(temp, newLength, other.data);
		delete[] data;
		data = temp;
		length = newLength - 1;
		return *this;
	}

	/*MyString operator + (const MyString& other) const
	{
		MyString result(*this);
		result += other;
		return result;
	}*/

	bool operator == (const MyString& other) const
	{
		return strcmp(data, other.data) == 0;
	}

	bool operator != (const MyString& other) const
	{
		if (strcmp(data, other.data) == 0)
			return false;

		return true;
	}

	~MyString()
	{
		delete[] data;
	}

	explicit operator bool() const
	{
		return length > 0;
	}

	friend ostream& operator << (ostream& out, const MyString& other);
	friend MyString operator+(const MyString& left, const MyString& right);
	friend istream& operator >> (istream& in, MyString& s);
};

istream& operator >> (istream& in, MyString& s)
{
	char buffer[256];
	in >> buffer;
	s = buffer;

	return in;
}

MyString operator + (const MyString& left, const MyString& right)
{
	MyString result(left);
	result += right;

	return result;
}

ostream& operator << (ostream& out, const MyString& other)
{
	if (other.data == nullptr)
		out << "";
	else
		out << other.data;

	return out;
}

int main()
{
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

	// level 1
	MyString greeting("Hello");
	MyString copy = greeting;

	copy[0] = 'J';
	cout << greeting << endl;
	cout << copy << endl;

	MyString empty;
	cout << "[" << empty << "], " << "length " << empty.getLength() << endl;

	MyString name("World");
	MyString message = greeting + ", " + name + "!";
	cout << message << " length " << message.getLength() << endl;
	name += "!!!";
	cout << name << endl;

	cout << (greeting == "Hello") << endl;
	cout << (copy != greeting) << endl;

	MyString a("один");
	MyString b("два");
	MyString c("три");

	cout << a << " " << b << " " << c << endl;

	a = b = c;
	cout << a << " " << b << " " << c << endl;

	c = c;
	cout << c << endl;

	const MyString title("OOP");
	cout << title[0] << endl;
	//title[0] = 'o';
	cout << MyString("Привіт").getLength() << endl;
	
	////////////////////////////////////////////////////////////////

	// level 2
	cout << "-----------\n";

	MyString names[5] = { "Олена", "Марко", "Анна", "Тарас", "Богдан" };
	for (int i = 0; i < 5; i++)
		cout << names[i] << endl;

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (names[j + 1] < names[j])
			{
				MyString temp = move(names[j]);
				names[j] = std::move(names[j + 1]);
				names[j + 1] = std::move(temp);
			}
		}
	}
	cout << "-----------\n";
	for (int i = 0; i < 5; i++)
		cout << names[i] << endl;
	/////////////////////

	//level 3
	MyString hello = "Hello, " + name;
	cout << hello << endl;

	if (!empty)
	{
		cout << "Строка (empty) пустая!\n";
	}

	MyString input;
	cout << "\nВвести слово!: ";
	cin >> input;

	if (input)
		cout << "Строка не (input) пустая!\n";
	else cout << "Строка (input) пустая!\n";

	return 0;
}

/*1) explicit запрещает неявное преобразование const char* в MyString. В классе это преобразование нужно, чтобы строковые
литералы автоматически превращались в объекты MyString при вызове операторов. Если добавить explicit, перестанут компилироваться
следующие строки:
MyString message = greeting + ", " + name + "!";
cout << (greeting == "Hello") << endl;
MyString names[5] = { "Олена", "Марко", "Анна", "Тарас", "Богдан" };
MyString hello = "Hello, " + name;
s = buffer;
В этих выражениях компилятор должен автоматически преобразовать строковый литерал или char* в MyString, используя конструктор
MyString(const char*), explicit отключает такое неявное преобразование.*/

/*2) Если не написать собственный operator=, компилятор создаст его автоматически. Он выполнит поверхностное копирование, скопирует
значения полей, включая сам указатель data, а не содержимое строки. При b = a произойдёт следующее: 
b.data получит тот же адрес, что и a.data.
Оба объекта будут указывать на один динамический буфер.
При изменении строки через один объект изменения будут видны через другой.
Когда один из объектов будет уничтожен, второй объект останется с указателем на уже освобождённую память и при повторном
срабатывании деструтора произойдёт краш.*/

/*3) Две версии нужны для работы с обычными и const объектами. Неконстантная [] возвращает char&, позволяет изменять символ.
Константная [] возвращает char, только чтение. Если оставить только неконстантную, обращение title[0] у константного объекта не скомпилируется.*/

/*4) strlen() считает байты, а не символы. В UTF-8 каждая кириллическая буква занимает 2 байта. Поэтому "Привіт" занимает больше*/

/*5) std::move позволяет использовать конструктор и оператор перемещения вместо копирования. Указатель на буфер передаётся другому
объекту, cами символы не копируются, cтарый объект остаётся без владения буфером. Без методов перемещения при каждом обмене выполнялось
бы глубокое копирование, что затратно.*/

/*6) Метод operator+ вызывается у левого операнда. В выражении: "Hello, " + name, левый операнд — const char*, а не MyString.
У него нет метода MyString::operator+. Дружественная функция принимает оба операнда как параметры. Компилятор может преобразовать "Hello, "
в MyString через конструктор.*/