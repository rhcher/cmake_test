#include <cctype>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

class StrVec
{
	friend std::ostream& operator<<(std::ostream& os, const StrVec& s);
	friend std::istream& operator>>(std::istream& in, StrVec& s);
	friend bool operator==(const StrVec& lhs, const StrVec& rhs);
	friend bool operator!=(const StrVec& lhs, const StrVec& rhs);
	friend bool operator<(const StrVec& lhs, const StrVec& rhs);

public:
	StrVec()
		: elements(nullptr), first_free(nullptr), cap(nullptr) {}
	StrVec(const std::initializer_list<std::string>& il);
	StrVec(const StrVec&);					   // 拷贝构造函数
	StrVec(StrVec&& s) noexcept;			   // 移动构造函数
	StrVec& operator=(const StrVec& rhs);	   // 拷贝赋值运算符
	StrVec& operator=(StrVec&& rhs) noexcept;  // 移动赋值运算符
	StrVec& operator=(const std::initializer_list<std::string>&);
	StrVec& operator+=(const StrVec& s);
	explicit operator bool() const;
	std::string& operator[](const size_t& index);
	const std::string& operator[](const size_t& index) const;
	~StrVec();	// 析构函数

	void push_back(const std::string& s);
	void push_back(std::string&& s) &&;
	void reserve(const size_t& sz);
	void resize(const size_t& sz, const std::string& s = std::string());
	size_t size() const
	{
		return first_free - elements;
	}
	size_t capacity() const
	{
		return cap - elements;
	}
	std::string* begin() const
	{
		return elements;
	}
	std::string* end() const
	{
		return first_free;
	}

private:
	static std::allocator<std::string> alloc;  // 分配元素
	std::string* elements;					   // 指向数组首元素的指针
	std::string* first_free;				   // 指向第一个空闲元素的指针
	std::string* cap;						   // 指向数组尾后位置的指针

	// 被添加元素的函数所使用
	void chk_n_alloc()
	{
		if (size() == capacity())
		{
			reallocate();
		}
	}
	// 工具函数，被拷贝构造函数、赋值运算符和析构函数使用
	std::pair<std::string*, std::string*> alloc_n_copy(const std::string* s1, const std::string* s2);
	// 销毁元素并释放内存
	void free();
	// 获得更多内存并拷贝已有元素
	void reallocate();
};
