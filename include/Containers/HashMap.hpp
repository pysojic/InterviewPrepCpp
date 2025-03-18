#pragma once

#include <vector>
#include <list>
#include <tuple>
#include <stdexcept>

namespace pysojic
{
    template <typename Key, typename Value, typename HashFunction = std::hash<Key>>
    class HashMap
    {
    public:
        HashMap();
        HashMap(const HashMap& other);
        HashMap& operator=(const HashMap& other);
        HashMap(HashMap&& other) noexcept;
        HashMap& operator=(HashMap&& other);
        
        void insert(const Key& key, const Value& val);
        void remove(const Key& key);
        Value& operator[](const Key& key);
        
        const Value& at(const Key& key) const;
        void find(const Key& key) const;
        bool empty() const noexcept;
        size_t size() const noexcept;
        double load_factor() const noexcept;
        
    private:
        size_t hash_function(const Key& key) const;
        size_t hash_function(const Key& key, size_t newBucketCount) const;
        void rehash(size_t count);

    private:
        std::vector<std::list<std::pair<Key, Value>>> m_Buckets;
        size_t m_NumElems;
        double m_MaxLoadFactor;
        inline static size_t m_InitialBucketCount = 10;
    };

    //------------Implementation--------------

    template <typename Key, typename Value, typename HashFunction>
    size_t HashMap<Key, Value, HashFunction>::hash_function(const Key& key) const
    {
        return HashFunction{}(key) % m_Buckets.size();
    }

    template <typename Key, typename Value, typename HashFunction>
    size_t HashMap<Key, Value, HashFunction>::hash_function(const Key& key, size_t newBucketCount) const
    {
        return HashFunction{}(key) % newBucketCount;
    }

    template <typename Key, typename Value, typename HashFunction>
    void HashMap<Key, Value, HashFunction>::rehash(size_t count) 
    {
        size_t bucketsCount = m_Buckets.size();
        if (count > bucketsCount)
        {
            std::vector<std::list<std::pair<Key, Value>>> newBuckets(count);
            
            for (auto& list : m_Buckets)
                for (auto& pair : list)
                {
                    size_t newHash = hash_function(pair.first, count);
                    newBuckets[newHash].push_back(std::move(pair));
                }
            m_Buckets = std::move(newBuckets);
        }
    }

    template <typename Key, typename Value, typename HashFunction>
    HashMap<Key, Value, HashFunction>::HashMap()
        : m_Buckets{m_InitialBucketCount}, m_NumElems{}, m_MaxLoadFactor{1.0}
    {}

    template <typename Key, typename Value, typename HashFunction>
    HashMap<Key, Value, HashFunction>::HashMap(const HashMap& other)
        : m_Buckets{other.m_Buckets}, m_NumElems{other.m_NumElems}, m_MaxLoadFactor{other.m_MaxLoadFactor}
    {}

    template <typename Key, typename Value, typename HashFunction>
    auto HashMap<Key, Value, HashFunction>::operator= (const HashMap& other) -> HashMap&
    {
        if (this != &other)
        {
            m_Buckets = other.m_Buckets;
            m_NumElems = other.m_NumElems;
            m_MaxLoadFactor = other.m_MaxLoadFactor;
        }

        return *this;
    }

    template <typename Key, typename Value, typename HashFunction>
    HashMap<Key, Value, HashFunction>::HashMap(HashMap&& other) noexcept
        : m_Buckets{std::move(other.m_Buckets)}, m_NumElems{std::exchange(other.m_NumElems, 0)}, 
        m_MaxLoadFactor{std::exchange(other.m_MaxLoadFactor, 0 )}
    {}

    template <typename Key, typename Value, typename HashFunction>
    auto HashMap<Key, Value, HashFunction>::operator= (HashMap&& other) -> HashMap&
    {
        if (this != &other)
        {
            m_Buckets = std::move(other.m_Buckets);
            m_NumElems = other.m_NumElems;
            m_MaxLoadFactor = other.m_MaxLoadFactor;

            other.m_NumElems = 0;
            other.m_MaxLoadFactor = 0;
        }

        return *this;
    }

    template <typename Key, typename Value, typename HashFunction>
    void HashMap<Key, Value, HashFunction>::insert(const Key& key, const Value& val)
    {
        size_t hashVal = hash_function(key);
        
        for (auto& [k, v] : m_Buckets[hashVal])
        {
            if (k == key)
            {
                v = val;
                return;
            }
        }

        m_Buckets[hashVal].push_back({key, val});
        ++m_NumElems;

        if (load_factor() > m_MaxLoadFactor)
            rehash(m_Buckets.size() * 2);
    }

    template <typename Key, typename Value, typename HashFunction>
    Value& HashMap<Key, Value, HashFunction>::operator[] (const Key& key)
    {
        size_t hashVal = hash_function(key);
        
        for (auto& list : m_Buckets)
            for (auto& [k, v] : list)
            {
                if (k == key)
                    return v;
            }
        
        m_Buckets[hashVal].push_back({key, Value{}});
        ++m_NumElems;

        if (load_factor() > m_MaxLoadFactor)
        {
            rehash(m_Buckets.size() * 2);
        }

        hashVal = hash_function(key);
        for (auto& list : m_Buckets)
            for (auto& [k, v] : list)
            {
                if (k == key)
                    return v;
            }

        throw std::runtime_error{"Unexpected error"};
    }

    template <typename Key, typename Value, typename HashFunction>
    const Value& HashMap<Key, Value, HashFunction>::at(const Key& key) const
    {
        size_t hashVal = hash_function(key);
        
        for (auto& list : m_Buckets)
            for (auto& [k, v] : list)
            {
                if (k == key)
                    return v;
            }

        throw std::out_of_range{"Key not found"};
    }

    template <typename Key, typename Value, typename HashFunction>
    bool HashMap<Key, Value, HashFunction>::empty() const noexcept
    {
        return m_NumElems == 0 ? true : false;
    }

    template <typename Key, typename Value, typename HashFunction>
    size_t HashMap<Key, Value, HashFunction>::size() const noexcept
    {
        return m_NumElems;
    }

    template <typename Key, typename Value, typename HashFunction>
    double HashMap<Key, Value, HashFunction>::load_factor() const noexcept
    {
        return static_cast<double>(m_NumElems) / m_Buckets.size();
    }

}