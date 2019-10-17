#pragma once

#include <ctime>
#include <vector>

#include "hash.h"
#include "RandomEngine.h"
#include "transaction.h"
using std::vector;


class Block {
private:
	string prev_block_hash_, merkle_root_, block_hash_, difficulty_target_;
	std::time_t timestamp_;
	unsigned int version_;
	unsigned int nonce_;
	vector<Transaction> tx_list_;

	string setDifficultyTarget_() { return "0000"; }

public:

	// constructors
	//Block() : difficulty_target_{ setDifficultyTarget_() }, timestamp_{ time(NULL) }, version_{ 0 }, nonce_{ 0 } { tx_list_.reserve(100); }
	//Block(const Transaction& tx) : difficulty_target_{ setDifficultyTarget_() }, timestamp_{ time(NULL) }, version_{ 0 }, nonce_{ 0 } { tx_list_.push_back(tx); }
	//Block(const vector<Transaction>& tx_list) : difficulty_target_{ setDifficultyTarget_() }, timestamp_{ time(NULL) }, version_{ 0 }, nonce_{ 0 } { tx_list_.insert(tx_list_.end(), tx_list.begin(), tx_list.end()); }
	Block(const string& prev_block_hash) : prev_block_hash_{ prev_block_hash }, difficulty_target_{ setDifficultyTarget_() }, timestamp_{ time(NULL) }, version_{ 0 }, nonce_{ 0 }
	{
		tx_list_.reserve(100);
	}


	// push_back transactions
	inline void addTransaction(const Transaction tx) { tx_list_.push_back(tx); }
	inline void addTransaction(const vector<Transaction> tx_list) { tx_list_.insert(tx_list_.end(), tx_list.begin(), tx_list.end()); }
	
	// setter functions

	// set merkle root hash
	void setMerkleRoot() {
		tx_list_.shrink_to_fit();

		vector<Hash> hash_list;
		hash_list.resize(tx_list_.size() / 2 + 1);
		//hash_list.reserve(tx_list_.size() / 2 + 1);	//set initial size to half of transaction quantity

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
				hash_list.resize(index / 2);
			}
		}
		// set merkle root hash
		merkle_root_ = hash_list[0].getHash();
		hash_list.clear();	// don't need anymore
	}

	inline void setPrevBlockHash(const string& hash) { prev_block_hash_ = hash; }
	void setNonce()
	{
		RandomEngine re;
		std::uniform_int_distribution<unsigned int> dist = re.uni_int_distr((unsigned int)0, UINT_MAX);
		nonce_ = dist(re.mt);
	}

	// hash the block 
	string mineBlock() 
	{ 
		string hash = "";
		bool hashed = false;
		timestamp_ = time(NULL);
		version_ = 0;
		nonce_ = 0;

		setMerkleRoot();

		RandomEngine re;
		std::uniform_int_distribution<unsigned int> dist = re.uni_int_distr((unsigned int)0, UINT_MAX);
		while (!hashed)
		{
			// reset timestamp
			time_t timestamp = time(NULL);
			if (timestamp != timestamp_)
			{
				version_ = 0;
				timestamp_ = timestamp;
			}

			nonce_ = dist(re.mt);

			hash = Hash(prev_block_hash_ + merkle_root_ + std::to_string(timestamp_) + std::to_string(version_) + std::to_string(nonce_)).getHash();

			int i = 0;
			for (i; i < difficulty_target_.size(); i++)
				if (hash[i] != difficulty_target_[i])
					break;

			if (i >= difficulty_target_.size())
			{
				hashed == true;
				break;
			}

			version_++;
		}
		block_hash_ = hash;
		return block_hash_;
	}

	// getter functions

	inline string getPrevBlockHash() const { return prev_block_hash_; }
	inline string getBlockHash() const { return block_hash_; }
	inline string getMerkleRoot() const { return merkle_root_; }
	inline string getDifficultyTarget() const { return difficulty_target_; }
	inline time_t getTimeStamp() const { return timestamp_; }
	inline unsigned int getVersion() const { return version_; }
	inline unsigned int getNonce() const { return nonce_; }
	inline vector<Transaction> getTransactionList() const { return tx_list_; }


	friend std::ostream& operator<<(std::ostream& os, const Block& block)
	{
		os << "Stats for current block:\n";
		os << "Previous block hash: \t" << block.prev_block_hash_ << "\n";
		os << "Merkle root: \t\t" << block.merkle_root_ << "\n";
		os << "Timestamp: \t\t" << block.timestamp_ << "\n";
		os << "Version: \t\t" << block.version_ << "\n";
		os << "Nonce: \t\t\t" << block.nonce_ << "\n";
		os << "Difficulty target: \t" << block.difficulty_target_ << "\n";
		os << "Block hash: \t\t" << block.block_hash_ << "\n";

		return os;
	}
};

