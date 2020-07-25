#include "pch.h"
#include "Point.h"


Point::Point()
{
}


Point::~Point()
{
}

void Point::savePoint(int total)
{
	int _point = calculatePoint(total);
	point += _point;
}

void Point::usePoint(int num)
{
	string number;
	string user = validdateUser(number);

	point -= num;
}

string Point::validdateUser(const string &_number)
{
	string buffer;
	vector<string> txt;
	user = "NA";

	try {
		ifstream file("point.txt");

		if (file.is_open()) {
			while (!file.eof()) {
				getline(file, buffer);
				istringstream str_stream(buffer);
				while (getline(str_stream, buffer, ' '))
				{
					txt.push_back(buffer);
				}

				if (!_number.compare(txt[1])) {
					user = txt[0];
					number = txt[1];
					point = stoi(txt[2]);
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

	return user;
}

int Point::calculatePoint(int total)
{
	int percent = 1;

	return floor(total * percent / 100);
}