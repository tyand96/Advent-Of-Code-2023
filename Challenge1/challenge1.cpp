#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <regex>
#include <vector>
#include <numeric>

const std::map<std::string, int> numbers = {
    {"one", 1},
    {"two", 2},
    {"three", 3},
    {"four", 4},
    {"five", 5},
    {"six", 6},
    {"seven", 7},
    {"eight", 8},
    {"nine", 9}
};

const std::regex e_part2("\\d|(one)|(two)|(three)|(four)|(five)|(six)|(seven)|(eight)|(nine)");
const std::regex e_part1("\\d");

/**
 * Finds the numbers in the line that is being read.
 * 
 * For part 2, the numbers can be words as well.
 * 
 * @param line The line being read.
 * @param part2 If this is being calculated for part2.
 * @returns A vector of the numbers extracted from the line as strings.
*/
std::vector<std::string> findNumberStrings(std::string line, const bool part2)
{
    std::vector<std::string> matches;
    std::vector<std::string> numberStrings = {"", ""};
    std::smatch m;
    while (std::regex_search(line, m, part2 ? e_part2 : e_part1))
    {
        if (m.str().find_first_not_of(' ') != -1)
        {
            matches.push_back(m.str());
            line = m.suffix().str();
        }
    }

    if (matches.size() == 0)
    {
        abort();
    }

    numberStrings[0] = matches.front();
    numberStrings[1] = matches.back();

    return numberStrings;
}

/**
 * Converts the extracted number string into an interger.
 * 
 * @param numString The number extracted from the line as a string.
 * @returns An integer of the number extracted from the line.
*/
int convertToNumber(const std::string &numString)
{
    try
    {
        return numbers.at(numString);
    }
    catch(const std::out_of_range &e)
    {
        return std::stoi(numString);
    }
}

/**
 * Finds two numbers from the line.
 * 
 * If it is part 2, then the numbers can be words.
 * 
 * @param line The line being read.
 * @param part2 A boolean indicating if this is for part 2 or not.
 * @returns A vector of integers read from the line.
*/
std::vector<int> findNumbers(const std::string &line, const bool part2)
{
    std::vector<int> numbers = {-1'000, -1'000};
    std::vector<std::string> s_firstNums = findNumberStrings(line, part2);

    numbers[0] = convertToNumber(s_firstNums[0]);
    numbers[1] = convertToNumber(s_firstNums[1]);

    return numbers;
}

/**
 * Extract the calibration value from the line.
 * 
 * This will concatenate the first and last numbers that appear in the line.
 * If there only exists one number, then this will return that integer twice.
 * 
 * 
 * @param line The line being parsed.
 * @param part2 A boolean indicating if this is for part 2.
 * @return The calibration value extracted.
*/
int extractCalibrationValue(const std::string &line, const bool part2=false)
{
    const std::vector<int> numberVals = findNumbers(line, part2);
    const int firstVal = numberVals[0];
    const int secondVal = numberVals[1];
    const int finalVal = firstVal*10 + secondVal;


    return finalVal;
}

int main()
{
    std::ifstream input("../input.txt");
    std::string line;
    int sum_part1 = 0;
    int sum_part2 = 0;
    while (input >> line)
    {
        sum_part1 += extractCalibrationValue(line, false);
        sum_part2 += extractCalibrationValue(line, true);
    }

    std::cout << "Part 1: " << sum_part1 << ". Part 2: " << sum_part2 << std::endl;
}