#include "Screen.hpp"
using namespace std;

TextQuery::TextQuery(ifstream& is)
	: file(new vector<string>)
{
	string text;
	while (getline(is, text))
	{
		file->push_back(text);
		int n = file->size() - 1;
		istringstream line(text);
		string word;
		while (line >> word)
		{
			auto& lines = wm[word];
			if (!lines)
			{
				lines.reset(new set<line_no>);
			}
			lines->insert(n);
		}
	}
}

QueryResult TextQuery::query(const std::string& sougth) const
{
	static shared_ptr<set<line_no>> nodata(new set<line_no>);
	auto loc = wm.find(sougth);
	if (loc == wm.end())
	{
		return QueryResult(sougth, nodata, file);
	}
	else
	{
		return QueryResult(sougth, loc->second, file);
	}
}

string make_plural(size_t ctr, const string& word, const string& ending)
{
	return (ctr > 1) ? word + ending : word;
}

ostream& print(ostream& os, const QueryResult& qr)
{
	os << qr.sougth << " occers " << qr.lines->size() << " " << make_plural(qr.lines->size(), "time", "s") << endl;
	for (auto item : *qr.lines)
	{
		os << "\t(line " << item + 1 << ") " << *(qr.file->begin() + item) << endl;
	}
	return os;
}

void runQueries(ifstream& infile)
{
	TextQuery tq(infile);
	while (true)
	{
		cout << "enter word to look for, or q to quit: ";
		string s;
		if (!(cin >> s) || s == "q")
			break;
		print(cout, tq.query(s));
	}
}
