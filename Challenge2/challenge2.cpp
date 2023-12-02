#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

const int maxNumReds = 12;
const int maxNumGreens = 13;
const int maxNumBlues = 14;

const std::regex redRegex("(\\d+)\\sred");
const std::regex blueRegex("(\\d+)\\sblue");
const std::regex greenRegex("(\\d+)\\sgreen");

struct CubeColors
{
    int numReds = 0;
    int numBlues = 0;
    int numGreens = 0;
};

struct Round
{
public:
    CubeColors colors;
    Round(const std::string &roundStr)
    {
        std::smatch m;
        if (std::regex_search(roundStr, m, redRegex))
        {
            colors.numReds = std::stoi(m[1].str());
        }

        if (std::regex_search(roundStr, m, blueRegex))
        {
            colors.numBlues = std::stoi(m[1].str());
        }

        if (std::regex_search(roundStr, m, greenRegex))
        {
            colors.numGreens = std::stoi(m[1].str());
        }
    }
};


struct Game
{
public:
    int gameId;
    std::vector<Round> rounds;
    Game(const std::string &line)
    {
        gameKeyword = "Game";
        const std::vector<std::string> parsedLine = splitLine(line);
        gameId = getGameId(parsedLine[0]); // The first contains the game id.
        for (int i=1; i<parsedLine.size(); i++) // No need to look at the first element anymore.
        {
            rounds.push_back(Round(parsedLine[i]));
        }
    }

private:
    std::string gameKeyword;
    std::vector<std::string> splitLine(const std::string &line)
    {
        std::vector<std::string> parsedLines;
        std::size_t prev=0, pos;
        while ((pos=line.find_first_of(";:", prev)) != -1)
        {
            if (pos > prev)
            {
                parsedLines.push_back(line.substr(prev, pos-prev));
            }
            prev = pos + 1;
        }
        if (prev < line.length())
        {
            parsedLines.push_back(line.substr(prev, -1));
        }

        return parsedLines;
    }

    int getGameId(std::string gameIdStr)
    {
        gameIdStr = gameIdStr.substr(gameKeyword.length(), -1);
        return std::stoi(gameIdStr);
    }

};

bool isValidGame(const Game &game)
{
    for(Round round: game.rounds)
    {
        if ((round.colors.numReds > maxNumReds) ||
            (round.colors.numBlues > maxNumBlues) ||
            (round.colors.numGreens > maxNumGreens)
        )
        {
            return false;
        }
    }

    return true;
}

CubeColors getMaxColors(const Game &game)
{
    CubeColors colors;
    for (Round round: game.rounds)
    {
        if (round.colors.numReds > colors.numReds)
        {
            colors.numReds = round.colors.numReds;
        }

        if (round.colors.numBlues > colors.numBlues)
        {
            colors.numBlues = round.colors.numBlues;
        }

        if (round.colors.numGreens > colors.numGreens)
        {
            colors.numGreens = round.colors.numGreens;
        }
    }

    return colors;
}



int part1(const std::vector<Game> &games)
{
    int score = 0;
    for (Game game: games)
    {
        if (isValidGame(game))
        {
            score += game.gameId;
        }
    }

    return score;
}

int part2(const std::vector<Game> &games)
{
    int score = 0;
    std::vector<CubeColors> maxColors;
    for (Game game: games)
    {
        maxColors.push_back(getMaxColors(game));
    }

    for (CubeColors maxGameColor: maxColors)
    {
        int gameScore = maxGameColor.numReds * maxGameColor.numBlues * maxGameColor.numGreens;
        score += gameScore;
    }

    return score;
}

int main()
{
    std::ifstream input("../input.txt");
    std::string line;
    std::vector<Game> games;
    int gameSum = 0;


    if (input.is_open())
    {
        while (std::getline(input, line))
        {
            games.push_back(Game(line));
        }
        input.close();
    }

    std::cout << "Part 1: " << part1(games) << std::endl;
    std::cout << "Part 2: " << part2(games) << std::endl;


    return 0;
}