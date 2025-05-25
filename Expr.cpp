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
KeywordExpr::KeywordExpr(KeywordType id, std::vector<Expr*>&& operands) : id(id), operands(std::move(operands)) {}
KeywordExpr::~KeywordExpr() { for (Expr* expr : operands) delete expr; }

double KeywordExpr::eval()
{
	std::vector<double> args(operands.size());
	for (size_t i = 0; i < args.size(); ++i) {
		args[i] = operands[i]->eval();
	}

	const KeywordInfo& info = KeywordInfo::getTable().getByID(id);
	if (info.argCount != -1 && info.argCount != args.size()) {
		throw std::runtime_error(std::format("Wrong number of arguments: Expected: {}, got: {}", info.argCount, args.size()));
	}
	return info.eval(args);
}

std::string KeywordExpr::toString() const
{
	std::string result = keywordToString(id);
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

KeywordType stringToKeyword(const std::string& str)
{
	return KeywordInfo::getTable().getByName(str).id;
}

std::string keywordToString(KeywordType id)
{
	return KeywordInfo::getTable().getByID(id).name;
}

// -----------------------------------------------------
static std::unordered_map<std::string, double> variables = {};

IdentifierExpr::IdentifierExpr(const std::string& name)
	: name(name) {
}

double IdentifierExpr::eval()
{
	return lookupIdentifier(name);
}

std::string IdentifierExpr::toString() const
{
	return name;
}

double IdentifierExpr::lookupIdentifier(const std::string& name)
{
	auto it = variables.find(name);
	if (it != variables.end()) {
		return it->second;
	}
	throw std::runtime_error(std::format("Identifier '{}' not found", name));
}

void IdentifierExpr::setIdentifier(const std::string& name, double value)
{
	variables[name] = value;
}