#pragma once

#include <vector>

#include "hash.h"
#include "transaction.h"
using std::vector;


class Block {
private:
	Hash prev_block_hash_, merkle_root_, difficulty_target_;
	//Time timestamp_;
	//Version version_;
	unsigned int nonce_;
	vector<Transaction> tx_list_;

public:

	// constructors
	Block() { tx_list_.reserve(100); }
	Block(const Transaction& tx) { tx_list_.push_back(tx); }
	Block(const vector<Transaction>& tx_list) { tx_list_.insert(tx_list_.end(), tx_list.begin(), tx_list.end()); }

	// push_back transactions
	inline void addTransaction(const Transaction tx) { tx_list_.push_back(tx); }
	inline void addTransaction(const vector<Transaction> tx_list) { tx_list_.insert(tx_list_.end(), tx_list.begin(), tx_list.end()); }
	
	// set merkle root hash
	void setMerkleRoot() {
		tx_list_.shrink_to_fit();

		vector<Hash> hash_list;
		hash_list.reserve(tx_list_.size() / 2 + 1);	//set initial size to half of transaction quantity

		// initial hashing:
			// create 1 hash from transactions data concatenation as strings
		for (int i = 0; i < tx_list_.size(); i += 2)
			hash_list[i / 2] = Hash(tx_list_[i].getAsString() + tx_list_[i + 1].getAsString());
		
		// hashing hashes together. Goal: 1 single hash from any number
		while (hash_list.size() > 1)
		{
			int index = 0;
			while (index < hash_list.size())
			{
				// create 1 hash from concatenation of 2 hashes
				hash_list[index / 2] = Hash(hash_list[index].getHash() + hash_list[index + 1].getHash());
				index += 2;
			}
		}
		// set merkle root hash
		merkle_root_ = hash_list[0];
		hash_list.clear();	// don't need anymore
	}

	Hash getMerkleRoot() const { return merkle_root_; }
};

