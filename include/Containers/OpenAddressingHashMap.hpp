#pragma once

#include <tuple>
#include <vector>
#include <stdexcept>

// The following uses linear probing
namespace pysojic
{
    template <typename Key, typename Value, typename HashFunction = std::hash<Key>>
    class OpenAddressingHashMap
    {
        class Entry
        {
            friend class OpenAddressingHashMap;
        public:
            enum class State
            {
                EMPTY,
                OCCUPIED,
                DELETED
            };

            Entry()
                : key_{}, value_{}, state_{State::EMPTY}
            {}
            Entry(const Key& key, const Value& val)
                : key_{key}, value_{val}, state_{State::OCCUPIED}
            {}

        private:
            Key key_;
            Value value_;
            State state_ = State::EMPTY;
        };

    public:
        OpenAddressingHashMap();

        void insert(const Key& key, const Value& val);
        void remove(const Key& key);
        Value& operator[](const Key& key);
        void rehash(size_t new_size);

        size_t size() const noexcept { return m_NumElems; }
        size_t capacity() const noexcept { return m_Arr.capacity(); }
        double load_factor() const noexcept { return static_cast<double>(m_NumElems) / m_Arr.size(); }

    private:
        size_t hash_function(const Key& key) const;
        size_t hash_function(const Key& key, size_t table_size) const;

    private:
        std::vector<Entry> m_Arr;
        size_t m_NumElems;
        double m_MaxLoadFactor;
    };

    //------------ Implementation ------------

    template <typename Key, typename Value, typename HashFunction>
    size_t OpenAddressingHashMap<Key, Value, HashFunction>::hash_function(const Key& key) const
    {
        return HashFunction{}(key) % m_Arr.size();
    }

    template <typename Key, typename Value, typename HashFunction>
    size_t OpenAddressingHashMap<Key, Value, HashFunction>::hash_function(const Key& key, size_t table_size) const
    {
        return HashFunction{}(key) % table_size;
    }

    template <typename Key, typename Value, typename HashFunction>
    OpenAddressingHashMap<Key, Value, HashFunction>::OpenAddressingHashMap()
        : m_Arr(19), m_NumElems(0), m_MaxLoadFactor(0.8)
    {
    }

    // Rehash: Create a new table of size new_size and reinsert all OCCUPIED entries
    template <typename Key, typename Value, typename HashFunction>
    void OpenAddressingHashMap<Key, Value, HashFunction>::rehash(size_t new_size)
    {
        std::vector<Entry> new_arr(new_size);

        for (size_t i = 0; i < m_Arr.size(); ++i)
        {
            if (m_Arr[i].state_ == Entry::State::OCCUPIED)
            {
                size_t index = hash_function(m_Arr[i].key_, new_size);
                while (new_arr[index].state_ == Entry::State::OCCUPIED)
                {
                    index = (index + 1) % new_size;
                }
                new_arr[index] = std::move(m_Arr[i]);
            }
        }
        m_Arr = std::move(new_arr);
    }

    template <typename Key, typename Value, typename HashFunction>
    void OpenAddressingHashMap<Key, Value, HashFunction>::insert(const Key& key, const Value& val)
    {
        // Rehash if the load factor is exceeded
        if (load_factor() >= m_MaxLoadFactor)
        {
            rehash(m_Arr.size() * 2);
        }
        
        size_t index = hash_function(key);
        size_t start = index;
        size_t first_deleted = m_Arr.size(); // an invalid index as a marker

        while (true)
        {
            if (m_Arr[index].state_ == Entry::State::EMPTY)
            {
                // If we saw a deleted slot earlier, use that instead
                if (first_deleted != m_Arr.size())
                {
                    index = first_deleted;
                }
                m_Arr[index] = Entry(key, val);
                ++m_NumElems;
                return;
            }
            else if (m_Arr[index].state_ == Entry::State::DELETED && first_deleted == m_Arr.size())
            {
                first_deleted = index;
            }
            else if (m_Arr[index].state_ == Entry::State::OCCUPIED && m_Arr[index].key_ == key)
            {
                m_Arr[index].value_ = val;
                return;
            }
            index = (index + 1) % m_Arr.size();
            if (index == start)
            {
                // Should never happen because we rehash before full
                throw std::runtime_error("HashMap is full, cannot insert");
            }
        }
    }

    template <typename Key, typename Value, typename HashFunction>
    Value& OpenAddressingHashMap<Key, Value, HashFunction>::operator[](const Key& key)
    {
        // Rehash if needed
        if (load_factor() >= m_MaxLoadFactor)
        {
            rehash(m_Arr.size() * 2);
        }
        
        size_t index = hash_function(key);
        size_t start = index;
        size_t first_deleted = m_Arr.size();

        while (true)
        {
            if (m_Arr[index].state_ == Entry::State::EMPTY)
            {
                // Not found; insert new element
                if (first_deleted != m_Arr.size())
                {
                    index = first_deleted;
                }
                m_Arr[index] = Entry(key, Value{}); // Default-constructed value
                ++m_NumElems;
                return m_Arr[index].value_;
            }
            else if (m_Arr[index].state_ == Entry::State::DELETED && first_deleted == m_Arr.size())
            {
                first_deleted = index;
            }
            else if (m_Arr[index].state_ == Entry::State::OCCUPIED && m_Arr[index].key_ == key)
            {
                return m_Arr[index].value_;
            }
            index = (index + 1) % m_Arr.size();
            if (index == start)
            {
                // Should never happen if rehashing is working correctly
                throw std::runtime_error("HashMap is full");
            }
        }
    }

    template <typename Key, typename Value, typename HashFunction>
    void OpenAddressingHashMap<Key, Value, HashFunction>::remove(const Key& key)
    {
        size_t index = hash_function(key);
        size_t start = index;

        while (m_Arr[index].state_ != Entry::State::EMPTY)
        {
            if (m_Arr[index].state_ == Entry::State::OCCUPIED && m_Arr[index].key_ == key)
            {
                m_Arr[index].state_ = Entry::State::DELETED;
                --m_NumElems;
                return;
            }

            index = (index + 1) % m_Arr.size();
            
            if (index == start)
                break;
        }
        throw std::out_of_range("Key not found");
    }
}