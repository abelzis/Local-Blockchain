#pragma once

#include <algorithm>
#include <ctime>
#include <vector>

#include "hash.h"
#include "RandomEngine.h"
#include "transaction.h"
using std::vector;

// USING libbitcoin
//#include <bitcoin/bitcoin.hpp>

// for release build - comment this define
//#define TEST_BUILD


bool isVerified_(const Transaction& tx) { return tx.getVerification(); }

class Block {
private:
	string prev_block_hash_, merkle_root_, block_hash_, difficulty_target_;
	std::time_t timestamp_;
	unsigned int version_;
	unsigned int nonce_;
	vector<Transaction> tx_list_;

#ifdef TEST_BUILD
	string setDifficultyTarget_() { return "0"; }
#else
	string setDifficultyTarget_() { return "0000"; }
#endif

	// payments has to be legal - you cant send 120 coins if you have 100
	// and if hashID is the same, it verifies that nothing has been changed in the transaction
	bool verifyTransaction_(const Transaction& tx)
	{
		if (tx.getAmount() <= tx.getSender().getBalance())
			if (tx.getHashID().getHash() == Hash(tx.getAsString()).getHash())
				return true;

		return false;
	}


	void updateUsers_()
	{
		for (int i = 0; i < tx_list_.size(); i++)
		{
			User user_A = tx_list_[i].getSender();
			User user_B = tx_list_[i].getReceiver();

			user_A.setBalance(user_A.getBalance() - tx_list_[i].getAmount());
			user_B.setBalance(user_B.getBalance() + tx_list_[i].getAmount());
		}
	}


	void updateTxList_()
	{
		vector<Transaction>::iterator bound = std::stable_partition(tx_list_.begin(), tx_list_.end(), isVerified_);
		tx_list_ = vector<Transaction>(tx_list_.begin(), bound);
	}

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
	void addTransaction(const Transaction& tx) 
	{ 
		tx_list_.push_back(tx); 
		tx_list_[tx_list_.size() - 1].verify(verifyTransaction_(tx));
	}
	void addTransaction(const vector<Transaction>& tx_list) 
	{ 
		vector<Transaction>::iterator it = tx_list_.end();
		tx_list_.insert(tx_list_.end(), tx_list.begin(), tx_list.end()); 
		//for (auto& i : tx_list_)
		for (it = tx_list_.begin(); it != tx_list_.end(); ++it)
			(*it).verify(verifyTransaction_(*it));
	}
	
	// setter functions

	inline void setTransactionList(const vector<Transaction>& tx_list) { tx_list_ = tx_list; }

	// set merkle root hash
	void setMerkleRoot() {
		tx_list_.shrink_to_fit();

		// from bitcoin
		if (tx_list_.empty())
		{
			merkle_root_ = Hash("").getHash();
			return;
		}
		else if (tx_list_.size() == 1)
		{
			merkle_root_ = Hash(tx_list_[0].getAsString()).getHash();
			return;
		}

		vector<Hash> hash_list;
		hash_list.resize(tx_list_.size() / 2 + 1);
		//hash_list.reserve(tx_list_.size() / 2 + 1);	//set initial size to half of transaction quantity

		// initial hashing:
			// create 1 hash from transactions data concatenation as strings
		for (int i = 0; i < tx_list_.size(); i += 2)
		{
			if (i + 1 < tx_list_.size())
				hash_list[i / 2] = Hash(tx_list_[i].getAsString() + tx_list_[i + 1].getAsString());
			else
				hash_list[i / 2] = Hash(tx_list_[i].getAsString() + tx_list_[i].getAsString());		// if amount is odd, duplicate last transaction
		}

		
		// hashing hashes together. Goal: 1 single hash from any number
		while (hash_list.size() > 1)
		{
			/*if (hash_list.size() == 3)
			{
				hash_list[0] = Hash(Hash(hash_list[0].getHash() + hash_list[1].getHash()).getHash() + hash_list[2].getHash());
				hash_list.resize(1);
				break;
			}*/

			// if hash amount is odd, duplicate last hash
			if (hash_list.size() % 2 != 0)
				hash_list.push_back(hash_list.back());

			int index = 0;
			while (index < hash_list.size())
			{
				// create 1 hash from concatenation of 2 hashes
				if (index + 1 < hash_list.size())
				{
					hash_list[index / 2] = Hash(hash_list[index].getHash() + hash_list[index + 1].getHash());
					//hash_list.resize(index / 2);
				}
				//else
				//{
				//	hash_list[index / 2] = Hash(hash_list[index].getHash());
				//	//hash_list_.resize()
				//}
				index += 2;
			}
			hash_list.resize(hash_list.size() / 2);
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
	bool mineBlock(const time_t& time_limit) 
	{ 
		updateTxList_();

		string hash = "";
		bool hashed = false;
		timestamp_ = time(NULL);
		version_ = 0;
		nonce_ = 0;

		setMerkleRoot();

		RandomEngine re;
		std::uniform_int_distribution<unsigned int> dist = re.uni_int_distr((unsigned int)0, UINT_MAX);
		time_t time_mark = time(NULL);
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
				hashed = true;
				break;
			}

			version_++;

			if (time(NULL) - time_mark > time_limit)
				return false;
		}
		block_hash_ = hash;

		updateUsers_();
		return true;
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
		os << "Previous block hash: \t\t" << block.prev_block_hash_ << "\n";
		os << "Amount of transactions: \t" << block.tx_list_.size() << "\n";
		os << "Merkle root: \t\t\t" << block.merkle_root_ << "\n";
		os << "Timestamp: \t\t\t" << block.timestamp_ << "\n";
		os << "Version: \t\t\t" << block.version_ << "\n";
		os << "Nonce: \t\t\t\t" << block.nonce_ << "\n";
		os << "Difficulty target: \t\t" << block.difficulty_target_ << "\n";
		os << "Block hash: \t\t\t" << block.block_hash_ << "\n";

		return os;
	}
};

