#pragma once
#include <fstream>
#include <sstream>
#include "Logger.h"

class FileProcessor
{
public:
	FileProcessor(bool useFakeData, Logger logger) : useFakeData(useFakeData), logger(logger) {}

	bool writeToFile(const std::string& filePath, const std::string& input, const std::string& code) {
		if (!useFakeData)
		{
			std::ofstream outputFile(filePath, std::ios::trunc);
			if (!outputFile)
			{
				logger.printError("Failed to open the file for writing");
				return false;
			}

			outputFile << input << std::endl;
			outputFile << code << std::endl;
		}
		return true;
	}

	std::string readFile(const std::string& filePath) {
		if (useFakeData)
		{

			//
//	std::string input = R"(  /*
//    * TestStateMachine
//    *   IdleState default
//    *     on Play -> RunningState    
//    *   RunningState // some comment
//    *     on Pause -> PausedState    
//    */
//)";
			return cleanString("/*\n\
	* TestStateMachine\n\
	*   IdleState default\n\
	*     on Play -> RunningState\n\
	*   RunningState // some comment\n\
	*     on Pause -> PausedState    \n\
	*     on Finish -> FinishedState \n\
	*     on Stop ->    IdleState\n\
	*   PausedState\n\
	*     on Resume -> RunningState\n\
	*     on Stop -> IdleState\n\
	*   FinishedState\n\
	*     on Replay -> RunningState\n\
	*/\n");
		}
		else {
			std::string input;
			try {
				input = readFileWithBOMCheck(filePath);
				input = cleanString(input);
				logger.printInput(input);
				return input;
			}
			catch (const std::exception& e) {
				logger.printError(e.what());
				return "";
			}
		}
	}

private:
	bool useFakeData;
	Logger logger;

	bool removeBOM(std::ifstream& file) {
		char bom[3] = { 0 };
		file.read(bom, 3);

		// Check if the file starts with the UTF-8 BOM
		if (bom[0] == '\xEF' && bom[1] == '\xBB' && bom[2] == '\xBF') {
			// BOM found, no need to put back the read characters
			return true;
		}
		else {
			// No BOM found, put back the read characters
			file.seekg(0);
			return false;
		}
	}

	std::string readFileWithBOMCheck(const std::string& filePath) {
		std::ifstream file(filePath, std::ios::binary);
		if (!file) {
			throw std::runtime_error("Failed to open the file for reading");
		}

		removeBOM(file);

		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	std::string cleanString(const std::string& str) {
		//to fix double new lines
		std::string result;
		result.reserve(str.size());
		for (char c : str) {
			if (c != '\r') {
				result += c;
			}
		}
		return result;
	}

};

