/*
	Author: Evren Keskin

	Date: 12/03/2018

	Description: 
		This program will create a hash table database of people 
		from a file of data
		It will then navigate that database to find people 
		the user is searching for
*/
#include <iostream>
#include <string>
#include <fstream>

#include "Person.h"
#include "Menu.h"
#include "HashTable.h"

int main()
{
	int numData = 0;
	std::ifstream inputFile;
	std::string fileName = "";
	std::string options[] =
	{
		"Search a person by their birthday",
		"Exit Program"
	};
	int optionsLength = 2;
	Menu birthdaySearch = Menu(options, optionsLength);
	bool usingProgram = true;
	while (usingProgram && !inputFile.is_open())
	{
		std::cout << "Enter file directory of source file:" << std::endl;
		fileName = birthdaySearch.takeStringInput();
		inputFile.open(fileName);
		if (!inputFile)
		{
			std::cout << "File not found" << std::endl;
			std::cout << "Do you want to keep searching for the file(y/n)?" << std::endl;
			char answer = birthdaySearch.takeCharInput();
			if (answer == 'n' || answer == 'N')
				usingProgram = false;
		}
	}
	if (!usingProgram)
		return 0;
	while (inputFile)
	{
		if (inputFile.eof()) break;
		std::string empty = "";
		getline(inputFile, empty, '\n');
		getline(inputFile, empty, '\n');
		numData++;
	}
	inputFile.close();

	HashTable<Birthday, Person> people = HashTable<Birthday, Person>(numData);
	int numOfCollisions = 0;

	std::string name = "names";
	std::string birthdayStr = "birthdays";
	Birthday birthday;

	inputFile.open(fileName);
	while (inputFile)
	{
		if (inputFile.eof()) break;
		getline(inputFile, name, '\n');
		if (inputFile.eof()) break;
		getline(inputFile, birthdayStr, '\n');
		birthday = birthday.stoBirthday(birthdayStr);
		Person newNamePerson = Person(name, birthday);

		numOfCollisions += people.put(birthday, newNamePerson);
		newNamePerson.~Person();
	}
	inputFile.close();

	
	
	int answer = 0;
	int inputInt = 0;
	Person result = Person();
	while (answer != optionsLength)
	{
		std::cout << "Hashtable has been made " << std::endl;
		std::cout << "Data Items : " << people.getUsedSize() << std::endl;
		std::cout << "Collisions : " << numOfCollisions << std::endl;
		std::cout << "Load factor: " << people.getLoadFactor() * 100 << "%" << std::endl;
		answer = birthdaySearch.printOptionsList();
 		switch (answer)
		{
		case 1:
			std::cout << "Enter the birthday to search as the hashkey;" << std::endl;
			birthdayStr = "";
			std::cout << "Enter the new persons birthday year (1900-2018):" << std::endl;
			inputInt = birthdaySearch.takeRangedIntInput(1900, 2018);
			birthdayStr += std::to_string(inputInt) + "-";
			std::cout << "Enter the birthday to search as the hashkey;" << std::endl;
			std::cout << birthdayStr << std::endl;
			std::cout << "Enter the new persons birthday month (1-12):" << std::endl;
			inputInt = birthdaySearch.takeRangedIntInput(1, 12);
			if (inputInt < 10)
				birthdayStr += '0';
			birthdayStr += std::to_string(inputInt) + "-";
			std::cout << "Enter the birthday to search as the hashkey;" << std::endl;
			std::cout << birthdayStr << std::endl;
			std::cout << "Enter the new persons birthday date (1-31):" << std::endl;
			inputInt = birthdaySearch.takeRangedIntInput(1, 31);
			if (inputInt < 10)
				birthdayStr += '0';
			birthdayStr += std::to_string(inputInt);
			
			if (people.get(birthday.stoBirthday(birthdayStr), result))
			{
				std::cout << "Your key has found:" << std::endl;
				std::cout << result.getName() << std::endl;
				std::cout << "With birthday: " << std::endl;
				std::cout << result.getBirthday().to_string() << std::endl;
			}
			else
			{
				std::cout << "Invalid key. Unable to find anyone in the hashtable" << std::endl;
			}
			birthdaySearch.pause();
			break;
		case 2:
			break;
		default:
			break;
		}
	}
	return 0;
}