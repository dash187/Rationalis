#pragma once

#include <unordered_map>
#include <string>
#include <array>

enum class IdentifierType {
	Sin,
	Cos,
	Tan,
	Asin,
	Acos,
	Atan,
	Sqrt,
	Log,
	Pi,
	E,
	Total
};

class IdentifierTable;

struct IdentifierInfo
{
	using EvalFunc = double (*)(const std::vector<double>&);

	IdentifierType id;
	std::string name;
	EvalFunc eval;
	int argCount;

	std::string toString() const;
	static const IdentifierTable& getTable();
};

class IdentifierTable
{
public:
	using TableType = std::array<IdentifierInfo,\
		static_cast<size_t>(IdentifierType::Total)>;

	const IdentifierInfo& setItem(IdentifierType, std::string, IdentifierInfo::EvalFunc, int);
	const IdentifierInfo& getByID(IdentifierType id) const;
	const IdentifierInfo& getByName(const std::string& name) const;
private:
	TableType table;
	std::unordered_map<std::string, IdentifierType> nameToID;
};

