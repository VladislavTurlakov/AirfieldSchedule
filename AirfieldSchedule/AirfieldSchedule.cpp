#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream> 
#include <iomanip>

using namespace std;

struct Data { // структура для хранения данных из файла
	char* boardingTime; // время посадки
	char* planeModel; // марка ЛА
	char* number; // бортовой номер
	char* airport; // аэродром посадки
};

struct Index { // cтруктура для сортировки данных
	int id;
	char* time;
};

const int MaxLen = 50; // максимальная длина строки
const char* FNAME = "InfoSchedule.txt";

// прототипы функций

void PrintTable(Index* IndexStrs, Data* Arr, int N_strings); // печать таблицы
int ReadFile(const char* FNAME); // проверка файла на ошибки и подсчет строк
int ReadTable(const char* FNAME, Data* Arr, int N_strings); // считывание данных из файла
void Sort(Index* IndexStrs, const int N_strings); // функция сортировки

int main() // главная функция программы
{
	int N_strings; // количество строк в файле
	int real_size; // количество строк без ошибок
	N_strings = ReadFile(FNAME);
	if (N_strings == 0) {
		return 0;
	}
	Data* Arr = new Data[N_strings];

	real_size = ReadTable(FNAME, Arr, N_strings);

	Index* IndexStrs = new Index[real_size];
	for (int i = 0; i < real_size; i++)
	{
		IndexStrs[i].id = i;
		IndexStrs[i].time = Arr[i].boardingTime;
	}
	Sort(IndexStrs, real_size);
	PrintTable(IndexStrs, Arr, real_size);
	system("pause");
	return 0;
}

void Sort(Index* IndexStrs, const int N_strings) { // функция сортировки пузырьком
	int time1;
	int time2;
	if (N_strings > 1) // сортировка только если в массиве есть хотя бы 2 элемента
	{
		for (int i = 0; i < N_strings - 1; i++) {
			for (int j = 0; j < N_strings - i - 1; j++) {
				time1 = ((int)IndexStrs[j].time[0] - 48) * 1000 + ((int)IndexStrs[j].time[1] - 48) * 100 +
					((int)IndexStrs[j].time[3] - 48) * 10 + (int)IndexStrs[j].time[4] - 48;
				time2 = ((int)IndexStrs[j + 1].time[0] - 48) * 1000 + ((int)IndexStrs[j + 1].time[1] - 48) * 100 +
					((int)IndexStrs[j + 1].time[3] - 48) * 10 + (int)IndexStrs[j + 1].time[4] - 48;

				if (time1 < time2) {
					swap(IndexStrs[j], IndexStrs[j + 1]);
				}
			}
		}
	}
}

int ReadTable(const char* FNAME, Data* Arr, int N_strings) {
	ifstream file(FNAME);
	char boardingTime[6]; //время посадки
	char planeModel[8]; //марка ЛА
	char number[7]; //бортовой номер
	char airport[4]; //аэродром
	bool error_find;
	int hour; //часы
	int minute; //минуты
	int count = 0;

	for (int i = 0; i < N_strings; i++) {
		error_find = false; // ошибки (пока) не найдены
		// считывание времени и проверка на корректность
		file >> boardingTime;
		hour = ((int)boardingTime[0] - 48) * 10 + ((int)boardingTime[1] - 48);
		minute = ((int)boardingTime[3] - 48) * 10 + ((int)boardingTime[4] - 48);

		if (boardingTime[2] != ':' || hour > 23 || minute > 59) {
			cout << endl << i + 1 << " cтрока:\n\tНеверный формат времени  (hh:mm)\n\n";
			error_find = true;
		}

		// считывание и проверка марки ЛА
		file >> planeModel;
		if (planeModel[2] != '-' || (int)planeModel[3] < 48 || (int)planeModel[3] > 57
			|| (int)planeModel[4] < 48 || (int)planeModel[4] > 57
			|| (int)planeModel[5] < 48 || (int)planeModel[5] > 57
			|| (int)planeModel[0] < -64 || (int)planeModel[1]<-64
			|| (int)planeModel[0] > -33 || (int)planeModel[1]>-33
			|| (int)planeModel[6]<-64 || (int)planeModel[6] > -33) {
			cout << endl << i + 1 << " cтрока:\n\tНеверный формат марки ЛА\n\n";
			error_find = true;
		}

		// считывание и проверка корректности бортового номера
		file >> number;
		if (number[1] != '-' || number[0] != 'Б' || (int)number[2] < 48 || (int)number[2] > 57
			|| (int)number[3] < 48 || (int)number[3] > 57
			|| (int)number[4] < 48 || (int)number[4] > 57
			|| (int)number[5] < 48 || (int)number[5] > 57) {
			cout << endl << i + 1 << " cтрока:\n\tНеверный формат бортового номера\n\n";
			error_find = true;
		}

		// считывание и проверка корректности аэродрома
		file >> airport;
		if (airport[0] != 'А' || airport[1] != 'П'
			|| (airport[2] != '1' && airport[2] != '2' && airport[2] != '3')) {
			cout << endl << i + 1 << " cтрока:\n\tНекорректный формат номера аэродрома\n\n";
			error_find = true;
		}

		if (!error_find) { // если не найдено ошибок
			count++; // увеличиваем счетчик корректных строк
			// выделение памяти под поля структуры
			Arr->boardingTime = new char[strlen(boardingTime)];
			Arr->planeModel = new char[strlen(planeModel)];
			Arr->number = new char[strlen(number)];
			Arr->airport = new char[strlen(airport)];

			// заполнение полей структуры
			strcpy(Arr->boardingTime, boardingTime);
			strcpy(Arr->planeModel, planeModel);
			strcpy(Arr->number, number);
			strcpy(Arr->airport, airport);
			Arr++;
		}
	}
	cout << endl;
	file.close();
	return count;
}

void PrintTable(Index* IndexStrs, Data* Arr, int N_strings) {
	const char LTCorn = (char)218;  // левый верхний угол
	const char RTCorn = (char)191;  // правый верхний угол
	const char HorLine = (char)196; // горизонтальная линия 
	const char VerLine = (char)179; // вертикальная линия 
	const char Krest = (char)197;   // пересечение
	const char LTurn = (char)180;   // поворот влево
	const char RTurn = (char)195;   // поворот вправо
	const char LBCorn = (char)192;  // левый нижний угол
	const char RBCorn = (char)217;  // правый нижний угол
	const char BTurn = (char)194;   // поворот вниз
	const char TTurn = (char)193;   // поворот вверх
	const int Width = 15; // ширина ячеек таблицы
	setlocale(LC_ALL, "C"); // отключение русского языка
	cout << "\t" << LTCorn << setfill(HorLine) << setw(20)
		<< BTurn << setw(20) << BTurn
		<< setw(20) << BTurn << setw(20) << RTCorn << endl;

	cout << "\t" << VerLine << setfill(' ')
		<< setw(19) << right << "Boarding time" << VerLine
		<< setw(19) << right << "Plane model" << VerLine
		<< setw(19) << right << "Board number" << VerLine
		<< setw(19) << right << "Airport" << VerLine << endl;

	for (int j = 1; j <= 3; j++) { // цикл по количеству аэродромов	
		for (int i = 0; i < N_strings; i++) {
			if ((int)Arr[IndexStrs[i].id].airport[2] - 48 == j) {
				cout << "\t" << RTurn << setfill(HorLine)
					<< setw(20) << Krest << setw(20)
					<< Krest << setw(20) << Krest << setw(20)
					<< LTurn << endl;

				cout << "\t" << VerLine << setfill(' ') << setw(19)
					<< right;
				setlocale(LC_ALL, "ru");
				cout << Arr[IndexStrs[i].id].boardingTime;
				setlocale(LC_ALL, "C");
				cout << VerLine << setw(19) << right;
				setlocale(LC_ALL, "ru");
				cout << Arr[IndexStrs[i].id].planeModel;
				setlocale(LC_ALL, "C");
				cout << VerLine << setw(19) << right;
				setlocale(LC_ALL, "ru");
				cout << Arr[IndexStrs[i].id].number;
				setlocale(LC_ALL, "C");
				cout << VerLine << setw(19) << right;
				setlocale(LC_ALL, "ru");
				cout << Arr[IndexStrs[i].id].airport;
				setlocale(LC_ALL, "C");
				cout << VerLine << endl;
			}
		}
	}

	// вывод конца таблицы
	cout << "\t" << LBCorn << setfill(HorLine)
		<< setw(20) << TTurn << setw(20) << TTurn
		<< setw(20) << TTurn << setw(20)
		<< RBCorn << endl;
}

int ReadFile(const char* FNAME) {
	setlocale(LC_ALL, "ru");

	ifstream file(FNAME);
	int count = 0; // счетчик строк в файле
	char c[MaxLen]; // массив для считывания строки

	// файл не найден
	if (!file)
	{
		cout << "Файл " << FNAME << " не найден\n";
		file.close(); // закрыть файл
		return 0; // выход по ошибке
	}

	if (file.peek() == EOF) // проверка на пустой файл
	{
		cout << "Файл \"" << FNAME << "\" пуст." << endl;
		file.close();
		return 0;
	}

	// пока не конец файла, стичываем сроки и подсчитываем их количество
	while (file.peek() != EOF) {
		file.getline(c, MaxLen);
		cout << c << endl; // печать считанной чтроки
		count++;
	}
	file.close();
	return count;
}