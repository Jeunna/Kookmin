#pragma once
#include "Log.h"

class Payment
{
public:
	enum state {
		show,
		card,
		cash,
		quit,
		tax
	};

	int amount;
	map<string, int> paying;
	bool isComplete = false;
	bool isTax = false;

	Payment(int total);
	~Payment();

	void pay();
	map<string, int> salePay();

	void cashPayment();
	void cardPayment();
	void plus_tax();
	void refund();
};

