#pragma once
#include "Payment.h"

class Sale
{
public:
	struct Item
	{
		int count;
		int price;
	};
	map<string, Item> sale_items;
	map<string, int> paying;
	bool isComplete = false;
	bool isTax = false;
	int total = 0;
	string today_date;
	Payment *payment;
	Log *log;

	Sale();
	~Sale();

	void addItem(const string &_item, int _num, int _price);
	void deleteItem(const string &_item);
	bool IsCompelete();
	void makePayment();
	void printItems();
	string currentDate();
	void plusTax();

	string saveSale();
	void logging();
	void updateStock();
};

