#pragma once
class Sign
{
public:
	string id;
	string password;
	string mode;

	Sign();
	~Sign();

	string checkInfo(string id, string password);
	string signIn();
	void signUp();
};