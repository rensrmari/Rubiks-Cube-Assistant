#include "tester.h"
#include <iostream>
#include <string>

#define RED_FG "\033[91m"
#define GREEN_FG "\033[92m"
#define RESET "\033[0m"

using namespace std;

Tester::Tester() {
    numTests = 0;
}

void Tester::test(const string& test, bool res) {
    numTests++;

    if (!res) {
        failedTests.push_back(test);
    }
}


int Tester::finishTests() const {
    cout << GREEN_FG << "Passed: " << numTests - failedTests.size() << " / " << numTests << endl;
    cout << RED_FG << "Failed: " << failedTests.size() << " / "  << numTests << endl;

    for (const string& test : failedTests) {
        cout << "\t- " << test << endl;
    }

    cout << RESET << endl;

    if (failedTests.size() == 0) {
        return 0;
    } else {
        return 1;
    }
}

