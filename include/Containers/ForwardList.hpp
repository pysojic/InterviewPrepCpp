#pragma once

#include <initializer_list>

namespace pysojic
{
    template <typename T>
    class ForwardList
    {
        template <typename K>
        class Node
        {
            friend ForwardList;
            explicit Node(const K& val);
            explicit Node(K&& val);
            Node(const Node& other);
            Node(Node&& other) noexcept;
            template <typename... Args>
            Node(Args&&... args);

            K m_Data;
            Node* m_Next;
        };

    public:
        ForwardList();
        ForwardList(size_t size);
        ForwardList(std::initializer_list<T> l);
        ForwardList(const ForwardList& other);
        ForwardList& operator=(const ForwardList& other);
        ForwardList(ForwardList&& other) noexcept;
        ForwardList& operator=(ForwardList&& other) noexcept;
        ~ForwardList();

        void push_front(const T& obj);
        void push_front(T&& obj);
        void push_back(const T& obj);
        void push_back(T&& obj);
        void pop_front();
        void pop_back();
        T& front();
        T& back();
        void clear();

        const T& front() const { return m_Head->m_Data; }
        const T& back() const { return m_Tail->m_Data; }
        size_t size() const noexcept { return m_Size; }
        void print() const;

    private:
        Node<T>* m_Head;
        Node<T>* m_Tail;
        size_t m_Size;
    };

    template <typename T>
    template <typename K>
    ForwardList<T>::Node<K>::Node(const K& val)
        : m_Data{val}, m_Next{nullptr}
    { }

    template <typename T>
    template <typename K>
    ForwardList<T>::Node<K>::Node(K&& val)
        : m_Data{std::move(val)}, m_Next{nullptr}
    { }


    template <typename T>
    template <typename K>
    ForwardList<T>::Node<K>::Node(const Node& other)
        : m_Data{other.m_Data}, m_Next{nullptr}
    { }


    template <typename T>
    template <typename K>
    ForwardList<T>::Node<K>::Node(Node&& other) noexcept
        : m_Data{std::exchange(other.m_Val, nullptr)}, m_Next{std::exchange(other.m_Next, nullptr)}
    { }

    template <typename T>
    template <typename K>
    template <typename... Args>
    ForwardList<T>::Node<K>::Node(Args&&... args)
        : m_Data{std::forward<Args>(args)...}, m_Next{nullptr}
    { }

    template <typename T>
    ForwardList<T>::ForwardList()
        : m_Head{nullptr}, m_Tail{nullptr}, m_Size{}
    {}

    template <typename T>
    ForwardList<T>::ForwardList(size_t size)
        : m_Size{size}
    {
        m_Head = new Node<T>{};
        Node<T>* curr = m_Head;

        for (size_t i = 1; i < size; ++i)
        {
            Node<T>* next = new Node<T>{};
            curr->m_Next = next;
            curr = next;
        }
        m_Tail = curr;
    }

    template <typename T>
    ForwardList<T>::ForwardList(std::initializer_list<T> l)
        : m_Size{l.size()}
    {
        auto it = l.begin();
        m_Head = new Node<T>{*it};
        it++;
        Node<T>* curr = m_Head;

        while (it != l.end())
        {
            Node<T>* next = new Node<T>{*it};
            curr->m_Next = next;
            curr = next;
            ++it;
        }
        m_Tail = curr;
    }

    template <typename T>
    ForwardList<T>::ForwardList(const ForwardList& other)
        : m_Head{nullptr}, m_Tail{nullptr}, m_Size{other.m_Size}
    {
        if (other.m_Head)
        {
            m_Head = new Node<T>{other.m_Head->m_Data};
            Node<T>* curr1 = m_Head;
            Node<T>* curr2 = other.m_Head->m_Next;

            while(curr2)
            {
                Node<T>* next = new Node<T>{curr2->m_Data};
                curr1->m_Next = next;
                curr1 = next;
                curr2 = curr2->m_Next;
            }
            m_Tail = curr1;
        }
    }

    template <typename T>
    ForwardList<T>& ForwardList<T>::operator=(const ForwardList& other)
    {
        if (this != &other)
        {
            if (other.m_Head)
            {
                m_Head = new Node<T>{other.m_Head->m_Data};
                Node<T>* curr1 = m_Head;
                Node<T>* curr2 = other.m_Head->m_Next;

                while(curr2)
                {
                    Node<T>* next = new Node<T>{curr2->m_Data};
                    curr1->m_Next = next;
                    curr1 = next;
                    curr2 = curr2->m_Next;
                }
                m_Tail = curr1;
            }
        }
    }

    template <typename T>
    ForwardList<T>::ForwardList(ForwardList&& other) noexcept
        : m_Head{std::exchange(other.m_Head, nullptr)}, m_Tail{std::exchange(other.m_Tail, nullptr)}, 
        m_Size{std::exchange(other.m_Size, 0)}
    {}

    template <typename T>
    ForwardList<T>& ForwardList<T>::operator=(ForwardList&& other) noexcept
    {
        if (this != &other)
        {
            m_Head = std::exchange(other.m_Head, nullptr);
            m_Tail = std::exchange(other.m_Tail, nullptr);
            m_Size = std::exchange(other.m_Size, 0);
        }
    }

    template <typename T>
    ForwardList<T>::~ForwardList()
    {
        clear();
    } 

    template<typename T>
    void ForwardList<T>::print() const
    {
        if (m_Head)
        {
            Node<T>* curr = m_Head;

            while(curr)
            {
                std::cout << curr->m_Data << ',';
                curr = curr->m_Next;
            }
        }
        std::cout << std::endl;
    }

    template<typename T>
    void ForwardList<T>::clear()
    {
        Node<T>* curr = m_Head;
        while(curr)
        {
            Node<T>* next = curr->m_Next;
            delete curr;
            curr = next;
        }

        m_Head = nullptr;
        m_Tail = nullptr;
    }

}
