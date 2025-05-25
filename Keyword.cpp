#include "Keyword.h"
#include <stdexcept>
#include <numeric>

const KeywordInfo& KeywordTable::setItem(KeywordType type, std::string name, KeywordInfo::EvalFunc f, int argc)
{
	size_t idx = static_cast<size_t>(type);
	table[idx] = { type, std::move(name), f, argc };
	nameToID[table[idx].name] = type;
	return table[idx];
}

const KeywordInfo& KeywordTable::getByID(KeywordType id) const
{
	return table[static_cast<size_t>(id)];
}

const KeywordInfo& KeywordTable::getByName(const std::string& name) const
{
	auto it = nameToID.find(name);
	if (it != nameToID.end())
	{
		return table[static_cast<size_t>(it->second)];
	}
	throw std::runtime_error("Keyword not found: " + name);
}

bool KeywordTable::contains(const std::string& name) const
{
	return nameToID.contains(name);
}

bool KeywordTable::contains(KeywordType id) const
{
	return id >= KeywordType::Sin && id < KeywordType::Total;
}

static void initTable(KeywordTable* table) 
{
	table->setItem(KeywordType::Sin, "sin", [](const std::vector<double>& args) { return std::sin(args[0]); }, 1);
	table->setItem(KeywordType::Cos, "cos", [](const std::vector<double>& args) { return std::cos(args[0]); }, 1);
	table->setItem(KeywordType::Tan, "tan", [](const std::vector<double>& args) { return std::tan(args[0]); }, 1);
	table->setItem(KeywordType::Asin, "arcsin", [](const std::vector<double>& args) { return std::asin(args[0]); }, 1);
	table->setItem(KeywordType::Acos, "arccos", [](const std::vector<double>& args) { return std::acos(args[0]); }, 1);
	table->setItem(KeywordType::Atan, "arctan", [](const std::vector<double>& args) { return std::atan(args[0]); }, 1);
	table->setItem(KeywordType::Sqrt, "sqrt", [](const std::vector<double>& args) { return std::sqrt(args[0]); }, 1);
	table->setItem(KeywordType::Log, "log", [](const std::vector<double>& args) { return std::log(args[0]); }, 1);
	table->setItem(KeywordType::Pi, "pi", [](const std::vector<double>&) { return 3.14159265358979323846; }, 0);
	table->setItem(KeywordType::E, "e", [](const std::vector<double>&) { return 2.71828182845904523536; }, 0);
	table->setItem(KeywordType::Mean, "mean", [](const std::vector<double>& args) { return std::accumulate(args.begin(), args.end(), 0.0) / args.size(); }, 2);
}

std::string KeywordInfo::toString() const
{
	return name;
}

const KeywordTable& KeywordInfo::getTable()
{
	static KeywordTable* table = nullptr;
	if (!table)
	{
		table = new KeywordTable();
		initTable(table);
	}
	return *table;
}