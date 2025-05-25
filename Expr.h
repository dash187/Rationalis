#pragma once

#include "Tokenizer.h"
#include "Keyword.h"
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

struct KeywordExpr : public Expr
{
	KeywordType id;
	std::vector<Expr*> operands;

	KeywordExpr(KeywordType id, std::vector<Expr*>&& operand);
	~KeywordExpr();
	double eval() override;
	std::string toString() const override;
};

KeywordType stringToKeyword(const std::string& str);
std::string keywordToString(KeywordType id);

struct IdentifierExpr : public Expr
{
	std::string name;

	IdentifierExpr(const std::string& name);
	double eval() override;
	std::string toString() const override;

	static double lookupIdentifier(const std::string& name);
	static void setIdentifier(const std::string& name, double value);
};