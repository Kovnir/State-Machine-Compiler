#pragma once
#include "TreeNode.cpp"
class CodeGenerator
{
public:
    // Disallow creating an instance of this object
    CodeGenerator() = delete;
    static std::string generate(const StateMachine& stateMachine);
private:
    static std::string generateCSharpCode(
        const std::string& stateMachineName,
        const std::string& defaultStateName,
        const std::vector<std::string>& allStates,
        const std::vector<std::string>& source,
        const std::vector<std::string>& triggersNames,
        const std::vector<std::string>& triggersDistinations);
    static void checkForDuplicates(const std::vector<std::string>& stateNames);
};

