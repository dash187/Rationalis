#include "Tokenizer.h"
#include "Parser.h"
#include "ParseRule.h"

#include <iostream>
#include <print>
#include <cassert>
#include <memory>

void shell()
{
	std::println("\nWelcome to the Pratt Parser shell!"
			     "\nType 'exit' to quit.");
	std::string input;
	while (true) {
		std::print("> ");
		std::getline(std::cin, input);
		if (input == "exit") break;
		try {
			auto parser = PrattParser(tokenize(input));
			auto expr = std::unique_ptr<Expr>{ parseExpr(parser) };
			assert(expr);
			std::println("Parsed expression: {} = {}", expr->toString(), expr->eval());
		}
		catch (const std::exception& e) {
			std::println("Error: {}", e.what());
		}
	}
}

void example0()
{
	auto input = "sin3 + 5 * (2 / 8) - 1";
	PrattParser parser(tokenize(input), 4);
	std::println("\n{}\n", parser.toString());
}

void example1()
{
	char input[] = "88";
	auto parser = PrattParser(tokenize(input));
	std::println("{}", parser.toString());
	auto* expr = nudLiteral(parser);
	std::println("Parsed expression: {} = {}\n", expr->toString(), expr->eval());
}

void example2()
{
	char input[] = "-1";
	auto parser = PrattParser(tokenize(input));
	std::println("{}", parser.toString());
	auto* expr = nudUnary(parser);
	std::println("Parsed expression: {} = {}\n", expr->toString(), expr->eval());
}

void example3()
{
	char input[] = "2*(1+2) - 2";
	auto parser = PrattParser(tokenize(input));
	auto* expr = parseExpr(parser);
	assert(expr);
	std::println("Parsed expression: {} = {}", expr->toString(), expr->eval());
}

void example4()
{
	char input[] = "1 + 1 * 2 - 3";
	auto parser = PrattParser(tokenize(input));
	auto* expr = parseExpr(parser);
	assert(expr);
	std::println("Parsed expression: {} = {}", expr->toString(), expr->eval());
}

int main(int argc, char** argv)
{
#if 0
	try {
		example0();
		example1();
		example2();
		example3();
		example4();
	}
	catch (const std::exception& e)
	{
		std::println("Error: {}", e.what());
	}
#endif

	shell();

	return 0;
}