#pragma once
class Point
{
public:
	Point();
	~Point();

	string user;
	string number;
	int point;

	void savePoint(int total);
	void usePoint(int num);
	string validdateUser(const string &number);
	int calculatePoint(int total);
	int getPoint(const string *user);
};

