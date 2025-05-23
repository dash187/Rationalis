#include "ParseRule.h"
#include "Parser.h"
#include "Identifier.h"
#include <string>
#include <print>

#define ERR(msg) throw std::runtime_error(msg)

const ParseTable& ParseRule::Table()
{
	static ParseTable table = {
		ParseRule{ nudUnary, ledBinary, 10, 10}, // Plus
		ParseRule{ nudUnary, ledBinary, 10, 10}, // Minus
		ParseRule{ nullptr, ledBinary, 20, 20 }, // Mult
		ParseRule{ nullptr, ledBinary, 20, 20 }, // Div
		ParseRule{ nullptr, ledBinary, 40, 30 }, // Pow
		ParseRule{ nudGroup, nullptr, 0, 0 },  // LBracket
		ParseRule{ nullptr, ledNone, 0, 0},  // RBracket
		ParseRule{ nudLiteral, nullptr, 0, 0 },  // Number
		ParseRule{ nudIdentifier, nullptr, 0, 0 },  // Identifier
		ParseRule{ nullptr, nullptr, 0, 0 },  // Comma
		ParseRule{ nullptr, nullptr, 0, 0 }   // EndOfFile
	};

    return table;
}

Expr* nudIdentifier(PrattParser& parser)
{
	const Token& identifier = parser.peek();
	if (identifier.type != TokenType::Identifier)
	{
		ERR("Expected an identifier");
	}
	parser.consume(); // Consume the identifier token

	const IdentifierInfo& identifierDetails = IdentifierInfo::getTable().getByName(identifier.content);
	std::vector<Expr*> arguments;
	arguments.reserve(identifierDetails.argCount); // Reserve space for arguments

	if (identifierDetails.argCount == 0) {
		return new IdentifierExpr(identifierDetails.id, {});
	}

	if (parser.peek().type != TokenType::LBracket)
	{
		ERR("Expected opening bracket");
	}
	parser.consume(); // Consume the opening bracket if there are arguments

	// Parse arguments separated by commas
	for (size_t i = 0; i < identifierDetails.argCount - 1; ++i) {
		arguments.push_back(parseExpr(parser, TokenType::Comma, 0));
	}
	// Parse the last argument, which is followed by a closing bracket
	arguments.push_back(parseExpr(parser, TokenType::RBracket, 0));

	if (parser.peek().type != TokenType::RBracket)
	{
		ERR("Expected closing bracket");
	}
	parser.consume(); // Consume the closing bracket	
	
	return new IdentifierExpr(identifierDetails.id, std::move(arguments));
}

Expr* nudLiteral(PrattParser& parser)
{
	const Token& peek = parser.peek();
	if (peek.type != TokenType::Number)
	{
		ERR("Expected a number");
	}
	parser.consume();
	return new NumberExpr{ std::stod(peek.content) };
}

Expr* nudUnary(PrattParser& parser)
{
	const Token& peek = parser.peek();
	if (peek.type != TokenType::Plus && peek.type != TokenType::Minus)
	{
		ERR("Expected unary operator");
	}
	parser.consume(); // Consume the unary operator token

	const Token& nextTok = parser.peek();
	const ParseRule& nextTokRule = ParseRule::Table()[static_cast<size_t>(nextTok.type)];
	if (!nextTokRule.nud) {
		ERR("No nud function for token: " + nextTok.toString());
	}
	return new UnaryExpr(peek.type, nextTokRule.nud(parser));
}

Expr* ledBinary(PrattParser& parser, Expr* expr1)
{
	const Token& peek = parser.peek();
	if (peek.type != TokenType::Plus && peek.type != TokenType::Minus &&
		peek.type != TokenType::Mult && peek.type != TokenType::Div && peek.type != TokenType::Pow)
	{
		ERR("Expected binary operator");
	}
	parser.consume(); // Consume the operator

	Expr* expr2 = parseExpr(parser, TokenType::EndOfFile, ParseRule::Table()[static_cast<size_t>(peek.type)].rbp);
	return new BinaryExpr(peek.type, expr1, expr2);
}

Expr* nudGroup(PrattParser& parser)
{
	parser.consume(); // Consume the opening bracket so we dont end in an infinite loop
	Expr* expr = parseExpr(parser, TokenType::RBracket);
	if (parser.peek().type != TokenType::RBracket)
	{
		ERR("Expected closing bracket");
	}
	parser.consume(); // Consume the closing bracket
	return expr;
}

Expr* parseExpr(PrattParser& parser, TokenType end, int minBindingPower)
{
	const auto& ruleTable = ParseRule::Table();
	const auto& tok = parser.peek();
	if (tok.type == TokenType::EndOfFile || tok.type == end)
	{
		ERR("Unexpected end of file");
	}
	const ParseRule& rule1 = ruleTable[static_cast<size_t>(tok.type)];
	if (!rule1.nud)
	{
		ERR("No nud function for token: " + tok.toString());
	}

	Expr* left = rule1.nud(parser);

	while (parser.peek().type != end && parser.peek().type != TokenType::EndOfFile)
	{
		const auto& nextTok = parser.peek();
		const ParseRule& rule2 = ruleTable[static_cast<size_t>(nextTok.type)];
		if (rule2.lbp <= minBindingPower)
		{
			break;
		}
		if (!rule2.led)
		{
			ERR("No led function for token: " + nextTok.toString());
		}
		left = rule2.led(parser, left);
	}

	return left;
}
