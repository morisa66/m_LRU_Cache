#ifndef __M_LRU_CACHE_H__
#define __M_LRU_CACHE_H__


#include "../_morisa.h"
#include "m_map.h"
#include "m_list.h"

MORISA_NAMESPACE_BEGIN

#define _MAP_LENGTH_ 4

// the oldest is on the tail, and the lasted is on the head.
template<typename K, typename V>
class m_LRU_cache
{
public:
	m_LRU_cache(int capacity , V default_value = V()): _capacity(capacity), _default_value(default_value){ }

	~m_LRU_cache() { }

	const int capacity() const noexcept { return _capacity; }

	void reverse(int capacity) { _capacity = capacity; };

	// find value by key in LRU cache.
	V get(K key)
	{
		m_node<K, V>* p = _m_map.get(key);
		if (p)
		{
			_m_list.disconnect(p);
			_m_list.push_back(p);
			return p->value;
		}
		return _default_value;
	}

	// if key is existed: change the value.
	// if key if not existed and _capacity is enough : insert the new key - value pair.
	// else : erase the oldest data, and insert the new key - value pair.
	void put(K key, V value)
	{
		m_node<K, V>* p = _m_map.get(key);
		if (p)
		{
			_m_list.disconnect(p);
			p->value = value;
			_m_list.push_back(p);
			return;
		}
		if (_m_list.size() < _capacity)
		{
			p = new m_node<K, V>(key, value);
		}
		else
		{
			p = _m_list.pop_front();
			p->key = key;
			p->value = value;

		}
		_m_map[key] = p;
		_m_list.push_back(p);
	}
	
	template <typename... Args>
	void put(K key, V value, Args&&... args)
	{
		put(key, value);
		put(std::forward<decltype(args)>(args)...);
	}

	void prints(std::ostream& os = std::cout)
	{
		_m_list.prints(os);
	}

	void prints_r(std::ostream& os = std::cout)
	{
		_m_list.prints_r(os);
	}

private :
	int _capacity;
	V _default_value;
	m_map<K, m_node<K, V>*> _m_map{ _MAP_LENGTH_, nullptr};
	m_list<K, V> _m_list;
};


MORISA_NAMESPACE_END

#endif 

