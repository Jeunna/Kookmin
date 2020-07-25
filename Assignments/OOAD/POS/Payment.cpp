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
		"�����ؾ��� �ݾ�: " << amount << "\n" <<
		"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n" <<
		"0. �޴� �ٽ� ����\n" <<
		"1. ī����� (�ݾ�)\n" <<
		"2. ���ݰ��� (�ݾ�)\n" <<
		"3. ����\n\n" <<

		//"[OPTIONAL]\n" <<
		//"4. (������) �Ѿ׿� tax ��� (x%)\n" <<
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
				cout << "�������� �ʴ� ����Դϴ�." << endl;
				break;
			}

			//cin.clear();
			//cin.ignore(1000, '\n');
			//item_info.clear();
		}
		catch (exception e) {
			cout << "������ �� ���� ����Դϴ�: " << e.what() << endl;
		}
	}

	return paying;
}

void Payment::cashPayment()
{
	cout << "���ݰ����� �ݾ� : ";
	int num;
	cin >> num;

	amount -= num;
	this->paying["����"] += num;

	if (amount != 0) {
		cout << "���� �ݾ�: " << amount << endl;
	}
	else {
		cout << "��� �Ϸ�!" << endl;
		isComplete = true;
	}

	cin.clear();
	cin.ignore(1000, '\n');
}

void Payment::cardPayment()
{
	cout << "ī������� �ݾ� : ";
	int num;
	cin >> num;

	amount -= num;
	this->paying["ī��"] += num;

	if (amount != 0) {
		cout << "���� �ݾ�: " << amount << endl;
	}
	else {
		cout << "��� �Ϸ�!" << endl;
		isComplete = true;
	}

	cin.clear();
	cin.ignore(1000, '\n');
}

void Payment::plus_tax()
{
	float _num;
	cout << "�߰��� tax(%) : ";
	cin >> _num;

	cout << amount << " -> ";
	amount += round(amount *_num/100); //�ݿø�
	cout << amount << endl;
	isTax = true;

	cin.clear();
	cin.ignore(1000, '\n');
}

void Payment::refund()
{

}
