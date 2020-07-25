#pragma once

class Log
{
public:
	Log();
	~Log();

	int todays = 0;

	void receipt(map<string, int> &paid, string &saled);
	string currentDate();
	void saveTodayTotal(int total);
	void getTodayTotal();
};

