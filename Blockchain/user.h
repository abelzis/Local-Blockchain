#pragma once

#include <string>
using std::string;

class User {
private:
	string name_, private_key_, public_key_;
	unsigned int balance_;	// money balance

public:

	User(const string& name, const string& private_key, const string& public_key, const unsigned int& balance) : name_{ name }, private_key_{ private_key },
		public_key_{ public_key }, balance_{ balance } {}

	void setName(const string& n) { name_ = n; }
	void setPrivateKey(const string& key) { private_key_ = key; }
	void setPublicKey(const string& key) { public_key_ = key; }
	void setBalance(const unsigned int val) { balance_ = val; }

	string getName() const { return name_; }
	string getPrivateKey() const { return private_key_; }
	string getPublicKey() const { return public_key_; }
	unsigned int getBalance() const { return balance_; }
}; 