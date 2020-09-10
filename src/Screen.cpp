#include "Screen.h"
using namespace std;
using namespace QueryText;

TextQuery::TextQuery(istream& in)
	: file(new vector<string>)
{
	string text;
	while (getline(in, text))
	{
		file->push_back(text);
		auto n = file->size() - 1;
		istringstream line(text);
		string word;
		while (line >> word)
		{
			wordProcess(&word);
			auto& lines = wm[word];
			if (!lines)
			{
				lines.reset(new set<line_no>);
			}
			lines->insert(n);
		}
	}
}

QueryResult TextQuery::query(const std::string& s) const
{
	static shared_ptr<set<line_no>> nodata(new set<line_no>);
	auto loc = wm.find(s);
	if (loc == wm.end())
	{
		return QueryResult(s, nodata, file);
	}
	return QueryResult(s, loc->second, file);
}

ostream& print(ostream& os, const QueryResult& qr)
{
	os << qr.sought << " occurs " << qr.lines->size() << " "
	   << "times" << endl;
	for (auto item : *qr.lines)
	{
		os << "\t(line " << item + 1 << ") " << *(qr.file->begin() + item) << endl;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const Query& query)
{
	return os << query.rep();
}

QueryResult OrQuery::eval(const TextQuery& text) const
{
	auto right = get_rhs().eval(text);
	auto left = get_lhs().eval(text);
	auto ret_lines = make_shared<set<line_no>>(left.begin(), left.end());
	ret_lines->insert(right.begin(), right.end());
	return QueryResult(rep(), ret_lines, left.get_file());
}

QueryResult AndQuery::eval(const TextQuery& text) const
{
	auto left = get_lhs().eval(text);
	auto right = get_rhs().eval(text);
	auto ret_lines = make_shared<set<line_no>>();
	set_intersection(left.begin(), left.end(), right.begin(), right.end(), inserter(*ret_lines, ret_lines->begin()));
	return QueryResult(rep(), ret_lines, left.get_file());
}

QueryResult NotQuery::eval(const TextQuery& text) const
{
	auto result = query.eval(text);
	auto ret_lines = make_shared<set<line_no>>();
	auto beg = result.begin();
	auto end = result.end();
	auto sz = result.get_file()->size();
	for (size_t n = 0; n != sz; ++n)
	{
		if (beg == end || *beg != n)
		{
			ret_lines->insert(n);
		}
		else if (beg != end)
		{
			++beg;
		}
	}
	return QueryResult(rep(), ret_lines, result.get_file());
}
