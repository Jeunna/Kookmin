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
		"★\t편\t의\t점\t★ - 관리자 모드\n" <<
		"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n" <<
		"0. 메뉴 다시 보기\n" <<
		"1. 전체 재고 조회\n" <<
		"2. 상품 정보 조회 (물건이름)\n" <<
		"3. 상품 추가 (물건이름 ±수량)\n" <<
		"4. 상품 삭제 (물건이름)\n" <<
		"5. 상품 정보 수정 (물건이름, 바꿀 항목, 바꿀 내용)\n" <<
		"6. 재고 정보 저장\n" <<
		"7. 종료\n\n" <<
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

	cout << "물건이름: ";

	string input;
	getline(cin, input);

	productList->get_info(input);
}

void Stock::addProduct()
{
	ProductList *productList;
	productList = ProductList::getInstatnce();

	cout << "입력하기: 상품 이름, 상품 설명, 제조한 곳, 유통기한, 가격, 갯수" << endl;
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

	cout << "삭제하기: 상품 이름" << endl;
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
	cout << "수정하기" << endl;
	cout << "상품 이름 : ";
	cin >> name;

	cout << "수정할 항목 : ";
	cout << "이름:0, 내용:1, 제조한 곳:2, 유통기한:3, 가격:4, 갯수:5" << endl;
	cin >> which;
	cout << "수정할 내용 : ";
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

	cout << "저장되었습니다." << endl;
}

