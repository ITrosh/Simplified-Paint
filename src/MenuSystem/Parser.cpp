#include "Parser.h"

using namespace std;

bool Parser::IsOpenLexeme(const string& str) {
    return any_of(_lexemes.begin(), _lexemes.end(), [&](const string& lexeme) {
        return lexeme == str;
    });
}

bool Parser::IsCloseLexeme(const string& str) {
    return any_of(_lexemes.begin(), _lexemes.end(), [&](const string& lexeme) {
        return "END_" + lexeme == str;
    });
}

bool Parser::IsStringValue(const string& str) {
    if (str[0] == '"' && str[str.length() - 1] == '"')
        return true;
    return false;
}

ErrorState Parser::TrySyntaxParse(const string& filename, dictionary& menuScheme) {
    ifstream file(filename);
    int level = 0, i = 1;
    vector<string> tempVec;
    string menuTab;
    for (string tmp; getline(file, tmp); i++) {
        tmp.erase(remove(tmp.begin(), tmp.end(), '\t'), tmp.end());
        if (IsOpenLexeme(tmp)) {
            level++;
        }
        else {
            if (IsCloseLexeme(tmp)) {
                level--;
            }
            else {
                if (IsStringValue(tmp)) {

                    tmp.erase(remove(tmp.begin(), tmp.end(), '"'), tmp.end());
                    if (level == 2) {
                        if (!tempVec.empty()) {
                            menuScheme[menuTab] = tempVec;
                            tempVec.clear();
                            menuTab.clear();
                        }
                        menuTab = tmp;
                    }
                    if (level == 4) {
                        tempVec.emplace_back(tmp);
                    }
                }
                else {
                    cerr << "There is a syntax error in the file: " << filename << " in line: " << i << endl;
                    cerr << "String with syntax error: " << tmp << endl;
                    return ErrorState::SYNTAX;
                }
            }
        }
    }
    if (!tempVec.empty()) {
        menuScheme[menuTab] = tempVec;
        tempVec.clear();
        menuTab.clear();
    }
    return ErrorState::SUCCESS;
}
/*void Parser::printMenuScheme() {
    auto it = _menuScheme.begin();
    for (; it != _menuScheme.end(); it++) {
        cout << it->first << endl;
        for (const auto& popupMenu: it->second) {
            cout << popupMenu << endl;
        }
    }
}*/