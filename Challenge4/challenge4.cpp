#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <regex>

constexpr uint LINEPARTS = 3;

const std::regex cardNumRegex("\\d+");

class CardContent
{
public:
    uint cardNum;
    std::set<uint> winningNumbers;
    std::set<uint> actualNumbers;

    CardContent()
    {
        cardNum = -1;
        winningNumbers = {};
        actualNumbers = {};
    }

    CardContent(uint _cardNum, std::set<uint> _winningNumbers, std::set<uint> _actualNumbers)
    {
        cardNum = _cardNum;
        winningNumbers = _winningNumbers;
        actualNumbers = _actualNumbers;
    }

    bool operator==(const CardContent &other) const
    {
        return cardNum == other.cardNum;
    }

    bool operator<(const CardContent &other) const
    {
        return cardNum < other.cardNum;
    }

    std::set<uint> getWinningMatches() const
    {
        std::set<uint> intersection;
        std::set_intersection(
            winningNumbers.begin(), winningNumbers.end(),
            actualNumbers.begin(), actualNumbers.end(),
            std::inserter(intersection, intersection.begin())
        );

        return intersection;
    }

    uint numWinningMatches() const
    {
        return (this->getWinningMatches()).size();
    }

    uint score() const
    {
        const uint nWinningMatches = numWinningMatches();
        if (nWinningMatches == 0)
        {
            return 0;
        }
        else
        {
            return (1 << (nWinningMatches - 1));
        }
    }
};

struct Card
{
    CardContent cardContents;
    uint numCards;

    Card(CardContent _cardContents, uint _numCards)
    {
        cardContents = _cardContents;
        numCards = _numCards;
    }
};

std::array<std::string,LINEPARTS> splitLine(const std::string &line)
{
    // First, split into the three parts.
    std::array<std::string, LINEPARTS> lineArr;
    std::size_t pos = 0;
    std::size_t prev = 0;
    std::size_t idx = 0;
    while ((pos=line.find_first_of(":|", prev)) != -1)
    {
        if (pos > prev)
        {
            lineArr[idx] = line.substr(prev, pos-prev);
        }
        prev = pos + 1;
        idx++;
    }
    if (prev < line.length())
    {
        lineArr[idx] = line.substr(prev, -1);
    }

    return lineArr;
}

uint extractCardNum(const std::string &cardNumString)
{
    std::smatch m;
    std::regex_search(cardNumString, m, cardNumRegex);
    return std::stoul(m.str());
}

std::set<uint> extractNumbers(std::string numsStr)
{
    std::set<uint> nums;
    std::smatch m;
    while (std::regex_search(numsStr, m, cardNumRegex))
    {
        nums.insert(std::stoul(m.str()));
        numsStr = m.suffix();
    }

    return nums;
}

void parseLine(const std::string &line, std::vector<Card> &cards)
{
    const std::array<std::string,LINEPARTS> lineSplit = splitLine(line);

    CardContent newCardContent(
        extractCardNum(lineSplit[0]),
        extractNumbers(lineSplit[1]),
        extractNumbers(lineSplit[2])
    );

    cards.push_back(Card(newCardContent, 1));
}

int part1(const std::vector<Card> &cards)
{
    uint score = 0;
    for (const Card &card: cards)
    {
        score += card.cardContents.score();
    }
    return score;
}

int part2(std::vector<Card> cards)
{
    uint score = 0;
    for (size_t cardIdx=0; cardIdx<cards.size() - 1; cardIdx++)
    {
        const CardContent cardContents = cards[cardIdx].cardContents;
        uint numMatches = cardContents.numWinningMatches();
        for (uint nextCardIdx=cardIdx+1; nextCardIdx<cardIdx+numMatches+1; nextCardIdx++)
        {
            cards[nextCardIdx].numCards += cards[cardIdx].numCards;
        } 
    }

    // Find how many cards we have.
    for (const auto &card: cards)
    {
        score += card.numCards;
    }

    return score;
}

int main()
{
    std::ifstream input("../input.txt");
    std::string line;
    std::vector<Card> cards;

    if (input.is_open())
    {
        while (std::getline(input, line))
        {
            parseLine(line, cards);
        }
        input.close();
    }

    std::cout << part1(cards) << std::endl;
    std::cout << part2(cards) << std::endl;

    return 0;
}