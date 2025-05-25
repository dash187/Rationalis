#pragma once

#include <unordered_map>
#include <string>
#include <array>

enum class KeywordType {
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
	Mean,
	Total
};

class KeywordTable;

struct KeywordInfo
{
	using EvalFunc = double (*)(const std::vector<double>&);

	KeywordType id;
	std::string name;
	EvalFunc eval;
	int argCount;

	std::string toString() const;
	static const KeywordTable& getTable();
};

class KeywordTable
{
public:
	using TableType = std::array<KeywordInfo,\
		static_cast<size_t>(KeywordType::Total)>;

	const KeywordInfo& setItem(KeywordType, std::string, KeywordInfo::EvalFunc, int);
	const KeywordInfo& getByID(KeywordType id) const;
	const KeywordInfo& getByName(const std::string& name) const;
	bool contains(const std::string& name) const;
	bool contains(KeywordType id) const;
private:
	TableType table;
	std::unordered_map<std::string, KeywordType> nameToID;
};

