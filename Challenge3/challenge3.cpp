#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <regex>
#include <algorithm>

const std::regex symbolRegex("([^\\w\\.\\n])");
const std::regex partRegex("\\d+");

struct Part
{
public:
    int val;
    int colIdx;
    int length;
    
    Part()
    {
        val = -1;
        colIdx = -1;
        length = -1;
    }

    Part(int _val, int _colIdx, int _length)
    {
        val = _val;
        colIdx = _colIdx;
        length = _length;
    }
};

struct Symbol
{
public:
    int startLoc;
    bool isGear;

    Symbol()
    {
        startLoc = -1;
        isGear = false;
    }

    Symbol(int _startLoc, bool _isGear)
    {
        startLoc = _startLoc;
        isGear = _isGear;
    }
};

void findSymbols(std::string line, const int lineCounter, std::map<int,std::vector<Symbol>> &symbols)
{
    std::smatch m;
    int pos = 0;
    std::vector<Symbol> symbolCols;
    while (std::regex_search(line, m, symbolRegex))
    {
        pos += m.position();
        bool isGear = (m.str() == "*");
        symbolCols.push_back(Symbol(pos, isGear));
        line = m.suffix();
        pos++;
    }
    if (symbolCols.size() > 0)
    {
        symbols.insert(std::pair<int,std::vector<Symbol>>(lineCounter, symbolCols));
    }
}

void findParts(std::string line, const int lineCounter, std::map<int,std::vector<Part>> &parts)
{
    std::smatch m;
    int pos = 0;
    std::vector<Part> rowParts;
    while (std::regex_search(line, m, partRegex))
    {
        pos += m.position();
        rowParts.push_back(Part(std::stoi(m.str()), pos, m.str().length()));
        line = m.suffix();
        pos += m.str().length();
    }
    parts.insert(std::pair<int,std::vector<Part>>(lineCounter, rowParts));
}

void parseLine(std::string line, const int lineCounter, std::map<int,std::vector<Symbol>> &symbols, std::map<int,std::vector<Part>> &parts)
{
    findSymbols(line, lineCounter, symbols);
    findParts(line, lineCounter, parts);
}

std::vector<Part> adjacentRowParts(const Symbol &symbol, const std::vector<Part> &partsAtRow)
{
    const int firstValidLoc = symbol.startLoc - 1;
    const int lastValidLoc = firstValidLoc + 2;
    std::vector<Part> adjRowParts(partsAtRow.size());
    auto it = std::copy_if(partsAtRow.begin(), partsAtRow.end(), adjRowParts.begin(), [firstValidLoc, lastValidLoc] (const Part &part) {
        const bool valid = ((firstValidLoc <= part.colIdx+part.length-1) && (part.colIdx <= lastValidLoc));
        return valid;
    });

    adjRowParts.resize(std::distance(adjRowParts.begin(), it));

    return adjRowParts;
}


std::vector<Part> findAdjacentParts(const int rowLoc, const Symbol &symbol, const std::map<int,std::vector<Part>> &parts, const bool gear=false)
{
    const int rowAboveIdx = rowLoc - 1;
    const int rowBelowIdx = rowLoc + 1;
    std::vector<Part> adjacentParts;

    if (gear && !symbol.isGear)
    {
        return adjacentParts; // Return an empty vector.
    }

    for (int rowIdx=rowAboveIdx; rowIdx<rowBelowIdx+1; rowIdx++)
    {
        if (parts.count(rowIdx) > 0)
        {
            const std::vector<Part> partsAtRow = parts.at(rowIdx);
            const std::vector<Part> adjRowParts = adjacentRowParts(symbol, partsAtRow);
            adjacentParts.insert(adjacentParts.begin(), adjRowParts.begin(), adjRowParts.end());
        }
    }

    return adjacentParts;
}

int part1(const std::map<int,std::vector<Part>> &parts, const std::map<int,std::vector<Symbol>> &symbols)
{
    int sum = 0;

    for (auto const &[rowLoc, rowSymbols]: symbols)
    {
        for (const Symbol &symbol: rowSymbols)
        {
            const std::vector<Part> adjacentParts = findAdjacentParts(rowLoc, symbol, parts);
            for (const Part &part: adjacentParts)
            {
                sum += part.val;
            }
        }
    }

    return sum;
}

int part2(const std::map<int,std::vector<Part>> &parts, const std::map<int,std::vector<Symbol>> &symbols)
{
    int sum = 0;

    for (const auto &[rowLoc, rowSymbols]: symbols)
    {
        for (const Symbol &symbol: rowSymbols)
        {
            const std::vector<Part> adjacentGearParts = findAdjacentParts(rowLoc, symbol, parts, true);
            if (adjacentGearParts.size() == 2)
            {
                int gearRatio = adjacentGearParts[0].val * adjacentGearParts[1].val;
                sum += gearRatio;
            }
        }
    }

    return sum;
}

int main()
{
    std::ifstream input("../input.txt");
    std::string line;
    int lineCounter = 0;
    std::map<int, std::vector<Symbol>> symbols;
    std::map<int, std::vector<Part>> parts;

    if (input.is_open())
    {
        while (std::getline(input, line))
        {
            parseLine(line, lineCounter, symbols, parts);
            lineCounter++;
        }
        input.close();
    }

    std::cout << part1(parts, symbols) << std::endl;
    std::cout << part2(parts, symbols) << std::endl;
    return 0;
}