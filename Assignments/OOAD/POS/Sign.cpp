#include "pch.h"
#include "Sign.h"


Sign::Sign()
{
}


Sign::~Sign()
{
}

string Sign::checkInfo(string id, string password) {
	string buffer;
	vector<string> txt;
	string status = "NA";

	try {
		ifstream file("user_info.txt");

		if (file.is_open()) {
			while (!file.eof()) {
				getline(file, buffer);
				istringstream str_stream(buffer);
				while (getline(str_stream, buffer, ' '))
				{
					txt.push_back(buffer);
				}

				if (!id.compare(txt[0])) {
					status = "id_exits";
					if (!password.compare(txt[1])) status = txt[2];
					break;
				}
				else txt.clear();
			}
			file.close();
		}

	}
	catch (exception e) {
		cout << e.what() << endl;
	}

	return status;
}


string Sign::signIn() {
	cout << "----- 로그인 -----" << endl;

	string id, pw;

	cout << "id: ";
	cin >> id;

	cout << "password: ";
	cin >> pw;

	string status = checkInfo(id, pw);

	if (!status.compare("id_exits")) {
		status = "wrong";
		cout << id << ": wrong password" << endl;
	}
	else if (!status.compare("NA")) {
		status = "not exits";
		cout << id << ": not exits" << endl;
	}

	cout << endl;

	return status;
}

void Sign::signUp() {
	cout << "----- 가 입 -----" << endl;

	string id, pw, mode;

	cout << "id: ";
	cin >> id;

	cout << "password: ";
	cin >> pw;

	cout << "mode: ";
	cin >> mode;

	string status = checkInfo(id, pw);

	if (!status.compare("id_exits")) {
		cout << "ID exits" << endl;
	}
	else if (!status.compare("NA")) {
		ofstream file("user_info.txt", ios::app);
		if (file.is_open()) {
			file << id << " " << password << " " << mode << endl;
		}
		file.close();
		cout << "saved: " << id << ": " << mode << endl;
	}

	cout << endl;
}