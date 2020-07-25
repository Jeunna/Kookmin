#include "pch.h"
#include "Order.h"
#include "ProductList.h"
#include "Sign.h"
#include "Stock.h"
#include <cctype>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>

enum state
{
	Start,
	Ordering,
	Confirm,
	Quit,
	Today
};

enum stock_state
{
	Menu,
	Show,
	Search,
	Add,
	Delete,
	Modify,
	Save,
	Exit
};

bool readyFlag;
mutex readyMutex;
condition_variable readyCondVar;
bool IsWorking;

void cashier(ProductList *items, Order *order);

//void worker(promise<int> *p) {
//	string input = "";
//	while (input.compare("qqq")) {
//		cin.clear();
//		cout << ">> ";
//		/*cin >> pos;*/
//		getline(cin, input);
//	}
//	//약속 이해하는 모습, 해당 결과는 future에 들어감
//	p->set_value(0);
//}
//
//void t_order(Order *order) {
//	promise<int> p;
//	future<int> data = p.get_future();
//
//	/*thread t(ordering, order, &p);*/
//	//t.join();
//
//	data.wait();
//}

int main()
{
	Sign *sign = new Sign();
	
	string status;
	while (status.compare("admin") && status.compare("cashier")) {
		status = sign->signIn();
	}
	cout << "-*-*- " << status << " -*-*-" << endl;

	/*ProductList *items = new ProductList();*/
	ProductList *productList;
	productList = ProductList::getInstatnce();
	
	int pos;

	if (!status.compare("admin")) {
		Stock *stock = new Stock();

		cin.clear();
		cin.ignore(1000, '\n');
		
		pos = Menu;
		while (pos != Exit) {
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
				case Menu:
					stock->menu();
					break;
				case Show:
					stock->getProductList();
					break;
				case Search:
					stock->findProduct();
					break;
				case Add:
					stock->addProduct();
					break;
				case Delete:
					stock->deleteProduct();
					break;
				case Modify:
					stock->modifyProduct();
					break;
				case Save:
					stock->saveProductList();
					break;
				case Exit:
					break;
				default:
					cout << "존재하지 않는 명령입니다." << endl;
					break;
				}
			}
			catch (exception e) {
				cout << "실행할 수 없는 명령입니다: " << e.what() << endl;
			}
		}
	}
	else {
		Order *order = new Order();

		cin.clear();
		cin.ignore(1000, '\n');

		//order->today_sale();
		pos = Start;
		while (pos != Quit) {
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
				case Start:
					order->start();
					break;
				case Ordering:
					order->makeSale();
					break;
				case Confirm:
					productList->print_list();
					break;
				case Quit:
					break;
				case Today:
					order->todayTotal();
					break;
				default:
					cout << "존재하지 않는 명령입니다." << endl;
					break;
				}

				//cin.clear();
				//cin.ignore(1000, '\n');
				//item_info.clear();
				productList->update_file();
			}
			catch (exception e) {
				cout << "실행할 수 없는 명령입니다: " << e.what() << endl;
			}
		}


		/*delete items, order;*/
	}



	//promise<int> p;
	////미래에 int 데이터를 돌려주겠다는 약속.
	//future<int> data = p.get_future();

	//thread t_order(ordering, &p);
	//thread t_cancle(cancle, &p);

	//// 약속된 데이터 받을때까지 기다림
	//data.wait();
	//cout << "받은 데이터" << data.get() << endl;
	//해당 쓰레드 실행
	///*t.join();*/

	return 0;
}

void cashier(ProductList *items, Order *order) {

}

//void start() {
//	cout <<
//		"\n" <<
//		"★\t편\t의\t점\t★\n" <<
//		"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n" <<
//		"1. 구매가능 상품 조회\n" <<
//		"2. 상품 입력 (물건이름 ±수량)\n" <<
//		"3. 상품 취소 (물건이름)\n" <<
//		"4. 입력된 상품 확인\n" <<
//		"5. 결제 (카드|현금 금액)\n" <<
//		"6. 종료\n\n" <<
//
//		"[OPTIONAL]\n" <<
//		"7. (결제전) 총액에 tax 계산 (x%)\n" <<
//		"8. 총매출 확인\n" <<
//		"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n" <<
//		endl;
//}