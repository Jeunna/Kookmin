#include "pch.h"
#include "Payment.h"


Payment::Payment(int total)
{
	amount = total;
	pay();
}


Payment::~Payment()
{
}

void Payment::pay()
{
	cout <<
		"\n" <<
		"결제해야할 금액: " << amount << "\n" <<
		"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n" <<
		"0. 메뉴 다시 보기\n" <<
		"1. 카드결제 (금액)\n" <<
		"2. 현금결제 (금액)\n" <<
		"3. 종료\n\n" <<

		//"[OPTIONAL]\n" <<
		//"4. (결제전) 총액에 tax 계산 (x%)\n" <<
		"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n" <<
		endl;
}

map<string, int> Payment::salePay()
{
	int pos = show;
	while (pos != quit && isComplete != true) {
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
			if (!amount) break;
			switch (pos)
			{
			case show:
				pay();
				break;
			case card:
				cardPayment();
				break;
			case cash:
				cashPayment();
				break;
			case quit:
				break;
			case tax:
				plus_tax();
				break;
			default:
				cout << "존재하지 않는 명령입니다." << endl;
				break;
			}

			//cin.clear();
			//cin.ignore(1000, '\n');
			//item_info.clear();
		}
		catch (exception e) {
			cout << "실행할 수 없는 명령입니다: " << e.what() << endl;
		}
	}

	return paying;
}

void Payment::cashPayment()
{
	cout << "현금결제할 금액 : ";
	int num;
	cin >> num;

	amount -= num;
	this->paying["현금"] += num;

	if (amount != 0) {
		cout << "남은 금액: " << amount << endl;
	}
	else {
		cout << "계산 완료!" << endl;
		isComplete = true;
	}

	cin.clear();
	cin.ignore(1000, '\n');
}

void Payment::cardPayment()
{
	cout << "카드결제할 금액 : ";
	int num;
	cin >> num;

	amount -= num;
	this->paying["카드"] += num;

	if (amount != 0) {
		cout << "남은 금액: " << amount << endl;
	}
	else {
		cout << "계산 완료!" << endl;
		isComplete = true;
	}

	cin.clear();
	cin.ignore(1000, '\n');
}

void Payment::plus_tax()
{
	float _num;
	cout << "추가할 tax(%) : ";
	cin >> _num;

	cout << amount << " -> ";
	amount += round(amount *_num/100); //반올림
	cout << amount << endl;
	isTax = true;

	cin.clear();
	cin.ignore(1000, '\n');
}

void Payment::refund()
{

}
