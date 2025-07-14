#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <regex>
#include <algorithm>
#include <memory>
using namespace std;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////****              lexical analyzer  start                  ****///////////////
//**** This Part contain reading of finite automata which is in table form (transition table) made on excel and downloaded as csv file ****// 
//**** and then creation of mapping table and advance table for identification of keywords, identifiers and literals  ****// 
//**** and make tables of it like compiler in lexical analyzer phase and make tokens and store it for further use  ****//
//**** write your test code in testcode.txt to beginmaking tokens of it ****// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ROWS 53  
#define COLS 27

int final[] = { 1,2,3,4,5,6,8,9,46,48,12,13,14,15,19,20,22,23,24,26,27,29,30,31,34,35,37,38,39,40,42,45,50 };
string transitionTable[ROWS][COLS];
string keyword[] = {
        "asm", "Wagarna", "new", "this", "auto", "enum", "operator", "throw", "Mantiqi", "explicit", "private",
        "True", "break", "export", "protected", "try", "case", "extern", "public", "typedef", "catch", "False", "register", "typeid",
        "Harf", "Ashriya", "typename", "Adadi", "class", "for", "Wapas", "union", "const", "dost", "short", "unsigned", "goto",
        "signed", "using", "continue", "Agar", "sizeof", "virtual", "default", "inline", "static", "Khali", "delete", "volatile", "do",
        "long", "struct", "double", "mutable", "switch", "while", "namespace", "template", "Marqazi", "Matn", "input->", "output<-"
};


char getNextChar(const string& input, int& position) {
    if (position < input.size()) {
        return input[position++];
    }
    return '\0';
}

bool checkfinal(int state) {
    int size = sizeof(final) / sizeof(final[0]);
    for (int i = 0; i < size; i++) {
        if (state == final[i]) {
            return true;
        }
    }
    return false;
}

bool checkkeyword(string str) {
    int size = sizeof(keyword) / sizeof(keyword[0]);
    for (int i = 0; i < size; i++) {
        if (str == keyword[i]) {
            return true;
        }
    }
    return false;
}


void readCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open the file!\n";
        return;
    }

    string line;
    int row = 0;

    while (getline(file, line) && row < ROWS) {
        stringstream ss(line);
        string value;
        int col = 0;

        while (getline(ss, value, ',') && col < COLS) {
            transitionTable[row][col] = value;
            col++;
        }
        row++;
    }
    file.close();
}




void displayTable(const string& code) {
    int advance[ROWS][COLS];
    int mappingtable[ROWS][COLS];

    //////////////////// alphabets /////////////////////

    string alphabets[COLS];
    string states[ROWS];
    int count = 0;
    for (int j = 1; j < COLS; j++)
    {
        alphabets[count] = transitionTable[0][j];
        count++;
    }
     /*for (int j = 0; j < COLS; j++)
     {
         cout << alphabets[j] <<" ";
     }
     cout << endl;*/
     ////////////////////////// states //////////////////
    count = 0;
    for (int i = 1; i < ROWS; i++)
    {
        states[count] = transitionTable[i][0];
        count++;
    }

    //////////////////////////// mapping /////////////////////

    for (int i = 0; i < COLS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (transitionTable[0][i] == alphabets[j] && !transitionTable[0][i].empty())
            {
                mappingtable[0][i] = j;
                break;
            }
            else
            {
                mappingtable[0][i] = -9;
            }
        }
    }

    for (int i = 1; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++)
        {
            for (int z = 0; z < ROWS; z++) {
                if (transitionTable[i][j] == states[z] && !transitionTable[i][j].empty())
                {
                    mappingtable[i][j] = z;
                    break;
                }
                else
                {
                    mappingtable[i][j] = -9;
                }

            }
        }
    }


    //cout << "\n*********************************mapping table**********************************\n\n";

    /*for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {

            cout << mappingtable[i][j] << "\t";
        }
        cout << endl << endl;
    }*/




    ///////////////////////////////////// ADVANCE TABLE ////////////////////////////////////



    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (i == 0 || j == 0) {
                advance[i][j] = mappingtable[i][j];
            }
            else if (mappingtable[i][j] != -9)
            {
                advance[i][j] = 1;
            }
            else {
                advance[i][j] = -9;
            }
        }
    }


    int colm = 1;
    for (int j = 1; j < ROWS; j++) {
        if (mappingtable[j][colm] != -9) {
            advance[j][colm] = 0;
        }
    }


    /*cout << "\n************************* Advance Table ****************************\n\n";
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
             cout << advance[i][j] << "\t";
        }
        cout << endl << endl;
    }*/


    ///////////////////////////////////////////// files /////////////////////////////////

    bool symbol1 = false;
    bool keyword1 = false;


    bool literal1 = false;

    ofstream symbol("symbol_table.txt");
    ofstream keyword("keyword_table.txt");
    ofstream literal("literal_table.txt");
    ofstream tokens("tokens_table.txt");
    ofstream error("error_table.txt");

    // Check if files are opened successfully
    if (!symbol.is_open() || !keyword.is_open() || !literal.is_open() || !tokens.is_open()) {
        cerr << "Error: Unable to open one or more files!" << endl;
        return;
    }

    // Write headers
    symbol << "Position\tValue\n";
    keyword << "Position\tValue\n";
    literal << "Position\tValue\n";
    tokens << "Token Table\n";


    int sympos = 1;
    int keypos = 1;
    int litpos = 1;


    //////////////////////////////////////////////// DRIVER ///////////////////////////////////////////////


    vector<string> keywordchecking;
    vector<string> symbolchecking;

    bool inout = false;
    bool adv2 = false;
    bool numbe = false;

    int st = 0;
    int alph = 0;
    int pos = 0;
    string storestring = "";



    string input = code;
    char chr = '#';


    bool lit = false;
    int litsecond = 0;

    string literalValue = "\"";

    bool aterror = false;

    bool chother = false;

    bool literror = false;

    bool checkingsecondcomma = true;

    while (chr != '\0')
    {

        numbe = false;
        keyword1 = false;
        inout = false;
        aterror = false;
        literalValue = "";
        literal1 = false;
        st = 0;
        checkingsecondcomma = true;
        chr = getNextChar(input, pos);

        if (chr == '"') {
            litsecond++;  
        }

        if (chr == '"' && litsecond % 2 != 0) {
            literror = true;
            lit = true;
            literalValue = "";
            int position = pos;
            while (lit) {
                chr = getNextChar(input, pos);
                cout << literalValue;
                if (chr == '"') {
                    lit = false;
                    break;
                }

                if (chr == '\0') {
                    cout << "error: String not closed" << endl;
                    error << "error could not find second \" in code \n";
                    lit = false;
                    //break;
                    checkingsecondcomma = false;
                    pos = position;
                }

                literalValue += chr;

                literal1 = true;
            }

            if (literal1 && checkingsecondcomma == true) {
                literal << litpos << "\t\t" << literalValue << "\n";
                tokens << "< literal, " << litpos << " > \n";
                litpos++;
            }
            chr = getNextChar(input, pos);
            litsecond++;
        }

        if (chr == '@')
        {
            error << "found @ in code which is not valid \n";
            chr = getNextChar(input, pos);
        }

        if (chr == '_') {
            symbol1 = true;
        }
        storestring = "";
        bool check = false;


        string ch;

        ch = "";
        ch = ch + chr;
        storestring += chr;
        for (int i = 0; i < COLS; i++)
        {
            if (ch == alphabets[i]) {
                alph = i;
                break;
            }
        }

        if (check == false)
        {
            if (chr >= '0' && chr <= '9') {
                ch = "num";
            }
            else if (chr >= 'a' && chr <= 'z' || chr >= 'A' && chr <= 'Z') {
                ch = "letter";
            }

            else if (mappingtable[st + 1][alph + 1] == -9 || chr == ' ') {
                ch = "other";
                chother = true;
            }

            if (checkkeyword(storestring) == true) {
                ch = "keyword";
            }

            for (int i = 0; i < COLS; i++)
            {
                if (ch == alphabets[i]) {
                    alph = i;
                    check = true;
                }
            }
        }


        bool adv = false;
        adv2 = false;
        while (chr != '\0' && checkfinal(st) != true && mappingtable[st + 1][alph + 1] != -9 && adv != true && lit == false && aterror == false && inout == false)
        {
            inout = false;
            check = false;
            adv = true;
            int newstate = mappingtable[st + 1][alph + 1];
            if (advance[st + 1][alph + 1] == 1)
            {
                bool check = false;
                chr = getNextChar(input, pos);

                
                if (chr == '"') {
                    litsecond++;
                }

                if (chr == '"' && litsecond % 2 != 0) {
                    literror = true;
                    lit = true;
                    literalValue = "";
                    int position = pos;

                    while (lit) {
                        chr = getNextChar(input, pos);
                        cout << literalValue;
                        if (chr == '"') {
                            lit = false;
                            break;
                        }

                        if (chr == '\0') {
                            cout << "error: String not closed" << endl;
                            error << "error could not find second \" in code \n";
                            lit = false;
                            checkingsecondcomma = false;
                            pos = position;

                        }

                        literalValue += chr;

                        literal1 = true;
                    }

                    if (literal1 && checkingsecondcomma == true) {
                        literal << litpos << "\t\t" << literalValue << "\n";
                        tokens << "< literal, " << litpos << " > \n";
                        litpos++;
                    }
                    chr = getNextChar(input, pos);
                    litsecond++;
                }
                if (chr == '@')
                {
                    error << "found @ in code which is not valid \n";
                    aterror = true;
                    break;
                }

                if (chr == '_') {
                    symbol1 = true;
                }
                ch = "";
                ch = ch + chr;
                    
                storestring += chr;

                for (int i = 0; i < COLS; i++)
                {
                    if (ch == alphabets[i]) {
                        alph = i;
                        check = true;

                    }
                }

                if (check == false)
                {
                    if (chr >= '0' && chr <= '9') {
                        check = true;
                        ch = "num";
                    }
                    else if (chr >= 'a' && chr <= 'z' || chr >= 'A' && chr <= 'Z') {
                        check = true;
                        ch = "letter";
                    }

                    else if (mappingtable[st + 1][alph + 1] == -9 || chr == ' ' || chr == ',') {
                        check = true;
                        ch = "other";
                        chother = true;


                    }

                    if (checkkeyword(storestring) == true) {
                        check = true;
                        ch = "keyword";
                        keyword1 = true;
                    }


                    if (storestring == "input") {
                        char tempchr1 = getNextChar(input, pos);
                        char tempchr2 = getNextChar(input, pos);
                        if (tempchr1 == '-' && tempchr2 == '>')
                        {
                            storestring = storestring + tempchr1 + tempchr2;
                            ch = "keyword";
                            inout = true;
                            keyword1 = true;
                        }
                        else {
                            pos = pos - 2;
                        }

                    }

                    if (storestring == "output") {
                        char tempchr1 = getNextChar(input, pos);
                        char tempchr2 = getNextChar(input, pos);
                        if (tempchr1 == '-' && tempchr2 == '<')
                        {
                            storestring = storestring + tempchr1 + tempchr2;
                            ch = "keyword";
                            inout = true;
                            keyword1 = true;
                        }
                        else {
                            pos = pos - 2;
                        }

                    }



                    for (int i = 0; i < COLS; i++)
                    {
                        if (ch == alphabets[i]) {
                            alph = i;
                            check = true;
                        }
                    }
                }
                adv = false;


                st = newstate;


            }
            else
            {
                adv2 = true;
                }


        }
        if (!storestring.empty() && storestring.back() == ' ') {
            storestring.pop_back();
        }
        bool checkingsome = false;
        for (int i = 0; i < COLS; i++)
        {
            if (storestring == alphabets[i]) {
                checkingsome = true;
            }
        }

        bool comma = false;
        if (storestring == ",") {
            comma = true;
        }



        if (((chr != ' ' && chother == true && inout != true ) || ((adv2 == true && chr!=' ' && inout != true && lit == false) && checkingsome != true)) && comma != true) {
            pos = pos - 1;
            storestring.pop_back();
        }
        if (!storestring.empty() && storestring.back() == ' ') {
            storestring.pop_back();
        }

        if (storestring != " " && storestring != "\n" && storestring != "\r" && !storestring.empty() && lit == false) {
            if (checkkeyword(storestring) || keyword1 == true) {

                int index = -1; 

                for (size_t i = 0; i < keywordchecking.size(); i++) {
                    if (keywordchecking[i] == storestring) {
                        index = i;
                        break;
                    }
                }

                if (index == -1) {
                    keyword << keypos << "\t\t" << storestring << "\n";
                    tokens << "< keyword," << keypos << " > \n";
                    keypos++;
                    keywordchecking.push_back(storestring);
                }
                else {
                    tokens << "< keyword," << index + 1 << " > \n";
                }
                
            }

            else if (symbol1 == true) {

                int index = -1;

                for (size_t i = 0; i < symbolchecking.size(); i++) {
                    if (symbolchecking[i] == storestring) {
                        index = i;
                        break;
                    }
                }

                if (index == -1) {
                    symbol << sympos << "\t\t" << storestring << "\n";

                    tokens << "< " << "id" << "," << sympos << " > \n";
                    sympos++;
                    symbolchecking.push_back(storestring);
                }
                else {
                    tokens << "< id," << index + 1 << " > \n";
                }

                
            }
            else if(checkfinal(st)){
                tokens << "< " << storestring << " > \n";
            }
            else if (checkingsome == true || comma == true) {
                tokens << "< " << storestring << " > \n";
            }

        }

        

        symbol1 = false;
        literal1 = false;
        keyword1 = false;

    }
    symbol.close();
    literal.close();
    keyword.close();
    tokens.close();
    error.close();
}

void normalizeSpaces(string& str) {
    string result;
    bool spaceFound = false;

    for (char c : str) {
        if (c == ' ') {
            if (!spaceFound) {
                result += c;
                spaceFound = true;
            }
        }
        else {
            result += c;
            spaceFound = false;
        }
    }

    str = result;
}

void convertNewlinesToSpace(string& str) {
    for (char& ch : str) {
        if (ch == '\n' || ch == '\r') {
            ch = ' ';
        }
    }
}
void replaceNonBreakingSpaces(string& str) {
    for (char& c : str) {
        if ((unsigned char)c == 160) {
            c = ' ';
        }
    }
}
void meow(string& arr) {
    replaceNonBreakingSpaces(arr); 
    convertNewlinesToSpace(arr);
    normalizeSpaces(arr);
}

void cleanString(string& str) {
    string result;
    for (char c : str) {
        if (c == ' ' || c == '\n' || c == '\r' || (unsigned char)c == 160) {
            continue; // skip unwanted characters
        }
        result += c;
    }
    str = result;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////****              lexical analyzer  end                  ****///////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////**********************************************************************************************************************/////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////****              Syntax analyzer  start                  ****///////////////
//**** in this part we generate parse tree using symbol table and check correct syntax according to the grammar provided converted to LL1 grammar ****//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//////////////// **** DECLARATIONS ****///////////////////////

const vector<string> keywords = { "Adadi", "Ashriya", "Harf", "Matn", "Mantiqi", "Agar", "Wagarna" };
vector<string> finalTokens;  // holds resolved values 
vector<string> rawTokens;    // holds the original representations 
vector<string> Tokens;        // check identifier 
int index = 0;

////////////////////////////////////////////////////////////////

struct TreeNode {
    string value;
    vector<shared_ptr<TreeNode>> children;

    TreeNode(const string& val) : value(val) {}

    void addChild(shared_ptr<TreeNode> child) {
        children.push_back(child);
    }

    void printTree(const string& prefix = "", bool isLast = true) const {
        cout << prefix << (isLast ? "|-- " : "|-- ") << value << endl;

        for (size_t i = 0; i < children.size(); ++i) {
            bool last = (i == children.size() - 1);
            children[i]->printTree(prefix + (isLast ? "    " : "|   "), last);
        }
    }
};

string currentToken() {
    if (index < finalTokens.size())
        return Tokens[index];
    return "$";
}

void nextToken() {
    if (index < finalTokens.size())
        index++;
}

shared_ptr<TreeNode> advance(const string& expected) {
    if (currentToken() == expected) {
        shared_ptr<TreeNode> node = make_shared<TreeNode>(rawTokens[index]);
        nextToken();
        return node;
    }
    cout << "Expected: " << expected << " but found: " << currentToken() << endl;
    return nullptr;
}



////////////////// ****** Forward declarations ****** /////////////////

shared_ptr<TreeNode> Function();
shared_ptr<TreeNode> ArgList();
shared_ptr<TreeNode> ArgListPrime(shared_ptr<TreeNode> argList);
shared_ptr<TreeNode> Arg();
shared_ptr<TreeNode> Declaration();
shared_ptr<TreeNode> Type();
shared_ptr<TreeNode> IdentList();
shared_ptr<TreeNode> IdentListPrime(shared_ptr<TreeNode> identList);
shared_ptr<TreeNode> Stmt();
shared_ptr<TreeNode> ForStmt();
shared_ptr<TreeNode> WhileStmt();
shared_ptr<TreeNode> IfStmt();
shared_ptr<TreeNode> ElsePart(shared_ptr<TreeNode> ifStmt);
shared_ptr<TreeNode> CompStmt();
shared_ptr<TreeNode> StmtList();
shared_ptr<TreeNode> StmtListPrime(shared_ptr<TreeNode> stmtList);
shared_ptr<TreeNode> Expr();
shared_ptr<TreeNode> ExprPrime(shared_ptr<TreeNode> expr);
shared_ptr<TreeNode> Rvalue();
shared_ptr<TreeNode> RvaluePrime(shared_ptr<TreeNode> rvalue);
shared_ptr<TreeNode> Compare(shared_ptr<TreeNode> node);
shared_ptr<TreeNode> ComparePrime(shared_ptr<TreeNode> node);
shared_ptr<TreeNode> CompareDoublePrime(shared_ptr<TreeNode> node);
shared_ptr<TreeNode> Mag();
shared_ptr<TreeNode> MagPrime(shared_ptr<TreeNode> mag);
shared_ptr<TreeNode> Term();
shared_ptr<TreeNode> TermPrime(shared_ptr<TreeNode> term);
shared_ptr<TreeNode> Factor();


////////////////////////////// ******** RECURSIVE DECENT PARSER ********* //////////////////////

shared_ptr<TreeNode> Function() {
    shared_ptr<TreeNode> functionNode = make_shared<TreeNode>("Function");

    shared_ptr<TreeNode> typeNode = Type();
    if (!typeNode) return nullptr;
    functionNode->addChild(typeNode);

    shared_ptr<TreeNode> idNode = advance("identifier");
    if (!idNode) return nullptr;
    functionNode->addChild(idNode);

    shared_ptr<TreeNode> parenNode = advance("(");
    if (!parenNode) return nullptr;
    functionNode->addChild(parenNode);

    shared_ptr<TreeNode> argListNode = ArgList();
    if (!argListNode) return nullptr;
    functionNode->addChild(argListNode);

    parenNode = advance(")");
    if (!parenNode) return nullptr;
    functionNode->addChild(parenNode);

    shared_ptr<TreeNode> compStmtNode = CompStmt();
    if (!compStmtNode) return nullptr;
    functionNode->addChild(compStmtNode);

    return functionNode;
}

// Argument list handling
shared_ptr<TreeNode> ArgList() {
    shared_ptr<TreeNode> argListNode = make_shared<TreeNode>("ArgList");

    if (currentToken() == "Adadi" || currentToken() == "Ashriya" ||
        currentToken() == "Harf" || currentToken() == "Matn" ||
        currentToken() == "Mantiqi") {
        shared_ptr<TreeNode> argNode = Arg();
        if (!argNode) return nullptr;
        argListNode->addChild(argNode);

        shared_ptr<TreeNode> argListPrimeNode = ArgListPrime(argListNode);
        if (!argListPrimeNode) return nullptr;
    }

    return argListNode;
}

shared_ptr<TreeNode> ArgListPrime(shared_ptr<TreeNode> argList) {
    if (currentToken() == ",") {
        shared_ptr<TreeNode> commaNode = advance(",");
        if (!commaNode) return nullptr;
        argList->addChild(commaNode);

        shared_ptr<TreeNode> argNode = Arg();
        if (!argNode) return nullptr;
        argList->addChild(argNode);

        return ArgListPrime(argList);
    }
    return argList;
}

shared_ptr<TreeNode> Arg() {
    shared_ptr<TreeNode> argNode = make_shared<TreeNode>("Arg");

    shared_ptr<TreeNode> typeNode = Type();
    if (!typeNode) return nullptr;
    argNode->addChild(typeNode);

    shared_ptr<TreeNode> idNode = advance("identifier");
    if (!idNode) return nullptr;
    argNode->addChild(idNode);

    return argNode;
}

// Declaration handling
shared_ptr<TreeNode> Declaration() {
    shared_ptr<TreeNode> declNode = make_shared<TreeNode>("Declaration");

    shared_ptr<TreeNode> typeNode = Type();
    if (!typeNode) return nullptr;
    declNode->addChild(typeNode);

    shared_ptr<TreeNode> identListNode = IdentList();
    if (!identListNode) return nullptr;
    declNode->addChild(identListNode);

    shared_ptr<TreeNode> colonNode = advance("::");
    if (!colonNode) return nullptr;
    declNode->addChild(colonNode);

    return declNode;
}

shared_ptr<TreeNode> Type() {
    shared_ptr<TreeNode> typeNode = make_shared<TreeNode>("Type");

    if (currentToken() == "Adadi" || currentToken() == "Ashriya" ||
        currentToken() == "Harf" || currentToken() == "Matn" ||
        currentToken() == "Mantiqi") {
        shared_ptr<TreeNode> typeToken = advance(currentToken());
        if (!typeToken) return nullptr;
        typeNode->addChild(typeToken);
        return typeNode;
    }

    cout << "Error in Type: Unexpected token " << currentToken() << endl;
    return nullptr;
}

shared_ptr<TreeNode> IdentList() {
    shared_ptr<TreeNode> identListNode = make_shared<TreeNode>("IdentList");

    shared_ptr<TreeNode> idNode = advance("identifier");
    if (!idNode) return nullptr;
    identListNode->addChild(idNode);

    return IdentListPrime(identListNode);
}

shared_ptr<TreeNode> IdentListPrime(shared_ptr<TreeNode> identList) {
    if (currentToken() == ",") {
        shared_ptr<TreeNode> commaNode = advance(",");
        if (!commaNode) return nullptr;
        identList->addChild(commaNode);

        shared_ptr<TreeNode> idNode = advance("identifier");
        if (!idNode) return nullptr;
        identList->addChild(idNode);

        return IdentListPrime(identList);
    }
    return identList;
}

// Statement handling
shared_ptr<TreeNode> Stmt() {
    shared_ptr<TreeNode> stmtNode = make_shared<TreeNode>("Stmt");

    if (currentToken() == "for") {
        shared_ptr<TreeNode> forNode = ForStmt();
        if (!forNode) return nullptr;
        stmtNode->addChild(forNode);
    }
    else if (currentToken() == "while") {
        shared_ptr<TreeNode> whileNode = WhileStmt();
        if (!whileNode) return nullptr;
        stmtNode->addChild(whileNode);
    }
    else if (currentToken() == "Agar") {
        shared_ptr<TreeNode> ifNode = IfStmt();
        if (!ifNode) return nullptr;
        stmtNode->addChild(ifNode);
    }
    else if (currentToken() == "{") {
        shared_ptr<TreeNode> compNode = CompStmt();
        if (!compNode) return nullptr;
        stmtNode->addChild(compNode);
    }
    else if (currentToken() == "Adadi" || currentToken() == "Ashriya" ||
        currentToken() == "Harf" || currentToken() == "Matn" ||
        currentToken() == "Mantiqi") {
        shared_ptr<TreeNode> declNode = Declaration();
        if (!declNode) return nullptr;
        stmtNode->addChild(declNode);
    }
    else if (currentToken() == "identifier" || currentToken() == "(" ||
        currentToken() == "number") {
        shared_ptr<TreeNode> exprNode = Expr();
        if (!exprNode) return nullptr;
        stmtNode->addChild(exprNode);

        shared_ptr<TreeNode> colonNode = advance("::");
        if (!colonNode) return nullptr;
        stmtNode->addChild(colonNode);
    }
    else {
        cout << "Error in Stmt: Unexpected token " << currentToken() << endl;
        return nullptr;
    }

    return stmtNode;
}

// Control structures
shared_ptr<TreeNode> ForStmt() {
    shared_ptr<TreeNode> forNode = make_shared<TreeNode>("ForStmt");

    shared_ptr<TreeNode> forToken = advance("for");
    if (!forToken) return nullptr;
    forNode->addChild(forToken);

    shared_ptr<TreeNode> parenNode = advance("(");
    if (!parenNode) return nullptr;
    forNode->addChild(parenNode);

    shared_ptr<TreeNode> expr1 = Expr();
    if (!expr1) return nullptr;
    forNode->addChild(expr1);

    shared_ptr<TreeNode> colon1 = advance("::");
    if (!colon1) return nullptr;
    forNode->addChild(colon1);

    shared_ptr<TreeNode> expr2 = Expr();
    if (!expr2) return nullptr;
    forNode->addChild(expr2);

    shared_ptr<TreeNode> colon2 = advance("::");
    if (!colon2) return nullptr;
    forNode->addChild(colon2);

    shared_ptr<TreeNode> expr3 = Expr();
    if (!expr3) return nullptr;
    forNode->addChild(expr3);

    parenNode = advance(")");
    if (!parenNode) return nullptr;
    forNode->addChild(parenNode);

    shared_ptr<TreeNode> stmtNode = Stmt();
    if (!stmtNode) return nullptr;
    forNode->addChild(stmtNode);

    return forNode;
}

shared_ptr<TreeNode> WhileStmt() {
    auto whileNode = make_shared<TreeNode>("WhileStmt");

    shared_ptr<TreeNode> whileToken = advance("while");
    if (!whileToken) return nullptr;
    whileNode->addChild(whileToken);

    shared_ptr<TreeNode> parenNode = advance("(");
    if (!parenNode) return nullptr;
    whileNode->addChild(parenNode);

    shared_ptr<TreeNode> exprNode = Expr();
    if (!exprNode) return nullptr;
    whileNode->addChild(exprNode);

    parenNode = advance(")");
    if (!parenNode) return nullptr;
    whileNode->addChild(parenNode);

    shared_ptr<TreeNode> stmtNode = Stmt();
    if (!stmtNode) return nullptr;
    whileNode->addChild(stmtNode);

    return whileNode;
}

shared_ptr<TreeNode> IfStmt() {
    shared_ptr<TreeNode> ifNode = make_shared<TreeNode>("IfStmt");

    shared_ptr<TreeNode> ifToken = advance("Agar");
    if (!ifToken) return nullptr;
    ifNode->addChild(ifToken);

    shared_ptr<TreeNode> parenNode = advance("(");
    if (!parenNode) return nullptr;
    ifNode->addChild(parenNode);

    shared_ptr<TreeNode> exprNode = Expr();
    if (!exprNode) return nullptr;
    ifNode->addChild(exprNode);

    parenNode = advance(")");
    if (!parenNode) return nullptr;
    ifNode->addChild(parenNode);

    shared_ptr<TreeNode> stmtNode = Stmt();
    if (!stmtNode) return nullptr;
    ifNode->addChild(stmtNode);

    shared_ptr<TreeNode> elseNode = ElsePart(ifNode);
    if (!elseNode) return nullptr;

    return elseNode;
}

shared_ptr<TreeNode> ElsePart(shared_ptr<TreeNode> ifStmt) {
    if (currentToken() == "Wagarna") {
        shared_ptr<TreeNode> elseToken = advance("Wagarna");
        if (!elseToken) return nullptr;
        ifStmt->addChild(elseToken);

        shared_ptr<TreeNode> stmtNode = Stmt();
        if (!stmtNode) return nullptr;
        ifStmt->addChild(stmtNode);
    }
    return ifStmt;
}

shared_ptr<TreeNode> CompStmt() {
    shared_ptr<TreeNode> compNode = make_shared<TreeNode>("CompStmt");

    shared_ptr<TreeNode> braceNode = advance("{");
    if (!braceNode) return nullptr;
    compNode->addChild(braceNode);

    shared_ptr<TreeNode> stmtListNode = StmtList();
    if (!stmtListNode) return nullptr;
    compNode->addChild(stmtListNode);

    braceNode = advance("}");
    if (!braceNode) return nullptr;
    compNode->addChild(braceNode);

    return compNode;
}

shared_ptr<TreeNode> StmtList() {
    shared_ptr<TreeNode> stmtListNode = make_shared<TreeNode>("StmtList");
    return StmtListPrime(stmtListNode);
}

shared_ptr<TreeNode> StmtListPrime(shared_ptr<TreeNode> stmtList) {
    if (currentToken() == "for" || currentToken() == "while" ||
        currentToken() == "Agar" || currentToken() == "{" ||
        currentToken() == "Adadi" || currentToken() == "Ashriya" ||
        currentToken() == "Harf" || currentToken() == "Matn" ||
        currentToken() == "Mantiqi" || currentToken() == "identifier" ||
        currentToken() == "(" || currentToken() == "number") {
        shared_ptr<TreeNode> stmtNode = Stmt();
        if (!stmtNode) return nullptr;
        stmtList->addChild(stmtNode);

        return StmtListPrime(stmtList);
    }
    return stmtList;
}

// Expression handling
shared_ptr<TreeNode> Expr() {
    shared_ptr<TreeNode> exprNode = make_shared<TreeNode>("Expr");

    if (currentToken() == "identifier") {
        shared_ptr<TreeNode> idNode = advance("identifier");
        if (!idNode) return nullptr;
        exprNode->addChild(idNode);

        shared_ptr<TreeNode> exprPrimeNode = ExprPrime(exprNode);
        if (!exprPrimeNode) return nullptr;
        return exprPrimeNode;
    }
    else if (currentToken() == "(" || currentToken() == "number") {
        shared_ptr<TreeNode> rvalueNode = Rvalue();
        if (!rvalueNode) return nullptr;
        exprNode->addChild(rvalueNode);
        return exprNode;
    }

    cout << "Error in Expr: Unexpected token " << currentToken() << endl;
    return nullptr;
}

shared_ptr<TreeNode> ExprPrime(shared_ptr<TreeNode> expr) {
    if (currentToken() == ":=") {
        shared_ptr<TreeNode> assignNode = make_shared<TreeNode>("Assignment");

        shared_ptr<TreeNode> assignOp = advance(":=");
        if (!assignOp) return nullptr;
        assignNode->addChild(expr->children[0]); // Move the identifier
        assignNode->addChild(assignOp);

        shared_ptr<TreeNode> rhs = Expr();
        if (!rhs) return nullptr;
        assignNode->addChild(rhs);

        return assignNode;
    }
    else {
        shared_ptr<TreeNode> termPrimeNode = TermPrime(expr);
        if (!termPrimeNode) return nullptr;

        shared_ptr<TreeNode> magPrimeNode = MagPrime(termPrimeNode);
        if (!magPrimeNode) return nullptr;

        return RvaluePrime(magPrimeNode);
    }
}

shared_ptr<TreeNode> Rvalue() {
    shared_ptr<TreeNode> rvalueNode = make_shared<TreeNode>("Rvalue");

    if (currentToken() == "(") {
        shared_ptr<TreeNode> parenNode = advance("(");
        if (!parenNode) return nullptr;
        rvalueNode->addChild(parenNode);

        shared_ptr<TreeNode> exprNode = Expr();
        if (!exprNode) return nullptr;
        rvalueNode->addChild(exprNode);

        parenNode = advance(")");
        if (!parenNode) return nullptr;
        rvalueNode->addChild(parenNode);

        shared_ptr<TreeNode> termPrimeNode = TermPrime(rvalueNode);
        if (!termPrimeNode) return nullptr;

        shared_ptr<TreeNode> magPrimeNode = MagPrime(termPrimeNode);
        if (!magPrimeNode) return nullptr;

        return RvaluePrime(magPrimeNode);
    }
    else if (currentToken() == "number") {
        shared_ptr<TreeNode> numNode = advance("number");
        if (!numNode) return nullptr;
        rvalueNode->addChild(numNode);

        shared_ptr<TreeNode> termPrimeNode = TermPrime(rvalueNode);
        if (!termPrimeNode) return nullptr;

        shared_ptr<TreeNode> magPrimeNode = MagPrime(termPrimeNode);
        if (!magPrimeNode) return nullptr;

        return RvaluePrime(magPrimeNode);
    }

    cout << "Error in Rvalue: Unexpected token " << currentToken() << endl;
    return nullptr;
}

shared_ptr<TreeNode> RvaluePrime(shared_ptr<TreeNode> rvalue) {
    if (currentToken() == "==" || currentToken() == "<" ||
        currentToken() == ">" || currentToken() == "!=") {
        shared_ptr<TreeNode> compareNode = make_shared<TreeNode>("Comparison");

        compareNode->addChild(rvalue);

        shared_ptr<TreeNode> compareOpNode = Compare(compareNode);
        if (!compareOpNode) return nullptr;

        shared_ptr<TreeNode> magNode = Mag();
        if (!magNode) return nullptr;
        compareNode->addChild(magNode);

        return RvaluePrime(compareNode);
    }
    return rvalue;
}

shared_ptr<TreeNode> Compare(shared_ptr<TreeNode> node) {
    if (currentToken() == "==") {
        shared_ptr<TreeNode> opNode = advance("==");
        if (!opNode) return nullptr;
        node->addChild(opNode);
        return node;
    }
    else if (currentToken() == "<") {
        shared_ptr<TreeNode> opNode = advance("<");
        if (!opNode) return nullptr;
        node->addChild(opNode);

        shared_ptr<TreeNode> primeNode = ComparePrime(node);
        if (!primeNode) return nullptr;
        return primeNode;
    }
    else if (currentToken() == ">") {
        shared_ptr<TreeNode> opNode = advance(">");
        if (!opNode) return nullptr;
        node->addChild(opNode);

        shared_ptr<TreeNode> doublePrimeNode = CompareDoublePrime(node);
        if (!doublePrimeNode) return nullptr;
        return doublePrimeNode;
    }
    else if (currentToken() == "!=") {
        shared_ptr<TreeNode> opNode = advance("!=");
        if (!opNode) return nullptr;
        node->addChild(opNode);
        return node;
    }

    cout << "Error in Compare: Unexpected token " << currentToken() << endl;
    return nullptr;
}

shared_ptr<TreeNode> ComparePrime(shared_ptr<TreeNode> node) {
    if (currentToken() == "=") {
        shared_ptr<TreeNode> opNode = advance("=");
        if (!opNode) return nullptr;
        // Combine with previous < to make <=
        node->children.back()->value = "<=";
        return node;
    }
    else if (currentToken() == ">") {
        shared_ptr<TreeNode> opNode = advance(">");
        if (!opNode) return nullptr;
        // Combine with previous < to make <>
        node->children.back()->value = "<>";
        return node;
    }
    return node;
}

shared_ptr<TreeNode> CompareDoublePrime(shared_ptr<TreeNode> node) {
    if (currentToken() == "=") {
        shared_ptr<TreeNode> opNode = advance("=");
        if (!opNode) return nullptr;
        // Combine with previous > to make >=
        node->children.back()->value = ">=";
        return node;
    }
    return node;
}

shared_ptr<TreeNode> Mag() {
    shared_ptr<TreeNode> magNode = make_shared<TreeNode>("Mag");

    shared_ptr<TreeNode> termNode = Term();
    if (!termNode) return nullptr;
    magNode->addChild(termNode);

    return MagPrime(magNode);
}

shared_ptr<TreeNode> MagPrime(shared_ptr<TreeNode> mag) {
    if (currentToken() == "+" || currentToken() == "-") {
        string op = currentToken();
        shared_ptr<TreeNode> opNode = advance(op);
        if (!opNode) return nullptr;

        shared_ptr<TreeNode> newMagNode = make_shared<TreeNode>("Mag");
        newMagNode->addChild(mag);
        newMagNode->addChild(opNode);

        shared_ptr<TreeNode> termNode = Term();
        if (!termNode) return nullptr;
        newMagNode->addChild(termNode);

        return MagPrime(newMagNode);
    }
    return mag;
}

shared_ptr<TreeNode> Term() {
    shared_ptr<TreeNode> termNode = make_shared<TreeNode>("Term");

    shared_ptr<TreeNode> factorNode = Factor();
    if (!factorNode) return nullptr;
    termNode->addChild(factorNode);

    return TermPrime(termNode);
}

shared_ptr<TreeNode> TermPrime(shared_ptr<TreeNode> term) {
    if (currentToken() == "*" || currentToken() == "/") {
        string op = currentToken();
        shared_ptr<TreeNode> opNode = advance(op);
        if (!opNode) return nullptr;

        shared_ptr<TreeNode> newTermNode = make_shared<TreeNode>("Term");
        newTermNode->addChild(term);
        newTermNode->addChild(opNode);

        shared_ptr<TreeNode> factorNode = Factor();
        if (!factorNode) return nullptr;
        newTermNode->addChild(factorNode);

        return TermPrime(newTermNode);
    }
    return term;
}

shared_ptr<TreeNode> Factor() {
    shared_ptr<TreeNode> factorNode = make_shared<TreeNode>("Factor");

    if (currentToken() == "(") {
        shared_ptr<TreeNode> parenNode = advance("(");
        if (!parenNode) return nullptr;
        factorNode->addChild(parenNode);

        shared_ptr<TreeNode> exprNode = Expr();
        if (!exprNode) return nullptr;
        factorNode->addChild(exprNode);

        parenNode = advance(")");
        if (!parenNode) return nullptr;
        factorNode->addChild(parenNode);
    }
    else if (currentToken() == "identifier") {
        shared_ptr<TreeNode> idNode = advance("identifier");
        if (!idNode) return nullptr;
        factorNode->addChild(idNode);
    }
    else if (currentToken() == "number") {
        shared_ptr<TreeNode> numNode = advance("number");
        if (!numNode) return nullptr;
        factorNode->addChild(numNode);
    }
    else {
        cout << "Error in Factor: Unexpected token " << currentToken() << endl;
        return nullptr;
    }

    return factorNode;
}





//////////////////////////// ***** HANDLING TOKENS ***** ////////////////////////////////////////

string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    if (start == string::npos) return "";
    return str.substr(start, end - start + 1);
}

map<int, string> loadTable(const string& filename) {
    map<int, string> table;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        int pos;
        string value;
        if (iss >> pos >> value) {
            table[pos] = value;
        }
    }
    return table;
}

string checkIdentifier(const string& tokenStr) {
    regex idPattern(R"(<\s*id\s*,\s*\d+\s*>)");

    if (regex_match(tokenStr, idPattern)) {
        return "identifier";
    }
    return "";
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////****              Syntax analyzer  end                  ****///////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////**********************************************************************************************************************/////////



int main() {

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////          lexical analyzer part start    /////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    string filename = "C:\\Users\\HP\\Desktop\\visualstudio\\lexical analyser\\lexical analyser\\transition_table.csv";
    readCSV(filename);

    ifstream inputFile("testcode.txt");

    if (!inputFile) {
        cerr << "Error: Unable to open file!" << endl;
        return 1;
    }

    inputFile.seekg(0, ios::end);
    int fileSize = inputFile.tellg();
    inputFile.seekg(0, ios::beg);

    string arr(fileSize, '\0');
    inputFile.read(&arr[0], fileSize);
    inputFile.close();

    meow(arr); 

    displayTable(arr);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   //////////////////////          lexical analyzer part end     /////////////////////////////////////////////
   //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //////**********************************************************************************************************************/////////

     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   //////////////////////          syntax analyzer part start    /////////////////////////////////////////////
   //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ifstream tokenFile("tokens_table.txt");
    if (!tokenFile) {
        cerr << "Could not open token file\n";
        return 1;
    }

    // Load all tables
    map<int, string> symbolTable = loadTable("symbol_table.txt");
    map<int, string> keywordTable = loadTable("keyword_table.txt");
    map<int, string> literalTable = loadTable("literal_table.txt");

    string line;

    // Regex for structured and simple tokens
    regex fullToken(R"(<\s*(keyword|literal|id)\s*,\s*(\d+)\s*>)");
    regex simpleToken(R"(<\s*([^<>]+?)\s*>)");


    while (getline(tokenFile, line)) {
        line = trim(line);
        if (line.empty()) continue;

        smatch match;

        if (regex_match(line, match, fullToken)) {
            string type = match[1];
            int pos = stoi(match[2]);

            if (type == "keyword" && keywordTable.count(pos)) {
                finalTokens.push_back(keywordTable[pos]);
                rawTokens.push_back("< keyword," + keywordTable[pos] + " >");
            }
            else if (type == "id" && symbolTable.count(pos)) {
                finalTokens.push_back(symbolTable[pos]);
                rawTokens.push_back("< id," + to_string(pos) + " >");
            }
            else if (type == "literal" && literalTable.count(pos)) {
                finalTokens.push_back(literalTable[pos]);
                rawTokens.push_back("< literal," + literalTable[pos] + " >");
            }
        }
        else if (regex_match(line, match, simpleToken)) {
            string token = trim(match[1]);
            finalTokens.push_back(token);
            rawTokens.push_back("< " + token + " >");
        }
    }

    cout << "Final Token Values:\n";
    for (int i = 0; i < finalTokens.size(); ++i) {
        cout << finalTokens[i] << " ";
    }
    cout << endl;

    cout << "\nRaw Tokens:\n";
    for (int i = 0; i < rawTokens.size(); ++i) {
        cout << rawTokens[i] << endl;
    }

    for (int i = 0; i < finalTokens.size(); ++i) {
        if (checkIdentifier(rawTokens[i]) == "identifier")
        {
            Tokens.push_back("identifier");
        }
        else
        {
            Tokens.push_back(finalTokens[i]);
        }
    }

    cout << "\nTokens:\n";
    for (int i = 0; i < Tokens.size(); ++i) {
        cout << Tokens[i] << endl;
    }


    ////////////////////////////////////////////////////////////////////

    cout << endl;

    shared_ptr<TreeNode> parseTree = Function();

    if (parseTree && currentToken() == "$") {
        cout << "\nParsing succeeded! Parse tree:" << endl;
        parseTree->printTree();
    }
    else {
        cout << "\nParsing failed at token: " << currentToken()
            << " (position " << index << ")" << endl;
    }


    return 0;

     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   //////////////////////          syntax analyzer part end     /////////////////////////////////////////////
   //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


   //////**********************************************************************************************************************/////////


}