#pragma once
#include "TreeNode.cpp"
class CodeGenerator
{
public:
    std::string generate(const StateMachine& stateMachine);
private:
    std::string generateCSharpCode(
        const std::string& stateMachineName,
        const std::vector<std::string>& source,
        const std::vector<std::string>& triggersNames,
        const std::vector<std::string>& triggersDistinations);
};

