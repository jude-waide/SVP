#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <stack>
#include <vector>
#include "Matrix.h"
#include "Vector.h"
#include "algorithms.h"
#include <iomanip>

using namespace std;
using namespace VecMaths;

int main(int argc, char* argv[]) {
    // Characters will be added to the stack
    stack<char> stack;
    vector<Vector<double>> vectorList;
    int column = 0;
    // Ignore first argument as it is the command itself rather than a program parameter
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        // Iterates over every character
        for (const auto &item : arg) {
            if (item != ']' && item != ',') stack.push(item);      // Add all characters which aren't a ] to the stack
            if (item == ']') {                                     // When a ] is encountered, process all previous characters until the next [
                char digit = '\0';          // Stores the current character
                string number;         // Stores the current number
                vector<double> numbers; // Stores the list of numbers in the current []

                while (digit != '[') {
                    if (stack.empty()) {    // Means the stack has been emptied without encountering the expected opening bracket
                        cerr << "Invalid arguments: ] with no [" << endl;
                        return 1;
                    }

                    digit = stack.top();    // Get the next character
                    stack.pop();

                    if (digit == ' ' || digit == '[') {             // In case of a space or opening bracket, current number is finished and it should be added to the numbers list
                        if (number.empty()) {
                            continue;
                        }
                        try {
                            numbers.push_back(stod(number));   // Convert to a float
                        }
                        catch (invalid_argument& err) {
                            cerr << "Invalid input - tried to convert '" << number << "' to a float" << endl;
                        }
                        number = "";
                    } else {
                        number = digit + number;    // Add the digit to the beginning of number as reading backwards
                    }
                }

                // Will be empty if it's the outer brackets
                if (!numbers.empty()) {
                    Vector<double> vec(numbers.size());
                    // Assign valuePointer to BaseVector in reverse order, since valuePointer were added to numbers in reverse
                    for (unsigned int j = 0; j < numbers.size(); j++) {
                        vec[j] = numbers[numbers.size()-1-j];
                    }
                    column += 1;
                    vectorList.push_back(vec);
                }
            }
        }
        // Add space to signify break between numbers
        stack.push(' ');
    }
    // Should be a single space left at the end
    while (!stack.empty()) {
        if (stack.top() == ' ') stack.pop();
        else {
            cerr << "Invalid arguments: [ with no ]" << endl;
            return 1;
        }
    }
    if (!stack.empty()) {
        cerr << "Invalid arguments: [ with no ]" << endl;
        return 1;
    }

    if (vectorList.empty()) {
        cerr << "Invalid arguments: no vectors" << endl;
        return 2;
    }

    // Create a matrix from the input vectors
    Matrix<double> M(vectorList);

    if (M.getColumnCount() != M.getRowCount()) {
        cerr << "Invalid arguments: matrix not square" << endl;
        return 3;
    }

    double solution = svp(M);

    // Write result
    ofstream resultFile("result.txt");
    resultFile << setprecision(20) << solution;
    resultFile.close();
    return 0;
}

