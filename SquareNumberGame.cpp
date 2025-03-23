#include <algorithm> // std::find, std::min_element
#include <cmath> // std::abs
#include <cstddef> // std::size_t
#include <iostream>
#include <vector>
#include "Random.h"

using Numbers = std::vector<int>;

namespace config
{
	constexpr int multiplierMin{ 2 };
	constexpr int multiplierMax{ 6 };
}

// Generates @count numbers starting at @start*@start and multiplies
// every square number by @multiplier.
Numbers generateNumbers(int start, int count, int multiplier)
{
	Numbers numbers(static_cast<std::size_t>(count));

	for (int index{ 0 }; index < count; ++index)
	{
		std::size_t uindex{ static_cast<std::size_t>(index) };
		numbers[uindex] = (start + index) * (start + index) * multiplier;
	}

	return numbers;
}

// Asks the user to input starting number, then generates array of numbers
Numbers setupGame()
{
	int start{};
	std::cout << "Start Where? ";
	std::cin >> start;

	int count{};
	std::cout << "How many? ";
	std::cin >> count;

	int multiplier{ Random::get(config::multiplierMin, config::multiplierMax) };

	std::cout << "I generated " << count << " square numbers. Do you know what each number is after multiplying it by " << multiplier << "?\n";

	return generateNumbers(start, count, multiplier);
}

// Return the user's guess
int getUserGuess()
{
	int guess{};
	
	std::cout << "> ";
	std::cin >> guess;

	return guess;
}

// Searches for the value @guess in @numbers and removes it.
// Returns true if the value was found. False otherwise.

bool findAndRemove(Numbers& numbers, int guess)
{
	auto found{ std::find(numbers.begin(), numbers.end(), guess) };

	if (found == numbers.end())
		return false;

	numbers.erase(found);
	return true;
}

// Finds the value in @numbers that is closest to @guess.
int findClosestNumber(const Numbers& numbers, int guess)
{
	return *std::min_element(numbers.begin(), numbers.end(),
		[=](int a, int b)
		{
			return std::abs(a - guess) < std::abs(b - guess);
		}
	);
}

// Called when the user guesses a number correctly.
void printSuccess(const Numbers& numbers)
{
	std::cout << "Nice! ";

	if (numbers.size() == 0)
	{
		std::cout << "You found all numbers, good job!\n";
	}
	else
	{
		std::cout << numbers.size() << " numbers(s) left.\n";
	}
}

// Called when the user guesses a number that is not in the numbers.
void printFailure(const Numbers& numbers, int guess)
{
	int closest{ findClosestNumber(numbers, guess) };

	std::cout << guess << " is wrong!\n";

	std::cout << "Try " << closest << " next time.\n";
}

int main()
{
	Numbers numbers{ setupGame() };

	while (true)
	{
		int guess{ getUserGuess() };

		if (!findAndRemove(numbers, guess))
		{
			printFailure(numbers, guess);
			break;
		}

		printSuccess(numbers);
		if (numbers.size() == 0)
			break;
	}

	return 0;
}