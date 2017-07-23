#include <Windows.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <Wininet.h>
#include <tchar.h>
#include <time.h>
#include <ctime>
#include <iomanip>
#include <istream>
#include <sstream>
#include <fstream>

#pragma comment (lib, "wininet.lib")

using namespace std;

//reverse characters
void reverse(char s[]) {
	int c, i, j;
	for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

int main() {
	while (1) {
		//time
		time_t timev;

		time(&timev);
		string value;


		HINTERNET connect = InternetOpen(_T("MyBrowser"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

		if (!connect) {
			cout << "connection failed or syntax error";
			return 0;
		}

		HINTERNET OpenAddress1 = InternetOpenUrl(connect,
			_T("http://query.yahooapis.com/v1/public/yql?q=")
			_T("select%20Rate%20from%20yahoo.finance.xchange%20where%20pair%20in%20(%22GBPUSD%22)")
			_T("&format=json&env=store://datatables.org/alltableswithkeys&callback="),
			NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION
			| INTERNET_FLAG_SECURE, 0);
		/*
		HINTERNET OpenAddress1 = InternetOpenUrl(connect,
		_T("https://api.instagram.com/v1/tags/london/media/recent?access_token=1138136632.1677ed0.d6f3a34dab8a431cbeb604ac93a6d13b"),
		NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE|INTERNET_FLAG_KEEP_CONNECTION,0);
		*/
		if (!OpenAddress1) {
			DWORD ErrorNum = GetLastError();
			cout << "Failed to open URL \nError No: " << ErrorNum;
			InternetCloseHandle(connect);
			return 0;
		}

		char DataReceived1[4096] = { '\0' };
		char DataReceived2[4096] = { '\0' };
		DWORD NumberOfBytesRead = 0;

		while (InternetReadFile(OpenAddress1, DataReceived1, 4096, &NumberOfBytesRead) && NumberOfBytesRead) {
			//cout << DataReceived1 << endl;
		}

		string data = string(DataReceived1);
		//	cout << data << endl;


		reverse(DataReceived1);
		char * pch;
		pch = strchr(DataReceived1, '\"');
		int start = pch - DataReceived1;
		pch = strchr(pch + 1, '\"');
		int end = pch - DataReceived1;
		for (int ii = 0, jj = end - 1; jj > start; ii++, jj--)
			DataReceived2[ii] = DataReceived1[jj];
		double n = atof(DataReceived2);

		//cout << "GBP to USD: " << n << " at: " << ctime(&timev) << endl;


		/*
		istringstream iss(DataReceived1);

		for (int i = 0; i < 8; i++){
		getline(iss, value, '}');
		cout << value << endl;

		}
		cout << value << endl; //" "<< ctime(&timev) << endl;
		*/


		InternetCloseHandle(OpenAddress1);
		InternetCloseHandle(connect);
		Sleep(3000);

	}
	system("PAUSE");

}
