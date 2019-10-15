#pragma once

#include <map>
#include <vector>

#include "hash.h"
#include "transaction.h"
using std::map;
using std::vector;


class Block {
private:
	Hash prev_block_hash, merkle_root, difficulty_target;
	Time timestamp;
	Version version;
	unsigned int nonce;
	vector<Transaction> trans_list;
	map<Transaction> trans_map;

public:

	

	inline void addTransaction(const Transaction tx) { trans_list.push_back(tx); }
	inline void addTransaction(const vector<Transaction> tx_list) { trans_list.insert(trans_list.end(), tx_list.begin(), tx_list.end()); }
	
	void setMerkleRoot() {
		
	}

	Hash getMerkleRoot() const { return merkle_root; }
};

