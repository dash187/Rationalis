#pragma once

#include "Tokenizer.h"
#include "Identifier.h"
#include <string>

struct Expr
{
    virtual ~Expr() = default;
	virtual double eval() = 0;
	virtual std::string toString() const = 0;
};

struct NumberExpr : public Expr
{
	double value;
	
	NumberExpr(double val);
	double eval() override;
	std::string toString() const override;
};

struct UnaryExpr : public Expr
{
	TokenType op;
	Expr* operand;
	
	UnaryExpr(TokenType op, Expr* operand);
	~UnaryExpr();
	double eval() override;
	std::string toString() const override;
};

struct BinaryExpr : public Expr
{
	TokenType op;
	Expr* left;
	Expr* right;
	
	BinaryExpr(TokenType op, Expr* l, Expr* r);
	~BinaryExpr();
	double eval() override;
	std::string toString() const override;
};

struct IdentifierExpr : public Expr
{
	IdentifierType id;
	std::vector<Expr*> operands;

	IdentifierExpr(IdentifierType id, std::vector<Expr*>&& operand);
	~IdentifierExpr();
	double eval() override;
	std::string toString() const override;
};

IdentifierType stringToIdentifier(const std::string& str);
std::string identifierToString(IdentifierType id);