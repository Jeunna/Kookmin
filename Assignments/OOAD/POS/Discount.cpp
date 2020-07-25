#include "pch.h"
#include "Discount.h"


Discount::Discount()
{
}


Discount::~Discount()
{
}

void Discount::vaildateDiscount(const string &method)
{
	string buffer;
	vector<string> txt;
	discount_method = "NA";

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

				if (!method.compare(txt[0])) {
					discount_method = txt[0];
					discount = stoi(txt[1]);
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
}

int Discount::calculateDiscount(int num)
{
	isDiscount = true;
	return floor(num * discount / 100);
}
