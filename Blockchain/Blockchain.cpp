// Blockchain.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "block.h"
#include "RandomEngine.h"

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

	std::uniform_int_distribution<int> dist = re.uni_int_distr(0, (int)user_count);
	for (int i = 0; i < tx_count; i++)
	{
		unsigned int random_num_A = (unsigned int)dist(re.mt);	// sender
		unsigned int random_num_B = (unsigned int)dist(re.mt);	// receiver

		// prevent sending to yourself
		while (random_num_A == random_num_B)
			random_num_B = dist(re.mt);

		// generate random amount that is less or equal than available cash
		std::uniform_int_distribution<int> dist_amount = re.uni_int_distr(1, (int)user_list[random_num_A].getBalance());
		unsigned int tx_amount = (unsigned int)dist_amount(re.mt);

		// create transaction
		tx_list.push_back(Transaction(i, user_list[random_num_A], user_list[random_num_B], tx_amount));
	}
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
	vector<Block> blockchain;

	generateUsers(user_list, user_gen_amount);	// generate random users
	generateTransactions(tx_list, tx_gen_amount, user_list, user_gen_amount);	// generate random transactions

	


}