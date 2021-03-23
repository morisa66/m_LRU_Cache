#ifndef __M_LIST_H__
#define __M_LIST_H__

#include <iostream>

#include "../_morisa.h"
#include "m_hash.h"


MORISA_NAMESPACE_BEGIN


template<typename K, typename V>
struct m_node
{
	m_node() {}
	m_node(K k, V v) :key(k), value(v) {}
	K key;
	V value;
	m_node* pre{ nullptr };
	m_node* next{ nullptr };
};

template<typename K, typename V>
class m_list
{
public:
	m_list()
	{
		_head = new m_node<K, V>;
		_tail = new m_node<K, V>;
		_head->pre = nullptr;
		_head->next = _tail;
		_tail->pre = _head;
		_tail->next = nullptr;
	}

	~m_list()
	{
		m_node<K, V>* p = _head;
		while (p)
		{
			m_node<K, V>* tmp = p;
			p = p->next;
			delete tmp;
		}
	}

	int size() noexcept { return _size; }

	void push_front(K key, V value)
	{
		push_front(new m_node(key, value));
	}

	void push_front(m_node<K, V>* node)
	{
		node->pre = _head;
		node->next = _head->next;
		_head->next = node;
		node->next->pre = node;
		++_size;
	}

	template <typename... Args>
	void push_front(K key, V value, Args&&... args)
	{
		push_front(key, value);
		push_front(std::forward<decltype(args)>(args)...);
	}

	void push_back(K key, V value)
	{
		push_back(new m_node<K, V>(key, value));
	}

	void push_back(m_node<K, V>* node)
	{
		node->pre = _tail->pre;
		node->next = _tail;
		_tail->pre = node;
		node->pre->next = node;
		++_size;
	}

	template <typename... Args>
	void push_back(K key, V value, Args&&... args)
	{
		push_back(key, value);
		push_back(std::forward<decltype(args)>(args)...);
	}

	m_node<K, V>* get(int i)
	{
		if (i >= _size || i < 0)
		{
			return nullptr;
		}
		m_node<K, V>* p = nullptr;
		if (i < (_size >> 1))
		{
			p = _head->next;
			for (int j = 0; j < i; ++j)
			{
				p = p->next;
			}
		}
		else
		{
			p = _tail->pre;
			for (int j = 0; j < _size - i - 1; ++j)
			{
				p = p->pre;
			}
		}
		return p;
	}

	void erase(m_node<K, V>* node)
	{
		node->pre->next = node->next;
		node->next->pre = node->pre;
		delete node;
		--_size;
	}

	void erase(int i)
	{
		m_node<K, V>* p = get(i);
		if (p)
		{
			erase(p);
		}
	}

	void disconnect(m_node<K, V>* node)
	{
		node->pre->next = node->next;
		node->next->pre = node->pre;
		--_size;
	}

	m_node<K, V>* pop_back()
	{
		m_node<K, V>* p = get(_size - 1);
		disconnect(p);
		return p;
	}

	m_node<K, V>* pop_front()
	{
		m_node<K, V>* p = get(0);
		disconnect(p);
		return p;
	}

	m_node<K, V>* find(K& key)
	{
		m_node<K, V>* p = _head->next;
		while (p != _tail)
		{
			if (m_hash(p->key) == m_hash(key))
			{
				return p;
			}
			p = p->next;
		}
		return nullptr;
	}



	void prints(std::ostream& os = std::cout)
	{
		for (m_node<K, V>* p = _head->next; p != _tail; p = p->next)
		{
			os << p->key << " -- " << p->value << std::endl;
		}
	}

	void prints_r(std::ostream& os = std::cout)
	{
		for (m_node<K, V>* p = _tail->pre; p != _head; p = p->pre)
		{
			os << p->key << " -- " << p->value << std::endl;
		}
	}

private:
	int _size{ 0 };
	m_node<K, V>* _head;
	m_node<K, V>* _tail;

};
MORISA_NAMESPACE_END

#endif
