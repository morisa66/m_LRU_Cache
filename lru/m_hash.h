#ifndef __M_HASH_H__
#define __M_HASH_H__

#include <string>

#include "../_morisa.h"

MORISA_NAMESPACE_BEGIN

// ELF hash implements.
inline size_t elf_hash(const char* str, size_t len)
{
	size_t _hash = 0;
	size_t _g = 0;
	for (size_t i = 0; i < len; ++i)
	{
		char c = str[i];
		_hash = (_hash << 4) + (c * 13);
		_g = _hash & 0xf0000000;
		if (_g)
		{
			_hash ^= (_g >> 24);
			_hash ^= _g;
		}
	}
	return _hash;
}

template<typename KEY>
inline size_t hash(const KEY& key)
{
	return (size_t)key;
}

template<>
inline size_t hash<std::string>(const std::string& key)
{
	return elf_hash(key.c_str(), key.length());
}

template<>
inline size_t hash<unsigned long long>(const unsigned long long& key)
{
	return elf_hash((const char*)&key, sizeof(unsigned long long));
}

template <typename...>
using m_void = void;

struct m_false { constexpr static bool value{ false }; };
struct m_true { constexpr static bool value{ true }; };

template <typename T, typename = m_void<>>
struct m_has_hash : m_false { };
template <typename T>
struct m_has_hash<T, m_void<decltype(std::declval<T&>().hash())>> : m_true { };

template <typename K>
std::enable_if_t<m_has_hash<K>::value, size_t> m_hash(K key)
{
	return key.hash();
}

template <typename K>
std::enable_if_t<!m_has_hash<K>::value, size_t> m_hash(K key)
{
	return hash(key);
}

MORISA_NAMESPACE_END

#endif
