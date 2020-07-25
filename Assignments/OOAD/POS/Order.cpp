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
		"★\t편\t의\t점\t★\n" <<
		"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n" <<
		"0. 메뉴 다시 보기\n" <<
		"1. 새결제하기\n" <<
		"2. 재고 체크하기\n" <<
		"3. 종료\n\n" <<

		"[OPTIONAL]\n" <<
		"4. 총매출 확인\n" <<
		"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n" <<
		endl;
}

void Order::sale() {
	cout <<
		"\n" <<
		"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n" <<
		"0. 메뉴 다시 보기\n" <<
		"1. 상품 입력 (물건이름 ±수량)\n" <<
		"2. 상품 취소 (물건이름)\n" <<
		"3. 입력된 상품 확인\n" <<
		"4. 결제하기\n" <<
		"5. 결제 종료\n\n" <<

		"[OPTIONAL]\n" <<
		"6. (결제전) 총액에 tax 계산 (x%)\n" <<
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
				cout << "존재하지 않는 명령입니다." << endl;
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
			cout << "실행할 수 없는 명령입니다: " << e.what() << endl;
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
	cout << "추가하기: 상품 이름, 갯수" << endl;
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
	cout << "삭제하기: 상품 이름" << endl;
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
	
	cout << "총매출: " << today_total << endl;
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
//	this->total += round(this->total *_num/100); //반올림
//	cout << this->total << endl;
//	this->tax = true;
//}
//
//void Order::payment(const char *_payment, int _num)
//{
//	string pay(_payment);
//	if (!strcmp(_payment, "카드")) {
//		//TODO: 카드 결제하는 func
//		cout << "카드 결제: " << _num << "원" << endl;
//		if (this->pay.find("카드") == this->pay.end()) {
//			this->pay["카드"] = _num;
//		}
//		else {
//			int money = this->pay["카드"];
//			this->pay["카드"] += _num;
//		}
//		cout << "남은 액수: " << total - this->pay["카드"] << endl;
//	}
//	else if (!strcmp(_payment, "현금")) {
//		//TODO: 현금 결제하는 func
//		cout << "현금 결제: " << _num << "원" << endl;
//
//		int money = 0;
//		if (total < _num) {
//			money = total;
//			cout << "거스름돈: " << _num - total << endl;
//		}
//		else {
//			money = _num;
//			if (this->pay.find("현금") == this->pay.end()) {
//				this->pay["현금"] = _num;
//			}
//			else {
//				this->pay["현금"] += _num;
//			}
//		}
//		cout << "남은 액수: " << total - money << endl;
//	}
//
//	if (total == 0) {
//		receipt(pay);
//		update_stock();
//		init();
//		items->update_file();
//		today_update();
//
//		cout << "결제 완료!" << endl;
//	}
//}
//
//void Order::payment(string &_payment, int _num)
//{
//	string pay = _payment;
//	int money = 0;
//	if (!pay.compare("카드")) {
//		//TODO: 카드 결제하는 func
//		cout << "카드 결제: " << _num << "원" << endl;
//		if (this->pay.find("카드") == this->pay.end()) {
//			this->pay["카드"] = _num;
//		}
//		else {
//			money = this->pay["카드"];
//			this->pay["카드"] += _num;
//		}
//		total -= this->pay["카드"];
//		cout << "남은 액수: " << total << "\n";
//	}
//	else if (!pay.compare("현금")) {
//		//TODO: 현금 결제하는 func
//		cout << "현금 결제: " << _num << "원" << endl;
//		
//		if (this->total < _num) {
//			money = _num - this->total;
//			cout << "거스름돈: " << money << endl;
//			this->total = 0;
//		}
//		else {
//			money = _num;
//			if (this->pay.find("현금") == this->pay.end()) {
//				this->pay["현금"] = _num;
//			}
//			else {
//				this->pay["현금"] += _num;
//			}
//			this->total -= money;
//		}
//		cout << "남은 액수: " << this->total << "\n";
//	}
//
//	if (this->total == 0) {
//		receipt(pay);
//		update_stock();
//		items->update_file();
//		today_update();
//		init();
//
//		cout << "결제 완료!" << endl;
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
//			<< "결제방식: " << _pay << "\n"
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
//		/*cout << current_date_time() << " 총매출: " << todays.at(0) << endl;*/
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
//	//날짜 총액
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
