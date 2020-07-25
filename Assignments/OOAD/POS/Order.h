#pragma once
#include "pch.h"
#include "ProductList.h"
#include "Sale.h"

class Order
{
public:
	enum sale_state {
		show,
		enter,
		cancle,
		check,
		pay,
		quit,
		tax
	};

	ProductList *productList;
	Sale *currentSale;

	int today_total = 0;
	//bool tax;
	//string user;
	/*string today_date;*/

	/*Order(ProductList *items);*/
	Order();
	~Order();

	void start();
	void sale();
	void saleItems();

	void makeSale();
	void enterItem();
	void deleteItem();
	void todayTotal();
	void printItems();
	void makePayment();

	void calculateTax();
	void updateStock();
};

