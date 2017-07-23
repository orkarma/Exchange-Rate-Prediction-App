//#include "print.h"
#include "windows.h"
#include "iostream"
#include <string>
#include "tchar.h"
#include "wininet.h"
#include "stdlib.h"
#include <vector>
#include <fstream>
#include <list>
#include <iterator>
#include <algorithm>
#include <sstream>
//#include <stdafx.h>
#pragma comment (lib,"wininet.lib")
using namespace std;

/*
void save() {
	string data = "hello";
	ofstream outfile;
	outfile.open("filename.txt");
	// output file is created if it doesn't exist
	//but what if the disk is full, for example?
	if (!outfile)
	{
		cout << "Cannot open output file\n";
		exit(1);
	}
	outfile << data << endl;
	outfile.close();
	//system("pause");

	//return 0;
}

void load() {
	string dataRead;
	string::size_type sz;
	ifstream infile;

	double mmm;
	infile.open("dataFinal.txt");
	if (dataRead.is_open()) {

		while (!infile.eof())
		{
			infile >> dataRead ;
			mmm = stod(dataRead &sz);
			cout << mmm << '\n';
		}
	}
	infile.close();
}

*/

::vector<std::string> splitString(const std::string &s, char delim) {
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

int dataRead(std::vector<double> & rates)
{
	string line;
	
	ifstream inFile("Data.txt");
	if (!inFile) 
	{
		cerr << "File Data.txt not found." << endl;
		return -1;
	}
	
	std::vector<string> line2split;
	double rate;

	while (getline(inFile, line)) 
	{
		if (line.empty()) continue;
		
		line2split = splitString(line, ',');

		rate = std::stod(line2split.at(1));

		rates.push_back(rate);
	}

	inFile.close();
	return 0;
}

int GBPtoUSD(int sampleSize, int interval /*, const std::string& currency = "GBPUSD"*/)
{

	HINTERNET connect = InternetOpen(_T("MyBrowser"),
	INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (!connect)
	{
		cout << "Connection with server failed";
		system("PAUSE");
		return -1;
	}
	ofstream output_file;
	output_file.open("Data.txt");
	char DataReceived1[4096] = { '\0' };
	DWORD NumberOfBytesRead = 0;

	while ( sampleSize >= 0) {
		 
			HINTERNET OpenAddress1 = InternetOpenUrl(connect, _T("http://finance.yahoo.com/d/quotes.csv?e=.csv&f=sl1d1t1&s=GBPUSD=X")
			_T("&format=json&env=store://datatables.org/alltableswithkeys&callback="), NULL, 0,
			INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_SECURE, 0);

		if (!OpenAddress1)
		{
			DWORD ErrorNum = GetLastError();
			cout << "failed to connect to API Url \nError No: " << ErrorNum;
			InternetCloseHandle(connect);
			//system("PAUSE");
			return -1;
		}
		InternetReadFile(OpenAddress1, DataReceived1, 4096, &NumberOfBytesRead) && NumberOfBytesRead;
		{
			cout << ".";
			output_file << DataReceived1;
			sampleSize = sampleSize - 1;
		}
		InternetCloseHandle(OpenAddress1);
		Sleep(interval);
	}
	InternetCloseHandle(connect);
	output_file.close();	
	return 0;
}


void processData(const std::vector<double> & rates) 
{
	// do the processing
	double totalRate = rates.front();
	for (unsigned int i = 1; i < rates.size(); ++i) 
	{
		totalRate += rates.at(i);
	}
	cout << "sum of rates is " << totalRate << " average " << totalRate / rates.size() << "\n";
}

int main()
{
	bool menuon;
	char mainmenu;
	int sampleSize = 10; /// total sample count
	int interval = 1000; /// sample taken every x milliseconds 

	std::vector<double> rates;

	cout << "Welcome to Ibby's money rate guess app" << endl;
	
	do {
		cout << "\n[1] Define sample size" << endl;
		cout << "[2] Define sample interval (milliseconds)" << endl;
		cout << "[3] Record GBP-USD data" << endl;
		cout << "[4] predict rate based on cumalitive average" << endl;
		cout << "[5] current rate for GBP-USD" << endl;
		cin >> mainmenu;
		switch (mainmenu)
		{
		case '1':
			cout << "please input how many samples you would like to take.."<< endl;
			cin >> sampleSize;
			break;
		case '2':
			cout << "please define how often you would like to sample data" << endl;
			cin >> interval;
			break;
		case '3':
			//int result = 
			cout << "Starting data collection...." << endl;
			GBPtoUSD(sampleSize, interval);

			break;
		case '4':
			dataRead(rates);
			processData(rates);
			break;
		case '5':
			break;
		}
	} while (menuon = true);
	return 0;
}

