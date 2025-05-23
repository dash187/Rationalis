#include "Identifier.h"
#include <stdexcept>

const IdentifierInfo& IdentifierTable::setItem(IdentifierType type, std::string name, IdentifierInfo::EvalFunc f, int argc)
{
	size_t idx = static_cast<size_t>(type);
	table[idx] = { type, std::move(name), f, argc };
	nameToID[table[idx].name] = type;
	return table[idx];
}

const IdentifierInfo& IdentifierTable::getByID(IdentifierType id) const
{
	return table[static_cast<size_t>(id)];
}

const IdentifierInfo& IdentifierTable::getByName(const std::string& name) const
{
	auto it = nameToID.find(name);
	if (it != nameToID.end())
	{
		return table[static_cast<size_t>(it->second)];
	}
	throw std::runtime_error("Identifier not found: " + name);
}

static void initTable(IdentifierTable* table) 
{
	table->setItem(IdentifierType::Sin, "sin", [](const std::vector<double>& args) { return std::sin(args[0]); }, 1);
	table->setItem(IdentifierType::Cos, "cos", [](const std::vector<double>& args) { return std::cos(args[0]); }, 1);
	table->setItem(IdentifierType::Tan, "tan", [](const std::vector<double>& args) { return std::tan(args[0]); }, 1);
	table->setItem(IdentifierType::Asin, "arcsin", [](const std::vector<double>& args) { return std::asin(args[0]); }, 1);
	table->setItem(IdentifierType::Acos, "arccos", [](const std::vector<double>& args) { return std::acos(args[0]); }, 1);
	table->setItem(IdentifierType::Atan, "arctan", [](const std::vector<double>& args) { return std::atan(args[0]); }, 1);
	table->setItem(IdentifierType::Sqrt, "sqrt", [](const std::vector<double>& args) { return std::sqrt(args[0]); }, 1);
	table->setItem(IdentifierType::Log, "log", [](const std::vector<double>& args) { return std::log(args[0]); }, 1);
	table->setItem(IdentifierType::Pi, "pi", [](const std::vector<double>&) { return 3.14159265358979323846; }, 0);
	table->setItem(IdentifierType::E, "e", [](const std::vector<double>&) { return 2.71828182845904523536; }, 0);
}

std::string IdentifierInfo::toString() const
{
	return name;
}

const IdentifierTable& IdentifierInfo::getTable()
{
	static IdentifierTable* table = nullptr;
	if (!table)
	{
		table = new IdentifierTable();
		initTable(table);
	}
	return *table;
}