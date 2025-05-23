#pragma once

#include <string>
#include <vector>
#include <ostream>

enum class TokenType : unsigned int {
Plus,
Minus,
Mult,
Div,
Pow,
LBracket,
RBracket,
Number,
Identifier,
Comma,
EndOfFile,
Total
};

std::string tokenTypeToString(TokenType type);

struct Token
{
	TokenType type;
	std::string content;

	static const Token END_OF_FILE;
	std::string toString() const;
};

std::ostream& operator<<(std::ostream& os, const Token& token);

struct Tokenizer
{
	std::string tokens;
	size_t pos = 0;

	char peek();
	char next();
	bool areTokensLeft();
	bool match(char c);
	void skipWhitespace();
	void skipNumber();
	void skipWord();
	Token getToken();
};

std::vector<Token> tokenize(const std::string& str);

