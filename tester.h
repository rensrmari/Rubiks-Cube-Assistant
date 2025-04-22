#ifndef TESTER_H
#define TESTER_H

#include <vector>
#include <string>
using namespace std;

class Tester {
public:
    /**
     * Constructor for Tester.
     */
    Tester();

    /**
     * Checks if a test has passed, storing failing tests.
     * @param test What is being tested.
     * @param res The result of the test (false is fail, true is pass).
     */
    void test(const string& test, bool res);

    /**
     * Prints the results of the tests and returns an integer representing the status.
     * @return 0 if all the tests pass, 1 if not.
     */
    int finishTests() const;
private:
    int numTests;
    vector<string> failedTests;
};

#endif