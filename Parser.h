#pragma once

#include "Tokenizer.h"
#include "Expr.h"
#include <vector>
#include <string>

struct PrattParser
{
private:
	std::vector<Token> toks;
	size_t pos;
public:
	PrattParser(std::vector<Token>&& tokens, size_t pos = 0);
	const Token& operator[](size_t index) const;
    const Token& peek() const;
	const Token& nextToken() const;
	bool consume(size_t count = 1);

	Expr* parseExpression();
	std::string toString() const;
};