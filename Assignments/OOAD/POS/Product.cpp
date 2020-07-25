#include "pch.h"
#include "Product.h"

Product::Product(string name, string content, string made_in, string date, int price, int stock)
{
	this->name = name;
	this->content = content;
	this->made_in = made_in;
	this->date = date;
	this->price = price;
	this->stock = stock;
}

Product::~Product()
{
}
