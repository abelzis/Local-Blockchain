// Blockchain.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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
		if (tx_amount <= user_list[random_num_A].getBalance())
		{
			tx_list.push_back(Transaction(i, user_list[random_num_A], user_list[random_num_B], tx_amount));
			user_list[random_num_A].setBalance(user_list[random_num_A].getBalance() - tx_amount);
			user_list[random_num_B].setBalance(user_list[random_num_B].getBalance() + tx_amount);

			i++;
		}
	}

	cout << "Successfully generated " << tx_count << " transactions. Total attempts: " << attempts << ".";
}

void blockchainMining(vector<Block>& blockchain, vector<Transaction> tx_list)
{
	Timer timer;

	// number of transactions in each block:
	int tx_limit = 100;

	int tx_counter = 0, block_counter = 0;

	vector<Transaction>::iterator it = tx_list.begin();
	while (!tx_list.empty())
	{
		Timer timer1;
		cout << "Beginning to mine block No. " << blockchain.size() + 1 << "...";
		// check if there is more than 100 available transactions
		if (tx_counter + tx_limit < tx_list.size())
		{
			//create new temporary block
			Block newBlock(blockchain[block_counter].getBlockHash());

			newBlock.addTransaction(vector<Transaction>(it, it + tx_limit));	//add transactions to that block

			newBlock.mineBlock();

			blockchain.push_back(newBlock);

			it += tx_limit;

			block_counter++;
			
			cout << "Done! Block Successfully mined in " << timer1.elapsed() << "s\n";
		}
		else// if (tx_counter + tx_limit >= tx_list.size())	// if less than 100 left
		{
			//create new temporary block
			Block newBlock(blockchain[block_counter].getPrevBlockHash());

			newBlock.addTransaction(vector<Transaction>(it, tx_list.end()));	//add transactions to that block

			tx_list.clear();

			newBlock.mineBlock();

			blockchain.push_back(newBlock);

			cout << "Done! Block Successfully mined in " << timer1.elapsed() << "s\n";

			block_counter++;

			break;
		}

		cout << blockchain[block_counter] << "\n";

		//for (int j = i; j < i + tx_limit; j++)
		//{
		//	
		//}
		tx_counter += tx_limit;
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
	vector<Block> blockchain;
	blockchain.push_back(genesis_block);

	blockchainMining(blockchain, tx_list);

}