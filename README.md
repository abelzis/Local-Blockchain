# What is this?
A simple local blockchain implementation. 

# How to install?
Download the latest CMake [here](https://cmake.org/download/) and generate new solution from downloaded source code folder. Build or debug desired projects and enjoy!

# More in depth
This is a blockchain simulation that createst fixed amount of users and fixed amount of random transactions between users. Then every fixed amount of transactions is put into a block. The program then tries to mine a block (searching for a hash that meets the difficulty requirement).

### Default stats

|Variable|Value|
|:-------|-----:|
|Number of users| `1000`| 
|Number of transactions| `10000`|
|Number of transactions in block| `100`|
|Block difficulty| `"0000"`|


### Program results
It takes on average about 3 minutes to mine one block (for my PC). It took me 107 minutes to mine 36 blocks with default difficulty:

![36 Blocks mined](https://i.gyazo.com/19ed54e966f639b90c0e14f6c02b17d8.png)

# Changelog
### [v2.0](https://github.com/abelzis/Local-Blockchain/releases/tag/v2.0) (2019-10-25)
- Transactions into block are now picked randomly
- Added transaction unique hash id to prevent double spending and changing any transaction values
- Changed block mining strategy: only 1 block from 5 possible contenders will be mined
- Fixed many out of range errors and other bugs

### [v1.0.1](https://github.com/abelzis/Local-Blockchain/releases/tag/v1.0.1) (2019-10-18)
**Initial Release**
