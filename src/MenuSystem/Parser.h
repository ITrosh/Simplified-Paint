#ifndef MAIN_CPP_PARSER_H
#define MAIN_CPP_PARSER_H

#include "Library.h"
#include <SDL.h>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>

// map[MainButtonName, vector[PopupButtonName]]
using dictionary = std::map<std::string, std::vector<std::string>>;

class Parser {
public:
    Parser() = default;

    static ErrorState TrySyntaxParse(const std::string& filename, dictionary& menuScheme);

    //void printMenuScheme();
private:
    inline static const std::vector<std::string> _lexemes = {"MENU", "TAB", "POPUP_MENU", "POPUP_TAB"};

    static bool IsOpenLexeme(const std::string& str);
    static bool IsCloseLexeme(const std::string& str);
    static bool IsStringValue(const std::string& str);
};

#endif //MAIN_CPP_PARSER_H