#pragma once

#include "user.h"

class Transaction {
private:
	unsigned int id_, amount_;
	User sender_, receiver_;	//

public:

	Transaction(const unsigned int& id, const User& a, const User& b, const unsigned int& amount) : id_(id), sender_(a), receiver_(b), amount_(amount) { }

	void setID(const unsigned int& id) { id_ = id; }

	unsigned int getID() const { return id_; }
	User getSender() const { return sender_; }		// returns sender of the transaction
	User getReceiver() const { return receiver_; }	// returns receiver of the transaction

	string getAsString() const { return std::to_string(id_) + std::to_string(amount_) + sender_.getName() + receiver_.getName(); }	// returns data of transaction as string
};

