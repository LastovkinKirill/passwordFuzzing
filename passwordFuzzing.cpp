// file of user's athentication 
#include <iostream>
#include <time.h> 
#include <Windows.h>
#include <fstream>
using namespace std;

inline int length(char* str);
bool authenticate(char* username, char* password);
void _fastcall mesFunClock(char loginUsername[20], char loginPassword[20], int N);
long _fastcall mesFunQueryPerf(char loginUsername[20], char loginPassword[20], int N);
long _fastcall mesFunQueryThread(char loginUsername[20], char loginPassword[20], int N);
void  mesFunThreadTimes(char loginUsername[20], char loginPassword[20], int N);

char findFirstLetter();
void crackAuth();



int main()
{
	char loginUsername[20] = "k";
	char loginPassword[20] = "";
	
	/*
	cout << "Please enter your details to log in \n";
	cout << "Username: ";
	cin >> loginUsername;
	cout << "\nPassword: ";
	cin >> loginPassword;
	*/

	//mesFunClock(loginPassword,loginUsername, 10000);			// тест функции clock()
	//mesFunQueryPerf(loginUsername, loginPassword, 100);	// тест функции QueryPerformanceCounter()
	//mesFunQueryThread(loginUsername, loginPassword, 100000);	// тест функции QueryThreadCycleTim()
	mesFunThreadTimes(loginUsername, loginPassword, 100000);
	//findFirstLetter();
	//crackAuth();
	return 0;
}



void _fastcall mesFunClock(char loginUsername[20],char loginPassword[20], int N)
	{
		// замеры с помощью clock()
		// не видит нечего в тактах, а в секундах ерунда какая-то
		clock_t start, finish;	
		long summCpuTime = 0;
		double summTimeSec = 0;

		for (int i = 0; i < N; i++)
		{
		start = clock();

		authenticate(loginUsername, loginPassword);

		finish = clock();

		long cpuTime = finish - start;
		double timeSec = static_cast<double>(finish - start) / CLOCKS_PER_SEC;
		summCpuTime += cpuTime;
		summTimeSec += timeSec;
		}
		long averageCpu = summCpuTime / N;
		double averageTSec = summTimeSec / N;
		cout << "Clock():\n" << " average CPU times: " << averageCpu << "\n"; // среднее при замерах
		cout << " average Sec. times: " << averageTSec << "\n"; // среднее при замерах
    }

long _fastcall mesFunQueryPerf(char loginUsername[20], char loginPassword[20], int N)
{
	// замеры с помощью QueryPerformanceFrequency()
	// есть результаты
	LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;
	long summ = 0;

	//for (int i = 0; i < N; i++)
	//{
		//QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&StartingTime);
		for (int i = 0; i < N; i++)
		{
			authenticate(loginUsername, loginPassword);
		}
		QueryPerformanceCounter(&EndingTime);
		//ElapsedMicroseconds = EndingTime.QuadPart - StartingTime.QuadPart;

		//ElapsedMicroseconds.QuadPart *= 1000000;
		//ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

		//cout << ElapsedMicroseconds.QuadPart<<'\n';
		summ += EndingTime.QuadPart - StartingTime.QuadPart;
	
	long average = summ ;
	return summ;
	cout<<"QueryPerformance...():\n"<<"	average CPU times: "<< average << "\n"; // среднее при замерах
}

long _fastcall mesFunQueryThread(char loginUsername[20], char loginPassword[20], int N)
{	
	// почему то если N=1000000 порядок падает, теперь уже не падает почему то
	// убрал последний символ у пароля минус получился 
	// при 1млн одну не сответсвтие первой буквы очень близко распознает

	// БУКВЫ КОТОРЫЕ НЕ ПОДХОДЯТ ДАЮТ ОКОЛО 1400, А БУКВА ПОДХОДЯЩАЯ ОКОЛО 1500
	uint64_t tstart, tend;
	unsigned long summ = 0;
	
	for (int i = 0; i < N; i++)
	{
		QueryThreadCycleTime(GetCurrentThread(), &tstart);

		authenticate(loginUsername, loginPassword);

		QueryThreadCycleTime(GetCurrentThread(), &tend);

		//cout << (tend - tstart) << '\n';

		summ += (tend - tstart);

	}
	long average = summ / N;
	//cout<<"QueryThreadCycleTime:\n	average CPU times: "<< average << '\n';

	return average;
}

void  mesFunThreadTimes(char loginUsername[20], char loginPassword[20], int N)

{
	FILETIME lpCreationTimeStart, lpCreationTimeEnd;
	FILETIME lpExitTimeStart, lpExitTimeEnd;
	FILETIME lpKernelTimeStart, lpKernelTimeEnd;
	FILETIME lpUserTimeStart, lpUserTimeEnd;

	

	GetThreadTimes(GetCurrentThread(),
		&lpCreationTimeStart,
		&lpExitTimeStart,
		&lpKernelTimeStart,
		&lpUserTimeStart);
	
	authenticate(loginUsername, loginPassword);

	GetThreadTimes(GetCurrentThread(),
		&lpCreationTimeEnd,
		&lpExitTimeEnd,
		&lpKernelTimeEnd,
		&lpUserTimeEnd);
	

	//unsigned long usertime = (unsigned long)&lpUserTimeStart - (unsigned long)&lpUserTimeEnd;
	unsigned long usertime = (unsigned long)&lpCreationTimeStart - (unsigned long)&lpCreationTimeEnd;

	//cout << (unsigned long)&lpUserTimeEnd << endl;

	cout << (unsigned long)&lpCreationTimeStart << endl;
	cout << (unsigned long)&lpExitTimeStart << endl;
	cout << (unsigned long)&lpKernelTimeStart << endl;
	cout << (unsigned long)&lpUserTimeStart << endl;
	cout << endl;
	cout << (unsigned long)&lpCreationTimeEnd << endl;
	cout << (unsigned long)&lpExitTimeEnd << endl;
	cout << (unsigned long)&lpKernelTimeEnd << endl;
	cout << (unsigned long)&lpUserTimeEnd << endl;


	cout << "usertime: "<< usertime << endl;
}

char findFirstLetter()
{
	char loginPassword[20] = "kirill12345";
	char alphabet[27] = "abcdefghijklmnopqrstuvwxyz";
	char letter[2];
	int summ = 0;
	int N = 100;
	struct correctLetter
	{
		char letter;
		long cpuTime = 0;
	};
	correctLetter cletter;

	letter[1] = '\0';
	Sleep(2000);
	for (int i = 0; i < 26; i++)
	{
		summ = 0;
		letter[0] = alphabet[i];
		//long averageCpUtime = mesFunQueryThread(letter, loginPassword, N);
		long averageCpUtime = mesFunQueryPerf(letter, loginPassword, N);

		if (cletter.cpuTime < averageCpUtime)
		{
			cletter.cpuTime = averageCpUtime;
			cletter.letter = letter[0];
		}
		
		cout << letter << ": " << averageCpUtime << '\n';
	}
	cout << cletter.letter;
	cout << cletter.cpuTime;
	return cletter.letter;
}

void crackAuth()
{
	char UNandPASSstring[50]; //mass where username and password chars

	char alphabet[27] = "abcdefghijklmnopqrstuvwxyz";
	int summ = 0;
	int N = 1000000;
	long crackCpuTime = 0;  // for chack one correcr string ans END of string
	long letterCpuTime = 0; // for chack one char
	char letter = ' ';
	bool usernameDone = false;
	struct UsrnAndPass
	{
		char username[20] = "";
		char password[20] = "";
	};

	UsrnAndPass correctStr;
	for (int nChar = 0; nChar < 20; nChar++)
	{
		crackCpuTime = letterCpuTime;

		for (int i = 0; i < 26; i++)
		{
			if (usernameDone)
			{
				nChar = 0;
				correctStr.password[nChar] = alphabet[i];
				correctStr.password[nChar + 1] = '\0';
			}
			else
			{
				correctStr.username[nChar] = alphabet[i];
				correctStr.username[nChar + 1] = '\0';
			}

			summ = 0;
			long averageCpUtime = mesFunQueryThread(correctStr.username, correctStr.password, N);

			if (letterCpuTime < averageCpUtime)
			{
				letterCpuTime = averageCpUtime;
				letter = alphabet[i];
			}
		}
		if (usernameDone)
			correctStr.username[nChar] = letter;
		else
			correctStr.password[nChar] = letter;

		if (letterCpuTime == crackCpuTime)
			usernameDone = true;

		cout << correctStr.username << endl;
		cout << correctStr.password << endl;
	}
}
// в проверке участвует символ конца строки, что помогает, избежать ошибки переполнения массива
// а также если введен пароль длиннее оригинала 
bool authenticate(char* username, char* password)
{	
	char* correctUsername = "kirill";
	char* correctPassword = "kirill12345";

	for (int i = 0; i < length(correctUsername) + 1; i++)
		if (correctUsername[i] != username[i])
			return false;

	for (int i = 0; i < length(correctPassword) + 1; i++)
		if (correctPassword[i] != password[i])
			return false;
	
	return true;
}

inline int length(char* str)
{
	char* pos = str;
	while (*pos != 0) pos++;
	int lenght = pos - str;
	return lenght;
}
