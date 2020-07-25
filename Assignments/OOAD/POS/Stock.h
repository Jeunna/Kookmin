#pragma once
class Stock
{
public:
	Stock();
	~Stock();

	void getProductList();
	void findProduct();
	void addProduct();
	void deleteProduct();
	void modifyProduct();
	void saveProductList();

	void menu();
};

