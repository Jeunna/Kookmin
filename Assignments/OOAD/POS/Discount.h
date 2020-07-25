#pragma once
class Discount
{
public:
	Discount();
	~Discount();

	int discount;
	bool isDiscount = false;
	string discount_method;

	void vaildateDiscount(const string &method);
	int calculateDiscount(int num);
};

