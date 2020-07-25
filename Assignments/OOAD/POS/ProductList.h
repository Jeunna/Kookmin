#pragma once
#include "Product.h"

class ProductList
{
private:
	static ProductList* productList;
	ProductList();
	~ProductList();

public:
	static ProductList* getInstatnce();

	struct Item
	{
		/*string name;*/
		string content;
		string made_in;
		string date; //expiration date
		int price;
		int stock;
	};

	// {key: name, value: item_information}
	map<string, Item> product_list;
	map<string, Product*> products;

	/*ProductList(const ProductList &itemList);*/

	void init();
	void get_list();
	void print_list();

	void addProduct(const vector<string> &product_info);
	void deleteProduct(const string & product_name);

	void update_info(string &_name, string &_info, int _which);
	void update_info(char *_name, char *_info, int _which);
	int get_price(const char *_name);
	int get_price(string &_name);
	void get_info(const char *_name);
	void get_info(string &_name);
	int get_stock(string &_name);

	void print_no_item(const char *_name);
	void print_no_item(const string &_name);
	void print_item(const string &_name);

	void update_file();
};

