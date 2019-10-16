#pragma once

#include <string>
#include <cmath>
using std::string;


class Hash
{
private:
	static const size_t hash_length = 81;	//324/4 = 81

	string hash;
	//324 bit length
	string default_string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-_=+[]{};:'\",.<>/?`~\\|ąčęėįšųūžĄČĘĖĮŠŲŪŽ";


	void hashingAlgorithm(string str)
	{
		string copy_default_string = default_string;

		// throw empty string exception if empty
		if (str.empty())
			//throw std::exception("Unable to hash empty string.\n");
		{
			for (unsigned int i = 0; i < hash_length; i++)
			{
				int a = 0;
				do
				{
					copy_default_string[i] = (copy_default_string[i] ^ copy_default_string[(i + a + (int)sqrt(a)) % copy_default_string.size()]) % 128;
					//std::cout << copy_default_string[i];
					a++;
				} while (copy_default_string[i] < '0' || (copy_default_string[i] > '9' && (copy_default_string[i] < 'a' || copy_default_string[i] > 'f')));	//limit to 0-9 + a-f

			}
		}

		//hashing
		for (unsigned int i = 0; i < str.length(); i++)
			for (unsigned int j = 0; j < hash_length; j++)
			{
				int a = 0;
				do
				{
					//hash formula
					copy_default_string[j] = (copy_default_string[j] + 13 * str[i] * str[i] + a * i * j + a * a + a + j + ((copy_default_string[(i * j + str[i]) % copy_default_string.length()] * str[i] + str[0]) ^ (default_string[i * str[i] % (hash_length - 1)]))) % 128;

					a++;
				} while (copy_default_string[j] < '0' || (copy_default_string[j] > '9' && (copy_default_string[j] < 'a' || copy_default_string[j] > 'f')));	//limit to 0-9 + a-f
			}
		// copy values to hash string
		for (int i = 0; i < hash_length; i++)
			hash += copy_default_string[i];
	}

public:

	//UI

	Hash(string str) { hashingAlgorithm(str); }

	Hash() { }

	string getHash() const { return hash; }

	inline void setHash(string str) { hashingAlgorithm(str); }
};