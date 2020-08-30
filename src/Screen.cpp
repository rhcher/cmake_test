#include "Screen.hpp"
using namespace std;

allocator<string> StrVec::alloc;

StrVec::StrVec(const initializer_list<string>& il)
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

void StrVec::push_back(std::string&& s) &&
{
	chk_n_alloc();
	alloc.construct(first_free++, std::move(s));
}

void StrVec::reallocate()
{
	auto newcapacity = size() ? size() * 2 : 1;
	auto newdata = alloc.allocate(newcapacity);
	auto dest = uninitialized_copy(make_move_iterator(begin()), make_move_iterator(end()), newdata);
	// auto dest = newdata;
	// auto elem = elements;
	// for (size_t i = 0; i != size(); ++i)
	// {
	// 	alloc.construct(dest++, std::move(*elem++));
	// }
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
	if (elements)
	{
		for_each(elements, first_free, [](const string& s) { alloc.destroy(&s); });
		alloc.deallocate(elements, cap - elements);
	}
	// while (first_free != elements)
	// {
	// 	alloc.destroy(--first_free);
	// }
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

StrVec::StrVec(StrVec&& s) noexcept
	: elements(s.elements), first_free(s.first_free), cap(s.cap)
{
	s.elements = s.first_free = s.cap = nullptr;
}

StrVec& StrVec::operator=(StrVec&& rhs) noexcept
{
	if (this != &rhs)
	{
		free();
		elements = rhs.elements;
		first_free = rhs.first_free;
		cap = rhs.cap;
		rhs.elements = rhs.first_free = rhs.cap = nullptr;
	}
	return *this;
}

ostream& operator<<(ostream& os, const StrVec& s)
{
	for (auto b = s.begin(); b != s.end(); ++b)
	{
		os << *b << endl;
	}
	return os;
}

istream& operator>>(istream& in, StrVec& s)
{
	for (auto b = s.begin(); b != s.end(); ++b)
	{
		in >> *b;
	}
	return in;
}

bool operator==(const StrVec& lhs, const StrVec& rhs)
{
	return lhs.size() == rhs.size();
}

bool operator!=(const StrVec& lhs, const StrVec& rhs)
{
	return !(lhs == rhs);
}

bool operator<(const StrVec& lhs, const StrVec& rhs)
{
	bool result = true;
	if (lhs.size() != rhs.size())
	{
		if (lhs.size() < rhs.size())
		{
			result = true;
		}
		else
		{
			result = false;
		}
	}
	else
	{
		for (auto p = lhs.begin(), b = rhs.begin(); p != lhs.end() && b != rhs.end(); ++p, ++b)
		{
			if (*p < *b)
			{
				result = true;
			}
			else
			{
				result = false;
			}
		}
	}
	return result;
}

StrVec& StrVec::operator=(const initializer_list<string>& il)
{
	return *this = StrVec(il);
}

StrVec& StrVec::operator+=(const StrVec& s)
{
	auto size = this->size() + s.size();
	auto elem = alloc.allocate(size);
	auto free = uninitialized_copy(this->begin(), this->end(), elem);
	free = uninitialized_copy(s.begin(), s.end(), free);
	elements = elem;
	first_free = free;
	cap = elements + size;
	return *this;
}

string& StrVec::operator[](const size_t& s)
{
	return *(elements + s);
}

const string& StrVec::operator[](const size_t& index) const
{
	return *(elements + index);
}

StrVec::operator bool() const
{
	return true;
}
