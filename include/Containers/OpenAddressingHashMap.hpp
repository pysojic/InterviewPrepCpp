#pragma once

#include <tuple>
#include <vector>
#include <stdexcept>
#include <climits>
#include <optional>

// See Rigtorp implementation for a more complete/fine-tuned impl at https://github.com/rigtorp/HashMap

// I use an initial capacity that is a power of two to replace the expensive % when hashing a key by a bitwise &
#define INITIAL_BUCKET_COUNT 16
static_assert((INITIAL_BUCKET_COUNT & (INITIAL_BUCKET_COUNT - 1)) == 0, "BUCKET_COUNT should be a power of 2!");

// The following uses linear probing
namespace pysojic
{
    template <typename Key, typename Value, typename HashFunction = std::hash<Key>>
    class OpenAddressingHashMap
    {
        static_assert(std::is_default_constructible_v<Value>, "Value not default constructible!");
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

        size_t bucket_count() const noexcept { return m_Arr.size(); }
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
        return HashFunction{}(key) & (m_Arr.size() - 1);
    }

    template <typename Key, typename Value, typename HashFunction>
    size_t OpenAddressingHashMap<Key, Value, HashFunction>::hash_function(const Key& key, size_t table_size) const
    {
        return HashFunction{}(key) & (table_size - 1);
    }

    // I use a power of two
    template <typename Key, typename Value, typename HashFunction>
    OpenAddressingHashMap<Key, Value, HashFunction>::OpenAddressingHashMap()
        : m_Arr(INITIAL_BUCKET_COUNT), m_NumElems(0), m_MaxLoadFactor(0.8)
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
                    index = (index + 1) & (new_size - 1);
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
        std::optional<size_t> first_deleted; // an invalid index as a marker

        while (true)
        {
            if (m_Arr[index].state_ == Entry::State::EMPTY)
            {
                // If we saw a deleted slot earlier, use that instead
                if (first_deleted)
                {
                    index = first_deleted.value();
                }
                m_Arr[index] = Entry(key, val);
                ++m_NumElems;
                return;
            }
            else if (m_Arr[index].state_ == Entry::State::DELETED && !first_deleted)
            {
                first_deleted = index;
            }
            else if (m_Arr[index].state_ == Entry::State::OCCUPIED && m_Arr[index].key_ == key)
            {
                m_Arr[index].value_ = val;
                return;
            }
            index = (index + 1) & (m_Arr.size() - 1);
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
        std::optional<size_t> first_deleted;

        while (true)
        {
            if (m_Arr[index].state_ == Entry::State::EMPTY)
            {
                // Not found; insert new element
                if (first_deleted)
                {
                    index = first_deleted.value();
                }
                m_Arr[index] = Entry(key, Value{}); // Default-constructed value
                ++m_NumElems;
                return m_Arr[index].value_;
            }
            else if (m_Arr[index].state_ == Entry::State::DELETED && !first_deleted)
            {
                first_deleted = index;
            }
            else if (m_Arr[index].state_ == Entry::State::OCCUPIED && m_Arr[index].key_ == key)
            {
                return m_Arr[index].value_;
            }
            index = (index + 1) & (m_Arr.size() - 1);
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

            index = (index + 1) & (m_Arr.size() - 1);
            
            if (index == start)
                break;
        }
        throw std::out_of_range("Key not found");
    }
}