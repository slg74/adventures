#include <pcrecpp.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void split_input_line(std::string& pattern, std::vector<string>& pattern_split);
void validate_token(std::vector<string>& pattern_split, std::string& converted_pattern);
void find_matching_lines(std::vector<string> lines, std::string pattern);

const std::string convert_regex(std::string& pattern);
const std::string token_to_regex(std::string token);

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <pattern> " << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string              pattern = argv[1];
    std::vector<string>      lines;         
    std::stringstream        input_file;
    std::string              text_line;

    input_file << std::cin.rdbuf();                      // slurp in the input file...

    while (std::getline(input_file, text_line)) {
        lines.push_back(text_line);                      // this builds our words-lines vector.
    }

    std::string converted_pattern = convert_regex(pattern); 

    find_matching_lines(lines, converted_pattern);

    return(EXIT_SUCCESS);
}


const std::string convert_regex(std::string& pattern) {
    std::vector<string> 	pattern_split;
    std::string                 converted_pattern;

    split_input_line(pattern, pattern_split);
    validate_token(pattern_split, converted_pattern);

    return converted_pattern;
}

void find_matching_lines(std::vector<string> lines, std::string pattern) {
    std::string		phrase;
    pcrecpp::RE		re(pattern);

    for (int i = 0; i < lines.size(); i++) {
        if (re.PartialMatch(lines[i], &phrase)) {
            std::cout << lines[i] << std::endl;
        }
    }
}

void split_input_line(std::string& pattern, std::vector<std::string>& pattern_split) {
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

void validate_token(std::vector<string>& pattern_split, std::string& converted_pattern) {
    pcrecpp::RE			re("%{(.)*}");

    converted_pattern.append("(");                                             
    for (int i=0; i < pattern_split.size(); i++) {
        if (re.PartialMatch(pattern_split[i])) {                               
            converted_pattern.append(token_to_regex(pattern_split[i]) + " ");

        } else {
            converted_pattern.append(pattern_split[i] + " ");
        }
    }
    converted_pattern.erase(converted_pattern.find_last_not_of(" \n\r\t") + 1);   // get rid of whitespace, newlines, carriage returns and tabs.
    converted_pattern.append(")");                                              
}

const std::string token_to_regex(std::string token) {
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
