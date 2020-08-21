#include "Screen.hpp"
using namespace std;

StrVec::StrVec(initializer_list<string>& il)
{
	auto capacity = il.size() ? il.size() + il.size() / 2 : 1;
	auto newdata = alloc.allocate(capacity);
	auto free = uninitialized_copy(il.begin(), il.end(), newdata);
	elements = newdata;
	first_free = free;
	cap = elements + capacity;
}

void StrVec::push_back(const std::string& s)
{
	chk_n_alloc();
	// 在first_free指向的元素中构造s的副本
	alloc.construct(first_free++, s);
}

void StrVec::reallocate()
{
	auto newcapacity = size() ? size() * 2 : 1;
	auto newdata = alloc.allocate(newcapacity);
	auto dest = newdata;
	auto elem = elements;
	for (size_t i = 0; i != size(); ++i)
	{
		alloc.construct(dest++, std::move(*elem++));
	}
	free();
	elements = newdata;
	first_free = dest;
	cap = elements + newcapacity;
}

pair<string*, string*> StrVec::alloc_n_copy(const string* b, const string* e)
{
	auto data = alloc.allocate(e - b);
	return {data, uninitialized_copy(b, e, data)};
}

void StrVec::free()
{
	for_each(first_free, elements, [](const string* s) { alloc.destroy(s); });
	// while (first_free != elements)
	// {
	// 	alloc.destroy(--first_free);
	// }
	alloc.deallocate(elements, cap - elements);
}

StrVec::StrVec(const StrVec& s)
{
	auto newdata = alloc_n_copy(s.begin(), s.end());
	elements = newdata.first;
	first_free = cap = newdata.second;
}

StrVec& StrVec::operator=(const StrVec& rhs)
{
	auto data = alloc_n_copy(rhs.begin(), rhs.end());
	free();
	elements = data.first;
	first_free = cap = data.second;
	return *this;
}

StrVec::~StrVec()
{
	free();
}

void StrVec::reserve(const size_t& sz)
{
	if (sz > capacity())
	{
		auto newdata = alloc.allocate(sz);
		auto elem = elements;
		auto dest = newdata;
		for (size_t i = 0; i != size(); ++i)
		{
			alloc.construct(dest++, std::move(*elem++));
		}
		free();
		elements = newdata;
		first_free = dest;
		cap = elements + sz;
	}
}

void StrVec::resize(const size_t& sz, const string& s)
{
	if (sz < capacity())
	{
		if (sz > size())
		{
			auto newfree = elements + sz;
			while (first_free != newfree)
				alloc.construct(first_free++, s);
		}
		else if (sz < size())
		{
			auto newfree = elements + sz;
			while (first_free != newfree)
			{
				alloc.destroy(--first_free);
			}
		}
	}
	else if (sz > capacity())
	{
		auto newsz = sz + sz / 2;
		reserve(newsz);
	}
}
