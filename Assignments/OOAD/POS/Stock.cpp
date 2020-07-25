#include "pch.h"
#include "Stock.h"
#include "ProductList.h"

Stock::Stock()
{
	menu();
}

Stock::~Stock()
{
}

void Stock::menu() {
	cout <<
		"\n" <<
		"��\t��\t��\t��\t�� - ������ ���\n" <<
		"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n" <<
		"0. �޴� �ٽ� ����\n" <<
		"1. ��ü ��� ��ȸ\n" <<
		"2. ��ǰ ���� ��ȸ (�����̸�)\n" <<
		"3. ��ǰ �߰� (�����̸� ������)\n" <<
		"4. ��ǰ ���� (�����̸�)\n" <<
		"5. ��ǰ ���� ���� (�����̸�, �ٲ� �׸�, �ٲ� ����)\n" <<
		"6. ��� ���� ����\n" <<
		"7. ����\n\n" <<
		"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n" <<
		endl;
}

void Stock::getProductList()
{
	ProductList *productList;
	productList = ProductList::getInstatnce();

	/*productList->get_list();*/
	productList->print_list();
}

void Stock::findProduct()
{
	ProductList *productList;
	productList = ProductList::getInstatnce();

	cout << "�����̸�: ";

	string input;
	getline(cin, input);

	productList->get_info(input);
}

void Stock::addProduct()
{
	ProductList *productList;
	productList = ProductList::getInstatnce();

	cout << "�Է��ϱ�: ��ǰ �̸�, ��ǰ ����, ������ ��, �������, ����, ����" << endl;
	string input;
	getline(cin, input);

	vector<string> product_info;
	istringstream str_stream(input);
	while (getline(str_stream, input, ' ')) {
		product_info.push_back(input);
	}

	productList->addProduct(product_info);
}

void Stock::deleteProduct()
{
	ProductList *productList;
	productList = ProductList::getInstatnce();

	cout << "�����ϱ�: ��ǰ �̸�" << endl;
	string input;
	getline(cin, input);

	vector<string> product_info;
	istringstream str_stream(input);
	while (getline(str_stream, input, ' ')) {
		product_info.push_back(input);
	}

	productList->deleteProduct(product_info.at(0));
}

void Stock::modifyProduct()
{
	ProductList *productList;
	productList = ProductList::getInstatnce();

	string name, info, which;
	cout << "�����ϱ�" << endl;
	cout << "��ǰ �̸� : ";
	cin >> name;

	cout << "������ �׸� : ";
	cout << "�̸�:0, ����:1, ������ ��:2, �������:3, ����:4, ����:5" << endl;
	cin >> which;
	cout << "������ ���� : ";
	cin >> info;

	productList->update_info(name, info, stoi(which));

	cin.clear();
	cin.ignore(1000, '\n');
}

void Stock::saveProductList()
{
	ProductList *productList;
	productList = ProductList::getInstatnce();

	productList->update_file();

	cout << "����Ǿ����ϴ�." << endl;
}

