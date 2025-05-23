#include "Expr.h"
#include "Tokenizer.h"
#include <cmath>
#include <unordered_map> 
#include <algorithm>

// -----------------------------------------------------
NumberExpr::NumberExpr(double val) : value(val) {}

double NumberExpr::eval() {
	return value;
}

std::string NumberExpr::toString() const
{
	return std::to_string(value);
}

// -----------------------------------------------------
UnaryExpr::UnaryExpr(TokenType op, Expr* operand) : op(op), operand(operand) {}
UnaryExpr::~UnaryExpr() { delete operand; }

double UnaryExpr::eval() {
	if (op == TokenType::Plus) {
		return operand->eval();
	}
	if (op == TokenType::Minus) {
		return -operand->eval();
	}
	throw std::runtime_error("Unknown unary operator");
}

std::string UnaryExpr::toString() const
{
	return std::format("({}{})", tokenTypeToString(op), operand->toString());
}

// -----------------------------------------------------
BinaryExpr::BinaryExpr(TokenType op, Expr* l, Expr* r) : op(op), left(l), right(r) {}
BinaryExpr::~BinaryExpr() { delete left; delete right; }

double BinaryExpr::eval() {
	if (op == TokenType::Plus) {
		return left->eval() + right->eval();
	}
	if (op == TokenType::Minus) {
		return left->eval() - right->eval();
	}
	if (op == TokenType::Mult) {
		return left->eval() * right->eval();
	}
	if (op == TokenType::Div) {
		return left->eval() / right->eval();
	}
	if (op == TokenType::Pow) {
		return std::pow(left->eval(), right->eval());
	}
	throw std::runtime_error("Unknown binary operator");
}

std::string BinaryExpr::toString() const
{
	return std::format("({} {} {})", left->toString(), tokenTypeToString(op) , right->toString());
}

// -----------------------------------------------------
IdentifierExpr::IdentifierExpr(IdentifierType id, std::vector<Expr*>&& operands) : id(id), operands(std::move(operands)) {}
IdentifierExpr::~IdentifierExpr() { for (Expr* expr : operands) delete expr; }

double IdentifierExpr::eval()
{
	std::vector<double> args(operands.size());
	for (size_t i = 0; i < args.size(); ++i) {
		args[i] = operands[i]->eval();
	}

	const IdentifierInfo& info = IdentifierInfo::getTable().getByID(id);
	if (info.argCount != args.size()) {
		throw std::runtime_error(std::format("Wrong number of arguments: Expected: {}, got: {}", info.argCount, args.size()));
	}
	return info.eval(args);
}

std::string IdentifierExpr::toString() const
{
	std::string result = identifierToString(id);
	if (operands.empty()) {
		return result;
	}
	result += "(";
	for (size_t i = 0; i < operands.size(); ++i) {
		result += operands[i]->toString();
		if (i + 1 < operands.size())
			result += ", ";
	}
	result += ")";
	return result;
}

IdentifierType stringToIdentifier(const std::string& str)
{
	try {
		return IdentifierInfo::getTable().getByName(str).id;
	}
	catch (...) {
		throw std::runtime_error("Unknown identifier: " + str);
	}
}

std::string identifierToString(IdentifierType id)
{
	const IdentifierInfo& info = IdentifierInfo::getTable().getByID(id);
	return info.toString();
}