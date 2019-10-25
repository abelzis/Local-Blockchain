#pragma once

#include "user.h"

class Transaction {
private:
	unsigned int id_, amount_;
	Hash hash_id_;
	User sender_, receiver_;	//
	bool verified_;

public:

	Transaction(const unsigned int& id, const User& a, const User& b, const unsigned int& amount) : id_(id), sender_(a), receiver_(b), amount_(amount), verified_(false) { hash_id_ = Hash(getAsString()); }

	void setID(const unsigned int& id) { id_ = id; }

	unsigned int getID() const { return id_; }
	unsigned int getAmount() const { return amount_; }
	User getSender() const { return sender_; }		// returns sender of the transaction
	User getReceiver() const { return receiver_; }	// returns receiver of the transaction
	bool getVerification() const { return verified_; }

	void verify(const bool& bl) { verified_ = bl; }

	string getAsString() const { return std::to_string(id_) + std::to_string(amount_) + sender_.getName() + receiver_.getName(); }	// returns data of transaction as string

	void setHashID() { hash_id_ = Hash(getAsString()); }
	Hash getHashID() const { return hash_id_; }



	inline friend bool operator==(const Transaction& left, const Transaction& right) 
	{
		return (left.id_ == right.id_ && left.amount_ == right.amount_ && left.hash_id_ == right.hash_id_ && left.sender_ == right.sender_
			&& left.receiver_ == right.receiver_);
	}
};

