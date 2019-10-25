// Blockchain.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <iostream>

#include "block.h"
#include "RandomEngine.h"
#include "TimerClass.h"

using std::cout;

// generate blockchain users
void generateUsers(vector<User>& user_list, const unsigned int& count)
{
	user_list.reserve(count);
	for (int i = 0; i < count; i++)
		user_list.push_back(User("User" + std::to_string(i), std::to_string(i), std::to_string(i), 100));	// give everyone 100 coins
}

// generate blockchain transactions
void generateTransactions(vector<Transaction>& tx_list, const unsigned int& tx_count, vector<User>& user_list, const unsigned int& user_count)
{
	tx_list.reserve(tx_count);

	RandomEngine re;

	int int_user_count = user_count - 1;
	std::uniform_int_distribution<int> dist = re.uni_int_distr(0, int_user_count);

	int i = 0, attempts = 0;	// successful transactions generated
	while (i < tx_count)
	{
		attempts++;
		//cout << "Attempt: " << attempts << ", i: " << i << "\n";
		unsigned int random_num_A = (unsigned int)dist(re.mt);	// sender
		unsigned int random_num_B = (unsigned int)dist(re.mt);	// receiver

		// prevent sending to yourself
		while (random_num_A == random_num_B)
			random_num_B = dist(re.mt);

		// generate random amount that is less or equal than available cash
		int int_user_balance = user_list[random_num_A].getBalance();
		unsigned int tx_amount = UINT_MAX;
		if (int_user_balance > 0)
		{
			std::uniform_int_distribution<int> dist_amount = re.uni_int_distr(1, int_user_balance);
			tx_amount = (unsigned int)dist_amount(re.mt);
		}


		// create transaction
		//if (tx_amount <= user_list[random_num_A].getBalance())
		//{
			tx_list.push_back(Transaction(i, user_list[random_num_A], user_list[random_num_B], tx_amount));
			user_list[random_num_A].setBalance(user_list[random_num_A].getBalance() - tx_amount);
			user_list[random_num_B].setBalance(user_list[random_num_B].getBalance() + tx_amount);

			i++;
		//}
	}

	cout << "Successfully generated " << tx_count << " transactions. Total attempts: " << attempts << ".";
}

void assignRandomIndexes(RandomEngine& re, vector<vector<unsigned int>>& tx_indexes, const int& tx_limit, const vector<Transaction>& tx_list,
	const int& tx_list_init_size, const int& participant_count)
{
	// randomly select transactions
	std::uniform_int_distribution<unsigned int> dist = re.uni_int_distr((unsigned int)0, (unsigned int)(tx_list_init_size - 1));

	tx_indexes.reserve(participant_count);
	for (int i = 0; i < participant_count; i++)
	{
		vector<unsigned int> temp;
		temp.reserve(tx_limit);
		for (int j = 0; j < tx_limit; j++)
		{
			unsigned int num = dist(re.mt);
			for (int a = 0; a < tx_list.size(); a++)
			{
				for (int z = 0; z < temp.size(); z++)	// GENERATED NUMBER MUST BE UNIQUE
				{
					if (temp[z] == num)
					{
						num = dist(re.mt);
						z = -1;
					}
				}
				if (tx_list[a].getID() == num)
					break;
				else if (a == tx_list.size() - 1 && tx_list[a].getID() != num)
				{
					num = dist(re.mt);
					a = -1;
				}
			}

			temp.push_back(num);
		}

		tx_indexes.push_back(temp);
	}
}

template <class ContainerA, class ContainerB>
void remove_elements(ContainerA& vDestination, const ContainerB& vSource)
{
	auto last = std::end(vDestination);
	std::for_each(std::begin(vSource), std::end(vSource), [&](typename ContainerB::const_reference val) {
		last = std::remove(std::begin(vDestination), last, val);
		});
	vDestination.erase(last, std::end(vDestination));
}

bool isVerified(const Transaction& tx) { return tx.getVerification(); }

vector<Transaction> updateTxList(vector<Transaction> tx_list)
{
	vector<Transaction>::iterator bound = std::stable_partition(tx_list.begin(), tx_list.end(), isVerified);
	return vector<Transaction>(tx_list.begin(), bound);
}

void blockchainMining(vector<Block>& blockchain, vector<Transaction> tx_list)
{
	Timer timer;

	RandomEngine re;

	// number of transactions in each block:
	int tx_limit = 100, tx_list_init_size = tx_list.size();

	int tx_counter = 0, block_counter = 0;

	vector<Transaction>::iterator it = tx_list.begin();
	while (!tx_list.empty())
	{
		Timer timer1;
		cout << "Beginning to mine block No. " << blockchain.size() + 1 << "...";
		// check if there is more than 100 available transactions

		//Block newBlock(blockchain[block_counter].getBlockHash());
		int participant_count = 5;
		vector<Block> new_block_participants(participant_count, Block(blockchain[block_counter].getBlockHash()));

		vector<vector<Transaction>> selected_tx;

		bool final_block = false;

		if (tx_list.size() > tx_limit)
		{
			vector<vector<unsigned int>> tx_indexes;

			assignRandomIndexes(re, tx_indexes, tx_limit, tx_list, tx_list_init_size, participant_count);

			selected_tx.reserve(participant_count);
			for (int i = 0; i < tx_indexes.size(); i++)
			{
				vector<Transaction> temp;
				temp.reserve(tx_limit);
				for (int j = 0; j < tx_indexes[i].size(); j++)
					temp.push_back(tx_list[tx_indexes[i][j]]);
				selected_tx.push_back(temp);
			}
		}
		else
		{
			selected_tx.reserve(participant_count);
			for (int i = 0; i < participant_count; i++)
				selected_tx.push_back(tx_list);

			final_block = true;
		}

		selected_tx.shrink_to_fit();

		for (int i = 0; i < participant_count; i++)
			//new_block_participants[i].addTransaction(vector<Transaction>(it, it + tx_limit));
			new_block_participants[i].addTransaction(selected_tx[i]);



		// create temporary vector of block participants



		time_t time_to_hash = 1;
		bool hashed = false, skip = false;;
		while (!hashed)
		{
			skip = false;
			for (int i = 0; i < participant_count; i++)
			{
				new_block_participants[i].setTransactionList(updateTxList(new_block_participants[i].getTransactionList()));

				if (new_block_participants[i].getTransactionList().empty())
				{
					skip = true;
					continue;
				}

				skip = false;

				hashed = new_block_participants[i].mineBlock(time_to_hash);
				if (hashed == true)
				{
					blockchain.push_back(new_block_participants[i]);

					//cout << "tx_list before erase:\n";
					//for (int j = 0; j < tx_list.size(); j++)
					//	cout << tx_list[j].getID() << " ";

					//cout << "\n\nerased:\n";
					//for (int j = 0; j < new_block_participants[i].getTransactionList().size(); j++)
					//	cout << new_block_participants[i].getTransactionList()[j].getID() << " ";

					//tx_list.erase(new_block_participants[i].getTransactionList().begin(), new_block_participants[i].getTransactionList().end());
					remove_elements(tx_list, new_block_participants[i].getTransactionList());

					//cout << "\n\ntx_list after erase:\n";
					//for (int j = 0; j < tx_list.size(); j++)
					//	cout << tx_list[j].getID() << " ";

					break;
				}
			}
			if (time_to_hash * 2 < INT_MAX)
				time_to_hash *= 2;

			if (skip == true)
				break;
		}







		//blockchain.push_back(newBlock);


		if (hashed)
		{

			it += tx_limit;

			block_counter++;

			cout << "Done! Block Successfully mined in " << timer1.elapsed() << "s\n";




			cout << blockchain[block_counter] << "\n";

			//for (int j = i; j < i + tx_limit; j++)
			//{
			//	
			//}
			tx_counter += tx_limit;
		}

		if (final_block || skip)
		{
			if (tx_list.size() > 0)
			{
				cout << "\nImpossible transactions to mine: ";
				for (int i = 0; i < tx_list.size(); i++)
					cout << tx_list[i].getID() << " ";
			}
			break;
		}
	}
	cout << "\nTotal time to mine all the blocks: " << timer.elapsed() << "s\n";
}

void printErrorMsg(const std::exception& msg) { cout << "Error: " << msg.what() << "\n"; }

int main(int argc, char* argv[])
{
	unsigned int user_gen_amount = 1000;	// default amount of users to be generated
	unsigned int tx_gen_amount = 10000;		// default amount of transactions to be generated

	if (argc > 1)
	{
		// user amount
		try 
		{
			user_gen_amount = std::stoi(argv[1]);
		}
		catch (std::exception ex)
		{
			printErrorMsg(ex);
			user_gen_amount = 1000;
		}

		// transaction amount
		if (argc > 2)
		{
			try
			{
				tx_gen_amount = std::stoi(argv[2]);
			}
			catch (std::exception ex)
			{
				printErrorMsg(ex);
				tx_gen_amount = 10000;
			}
		}
	}

	vector<User> user_list;
	vector<Transaction> tx_list;

	Timer timer;
	cout << "Beginning generating users...\n";
	generateUsers(user_list, user_gen_amount);	// generate random users
	cout << "Successfully generated " << user_gen_amount << " users each having " << 100 << " coins.\n" << " Time elapsed: " << timer.elapsed() << "s\n";

	timer.reset();
	cout << "Beginning generating transactions...\n";
	generateTransactions(tx_list, tx_gen_amount, user_list, user_gen_amount);	// generate random transactions
	cout << " Time elapsed: " << timer.elapsed() << "s\n";


	Block genesis_block(Hash("Genesis Block").getHash());
	genesis_block.setPrevBlockHash("");



	timer.reset();
	cout << "\n\nBeginning to mine genesis block...";
	genesis_block.mineBlock(100000);
	cout << "Done! Block Successfully mined in " << timer.elapsed() << "s\n";
	cout << genesis_block << "\n";

	vector<Block> blockchain;
	blockchain.push_back(genesis_block);

	blockchainMining(blockchain, tx_list);

}