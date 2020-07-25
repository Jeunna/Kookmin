#include "pch.h"
#include "Sale.h"
#include "ProductList.h"

Sale::Sale()
{
}


Sale::~Sale()
{
}

void Sale::addItem(const string &_item, int _num, int _price)
{
		try {
			if (sale_items.find(_item) != sale_items.end()) {
				int count = sale_items[_item].count;
				sale_items[_item].count = _num + count;
				sale_items[_item].price = _price;
			}
			else {
				sale_items[_item].count = _num;
				sale_items[_item].price = _price;
			}
			printItems();
		}
		catch (exception e) {
			cout << e.what() << endl;
		}
}

void Sale::deleteItem(const string &_item)
{
	try {
		if (sale_items[_item].count) {
			sale_items.erase(_item);
		}
		printItems();
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
}

bool Sale::IsCompelete()
{
	return isComplete;
}


void Sale::printItems()
{
	map<string, Item>::iterator iter;

	int total = 0;
	cout << "------------------------------\n"
		"ITEM\tNUM\tPRICE\tTOTAL\n";
	for (iter = sale_items.begin(); iter != sale_items.end(); iter++) {
		string str(iter->first);
		cout << iter->first << "\t" << iter->second.count << "\t" << (iter->second.price) << "\t" << (iter->second.count) * (iter->second.price) << "\n";
		total += (iter->second.count) * (iter->second.price);
	}
	this->total = total;
	cout << "\ntotal: " << total << "\n"
		"------------------------------\n"
		<< endl;

	//this->save_total = this->total;
}

string Sale::saveSale()
{
	map<string, Item>::iterator iter;

	string order = "";
	order = "ITEM\tNUM\tPRICE\n";
	for (iter = sale_items.begin(); iter != sale_items.end(); iter++) {
		string str(iter->first);
		order += iter->first + "\t";
		order += to_string(iter->second.count) + "\t";
		order += to_string(iter->second.price) + "\n";
	}
	order += "\ntotal: " + to_string(total);
	if (isTax) order += "\n(Tax 3.3% 포함)\n";

	return order;
}

void Sale::logging()
{
	string saved = saveSale();

	Log *log = new Log();
	log->saveTodayTotal(total);
	log->receipt(paying, saved);
	delete log;
}

void Sale::updateStock()
{
	ProductList *productList;
	productList = ProductList::getInstatnce();

	map<string, Item>::iterator iter;

	for (iter = sale_items.begin(); iter != sale_items.end(); iter++) {
		string name(iter->first);

		int stock;
		stock = productList->get_stock(name);
		stock -= iter->second.count;

		string info = to_string(stock);

		productList->update_info(name, info, 5);
	}
}


void Sale::makePayment()
{
	payment = new Payment(total);
	paying = payment->salePay();
	/*total = payment->amount;*/

	isComplete = true;
}

void Sale::plusTax()
{
	float _num;
	cout << "추가할 tax(%) : ";
	cin >> _num;

	cout << total << " -> ";
	total += round(total *_num / 100); //반올림
	cout << total << endl;
	isTax = true;

	cin.clear();
	cin.ignore(1000, '\n');
}
