#pragma once

#include <vector>

#include "hash.h"
#include "transaction.h"
using std::vector;


class Block {
private:
	string prev_block_hash_, merkle_root_, block_hash_, difficulty_target_;
	//Time timestamp_;
	//Version version_;
	unsigned int nonce_;
	vector<Transaction> tx_list_;

	string setDifficultyTarget_() { return "000"; }

public:

	// constructors
	Block() : difficulty_target_{ setDifficultyTarget_() }, nonce_{ 0 } { tx_list_.reserve(100); }
	Block(const Transaction& tx) : difficulty_target_{ setDifficultyTarget_() }, nonce_{ 0 } { tx_list_.push_back(tx); }
	Block(const vector<Transaction>& tx_list) : difficulty_target_{ setDifficultyTarget_() }, nonce_{ 0 } { tx_list_.insert(tx_list_.end(), tx_list.begin(), tx_list.end()); }

	// push_back transactions
	inline void addTransaction(const Transaction tx) { tx_list_.push_back(tx); }
	inline void addTransaction(const vector<Transaction> tx_list) { tx_list_.insert(tx_list_.end(), tx_list.begin(), tx_list.end()); }
	
	// setter functions

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
		merkle_root_ = hash_list[0].getHash();
		hash_list.clear();	// don't need anymore
	}

	inline void setPrevBlockHash(const string& hash) { prev_block_hash_ = hash; }
	inline void setNonce(const unsigned int& num) { nonce_ = num; }

	// getter functions

	inline string getPrevBlockHash() const { return prev_block_hash_; }
	inline string getMerkleRoot() const { return merkle_root_; }
	inline string getDifficultyTarget() const { return difficulty_target_; }
	inline unsigned int getNonce() const { return nonce_; }
	inline vector<Transaction> getTransactionList() const { return tx_list_; }
};

