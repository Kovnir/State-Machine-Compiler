#pragma once
#include "TreeNode.cpp"

using namespace std;

class CodeGenerator
{
public:
    // Disallow creating an instance of this object
    CodeGenerator() = delete;
    static string generate(const StateMachine& stateMachine);
private:
    static string generateCSharpCode(
        const string& stateMachineName,
        const string& defaultStateName,
        const vector<string>& allStates,
        const vector<string>& source,
        const vector<string>& triggersNames,
        const vector<string>& triggersDistinations);
    static void checkForDuplicates(const vector<string>& stateNames);
};

