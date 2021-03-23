#ifndef __M_MAP_H__
#define __M_MAP_H__

#include "../_morisa.h"
#include "m_list.h"
#include "m_hash.h"


MORISA_NAMESPACE_BEGIN


template<typename K, typename V>
class m_map
{
public:
	m_map(size_t length = 4, V default_value = V()) :_length(length), _default_value(default_value)
	{
		_tables = new m_list<K, V>[1 << _length];
	}

	~m_map()
	{
		delete[] _tables;
	}

	void put(K key, V value)
	{
		size_t _i = m_hash(key) & ((1 << _length) - 1);
		m_node<K, V>* p = _tables[_i].find(key);
		if (p)
		{
			p->value = value;
		}
		else
		{
			_tables[_i].push_back(key, value);
		}
	}

	template <typename... Args>
	void put(K key, V value, Args&&... args)
	{
		put(key, value);
		put(std::forward<decltype(args)>(args)...);
	}

	const V get(K key)
	{
		size_t _i = m_hash(key) & ((1 << _length) - 1);
		m_node<K, V>* p = _tables[_i].find(key);
		if (p)
		{
			return p->value;
		}
		return _default_value;
	}

	V& operator[](K key)
	{
		size_t _i = (m_hash(key)) & ((1 << _length) - 1);
		m_node<K, V>* p = _tables[_i].find(key);
		if (!p)
		{
			p = new m_node<K, V>(key, _default_value);
			_tables[_i].push_back(p);
		}
		return p->value;
	}

	void prints(std::ostream& os = std::cout)
	{
		for (int i = 0; i < (1 << _length); ++i)
		{
			os << "the " << i << "TH table is :" << std::endl;
			_tables[i].prints();
		}
	}

	void prints_r(std::ostream& os = std::cout)
	{
		for (int i = 0; i < (1 << _length); ++i)
		{
			os << "the " << i << "TH table is :" << std::endl;
			_tables[i].prints_r();
		}
	}

private:
	size_t _length;
	V _default_value;
	m_list<K, V>* _tables;
};

MORISA_NAMESPACE_END

#endif