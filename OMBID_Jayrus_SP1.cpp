#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class GridPuzzleSolver {
private:
    vector<vector<char>> grid;
    vector<vector<int>> colClues;
    vector<vector<int>> rowClues;
    vector<vector<vector<char>>> allRowPatterns;

public:
    // Constructor
    GridPuzzleSolver(vector<vector<int>>& rows, vector<vector<int>>& cols) {
        rowClues = rows;
        colClues = cols;
        grid = vector<vector<char>>(rowClues.size(), vector<char>(colClues.size(), '.'));
        
        generateAllRowPatterns(rowClues, colClues, allRowPatterns);
    }

    // Display grid
    void printColClues(int level, int maxRowSize) {
        bool hasMore = false;

        for (int i = 0; i < maxRowSize; i++) cout << "  ";

        for (const auto& clue : colClues) {
            if (level < clue.size()) {
                cout << clue[level] << " ";
                if (level + 1 < clue.size()) hasMore = true;
            } else {
                cout << "  ";
            }
        }

        cout << endl;

        if (hasMore) {
            printColClues(level + 1, maxRowSize);
        }
    }

    void printGrid() {
        int maxRowSize = 0;
        for (const auto& row : rowClues) {
            maxRowSize = max(maxRowSize, (int)row.size());
        }

        printColClues(0, maxRowSize);

        for (int i = 0; i < grid.size(); i++) {

            for (int j = 0; j < maxRowSize - rowClues[i].size(); j++)
                cout << "  ";

            for (int clue : rowClues[i])
                cout << clue << " ";

            for (char cell : grid[i])
                cout << cell << " ";

            cout << endl;
        }
    }

    // Row Validation
    bool isValidRow(const vector<vector<char>>& grid, const vector<vector<int>>& rowClues) {
        
        for (int i = 0; i < grid.size(); i++) {

            vector<int> curr_row = rowClues[i];
            int iterator = 0;
            vector<char> hashtags;

            for (int j = 0; j < grid[i].size(); j++) {
                if (grid[i][j] == '#') {
                    if (iterator == curr_row.size() || hashtags.size() > curr_row[iterator]) {
                        return false;
                    }
                    hashtags.push_back(grid[i][j]);
                } else {
                    if (!hashtags.empty()) {
                        if (hashtags.size() != curr_row[iterator])
                            return false;

                        hashtags = {};
                        iterator++;
                    }
                }
            }

            if (!hashtags.empty()) {
                if (iterator >= curr_row.size() || hashtags.size() != curr_row[iterator])
                    return false;
                iterator++;
            }
            if (iterator != curr_row.size())
                return false;
        }
        return true;
    }

    // Generate rows
    void generateRowPatternsHelper(int index,
            const vector<int>& clue,
            vector<char>& row,
            int pos,
            int colSize,
            vector<vector<char>>& row_patterns) {

        if (index == clue.size()) {
            for (int i = pos; i < colSize; i++) {
                row[i] = '.';
            }
            row_patterns.push_back(row);
            return;
        }

        int remainingBlocks = 0;
        for (int i = index; i < clue.size(); i++) {
            remainingBlocks += clue[i];
        }

        int remainingSpaces = clue.size() - (index + 1);

        for (int start = pos; start <= colSize - (remainingBlocks + remainingSpaces); start++) {
            for (int i = pos; i < start; i++) {
                row[i] = '.';
            }

            for (int i = 0; i < clue[index]; i++) {
                row[start + i] = '#';
            }

            int nextPos = start + clue[index];

            if (index < clue.size() - 1) {
                row[nextPos] = '.';
                nextPos++;
            }

            generateRowPatternsHelper(index + 1, clue, row, nextPos, colSize, row_patterns);
        }
    }

    vector<vector<char>> generateRowPatterns(const vector<int>& clue, int colSize) {

        vector<vector<char>> row_patterns;
        vector<char> row(colSize, '.');

        generateRowPatternsHelper(0, clue, row, 0, colSize, row_patterns);

        return row_patterns;
    }

    void generateAllRowPatterns(const vector<vector<int>>& rowClues,
        const vector<vector<int>>& colClues,
        vector<vector<vector<char>>>& allRowPatterns) {

        int colSize = colClues.size();

        for (const auto& clue : rowClues) {
            allRowPatterns.push_back(generateRowPatterns(clue, colSize));
        }
    }

    // Validate partially filled columns
    bool isValidPartialColumn(const vector<vector<char>>& grid,
        const vector<vector<int>>& colClues,
        int currentRow) {

        int cols = grid[0].size();

        for (int col = 0; col < cols; col++) {

            int clueIndex = 0;
            int count = 0;

            for (int row = 0; row <= currentRow; row++) {

                if (grid[row][col] == '#') {
                    count++;

                    if (clueIndex >= colClues[col].size()) return false;
                    if (count > colClues[col][clueIndex]) return false;

                } else {
                    if (count > 0) {
                        clueIndex++;
                        count = 0;
                    }
                }
            }

            if (count > 0) {
                if (clueIndex >= colClues[col].size()) return false;
                if (count > colClues[col][clueIndex]) return false;
            }
        }
        return true;
    }

    // Recursive solver
    void Solver(int currentRow,
        vector<vector<char>>& grid,
        const vector<vector<vector<char>>>& allRowPatterns,
        const vector<vector<int>>& colClues,
        const vector<vector<int>>& rowClues) {
        
        // checks if rows matches with columns
        if (currentRow == grid.size()) {
            if (isValidRow(grid, rowClues)) {
                printGrid();
                cout << endl;
            }
            return;
        }

        for (const auto& row_pattern : allRowPatterns[currentRow]) {
            grid[currentRow] = row_pattern;

            if (isValidPartialColumn(grid, colClues, currentRow)) {
                Solver(currentRow + 1, grid, allRowPatterns, colClues, rowClues);
            }
        }
    }

    void start() {
        Solver(0, grid, allRowPatterns, colClues, rowClues);
    }
};

int main() {
    vector<vector<int>> rowClues = { {3}, {1}, {1,1}, {3}, {4} };
    vector<vector<int>> colClues = { {3}, {2}, {1,2}, {1,1}, {3} };

    GridPuzzleSolver puzzle(rowClues, colClues);
    puzzle.start();

}