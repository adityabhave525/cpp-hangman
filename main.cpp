#include <iostream>
#include <vector>
#include <string_view>
#include "Random.h"

namespace Settings
{
    constexpr int wrongGuessesAllowed{ 6 };
}

namespace WordList
{
    // Define your list of words here
    std::vector<std::string_view> words{ "mystery", "broccoli" , "account", "almost", "spaghetti", "opinion", "beautiful", "distance", "luggage" };

    std::string_view getRandomWord()
    {
        return words[Random::get<std::size_t>(0, words.size() - 1)];
    }
}

class Session
{
private:
    std::string_view m_word{ WordList::getRandomWord() };
    int m_wrongGuessesLeft{ Settings::wrongGuessesAllowed };
    std::vector<bool> m_letterGuessed{ std::vector<bool>(26) };
    
    std::size_t toIndex(char c) const { return static_cast<std::size_t>((c % 32) - 1); }

public:
    std::string_view getWord() const { return m_word; }

    int wrongGuessesLeft() const { return m_wrongGuessesLeft; }
    void removeGuess() { --m_wrongGuessesLeft; }

    bool isLetterGuessed(char c) const { return m_letterGuessed[toIndex(c)]; }
    void setLetterGuessed(char c) { m_letterGuessed[toIndex(c)] = true; }

    bool isLetterInWord(char c) const
    {
        for (auto ch : m_word)
        {
            if (ch == c)
                return true;
        }

        return false;
    }

    bool won()
    {
        for (auto c : m_word)
        {
            if (!isLetterGuessed(c))
                return false;
        }

        return true;
    }
};

void draw(const Session& s)
{
    std::cout << '\n';

    std::cout << "The word: ";
    for (auto c : s.getWord())
    {
        if (s.isLetterGuessed(c))
            std::cout << c;
        else
            std::cout << "_";
    }

    std::cout << "   Wrong guesses: ";
    for (int i{ 0 }; i < s.wrongGuessesLeft(); ++i)
        std::cout << '+';

    for (char c{ 'a' }; c <= 'z'; ++c)
        if (s.isLetterGuessed(c) && !s.isLetterInWord(c))
            std::cout << c;

    std::cout << '\n';
}

char getGuess(const Session& s)
{
    while (true)
    {
        std::cout << "Enter your next letter: ";

        char c{};
        std::cin >> c;

        if (!std::cin)
        {
            std::cin.clear();
            std::cout << "That wasn't a valid input.  Try again.\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (c < 'a' || c > 'z')
        {
            std::cout << "That wasn't a valid input.  Try again.\n";
            continue;
        }

        if (s.isLetterGuessed(c))
        {
            std::cout << "You already guessed that.  Try again.\n";
            continue;
        }

        return c;
    }
}

void handleGuess(Session& s, char c)
{
    s.setLetterGuessed(c);
    
    if (s.isLetterInWord(c))
    {
        std::cout << "Yes, '" << c << "' is in the word!\n";
        return;
    }

    std::cout << "No, '" << c << "' is not in the word!\n";
    s.removeGuess();
}

int main()
{
    std::cout << "Welcome to C++man (a variant of Hangman)\n";
    std::cout << "To win: guess the word.  To lose: run out of pluses.\n";

    Session s{};

    while (s.wrongGuessesLeft() && !s.won())
    {
        draw(s);
        char c{ getGuess(s) };
        handleGuess(s, c);
    }

    draw(s);

    if (!s.wrongGuessesLeft())
        std::cout << "You lost!  The word was: " << s.getWord() << '\n';
    else
        std::cout << "You won!\n";

    return 0;
}