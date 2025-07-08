#pragma once

#include <cstring>
#include <algorithm>
#include <iostream>

class String 
{
public:
    String() noexcept;
    String(const char* s);
    String(const String& other);
    String(String&& other) noexcept;
    String& operator=(const String& other);
    String& operator=(String&& other) noexcept;
    ~String() noexcept;

    char& operator[](std::size_t i) noexcept { return data()[i]; }
    const char& operator[](std::size_t i) const { return data()[i]; }
    std::size_t size() const noexcept { return is_sso() ? m_sso.size : m_heap.size; }
    const char* c_str() const noexcept { return data(); }
    void append(const char* s);

private:
    bool is_sso() const noexcept { return m_sso.flag == 1; }
    const char* data() const noexcept { return is_sso() ? m_sso.buffer : m_heap.data; }
    char* data() noexcept { return is_sso() ? m_sso.buffer : m_heap.data; }
    void init_sso() noexcept;
    void init_heap(std::size_t len);

private:
    static constexpr std::size_t SSO_THRESHOLD = 15;
    // tag: m_sso.flag==1 means SSO mode, else heap mode
    union 
    {
        struct 
        {
            char buffer[SSO_THRESHOLD + 1];
            std::uint8_t size;
            std::uint8_t flag; // 1 byte: 1=SSO, 0=heap
            // pad to align to 16 bytes total
            char pad[sizeof(void*) - 2];
        } m_sso;
        struct
        {
            char* data;
            std::size_t size;
            std::size_t capacity;
        } m_heap;
    };
};

//--------- Implementation ---------

String::String()
{
    init_sso();
}

String::String(const char* s)
{
    std::size_t len = std::strlen(s);
    if (len <= SSO_THRESHOLD) 
    {
        init_sso();
        std::memcpy(m_sso.buffer, s, len + 1);
        m_sso.size = len;
    } 
    else 
    {
        init_heap(len);
        std::memcpy(m_heap.data, s, len + 1);
        m_heap.size = len;
    }
}

String::String(const String& other)
{
    if (other.is_sso()) 
    {
        init_sso();
        std::memcpy(m_sso.buffer, other.m_sso.buffer, other.m_sso.size + 1);
        m_sso.size = other.m_sso.size;
    } 
    else 
    {
        init_heap(other.m_heap.size);
        std::memcpy(m_heap.data, other.m_heap.data, other.m_heap.size + 1);
        m_heap.size = other.m_heap.size;
    }
}

String::String(String&& other) noexcept
{
    if (other.is_sso()) 
    {
        init_sso();
        std::memcpy(m_sso.buffer, other.m_sso.buffer, other.m_sso.size + 1);
        m_sso.size = other.m_sso.size;
    } 
    else 
    {
        // steal heap
        m_heap.data = other.m_heap.data;
        m_heap.size = other.m_heap.size;
        m_heap.capacity = other.m_heap.capacity;
        other.init_sso();
    }
}

String& String::operator=(const String& other)
{
    if (this != &other) 
    {
        this->~String();
        new(this) String(other);
    }
    return *this;
}

String& String::operator=(String&& other) noexcept
{
    if (this != &other) 
    {
        this->~String();
        new(this) String(std::move(other));
    }
    return *this;
}

String::~String() noexcept
{
    if (!is_sso()) 
    {
        delete[] m_heap.data;
    }
}

void String::append(const char* s)
{
    std::size_t add_len = std::strlen(s);
    std::size_t old_len = size();
    std::size_t new_len = old_len + add_len;

    if (new_len <= SSO_THRESHOLD) 
    {
        // stay in SSO
        std::memcpy(m_sso.buffer + old_len, s, add_len + 1);
        m_sso.size = new_len;
    } else 
    {
        // need heap
        std::size_t new_cap = std::max<std::size_t>(new_len, old_len * 2);
        char* new_data = new char[new_cap + 1];

        // copy existing
        std::memcpy(new_data, data(), old_len);
        // append
        std::memcpy(new_data + old_len, s, add_len + 1);

        // clean up old
        if (!is_sso()) delete[] m_heap.data;

        m_heap.data = new_data;
        m_heap.size = new_len;
        m_heap.capacity = new_cap;
        m_sso.flag = 0;  // mark as heap
    }
}

void String::init_sso() noexcept
{
    m_sso.flag = 1;
    m_sso.size = 0;
    m_sso.buffer[0] = '\0';
}

void String::init_heap(size_t len)
{
    m_sso.flag = 0;
    m_heap.capacity = len;
    m_heap.size = len;
    m_heap.data = new char[len + 1];
    m_heap.data[len] = '\0';
}