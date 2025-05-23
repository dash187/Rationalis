#include "Parser.h"
#include "ParseRule.h"
#include <print>
#include <sstream>

PrattParser::PrattParser(std::vector<Token>&& tokens, size_t pos) : toks(std::move(tokens)), pos(pos)
{
}

// This just wraps the parseExpr function from ParseRule.h
Expr* PrattParser::parseExpression()
{
	return ::parseExpr(*this);
}

const Token& PrattParser::operator[](size_t index) const
{
	if (index < toks.size())
		return toks[index];
	return Token::END_OF_FILE;
}

const Token& PrattParser::peek()const
{
	return (*this)[pos];
}

const Token& PrattParser::nextToken() const
{
	return (*this)[pos + 1];
}

bool PrattParser::consume(size_t count)
{
	pos += count;
	return pos < toks.size();
}

std::string PrattParser::toString() const
{
	std::stringstream ss;
	const size_t interval = 10;
	size_t start = std::max(0, static_cast<int>(pos) - static_cast<int>(interval));
	size_t end = std::min(toks.size(), pos + interval);

	for (size_t i = start; i < end; ++i) {
		if (i == pos) {
			ss << "pos > " << toks[i].toString();
		} else {
			ss << "      " << toks[i].toString();
		}
		if (i < end - 1) {
			ss << "\n";
		}
	}
	return ss.str();
}
