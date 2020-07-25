#include "pch.h"
#include "Log.h"

Log::Log()
{
}


Log::~Log()
{
}

void Log::receipt(map<string, int> &paid, string &saled)
{
	ofstream file("receipt.txt", ios::app);

	map<string, int>::iterator iter;
	string _pay = "";
	for (iter = paid.begin(); iter != paid.end(); iter++) {
		_pay += iter->first + "(";
		_pay += to_string(iter->second) + ") ";
	}

	if (file.is_open()) {
		file << "----------------------------------" << "\n"
			<< currentDate() << "\n"
			<< saled << "\n"
			<< "결제방식: " << _pay << "\n"
			/*<< "User: " << user << "\n"*/
			<< "----------------------------------" << "\n"
			<< endl;
	}

	file.close();
}

string Log::currentDate()
{
	time_t now_time;
	char buf[256];

	time(&now_time);
	ctime_s(buf, sizeof(buf), &now_time);
	/*cout << buf << endl;*/

	string _buf(buf);
	return _buf;
}

void Log::saveTodayTotal(int total)
{
	todays += total;

	ofstream file("total.txt", ios::out);

	if (file.is_open()) {
		file << todays << " " << currentDate() << endl;
	}
	file.close();
}

void Log::getTodayTotal()
{
	string buffer;
	ifstream file("total.txt");
	if (file.is_open()) {
		while (!file.eof())
		{
			try {
				getline(file, buffer);
				istringstream str_stream(buffer);
				vector<string> info;
				while (getline(str_stream, buffer, ' ')) {
					info.push_back(buffer);
				}
				todays = stoi(info.at(0));
			}
			catch (exception e) {
				cout << e.what() << endl;
			}

		}
	}
}
