#pragma once
#include <algorithm>
#include <cctype>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

class QueryResult;
class TextQuery
{
public:
	using line_no = std::vector<std::string>::size_type;
	TextQuery() = default;
	TextQuery(std::ifstream&);
	QueryResult query(const std::string&) const;

private:
	std::shared_ptr<std::vector<std::string>> file;
	std::map<std::string, std::shared_ptr<std::set<line_no>>> wm;
};

class QueryResult
{
	friend std::ostream& print(std::ostream&, const QueryResult&);

public:
	using line_no = std::vector<std::string>::size_type;
	QueryResult() = default;
	QueryResult(std::string s, std::shared_ptr<std::set<line_no>> l, std::shared_ptr<std::vector<std::string>> f)
		: sougth(s), lines(l), file(f) {}

private:
	std::string sougth;
	std::shared_ptr<std::set<line_no>> lines;
	std::shared_ptr<std::vector<std::string>> file;
};

void runQueries(std::ifstream& infile);
