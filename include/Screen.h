#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace QueryText
{
class QueryResult;
class TextQuery
{
public:
	using line_no = std::vector<std::string>::size_type;
	explicit TextQuery(std::istream& in);
	QueryResult query(const std::string& s) const;

private:
	const std::string punctuation = {R"(!@#$%^&*()-_=+:;"'<,>.?/\)"};
	void wordProcess(std::string* s)
	{
		auto beg = s->find_first_not_of(punctuation);
		auto end = s->find_first_of(punctuation, beg);
		*s = s->substr(beg, end - beg);
		for (auto& item : *s)
		{
			item = tolower(item);
		}
	}
	std::shared_ptr<std::vector<std::string>> file;				   // 输入文件
	std::map<std::string, std::shared_ptr<std::set<line_no>>> wm;  // 每个单词到它所出现的行号的映射
};

std::ostream& print(std::ostream& os, const QueryResult& qr);

class QueryResult
{
	friend std::ostream& print(std::ostream& os, const QueryResult& qr);

public:
	using line_no = TextQuery::line_no;
	QueryResult(std::string s, std::shared_ptr<std::set<line_no>> p, std::shared_ptr<std::vector<std::string>> f)
		: sought(std::move(s)), lines(std::move(p)), file(std::move(f)) {}
	std::set<line_no>::iterator begin()
	{
		return lines->begin();
	}
	std::set<line_no>::iterator end()
	{
		return lines->end();
	}
	std::shared_ptr<std::vector<std::string>> get_file()
	{
		return file;
	}

private:
	std::string sought;								 // 查询单词
	std::shared_ptr<std::set<line_no>> lines;		 // 出现的行号
	std::shared_ptr<std::vector<std::string>> file;	 // 输入文件
};

class QueryBase
{
	friend class Query;

public:
	QueryBase() = default;
	QueryBase(const QueryBase& q) = default;
	QueryBase(QueryBase&& q) = default;
	QueryBase& operator=(const QueryBase& q) = default;
	QueryBase& operator=(QueryBase&& q) = default;

protected:
	using line_no = TextQuery::line_no;
	virtual ~QueryBase() = default;

private:
	[[nodiscard]] virtual QueryResult eval(const TextQuery& t) const = 0;  // eval返回与当前QUERY匹配的QueryResult
	[[nodiscard]] virtual std::string rep() const = 0;					   // 表示查询的一个string
};

class Query
{
	friend Query operator~(const Query& operand);
	friend Query operator|(const Query& lhs, const Query& rhs);
	friend Query operator&(const Query& lhs, const Query& rhs);

public:
	explicit Query(const std::string& s);
	[[nodiscard]] QueryResult eval(const TextQuery& t) const
	{
		return q->eval(t);
	}
	[[nodiscard]] std::string rep() const
	{
		return q->rep();
	}

private:
	explicit Query(std::shared_ptr<QueryBase> query)
		: q(std::move(query)) {}
	std::shared_ptr<QueryBase> q;
};

class WordQuery : public QueryBase
{
	friend class Query;
	explicit WordQuery(std::string s)
		: query_word(std::move(s)) {}
	[[nodiscard]] QueryResult eval(const TextQuery& t) const override
	{
		return t.query(query_word);
	}
	[[nodiscard]] std::string rep() const override
	{
		return query_word;
	}
	std::string query_word;	 // 要查找的单词
};

inline Query::Query(const std::string& s)
	: q(new WordQuery(s)) {}

class NotQuery : public QueryBase
{
	friend Query operator~(const Query& operand);
	explicit NotQuery(Query q)
		: query(std::move(q)) {}
	[[nodiscard]] std::string rep() const override
	{
		return "~(" + query.rep() + ")";
	}
	[[nodiscard]] QueryResult eval(const TextQuery& t) const override;
	Query query;
};

inline Query operator~(const Query& operand)
{
	return Query(std::shared_ptr<QueryBase>(new NotQuery(operand)));
}

class BinaryQuery : public QueryBase
{
protected:
	BinaryQuery(Query l, Query r, std::string s)
		: lhs(std::move(l)), rhs(std::move(r)), opSym(std::move(s)) {}
	[[nodiscard]] std::string rep() const override
	{
		return "(" + lhs.rep() + " " + opSym + " " + rhs.rep() + ")";
	}
	[[nodiscard]] Query get_lhs() const
	{
		return lhs;
	}
	[[nodiscard]] Query get_rhs() const
	{
		return rhs;
	}

private:
	Query lhs, rhs;		// 左侧和右侧的运算对象
	std::string opSym;	// 运算符的名字
};

class AndQuery : public BinaryQuery
{
	friend Query operator&(const Query& lhs, const Query& rhs);
	AndQuery(const Query& lhs, const Query& rhs)
		: BinaryQuery(lhs, rhs, "&") {}
	[[nodiscard]] QueryResult eval(const TextQuery& t) const override;
};

inline Query operator&(const Query& lhs, const Query& rhs)
{
	return static_cast<Query>(std::shared_ptr<QueryBase>(new AndQuery(lhs, rhs)));
}

class OrQuery : public BinaryQuery
{
	friend Query operator|(const Query& lhs, const Query& rhs);
	OrQuery(const Query& lhs, const Query& rhs)
		: BinaryQuery(lhs, rhs, "|") {}
	[[nodiscard]] QueryResult eval(const TextQuery& t) const override;
};

inline Query operator|(const Query& lhs, const Query& rhs)
{
	return Query(std::shared_ptr<QueryBase>(new OrQuery(lhs, rhs)));
}
};	// namespace QueryText
