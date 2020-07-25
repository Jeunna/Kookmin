#include "pch.h"
#include "Order.h"

Order::Order() {
	productList = ProductList::getInstatnce();
	start();
}

void Order::start()
{
	cout <<
		"\n" <<
		"��\t��\t��\t��\t��\n" <<
		"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n" <<
		"0. �޴� �ٽ� ����\n" <<
		"1. �������ϱ�\n" <<
		"2. ��� üũ�ϱ�\n" <<
		"3. ����\n\n" <<

		"[OPTIONAL]\n" <<
		"4. �Ѹ��� Ȯ��\n" <<
		"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n" <<
		endl;
}

void Order::sale() {
	cout <<
		"\n" <<
		"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n" <<
		"0. �޴� �ٽ� ����\n" <<
		"1. ��ǰ �Է� (�����̸� ������)\n" <<
		"2. ��ǰ ��� (�����̸�)\n" <<
		"3. �Էµ� ��ǰ Ȯ��\n" <<
		"4. �����ϱ�\n" <<
		"5. ���� ����\n\n" <<

		"[OPTIONAL]\n" <<
		"6. (������) �Ѿ׿� tax ��� (x%)\n" <<
		"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n" <<
		endl;
}

void Order::saleItems() {

	int pos = show;
	while (pos != quit) {
		try {
			cout << ">> ";

			string input;
			getline(cin, input);

			vector<string> item_info;
			istringstream str_stream(input);
			while (getline(str_stream, input, ' ')) {
				item_info.push_back(input);
			}

			pos = stoi(item_info.at(0));

			switch (pos)
			{
			case show:
				sale();
				break;
			case enter:
				enterItem();
				break;
			case cancle:
				deleteItem();
				break;
			case check:
				printItems();
				break;
			case pay:
				makePayment();
				break;
			case quit:
				break;
			case tax:
				calculateTax();
				break;
			default:
				cout << "�������� �ʴ� ����Դϴ�." << endl;
				break;
			}

			//cin.clear();
			//cin.ignore(1000, '\n');
			//item_info.clear();

			if (currentSale->isComplete == true) {
				currentSale->logging();
				today_total += currentSale->total;
				currentSale->updateStock();
				delete currentSale;

				cout << "===================================" << endl;
				cout << endl;
				start();
				break;
			}
		}
		catch (exception e) {
			cout << "������ �� ���� ����Դϴ�: " << e.what() << endl;
		}
	}
}

void Order::makeSale()
{
	currentSale = new Sale();
	sale();
	saleItems();
}

void Order::enterItem()
{
	cout << "�߰��ϱ�: ��ǰ �̸�, ����" << endl;
	string input;
	getline(cin, input);

	vector<string> product_info;
	istringstream str_stream(input);
	while (getline(str_stream, input, ' ')) {
		product_info.push_back(input);
	}

	int price;
	price = productList->get_price(product_info.at(0));
	currentSale->addItem(product_info.at(0), stoi(product_info.at(1)), price);
}

void Order::deleteItem()
{
	cout << "�����ϱ�: ��ǰ �̸�" << endl;
	string input;
	getline(cin, input);

	vector<string> product_info;
	istringstream str_stream(input);
	while (getline(str_stream, input, ' ')) {
		product_info.push_back(input);
	}

	currentSale->deleteItem(product_info.at(0));
}

void Order::printItems() {
	currentSale->printItems();
}

void Order::todayTotal()
{
	if (!today_total) {
		Log *log = new Log();
		log->getTodayTotal();
		today_total = log->todays;
	}
	
	cout << "�Ѹ���: " << today_total << endl;
}

void Order::makePayment()
{
	currentSale->makePayment();

}

void Order::calculateTax()
{
	currentSale->plusTax();
}

//void Order::plus_tax(float _num)
//{
//	cout << this->total << " -> ";
//	this->total += round(this->total *_num/100); //�ݿø�
//	cout << this->total << endl;
//	this->tax = true;
//}
//
//void Order::payment(const char *_payment, int _num)
//{
//	string pay(_payment);
//	if (!strcmp(_payment, "ī��")) {
//		//TODO: ī�� �����ϴ� func
//		cout << "ī�� ����: " << _num << "��" << endl;
//		if (this->pay.find("ī��") == this->pay.end()) {
//			this->pay["ī��"] = _num;
//		}
//		else {
//			int money = this->pay["ī��"];
//			this->pay["ī��"] += _num;
//		}
//		cout << "���� �׼�: " << total - this->pay["ī��"] << endl;
//	}
//	else if (!strcmp(_payment, "����")) {
//		//TODO: ���� �����ϴ� func
//		cout << "���� ����: " << _num << "��" << endl;
//
//		int money = 0;
//		if (total < _num) {
//			money = total;
//			cout << "�Ž�����: " << _num - total << endl;
//		}
//		else {
//			money = _num;
//			if (this->pay.find("����") == this->pay.end()) {
//				this->pay["����"] = _num;
//			}
//			else {
//				this->pay["����"] += _num;
//			}
//		}
//		cout << "���� �׼�: " << total - money << endl;
//	}
//
//	if (total == 0) {
//		receipt(pay);
//		update_stock();
//		init();
//		items->update_file();
//		today_update();
//
//		cout << "���� �Ϸ�!" << endl;
//	}
//}
//
//void Order::payment(string &_payment, int _num)
//{
//	string pay = _payment;
//	int money = 0;
//	if (!pay.compare("ī��")) {
//		//TODO: ī�� �����ϴ� func
//		cout << "ī�� ����: " << _num << "��" << endl;
//		if (this->pay.find("ī��") == this->pay.end()) {
//			this->pay["ī��"] = _num;
//		}
//		else {
//			money = this->pay["ī��"];
//			this->pay["ī��"] += _num;
//		}
//		total -= this->pay["ī��"];
//		cout << "���� �׼�: " << total << "\n";
//	}
//	else if (!pay.compare("����")) {
//		//TODO: ���� �����ϴ� func
//		cout << "���� ����: " << _num << "��" << endl;
//		
//		if (this->total < _num) {
//			money = _num - this->total;
//			cout << "�Ž�����: " << money << endl;
//			this->total = 0;
//		}
//		else {
//			money = _num;
//			if (this->pay.find("����") == this->pay.end()) {
//				this->pay["����"] = _num;
//			}
//			else {
//				this->pay["����"] += _num;
//			}
//			this->total -= money;
//		}
//		cout << "���� �׼�: " << this->total << "\n";
//	}
//
//	if (this->total == 0) {
//		receipt(pay);
//		update_stock();
//		items->update_file();
//		today_update();
//		init();
//
//		cout << "���� �Ϸ�!" << endl;
//	}
//}
//
//void Order::receipt(string payment)
//{
//
//	ofstream file("receipt.txt", ios::app);
//
//	map<string, int>::iterator iter;
//	string _pay = "";
//	for (iter = this->pay.begin(); iter != this->pay.end(); iter++) {
//		_pay += iter->first + "(";
//		_pay += to_string(iter->second) + ") ";
//	}
//
//	string order = save_order();
//	if (file.is_open()) {
//		file << "----------------------------------" << "\n"
//			<< current_date_time() << "\n"
//			<< order << "\n"
//			<< "�������: " << _pay << "\n"
//			<< "User: " << user << "\n"
//			<< "----------------------------------" << "\n"
//			<< endl;
//	}
//	file.close();
//}
//
//void Order::update_stock()
//{
//	map<string, int>::iterator iter;
//
//	for (iter = this->order_list.begin(); iter != this->order_list.end(); iter++) {
//		string str(iter->first);
//		items->product_list[str].stock -= iter->second;
//	}
//}
//
//void Order::today_sale()
//{
//	ifstream file("today_sale.txt");
//	string buffer;
//
//	try {
//		vector<string> todays;
//		if (file.is_open()) {
//			while (!file.eof())
//			{
//				getline(file, buffer);
//				istringstream str_stream(buffer);
//				while (getline(str_stream, buffer, ' ')) {
//					todays.push_back(buffer);
//				}
//			}
//			file.close();
//		}
//
//		today_sales = stoi(todays.at(0));
//		/*cout << current_date_time() << " �Ѹ���: " << todays.at(0) << endl;*/
//	}
//	catch (exception& e) {
//		cout << e.what() << endl;
//	}
//}
//
//
//void Order::today_update()
//{
//	today_sale();
//
//	this->today_sales += this->save_total;
//	
//	//��¥ �Ѿ�
//	ofstream file("today_sale.txt", ios::out);
//	if (file.is_open()) {
//		file << this->today_sales;
//	}
//	file.close();
//
//	
//}
//
//void Order::today_check()
//{
//}

//void Order::init()
//{
//	order_list.clear();
//	total = 0;
//	tax = false;
//	save_total = 0;
//	pay.clear();
//}
