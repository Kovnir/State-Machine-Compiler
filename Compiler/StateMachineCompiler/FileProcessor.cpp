#pragma once
#include <fstream>
#include <sstream>
#include "Logger.h"

using namespace std;

class FileProcessor
{
public:
	FileProcessor(bool useFakeData, Logger logger) : useFakeData(useFakeData), logger(logger) {}

	bool writeToFile(const string& filePath, const string& input, const string& code) {
		if (!useFakeData)
		{
			ofstream outputFile(filePath, ios::trunc);
			if (!outputFile)
			{
				logger.printError("Failed to open the file for writing");
				return false;
			}

			outputFile << input << endl;
			outputFile << code << endl;
		}
		return true;
	}

	string readFile(const string& filePath) {
		if (useFakeData)
		{

			//
//	string input = R"(  /*
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
			string input;
			try {
				input = readFileWithBOMCheck(filePath);
				input = cleanString(input);
				logger.printInput(input);
				return input;
			}
			catch (const exception& e) {
				logger.printError(e.what());
				return "";
			}
		}
	}

private:
	bool useFakeData;
	Logger logger;

	bool removeBOM(ifstream& file) {
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

	string readFileWithBOMCheck(const string& filePath) {
		ifstream file(filePath, ios::binary);
		if (!file) {
			throw runtime_error("Failed to open the file for reading");
		}

		removeBOM(file);

		stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	string cleanString(const string& str) {
		//to fix double new lines
		string result;
		result.reserve(str.size());
		for (char c : str) {
			if (c != '\r') {
				result += c;
			}
		}
		return result;
	}

};

