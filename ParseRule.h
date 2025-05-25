#pragma once

#include <array>
#include "Tokenizer.h"
#include "Expr.h"

// Avoid circular dependencies
struct PrattParser;
struct ParseRule;

using NudFunc = Expr * (*)(PrattParser&);
using LedFunc = Expr * (*)(PrattParser&, Expr*);
using ParseTable = std::array<ParseRule, static_cast<size_t>(TokenType::Total)>;

struct ParseRule
{
	// What to do if this token appears at the start of an expression, e.g. the + in "+1"
	NudFunc nud = nullptr; 
	// What to do if this token appears in the middle of an expression, e.g. the + in "1+2"
	LedFunc led = nullptr; 
	// Left binding power (precedence)
	int lbp = 0;
	// Right binding power (precedence)
	int rbp = 0;

	static const ParseTable& Table();
};

inline Expr* ledNone(PrattParser& parser, Expr* left) { return left; }
Expr* nudKeyword(PrattParser& parser);
Expr* nudLiteral(PrattParser& parser);
Expr* nudIdentifier(PrattParser& parser);
Expr* nudUnary(PrattParser& parser);
Expr* nudGroup(PrattParser& parser);
Expr* ledEquals(PrattParser& parser, Expr* left);
Expr* ledBinary(PrattParser& parser, Expr* left);
Expr* parseExpr(PrattParser& parser, TokenType end = TokenType::EndOfFile, int minBindingPower = 0);