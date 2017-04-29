#include <pcrecpp.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void SplitInputLine(std::string& pattern, std::vector<string>& pattern_split);
void ValidateToken(std::vector<string>& pattern_split, std::string& converted_pattern);
void FindMatchingLines(std::vector<string> lines, std::string pattern);

const std::string RegexConverter(std::string& pattern);
const std::string TokenToRegex(std::string token);

int main(int argc, char **argv) {
    
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <pattern> " << std::endl;
	    exit(EXIT_FAILURE);
    }

    std::string pattern      = argv[1]; 
    std::vector<string>      lines;         
    std::stringstream        input_file;
    std::string              text_line;

    input_file << std::cin.rdbuf();                      // slurp in the input file...

    while (std::getline(input_file, text_line)) {
        lines.push_back(text_line);                      // this builds our words-lines vector.
    }

    std::string converted_pattern = RegexConverter(pattern);

    FindMatchingLines(lines, converted_pattern);

    return(EXIT_SUCCESS);
}


const std::string RegexConverter(std::string& pattern) {

    std::vector<string> 	pattern_split;
    std::string             converted_pattern;

    SplitInputLine(pattern, pattern_split);
    ValidateToken(pattern_split, converted_pattern);

    return converted_pattern;
}

void SplitInputLine(std::string& pattern, std::vector<std::string>& pattern_split) {

    char separator = ' ';

    std::size_t p;
    std::size_t q;

    // TODO - refactor. 
    for (p = 0, q = 0; p != pattern.npos; p = q) {
        pattern_split.push_back(
	    pattern.substr(p + (p != 0),
                      (q = pattern.find(separator, p+1)) - p - (p != 0))
	    );
    }
}

void ValidateToken(std::vector<string>& pattern_split, std::string& converted_pattern) {

    pcrecpp::RE			re("%{(.)*}");

    converted_pattern.append("(");                                             

    for (int i=0; i < pattern_split.size(); i++) {
        if (re.PartialMatch(pattern_split[i])) {                               
            converted_pattern.append(TokenToRegex(pattern_split[i]) + " ");

        } else {
            converted_pattern.append(pattern_split[i] + " ");
        }
    }
    converted_pattern.erase(converted_pattern.find_last_not_of(" \n\r\t") + 1);   // get rid of whitespace, newlines, carriage returns and tabs.

    converted_pattern.append(")");                                              
}

const std::string TokenToRegex(std::string token) {

    std::string converted_word;

    if ((token.length() == 4) || (token.length() == 5)) {
	    converted_word = "(.*)"; 
    } else if (token.length() == 6) {
	    converted_word = std::string("{.}*{") + token[4] + "}"; 
    } else {
	    exit(EXIT_FAILURE);
    }
    return converted_word;
}

void FindMatchingLines(std::vector<string> lines, std::string pattern) {
    std::string		phrase;
    pcrecpp::RE		re(pattern);

    for (int i = 0; i < lines.size(); i++) {
        if (re.PartialMatch(lines[i], &phrase)) {
            std::cout << lines[i] << std::endl;
        }
    }
}
