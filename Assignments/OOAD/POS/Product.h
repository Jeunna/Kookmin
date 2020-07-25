#pragma once
class Product
{
public:
	string name = "";
	string content = "";
	string made_in = "";
	string date = ""; //expiration date
	int price = 0;
	int stock = 0;

	Product(
		string name,
		string content,
		string made_in,
		string date, //expiration date
		int price,
		int stock
	);
	~Product();
};

