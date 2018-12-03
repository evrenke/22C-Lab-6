#pragma once

#include "Hashnode.h"
// Hash table class template
template <typename K, typename V>
class HashTable
{
private:
	HashNode<K, V> **table;//array of hashnode pointers
	int tableSize = 0;
	int filledSize = 0;

	int hashFunc(K key)
	{
		std::string keyString = key.to_string();
		unsigned int sum = 0;
		for (unsigned int ind = 0; ind < keyString.length(); ind++)
		{
			if (keyString.at(ind) >= 48 && keyString.at(ind) <= 57)
				sum += keyString.at(ind) - 48;
		}
		while (sum >= 10)
		{
			std::string sumStr = std::to_string(sum);
			sum = 0;
			for (unsigned int ind = 0; ind < sumStr.length(); ind++)
				sum += sumStr.at(ind) - 48;
		}
		return sum;
	}
public:
	HashTable(int TABLE_SIZE_SET)
	{
		// construct zero initialized hash table of size
		tableSize = TABLE_SIZE_SET;
		table = new HashNode<K, V> *[tableSize];
		for (int ind = 0; ind < tableSize; ind++)
			table[ind] = nullptr;
	}

	~HashTable()
	{
		// destroy all hashnodes
		for (int i = 0; table != nullptr && i < tableSize; i++)
		{
			HashNode<K, V> *entry = table[i];
			if (entry != nullptr)
				delete entry;
			table[i] = nullptr;
		}
		// destroy the hash table
		delete[] table;
		table = nullptr;
	}

	bool get(const K &key, V &value)
	{
		unsigned long hashValue = hashFunc(key);
		HashNode<K, V> *entry;
		int quadProbe = 0;
		int hashIndex = hashValue;
		while (table[hashIndex] != nullptr)
		{
			entry = table[hashIndex];
			if (entry->getKey() == key)
			{
				value = entry->getValue();
				return true;
			}
			quadProbe++;
			hashIndex = (hashValue + quadProbe * quadProbe) % tableSize;
		}
		return false;
	}

	int put(const K &key, const V &value)
	{
		int collisionNum = 0;
		unsigned long hashValue = hashFunc(key);
		V notconst = value;
		HashNode<K, V> *entry;

		entry = new HashNode<K, V>(key, value);
		int quad = 0;
		int hashIndex = 0;
		bool isLooking = true;
		while (isLooking && quad < tableSize)
		{
			hashIndex = (hashValue + quad * quad) % tableSize;
			//std::cout << "QUAD PROBE INFO:" << std::endl;
			//std::cout << "Hash value" << hashIndex << std::endl;
			//std::cout << "Non mod number" << hashValue + quad * quad << std::endl;
			if (table[hashIndex] == nullptr)
			{
				table[hashIndex] = entry;
				isLooking = false;
				filledSize++;
			}
			else collisionNum++;
			quad++;
		}
		if (quad == tableSize)
		{
			return 0; //no collision count since insertion failed
		}

		if (getLoadFactor() >= 0.5)
			extendTable();
		return collisionNum;
	}

	bool remove(const K &key)
	{
		unsigned long hashValue = hashFunc(key);
		std::cout << hashValue << std::endl;
		HashNode<K, V> *entry = table[hashValue];
		int quadProbe = 0;
		int hashIndex = hashValue;

		while (entry != nullptr && entry->getKey() != key)
		{
			quadProbe++;
			hashIndex = (hashValue + quadProbe * quadProbe) % tableSize;
			entry = table[hashIndex];
		}

		if (entry == nullptr)
		{
			return false;
		}
		else
		{
			delete entry;
			table[hashIndex] = nullptr;
			filledSize--;
		}
	}
	double getLoadFactor()
	{
		return ((double)filledSize) / tableSize;
	}
	int getTableSize()
	{
		return tableSize;
	}
	int getUsedSize()
	{
		return filledSize;
	}
	void extendTable()
	{
		HashNode<K, V> **newTable;

		newTable = new HashNode<K, V> *[tableSize * 2];
		for (int ind = 0; ind < tableSize * 2; ind++)
			newTable[ind] = nullptr;

		for (int i = 0;  i < tableSize; i++)
		{
			if (table[i] != nullptr)
			{
				K key = table[i]->getKey();
				V value = table[i]->getValue();
				HashNode<K, V> *entry = new HashNode<K, V>(key, value);
				newTable[i] = entry;
			}
		}

		for (int i = 0; table != nullptr && i < tableSize; i++)
		{
			HashNode<K, V> *entry = table[i];
			if (entry != nullptr)
				delete entry;
			table[i] = nullptr;
		}
		// destroy the hash table
		delete[] table;
		table = nullptr;

		table = newTable;
		tableSize *= 2;
	}
};