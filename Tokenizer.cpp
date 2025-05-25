#include "Tokenizer.h"

#include <stdexcept>
#include <iostream>
#include "Keyword.h"

const Token Token::END_OF_FILE{ TokenType::EndOfFile, "\0" };

std::string tokenTypeToString(TokenType type)
{
	switch (type)
	{
	case TokenType::Plus:
		return "+";
	case TokenType::Minus:
		return "-";
	case TokenType::Mult:
		return "*";
	case TokenType::Div:
		return "/";
	case TokenType::Pow:
		return "^";
	case TokenType::LBracket:
		return "(";
	case TokenType::RBracket:
		return ")";
	case TokenType::Number:
		return "Number";
	case TokenType::Keyword:
		return "Keyword";
	case TokenType::Comma:
		return ",";
	default:
		return "Unknown token type";
	}
}

std::ostream& operator<<(std::ostream& os, const Token& token)
{
	return os << token.toString();
}

char Tokenizer::peek()
{
	if (pos < tokens.size())
		return tokens[pos];
	return '\0';
}

char Tokenizer::next()
{
	if (pos + 1 < tokens.size())
		return tokens[pos + 1];
	return '\0';
}

bool Tokenizer::areTokensLeft()
{
	return pos < tokens.size();
}

bool Tokenizer::match(char c)
{
	if (pos < tokens.size() && tokens[pos] == c)
	{
		pos++;
		return true;
	}
    return false;
}

void Tokenizer::skipWhitespace()
{
	while (pos < tokens.size() && isspace(tokens[pos]))
		pos++;
}

void Tokenizer::skipNumber()
{
	while (pos < tokens.size() && isdigit(tokens[pos]))
	{
		pos++;
	}
	if (pos < tokens.size() && tokens[pos] == '.')
	{
		pos++;
		while (pos < tokens.size() && isdigit(tokens[pos]))
		{
			pos++;
		}
	}
}

void Tokenizer::skipWord()
{
	while (pos < tokens.size() && isalpha(tokens[pos]))
	{
		pos++;
	}
}

Token Tokenizer::getToken()
{
	if (match('+'))
		return Token{ TokenType::Plus, "+" };
	if (match('*'))
		return Token{ TokenType::Mult, "*" };
	if (match('-'))
		return Token{ TokenType::Minus, "-" };
	if (match('/'))
		return Token{ TokenType::Div, "/" };
	if (match('^'))
		return Token{ TokenType::Pow, "^" };
	if (match('('))
		return Token{ TokenType::LBracket, "(" };
	if (match(')'))
		return Token{ TokenType::RBracket, ")" };
	if (match('='))
		return Token{ TokenType::Equals, "=" };
	if (match(','))
		return Token{ TokenType::Comma, "," };
	if (isdigit(peek()) || peek() == '.') {
		size_t start = pos;
		skipNumber();
		return Token{ TokenType::Number, tokens.substr(start, pos - start) };
	}
	if (isalpha(peek())) {
		size_t start = pos;
		skipWord();
		std::string word = tokens.substr(start, pos - start);
		if (KeywordInfo::getTable().contains(word)) {
			return Token{ TokenType::Keyword, word };
		}
		else {
			return Token{ TokenType::Identifier, word };
		}
	}
	if (match('\0'))
		return Token::END_OF_FILE;
	throw std::runtime_error(std::format("Unknown token: '{}'", peek()));
}

std::vector<Token> tokenize(const std::string& str)
{
	Tokenizer tokenizer{ str, 0 };
	std::vector<Token> toks;

	tokenizer.skipWhitespace();
	while (tokenizer.areTokensLeft())
	{
		toks.push_back(tokenizer.getToken());
		tokenizer.skipWhitespace();
	}
	return toks;
}

std::string Token::toString() const
{
	return std::format("Token{{ type: {}, content: \"{}\" }}", tokenTypeToString(type), content);
}
