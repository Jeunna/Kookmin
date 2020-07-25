#include "pch.h"
#include "ProductList.h"

ProductList* ProductList::productList = NULL;

ProductList* ProductList::getInstatnce()
{
	if (!productList) {
		productList = new ProductList();
	}
	return productList;
}

ProductList::ProductList()
{
	init();
	get_list();
}

ProductList::~ProductList()
{
	delete &product_list;
}

void ProductList::init()
{
	product_list.clear();
	products.clear();
}

void ProductList::get_list()
{
	string buffer;
	ifstream file("product_list.txt");
	if (file.is_open()) {
		while (!file.eof())
		{
			try {
				getline(file, buffer);
				istringstream str_stream(buffer);
				vector<string> product_info;
				while (getline(str_stream, buffer, ' ')) {
					product_info.push_back(buffer);
				}
				//struct Item item;

				//item.content = product_info.at(1);
				//item.made_in = product_info.at(2);
				//item.date = product_info.at(3);
				//item.price = stoi(product_info.at(4));
				//item.stock = stoi(product_info.at(5));

				Product *product = new Product(product_info.at(0), product_info.at(1), product_info.at(2), product_info.at(3), stoi(product_info.at(4)), stoi(product_info.at(5)));
				products.insert(pair<string, Product*>(product->name, product));

				/*product_list[product_info.at(0)] = item;*/

			}
			catch (exception& e) {
				/*cout << e.what() << endl;*/
			}

		}
		file.close();
	}
	else {
		cout << "the item list is not exist" << endl;
	}
}

void ProductList::print_list()
{
	map<string, Product*>::iterator iter;

	cout << "##############################################" << endl;
	cout << "ITEM\tCONTENT\tMADE IN\tDATE\tPRICE\tSTOCK" << endl;
	for (iter = products.begin(); iter != products.end(); iter++) {
		cout << (*iter).second->name << "\t"
			<< (*iter).second->content << "\t"
			<< (*iter).second->made_in << "\t"
			<< (*iter).second->date << "\t"
			<< (*iter).second->price << "\t"
			<< (*iter).second->stock << endl;
	}
	cout << "\ntotal: " << products.size() << "\n";
	cout << "##############################################\n" << endl;
}

// Item print
//void ProductList::print_list()
//{
//	map<string, Item>::iterator iter;
//	
//	cout << "##############################################" << endl;
//	cout << "ITEM\tCONTENT\tMADE IN\tDATE\tPRICE\tSTOCK" << endl;
//	for (iter = product_list.begin(); iter != product_list.end(); iter++) {
//		cout << iter->first << "\t"
//			<< iter->second.content << "\t"
//			<< iter->second.made_in << "\t"
//			<< iter->second.date << "\t"
//			<< iter->second.price << "\t"
//			<< iter->second.stock << endl;
//	}
//	cout << "\ntotal: " << product_list.size() << "\n";
//	cout << "##############################################\n" << endl;
//}

void ProductList::addProduct(const vector<string> & product_info)
{
	Product *product = new Product(product_info.at(0), product_info.at(1), product_info.at(2), product_info.at(3), stoi(product_info.at(4)), stoi(product_info.at(5)));
	products.insert(pair<string, Product*>(product->name, product));

	cout << "added: " << product->name << endl;
}

void ProductList::deleteProduct(const string & product_name)
{
	cout << "정말 삭제하시겠습니까? (y/n)" << endl;

	string confirm;
	cin >> confirm;

	if (!confirm.compare("y") || !confirm.compare("Y")) {
		products.erase(product_name);
		cout << "deleted: " << product_name << endl;
	}
	else {
		cout << "취소함" << endl;
	}

	cin.clear();
	cin.ignore(1000, '\n');
}

void ProductList::update_info(string &_name, string &_info, int _which)
{
	if (products.count(_name)) {

		// 0: name, 1: content, 2: made_in, 3: date, 4: price, 5: stock
		switch (_which)
		{
		case 0:
			products.find(_name)->second->name = _info;
			products.insert(pair<string, Product*>(_info, products.find(_name)->second));
			products.erase(_name);
			cout << "updated: " << _name << "'s name, " << _name << " -> " << _info << endl;
			break;
		case 1:
			cout << "updated: " << _name << "'s content, " << products[_name]->content << " -> " << _info << endl;
			products.find(_name)->second->content = _info;
			break;
		case 2:
			cout << "updated: " << _name << "'s made_in, " << products[_name]->made_in << " -> " << _info << endl;
			products.find(_name)->second->made_in = _info;
			break;
		case 3:
			cout << "updated: " << _name << "'s date, " << products[_name]->date << " -> " << _info << endl;
			products.find(_name)->second->date = _info;
			break;
		case 4:
			cout << "updated: " << _name << "'s price, " << products[_name]->price << " -> " << _info << endl;
			products.find(_name)->second->price = stoi(_info);
			break;
		case 5:
			cout << "updated: " << _name << "'s stock, " << products[_name]->stock << " -> " << _info << endl;
			products.find(_name)->second->stock = stoi(_info);
			break;
		default:
			break;
		}
	}
	else {
		print_no_item(_name);
	}
}

void ProductList::update_info(char *_name, char *_info, int _which)
{
	string name(_name);
	string info(_info);

	if (products.count(name)) {

		// 0: name, 1: content, 2: made_in, 3: date, 4: price, 5: stock
		switch (_which)
		{
		case 0:
			products[info] = products[name];
			products.erase(name);
			cout << "updated: " << name << "'s name, " << name << " -> " << info << endl;
			break;
		case 1:
			cout << "updated: " << name << "'s content, " << products[name]->content << " -> " << info << endl;
			products[name]->content = info;
			break;
		case 2:
			cout << "updated: " << name << "'s made_in, " << products[name]->made_in << " -> " << info << endl;
			products[name]->made_in = info;
			break;
		case 3:
			cout << "updated: " << name << "'s date, " << products[name]->date << " -> " << info << endl;
			products[name]->date = info;
			break;
		case 4:
			cout << "updated: " << name << "'s price, " << products[name]->price << " -> " << info << endl;
			products[name]->price = stoi(info);
			break;
		case 5:
			cout << "updated: " << name << "'s stock, " << products[name]->stock << " -> " << info << endl;
			products[name]->stock = stoi(info);
			break;
		default:
			break;
		}
	}
	else {
		print_no_item(_name);
	}
}

//void ProductList::update_info(const char *_name, const char *_info, int _which)
//{
//	string name(_name);
//	string info(_info);
//
//	if (product_list.count(name)) {
//
//		// 0: name, 1: content, 2: made_in, 3: date, 4: price, 5: stock
//		switch (_which)
//		{
//		case 0:
//			product_list[info] = product_list[name];
//			product_list.erase(name);
//			cout << "updated: " << name << "'s name, " << name << " -> " << info << endl;
//			break;
//		case 1:
//			cout << "updated: " << name << "'s content, " << product_list[name].content << " -> " << info << endl;
//			product_list[name].content = info;
//			break;
//		case 2:
//			cout << "updated: " << name << "'s made_in, " << product_list[name].made_in << " -> " << info << endl;
//			product_list[name].made_in = info;
//			break;
//		case 3:
//			cout << "updated: " << name << "'s date, " << product_list[name].date << " -> " << info << endl;
//			product_list[name].date = info;
//			break;
//		case 4:
//			cout << "updated: " << name << "'s price, " << product_list[name].price << " -> " << info << endl;
//			product_list[name].price = stoi(info);
//			break;
//		case 5:
//			cout << "updated: " << name << "'s stock, " << product_list[name].stock << " -> " << info << endl;
//			product_list[name].stock = stoi(info);
//			break;
//		default:
//			break;
//		}
//	}
//	else {
//		print_no_item(_name);
//	}
//}

int ProductList::get_price(const char *_name) {
	string name(_name);

	// product_list.find(name) != product_list.end()
	if (products.count(name)) {
		return products[name]->price;
	}
	else {
		print_no_item(_name);
		return 0;
	}
	return 0;
}

//int ProductList::get_price(const char *_name) {
//	string name(_name);
//
//	// product_list.find(name) != product_list.end()
//	if (product_list.count(name)) {
//		return product_list[name].price;
//	}
//	else {
//		print_no_item(_name);
//		return 0;
//	}
//	return 0;
//}

int ProductList::get_price(string &_name)
{
	if (products.count(_name)) {
		return products[_name]->price;
	}
	else {
		print_no_item(_name);
		return 0;
	}
	return 0;
}

void ProductList::get_info(const char * _name)
{
	string name(_name);

	// product_list.find(name) != product_list.end()
	if (products.count(name)) {
		print_item(name);
	}
	else {
		print_no_item(_name);
	}

}

void ProductList::get_info(string & _name)
{

	if (products.count(_name)) {
		print_item(_name);
	}
	else {
		print_no_item(_name);
	}
}

int ProductList::get_stock(string & _name)
{
	if (products.count(_name)) {
		return products[_name]->stock;
	}
	else {
		print_no_item(_name);
		return 0;
	}
	return 0;
}

//int ProductList::get_price(string &_name)
//{
//	if (product_list.count(_name)) {
//		return product_list[_name].price;
//	}
//	else {
//		print_no_item(_name);
//		return 0;
//	}
//	return 0;
//}

void ProductList::print_no_item(const char * _name)
{
	cout << "there's no \"" << _name << "\" in the warehouse" << endl;
}

void ProductList::print_no_item(const string &_name)
{
	cout << "there's no \"" << _name << "\" in the warehouse" << endl;
}

void ProductList::print_item(const string &_name)
{
	cout << "##############################################" << endl;
	cout << "ITEM\tCONTENT\tMADE IN\tDATE\tPRICE\tSTOCK" << endl;
	cout << products[_name]->name << "\t"
		<< products[_name]->content << "\t"
		<< products[_name]->made_in << "\t"
		<< products[_name]->date << "\t"
		<< products[_name]->price << "\t"
		<< products[_name]->stock << endl;
	cout << "##############################################\n" << endl;
}

void ProductList::update_file()
{
	//app은 보존하고 끝에 쓰기
	//out은 있으면 다 지우고 쓰기
	ofstream file("product_list.txt", ios::out);

	map<string, Product*>::iterator iter;

	if (file.is_open()) {
		for (iter = products.begin(); iter != products.end(); iter++) {
			file << (*iter).second->name << " " <<
				(*iter).second->content << " " <<
				(*iter).second->made_in << " " <<
				(*iter).second->date << " " <<
				(*iter).second->price << " " <<
				(*iter).second->stock << "\n";
		}
		file.close();
	}

}

//void ProductList::update_file()
//{
//	//app은 보존하고 끝에 쓰기
//	//out은 있으면 다 지우고 쓰기
//	ofstream file("product_list.txt", ios::out);
//
//	map<string, Item>::iterator iter;
//
//	if (file.is_open()) {
//		for (iter = product_list.begin(); iter != product_list.end(); iter++) {
//			file << iter->first << " " <<
//				iter->second.content << " " <<
//				iter->second.made_in << " " <<
//				iter->second.date << " " <<
//				iter->second.price << " " <<
//				iter->second.stock << "\n";
//		}
//		file.close();
//	}
//
//}
