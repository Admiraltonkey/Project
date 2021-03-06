// lets_go_travelling.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
//Список всех макросов используемых в программе. Цель исспользования - избежать появление "магических чисел" 
#define SUCCSESS 0			//используется для возврата при успешном завершении операции
#define FAIL 1				//используется для возврата при не успешном завершении операции
#define WRONG_COMMAND -1	//используется для возврата при некорректном вводе команды
#define USER 0				//используется в authorization() для возврата типа авторизованного пользователя
#define ADMIN 1				//используется в authorization() для возврата типа авторизованного пользователя
#define NO_EXIST 2			//используется для возврата при отсуствии элемента
#define EXIT 3				//используется для возврата при выходе из authorization()


using namespace std;		//для использования пространства имен std

struct user					//структура описывающая запись пользователя
{
	bool admin;
	string login;
	string password;
};

struct tour					//структура описывающая запись путевки
{
	int cost;
	int days;
	string type_tour;
	string country;
	string type_transport;
	string eat;
};

user auth_user;				//для хранения авторизованного пользователя
vector<user> users_list;	//для хранения списка пользователей
vector<tour> tours_list;	//для хранения списка путевок
vector<tour> temp_list;		//временный vector для путевок. используется при сортировке
long int users_list_size = 0;	//размер списка пользователей. используется при считывании/записи из/в файл
long int tours_list_size = 0;	//размер списка путевок. используется при считывании/записи из/в файл
bool isChanged = false;			//флаг для обозначения состояния записей в users_list/tours_list. Если false записи не изменялись/не добавлялись
bool isSaved = true;			//флаг. True - списки сохранены. False - списки изменены, но не созранены

int authorization();			//запускает процесс авторизации. возращает 0 - для пользователя, 1 - для администратора
void show_authorization_menu(); //отображает меню авторизации
int get_user();					//считывает данный пользователя для авторизоции
int search_user(string const, string const); // проверяет наличие пользователя в user_lists. 
void read_data();							// считывает списки путевок и пользователей из файла
void read_user(ifstream & ifs);				//считывает пользователя из файла
void read_tour(ifstream &ifs);				//считывает путевку из файла
void write_user(ofstream & ofs, user & temp); //записывает пользователя в файл
void write_tour(ofstream & ofs, tour & temp); //записывает путевку в файл
int read_command();								//считывает комманду с клавиатуры
string read_string();							//считывает строку с клавиатуры
int read_number();								//считывает число с клавиатуры
bool check_text(string);						//проверяет длину вводимой строки
void show_all_users();							//показывает список всех пользователей
void show_all_tours();							//показывает список всех путевок
void show_admin_menu();							//показывает меню администратора
void show_tour_list_menu();						//показывает меню управления списком путево
void show_user_list_menu();						//показывает меню управления списком пользователей
void show_data_menu();							//показывает меню работы с файлами
void admin_module();							//запускает модуль администратора
void tours_menu();								//запускает меню управления списком путевок
void users_menu();								//запускает меню управления списком пользователей
void data_menu();								//запускает меню работы с файлами
void add_new_tour();							//добавляет новую путевку
int check_tour(tour &);							//проверяет наличие такойже путевки
void delete_tour();								//удаляет путевку
int search_tour(tour &);						//ищет путевку в списке
bool compare_tours(tour &, tour &);				//спавнивает два тура
bool you_sure();								//запрашивает подтверждение действия
void change_tour();								//изменяет путевку
void add_new_user();							//добавляет нового пользователя
int check_user(user &);							//проверяет наличие такого же пользователя
void change_user();								//меняет информацию о пользователе
void delete_user();								//удаляет пользователя
void save_data();								//записывает всю информаци в файлы
void user_module();								//запускает модуль пользователя
void sort_menu();								//запускает менб сортировки
void filt_menu();								//запускает меню фильтрации
void goal();									//запускает выполнение индивидуального задания
void sort_up_cost(vector<tour> temp = tours_list);	//сортировка пувтевок по возр цены
void sort_down_cost(vector<tour> temp = tours_list); //сортировка пувтевок по убыв цены
void show_sort_tours(vector<tour> & temp);			//показывает отсортированные список путевок
void sort_up_days(vector<tour> temp = tours_list);	////сортировка пувтевок по возр кол дней
void sort_down_days(vector<tour> temp = tours_list);//сортировка пувтевок по убыв кол дней
void filt_by_country();								//фильтр по стране
void filt_by_type_tour();							//фильтр по типу путевки
void filt_by_type_transport();						//фильтр по типу транспорта
void show_tour(tour & temp);						//показывает путевку
void filt_by_eat();									//фильтр по типу питания


int main()
{
	SetConsoleCP(1251);  //для поддержки киррилицы
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "ru");
	read_data(); //считывание информации из файлов
	while (true)
	{
		switch (authorization()) //запуск авторизации
		{
		case 0:
			user_module();		//запуск модуля пользователя
			break;
		case 1:
			admin_module();		//запуск модуля администратора
			break;
		case 2:
			system("cls"); //очистка экрана
			cout << "Неверный логин или пароль" << endl;
			cin.get(); //ожидние ввода. используется для создания пауза. 
			break;
		case 3:
			return EXIT_SUCCESS; //макрос компилятора. возращает код успешного завершения программы
		}
	}
	return EXIT_SUCCESS;
}

int authorization()
{
	while (true)
	{
		system("cls");
		show_authorization_menu();
		switch (read_command())
		{
		case 1:
			return get_user();
			break;
		case 2:
			return EXIT;
			break;
		default:
			system("cls");
			cout << "Неверная команда" << endl;
			cin.get();
			break;
		}
	}
}

void show_authorization_menu()
{
	cout << "Добро пожаловать в Lets go Travelling!" << endl
		<< "1. Авторизация" << endl
		<< "2. Выход" << endl;
}

int get_user()
{
	string login;
	string password;
	system("cls");
	cout << "Введите логин" << endl;
	login = read_string();
	cout << "Введите пароль" << endl;
	password = read_string();
	for (unsigned int i = 0; i < users_list.size(); ++i)
	{
		if (users_list[i].login == login && users_list[i].password == password)
		{
			auth_user = users_list[i];
			if (auth_user.admin == 0)   //определение типа зашедшего
				return USER;
			else
				return ADMIN;
		}
	}
	return NO_EXIST;  //если не найден 
}

int search_user(string const login, string const password)
{
	for (unsigned int i = 0; i < users_list.size(); ++i)
	{
		if (users_list[i].login == login && users_list[i].password == password)
		{
			return i; // возращает номер найденного элемента в списке
		}
	}
	return NO_EXIST;
}

void read_data()
{
	if (isChanged && !isSaved) // проверка - сохранены ли изменения
	{
		system("cls");
		cout << "Изменения не сохранены!" << endl;
		if (!you_sure())
			return;
	}
	users_list.clear(); // предварительная очистка списков
	tours_list.clear();
	ifstream users_file("D:\\user_file.txt", ios::binary | ios::in); //открытие файлов
	ifstream tours_file("D:\\tour_file.txt", ios::binary | ios::in);
	if (!users_file.is_open()) // проверка открытия файлов
	{
		system("cls");
		cout << "Ошибка открытия списка пользователей" << endl;
		cin.get();
		add_new_user(); //если файл не открылся, то создаем нового пользователя
		return;
	}
	if (!tours_file.is_open())
	{
		system("cls");
		cout << "Ошибка открытия списка туров" << endl;
		cin.get();
		return;
	}
	users_file.read((char *)&users_list_size, sizeof(users_list_size)); //считывание из файла
	for (int i = 0; i < users_list_size; i++)
	{
		read_user(users_file);
	}
	tours_file.read((char *)&tours_list_size, sizeof(tours_list_size));
	for (int i = 0; i < tours_list_size; i++)
	{
		read_tour(tours_file);
	}
	users_file.close(); // закрытие файлов
	tours_file.close();
}

void read_user(ifstream & ifs)
{
	user temp;												//т. к. string сам по себе не записывается в файл, считываем его частями
	ifs.read((char *)&temp.admin, sizeof(temp.admin));		//отдельно считываем размер строки и саму строку. (так для каждого поля string структуры)
	unsigned int len = 0;									//переменная для длины строки
	ifs.read((char *)&len, sizeof(len));
	char * buf = new char[len];								//переменная для хранения строки
	ifs.read(buf, len);
	temp.login = buf;
	delete[] buf;
	len = 0;
	ifs.read((char *)&len, sizeof(len));
	buf = new char[len];
	ifs.read(buf, len);
	temp.password = buf;
	delete[] buf;
	users_list.push_back(temp);
}

void read_tour(ifstream &ifs)
{
	tour temp;
	ifs.read((char *)&temp.cost, sizeof(temp.cost));
	ifs.read((char *)&temp.days, sizeof(temp.days));
	unsigned int len = 0;
	ifs.read((char *)&len, sizeof(len));
	char * buf = new char[len];
	ifs.read(buf, len);
	temp.type_tour = buf;
	delete[] buf;
	ifs.read((char *)&len, sizeof(len));
	buf = new char[len];
	ifs.read(buf, len);
	temp.country = buf;
	delete[] buf;
	ifs.read((char *)&len, sizeof(len));
	buf = new char[len];
	ifs.read(buf, len);
	temp.type_transport = buf;
	delete[] buf;
	ifs.read((char *)&len, sizeof(len));
	buf = new char[len];
	ifs.read(buf, len);
	temp.eat = buf;
	delete[] buf;
	tours_list.push_back(temp);
}

void write_user(ofstream & ofs, user & temp)
{
	ofs.write((char *)&temp.admin, sizeof(temp.admin));			//запись string тоже раздельная. отдельн записывается длина строки и строка
	unsigned int len = temp.login.length() + 1;
	ofs.write((char *)&len, sizeof(len));
	ofs.write((char *)temp.login.c_str(), len);
	len = temp.password.length() + 1;
	ofs.write((char *)&len, sizeof(len));
	ofs.write((char *)temp.password.c_str(), len);
}

void write_tour(ofstream & ofs, tour & temp)
{
	ofs.write((char *)&temp.cost, sizeof(temp.cost));
	ofs.write((char *)&temp.days, sizeof(temp.days));
	unsigned int len = temp.type_tour.length() + 1;
	ofs.write((char *)&len, sizeof(len));
	ofs.write((char *)temp.type_tour.c_str(), len);
	len = temp.country.length() + 1;
	ofs.write((char *)&len, sizeof(len));
	ofs.write((char *)temp.country.c_str(), len);
	len = temp.type_transport.length() + 1;
	ofs.write((char *)&len, sizeof(len));
	ofs.write((char *)temp.type_transport.c_str(), len);
	len = temp.eat.length() + 1;
	ofs.write((char *)&len, sizeof(len));
	ofs.write((char *)temp.eat.c_str(), len);
}

int read_command()  //считывает и проверяет корректность вводимой команды
{
	string string_command;
	getline(cin, string_command);
	try
	{
		return stoi(string_command); //если stoi генерирует исключение то ввод некорректный(введен текст)
	}
	catch (...)
	{
		return WRONG_COMMAND;
	}
}

string read_string() //считывает и проверяет корректность вводимой строки
{
	string string_line;
	getline(cin, string_line);
	while (!check_text(string_line))
	{
		cout << "Длина вводимой строки не должна превышать 60 символов" << endl;
		getline(cin, string_line);
	}
	return string_line;
}

int read_number()	//считывает и проверяет корректность вводимого числа
{
	string string_number;
	int number = -1;
	while (true)
	{
		getline(cin, string_number);
		try
		{
			number = stoi(string_number);
		}
		catch (...)
		{
			cout << "Вводите положителные числовые значения" << endl;
		}
		if (number >= 0)
			return number;
		else
		{
			cout << "Вводите положителные числовые значения" << endl;
		}
	}
}

bool check_text(string text) // проверка длины строки
{
	if (text.length() > 60)
		return false;
	else
		return true;
}

void show_all_users()
{
	system("cls");
	cout << "----------Логин / Статус----------" << endl;
	for (unsigned int i = 0; i < users_list.size(); ++i)
	{
		string temp;
		if (users_list[i].admin == 1)
			temp = users_list[i].login + " / Администратор";
		else
			temp = users_list[i].login + " / Пользователь";
		cout << temp << endl;
	}
	cin.get();
}

void show_all_tours()
{
	system("cls");
	cout << "-----Все доступные путевки-----" << endl;
	for (unsigned int i = 0; i < tours_list.size(); ++i)
	{
		show_tour(tours_list[i]);
	}
	cin.get();
}

void show_admin_menu()
{
	cout << "-----Режим Администратора-----" << endl
		<< "1. Управление списком путевок" << endl
		<< "2. Управление списком пользователей" << endl
		<< "3. Работа с данными" << endl
		<< "4. Выход" << endl;
}

void show_tour_list_menu()
{
	cout << "-----Управление списком путевок-----" << endl
		<< "1. Просмотреть весь список путевок" << endl
		<< "2. Добавить новую путевку" << endl
		<< "3. Редактировать информацию о путевке" << endl
		<< "4. Удалить путевку" << endl
		<< "5. Назад" << endl;
}

void show_user_list_menu()
{
	cout << "-----Управление списком пользователей-----" << endl
		<< "1. Просмотр всего списка пользователей" << endl
		<< "2. Добавить нового пользователя" << endl
		<< "3. Редактировать учетную запись" << endl
		<< "4. Удаление учетной записи" << endl
		<< "5. Назад" << endl;
}

void show_data_menu()
{
	cout << "-----Работа с данными-----" << endl
		<< "1. Сохранить информацию в файл" << endl
		<< "2. Загрузить информацию из файла" << endl
		<< "3. Назад" << endl;
}


void admin_module()
{

	while (true)
	{
		system("cls");
		show_admin_menu();
		switch (read_command())
		{
		case 1:
			tours_menu();
			break;
		case 2:
			users_menu();
			break;
		case 3:
			data_menu();
			break;
		case 4:
			if (isChanged && !isSaved) // порерка перед выходом - сохранены ли изменения
			{
				system("cls");
				cout << "Изменения не сохранены!" << endl;
				if (you_sure())
					return;
			}
			else
				return;
		default:
			system("cls");
			cout << "Неверная команда" << endl;
			cin.get();
			break;
		}
	}
}

void tours_menu()
{
	while (true)
	{
		system("cls");
		show_tour_list_menu();
		switch (read_command())
		{
		case 1:
			show_all_tours();
			break;
		case 2:
			add_new_tour();
			break;
		case 3:
			change_tour();
			break;
		case 4:
			delete_tour();
			break;
		case 5:
			return;
		default:
			system("cls");
			cout << "Неверная команда" << endl;
			cin.get();
			break;
		}
	}
}

void users_menu()
{
	while (true)
	{
		system("cls");
		show_user_list_menu();
		switch (read_command())
		{
		case 1:
			show_all_users();
			break;
		case 2:
			add_new_user();
			break;
		case 3:
			change_user();
			break;
		case 4:
			delete_user();
			break;
		case 5:
			return;
		default:
			system("cls");
			cout << "Неверная команда" << endl;
			cin.get();
			break;
		}
	}
}

void data_menu()
{
	while (true)
	{
		system("cls");
		show_data_menu();
		switch (read_command())
		{
		case 1:
			save_data();
			break;
		case 2:
			read_data();
			break;
		case 3:
			return;
		default:
			system("cls");
			cout << "Неверная команда" << endl;
			cin.get();
			break;
		}
	}
}

void add_new_tour()
{
	tour temp;
	system("cls");
	cout << "-----Добавление новой путевки-----" << endl;
	cout << "Введите тип путевки: " << endl;
	temp.type_tour = read_string();
	cout << "Введите страну пребывания: " << endl;
	temp.country = read_string();
	cout << "Введите тип транспорта: " << endl;
	temp.type_transport = read_string();
	cout << "Введите количество дней: " << endl;
	temp.days = read_number();
	cout << "Введите тип питания: " << endl;
	temp.eat = read_string();
	cout << "Введите стоимость: " << endl;
	temp.cost = read_number();
	if (check_tour(temp) == SUCCSESS)
	{
		tours_list.push_back(temp);
		tours_list_size++;
		cout << "-----Новая путевка добавлен-----" << endl;
		isChanged = true;
		isSaved = false;
	}
	else
	{
		system("cls");
		cout << "Такая путевка уже существует" << endl;
	}
	cin.get();
}

int check_tour(tour & temp)
{
	for (int i = 0; i < tours_list_size; ++i)
	{
		if (compare_tours(temp, tours_list[i]))
		{
			return FAIL;
		}
	}
	return SUCCSESS;
}

void delete_tour()
{
	tour temp;
	int deleted_tour;
	system("cls");
	cout << "-----Удаление путевки-----" << endl;
	cout << "Введите тип путевки: " << endl;
	temp.type_tour = read_string();
	cout << "Введите страну пребывания: " << endl;
	temp.country = read_string();
	cout << "Введите тип транспорта: " << endl;
	temp.type_transport = read_string();
	cout << "Введите количество дней: " << endl;
	temp.days = read_number();
	cout << "Введите тип питания: " << endl;
	temp.eat = read_string();
	cout << "Введите стоимость: " << endl;
	temp.cost = read_number();
	deleted_tour = search_tour(temp);
	if (deleted_tour == NO_EXIST)
	{
		system("cls");
		cout << "Такая путевка не найдена" << endl;
		cin.get();
	}
	else if (you_sure())
	{
		tours_list.erase(tours_list.begin() + deleted_tour);
		tours_list_size--;
		cout << "-----Путевка удалена-----" << endl;
		isChanged = true;
		isSaved = false;
	}
	else
		cout << "-----Удаление отменено-----" << endl;
	cin.get();
}


int search_tour(tour & temp)
{
	for (unsigned int i = 0; i < tours_list.size(); ++i)
	{
		if (compare_tours(tours_list[i], temp))
			return i;
	}
	return NO_EXIST;
}

bool compare_tours(tour & tour_1, tour & tour_2)
{
	if (tour_1.type_tour == tour_2.type_tour &&
		tour_1.type_transport == tour_2.type_transport &&
		tour_1.country == tour_2.country &&
		tour_1.days == tour_2.days &&
		tour_1.eat == tour_2.eat &&
		tour_1.cost == tour_2.cost)
		return true;
	else
		return false;
}

bool you_sure()
{
	string answer;
	system("cls");
	cout << "Вы уверены? (да / нет)" << endl;
	answer = read_string();
	if (answer == "да" || answer == "д")
		return true;
	else
		return false;
}

void change_tour()
{
	tour temp;
	int changed_tour;
	system("cls");
	cout << "-----Изменение путевки-----" << endl;
	cout << "Введите тип путевки: " << endl;
	temp.type_tour = read_string();
	cout << "Введите страну пребывания: " << endl;
	temp.country = read_string();
	cout << "Введите тип транспорта: " << endl;
	temp.type_transport = read_string();
	cout << "Введите количество дней: " << endl;
	temp.days = read_number();
	cout << "Введите тип питания: " << endl;
	temp.eat = read_string();
	cout << "Введите стоимость: " << endl;
	temp.cost = read_number();
	changed_tour = search_tour(temp);
	if (changed_tour < 0)
	{
		system("cls");
		cout << "Такая путевка не найдена" << endl;
		cin.get();
	}
	else if (you_sure())
	{
		cout << "Введите новую информацию" << endl;
		cout << "Введите тип путевки: " << endl;
		temp.type_tour = read_string();
		cout << "Введите страну пребывания: " << endl;
		temp.country = read_string();
		cout << "Введите тип транспорта: " << endl;
		temp.type_transport = read_string();
		cout << "Введите количество дней: " << endl;
		temp.days = read_number();
		cout << "Введите тип питания: " << endl;
		temp.eat = read_string();
		cout << "Введите стоимость: " << endl;
		temp.cost = read_number();
		tours_list.erase(tours_list.begin() + changed_tour);
		tours_list.push_back(temp);
		cout << "-----Путевка изменена-----" << endl;
		isChanged = true;
		isSaved = false;
	}
	cin.get();
}

void add_new_user()
{
	system("cls");
	cout << "-----Добавление нового пользователя-----" << endl;
	user temp;
	cout << "Введите логин: " << endl;
	temp.login = read_string();
	cout << "Введите пароль: " << endl;
	temp.password = read_string();
	cout << "Введите статус (0 - пользователь, 1 - администратор)" << endl;
	temp.admin = read_number();
	if (check_user(temp) == SUCCSESS)
	{
		users_list.push_back(temp);
		users_list_size++;
		cout << "-----Пользователь добавлен-----" << endl;
		isChanged = true;
		isSaved = false;
	}
	else
	{
		system("cls");
		cout << "Такой пользователь существует" << endl;
	}
	cin.get();
}

int check_user(user & temp)
{
	for (int i = 0; i < users_list_size; ++i)
	{
		if (temp.login == users_list[i].login)
		{
			return FAIL;
		}
	}
	return SUCCSESS;
}

void change_user()
{
	system("cls");
	cout << "-----Изменение пользователя-----" << endl;
	user temp;
	int changed_user;
	cout << "Введите логин: " << endl;
	temp.login = read_string();
	cout << "Введите пароль: " << endl;
	temp.password = read_string();
	changed_user = search_user(temp.login, temp.password);
	if (changed_user == NO_EXIST)
	{
		system("cls");
		cout << "Такого пользователя не существует" << endl;
		cin.get();
	}

	else if (you_sure())
	{
		cout << "Введите новый логин: " << endl;
		temp.login = read_string();
		cout << "Введите новый пароль: " << endl;
		temp.password = read_string();
		cout << "Введите статус (0 - пользователь, 1 - администратор)" << endl;
		temp.admin = read_number();
		users_list.erase(users_list.begin() + changed_user);
		users_list.push_back(temp);
		cout << "-----Информация о пользователе изменена-----" << endl;
		isChanged = true;
		isSaved = false;
	}
	cin.get();
}

void delete_user()
{
	system("cls");
	user temp;
	int deleted_user;
	cout << "-----Удаление пользователя-----" << endl;
	cout << "Введите логин: " << endl;
	temp.login = read_string();
	cout << "Введите пароль: " << endl;
	temp.password = read_string();
	deleted_user = search_user(temp.login, temp.password);
	if (deleted_user == NO_EXIST)
	{
		system("cls");
		cout << "Такой пользователь не найдена" << endl;
		cin.get();
	}
	else if (you_sure())
	{
		users_list.erase(users_list.begin() + deleted_user);
		users_list_size--;
		cout << "-----Пользователь удален-----" << endl;
		isChanged = true;
		isSaved = false;
	}
	else
		cout << "-----Удаление отменено-----" << endl;
	cin.get();
}

void save_data()
{
	isChanged = false;
	isSaved = true;
	ofstream users_file("D:\\user_file.txt", ios::binary | ios::out);
	ofstream tours_file("D:\\tour_file.txt", ios::binary | ios::out);
	if (!users_file)
	{
		system("cls");
		cout << "Ошибка открытия списка пользователей" << endl;
		cin.get();
		return;
	}
	if (!tours_file)
	{
		system("cls");
		cout << "Ошибка открытия списка туров" << endl;
		cin.get();
		return;
	}
	users_file.write((char *)&users_list_size, sizeof(users_list_size));
	for (int i = 0; i < users_list_size; ++i)
	{
		write_user(users_file, users_list[i]);
	}
	tours_file.write((char *)&tours_list_size, sizeof(tours_list_size));
	for (int i = 0; i < tours_list_size; ++i)
	{
		write_tour(tours_file, tours_list[i]);
	}
	users_file.close();
	tours_file.close();
}

void show_user_menu()
{
	cout << "-----Режим Пользователя-----" << endl
		<< "1. Просмотр всех путевок" << endl
		<< "2. Сортировка путевок" << endl
		<< "3. Фильтрация путевок" << endl
		<< "4. Выполнить задание" << endl
		<< "5. Выход" << endl;
}

void show_sort_menu()
{
	cout << "-----Варианты сортировки-----" << endl
		<< "1. Сортировка по возрастанию цены" << endl
		<< "2. Сортировка по убыванию цены" << endl
		<< "3. Сортировка по возрастанию количества дней" << endl
		<< "4. Сортировка по убыванию количества дней" << endl
		<< "5. Назад" << endl;
}

void show_filt_menu()
{
	cout << "-----Вариианты фильтрации-----" << endl
		<< "1. По стране пребывания" << endl
		<< "2. По типу путевки" << endl
		<< "3. По типу транспорта" << endl
		<< "4. По типу питания" << endl
		<< "5. Назад" << endl;
}


void user_module()
{
	while (true)
	{
		system("cls");
		show_user_menu();
		switch (read_command())
		{
		case 1:
			show_all_tours();
			break;
		case 2:
			sort_menu();
			break;
		case 3:
			filt_menu();
			break;
		case 4:
			goal();
			break;
		case 5:
			return;
		default:
			system("cls");
			cout << "Неверная команда" << endl;
			cin.get();
			break;
		}
	}
}

void sort_menu()
{
	while (true)
	{
		system("cls");
		show_sort_menu();
		switch (read_command())
		{
		case 1:
			sort_up_cost();
			break;
		case 2:
			sort_down_cost();
			break;
		case 3:
			sort_up_days();
			break;
		case 4:
			sort_down_days();
			break;
		case 5:
			return;
		default:
			system("cls");
			cout << "Неверная команда" << endl;
			cin.get();
			break;
		}
	}
}

void filt_menu()
{
	while (true)
	{
		system("cls");
		show_filt_menu();
		switch (read_command())
		{
		case 1:
			filt_by_country();
			break;
		case 2:
			filt_by_type_tour();
			break;
		case 3:
			filt_by_type_transport();
			break;
		case 4:
			filt_by_eat();
			break;
		case 5:
			return;
		default:
			system("cls");
			cout << "Неверная команда" << endl;
			cin.get();
			break;
		}
	}
}

void goal()
{
	system("cls");
	string type;
	int cost;
	cout << "-----Индивидуальное задание-----" << endl
		<< "Введите нужный тип путевки: " << endl;
	type = read_string();
	cout << "Введите максимальную цену: " << endl;
	cost = read_number();
	temp_list.clear();
	for (unsigned int i = 0; i < tours_list.size(); ++i)
	{
		if (tours_list[i].type_tour == type && tours_list[i].cost <= cost)
			temp_list.push_back(tours_list[i]);
	}
	if (temp_list.size() > 0)
	{
		sort_down_cost(temp_list);
	}
	else
	{
		system("cls");
		cout << "Таких путевок не существует" << endl;
		cin.get();
	}

}

void sort_up_cost(vector<tour> temp)
{
	for (unsigned int i = 0; i < temp.size() - 1; ++i)
	{
		for (unsigned int j = i; j < temp.size(); ++j)
		{
			if (temp[i].cost > temp[j].cost)
			{
				tour temp_tour = temp[i];
				temp[i] = temp[j];
				temp[j] = temp_tour;
			}
		}
	}
	show_sort_tours(temp);
	temp.clear();
}

void sort_down_cost(vector<tour> temp)
{
	for (unsigned int i = 0; i < temp.size() - 1; ++i)
	{
		for (unsigned int j = i; j < temp.size(); ++j)
		{
			if (temp[i].cost < temp[j].cost)
			{
				tour temp_tour = temp[i];
				temp[i] = temp[j];
				temp[j] = temp_tour;
			}
		}
	}
	show_sort_tours(temp);
	temp.clear();
}

void show_sort_tours(vector<tour> & temp)
{
	for (unsigned int i = 0; i < temp.size(); ++i)
	{
		show_tour(temp[i]);
	}
	cin.get();
}

void sort_up_days(vector<tour> temp)
{
	for (unsigned int i = 0; i < temp.size() - 1; ++i)
	{
		for (unsigned int j = i; j < temp.size(); ++j)
		{
			if (temp[i].days > temp[j].days)
			{
				tour temp_tour = temp[i];
				temp[i] = temp[j];
				temp[j] = temp_tour;
			}
		}
	}
	show_sort_tours(temp);
	temp.clear();
}

void sort_down_days(vector<tour> temp)
{
	for (unsigned int i = 0; i < temp.size() - 1; ++i)
	{
		for (unsigned int j = i; j < temp.size(); ++j)
		{
			if (temp[i].days < temp[j].days)
			{
				tour temp_tour = temp[i];
				temp[i] = temp[j];
				temp[j] = temp_tour;
			}
		}
	}
	show_sort_tours(temp);
	temp.clear();
}

void filt_by_country()
{
	string country;
	system("cls");
	cout << "Введите желаемую страну:" << endl;
	country = read_string();
	cout << "Найденный варианты: " << endl;
	for (int i = 0; i < tours_list_size; ++i)
	{
		if (tours_list[i].country == country)
			show_tour(tours_list[i]);
	}
	cin.get();
}

void filt_by_type_tour()
{
	string type_tour;
	system("cls");
	cout << "Введите желаемый тип:" << endl;
	type_tour = read_string();
	cout << "Найденный варианты: " << endl;
	for (int i = 0; i < tours_list_size; ++i)
	{
		if (tours_list[i].type_tour == type_tour)
			show_tour(tours_list[i]);
	}
	cin.get();
}

void filt_by_type_transport()
{
	string type_transport;
	system("cls");
	cout << "Введите желаемый тип:" << endl;
	type_transport = read_string();
	cout << "Найденный варианты: " << endl;
	for (int i = 0; i < tours_list_size; ++i)
	{
		if (tours_list[i].type_transport == type_transport)
			show_tour(tours_list[i]);
	}
	cin.get();
}

void filt_by_eat()
{
	string eat;
	system("cls");
	cout << "Введите желаемый тип:" << endl;
	eat = read_string();
	cout << "Найденный варианты: " << endl;
	for (int i = 0; i < tours_list_size; ++i)
	{
		if (tours_list[i].eat == eat)
			show_tour(tours_list[i]);
	}
	cin.get();
}

void show_tour(tour & temp)
{
	cout << "-------------------------------------------------" << endl;
	cout << "Тип путевки: " << temp.type_tour << endl
		<< "Страна пребывания: " << temp.country << endl
		<< "Вид транспорта: " << temp.type_transport << endl
		<< "Количество дней: " << temp.days << endl
		<< "Питание: " << temp.eat << endl
		<< "Стоимость: " << temp.cost << endl;
	cout << "-------------------------------------------------" << endl;
}

