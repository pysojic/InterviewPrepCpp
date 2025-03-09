#pragma once

#include <stdexcept>
#include <initializer_list>


template <typename T>
class List
{   
    template <typename K>
    struct Node
    {
        explicit Node(const K& val) noexcept
            : m_Data{val}, m_Prev{nullptr}, m_Next{nullptr}
        {}
        explicit Node(K&& val) noexcept
            : m_Data{std::move(val)}, m_Prev{nullptr}, m_Next{nullptr}
        {}
        Node(const Node& other)
            : m_Data{other.m_Data}, m_Prev{nullptr}, m_Next{nullptr}
        {}
        template<typename... Args>
        Node(Args&&... args) noexcept // Contruct the data in-place by passing the object's contructor args
            : m_Data{std::forward<Args>(args)...}, m_Prev{nullptr}, m_Next{nullptr}
        {}
        Node(Node&& other) noexcept
        {
            m_Data = std::move(other.m_Data);
            m_Prev = std::exchange(other.m_Prev, nullptr);
            m_Next = std::exchange(other.m_Next, nullptr);
        }

        K m_Data{};
        Node* m_Prev = nullptr;
        Node* m_Next = nullptr;
    };

public:
    List()
        : m_Head{nullptr}, m_Tail{nullptr}, m_Size{}
    {}
    List(size_t size) 
        : m_Size{size}
    {
        if (size > 1)
        {
            m_Head = new Node<T>{};
            Node<T>* curr = m_Head;
            while(curr)
            {
                Node<T>* newNode = new Node<T>{};
                curr->m_Next = newNode;
                newNode->m_Prev = curr;
                curr = newNode;
            }
            m_Tail = curr;
        }
        else if (size == 1)
        {
            m_Head = new Node<T>{};
            m_Tail = m_Head;
        }
        else
        {
            m_Head = nullptr;
            m_Tail = nullptr;
        }
    }
    List(size_t size, const T& val)
        : m_Size{size}
    {
        if (size > 1)
        {
            Node<T>* m_Head = new T{val};
            Node<T>* curr = m_Head;

            while(curr)
            {
                Node<T>* next = new Node{val};
                curr->m_Next = next;
                next->m_Prev = curr;
                curr = next;
            }
            m_Tail = curr;
        }
        else if (size == 1)
        {
            Node<T>* m_Head = new Node{val};
            m_Tail = m_Head;
        }
        else
        {
            m_Head = nullptr;
            m_Tail = nullptr;
        }
    }
    List(std::initializer_list<T> list)
        : m_Size{list.size()}
    {
        if (m_Size)
        {
            auto it = list.begin();
            m_Head = new Node<T>{*it};
            Node<T>* curr = m_Head;
            ++it;
            while(it != list.end())
            {
                Node<T>* next = new Node<T>{*it};
                curr->m_Next = next;
                next->m_Prev = curr;
                curr = next;
                ++it;
            }
            m_Tail = curr;
        }
    }
    List(const List& other)
        : m_Size{other.m_Size}
    {
        // Check that the other list is not empty
        if (other.m_Head)
        {
            m_Head = new Node<T>{other.m_Head->m_Data};
            Node<T>* curr = m_Head;
            Node<T>* otherCurr = other.m_Head->m_Next;
            while(otherCurr)
            {
                Node<T>* next = new Node<T>{otherCurr->m_Data};
                curr->m_Next = next;
                next->m_Prev = curr;
                curr = next;
                otherCurr = otherCurr->m_Next;
            }
            curr->m_Next = nullptr;
            m_Tail = curr;
        }
    }
    List& operator=(const List& other)
    {
        if (this != &other)
        {
            if (m_Head)
            {
                clear();
            }
            // Check that the other list is not empty
            if (other.m_Head)
            {
                m_Head = new Node<T>{other.m_Head->m_Data};
                Node<T>* curr = m_Head;
                Node<T>* otherCurr = other.m_Head->m_Next;
                while(otherCurr)
                {
                    Node<T>* next = new Node<T>{otherCurr->m_Data};
                    curr->m_Next = next;
                    next->m_Prev = curr;
                    curr = next;
                    otherCurr = otherCurr->m_Next;
                }
                curr->m_Next = nullptr;
                m_Tail = curr;
            }
        }
        return *this;
    }
    List(List&& other) noexcept
        : m_Head{std::exchange(other.m_Head, nullptr)}, m_Tail{std::exchange(other.m_Tail, nullptr)}, 
        m_Size{std::exchange(other.m_Size, 0)}
    {
    }
    List& operator= (List&& other) noexcept
    {
        if (this != &other)
        {
            clear();
            m_Head = std::exchange(other.m_Head, nullptr);
            m_Tail = std::exchange(other.m_Tail, nullptr);
            m_Size = std::exchange(other.m_Size, 0);
        }
        return *this;
    }
    ~List() noexcept
    {
        clear();
    }

    void push_front(const T& elem) 
    {
        if (m_Head)
        {
            Node<T>* oldHead = m_Head;
            m_Head = new Node<T>{elem};
            oldHead->m_Prev = m_Head;
            m_Head->m_Next = oldHead;
        }
        else
        {
            m_Head = new Node<T>{elem};
            m_Tail = m_Head;
        }
        ++m_Size;
    }

    void push_front(T&& elem) 
    {
        if (m_Head)
        {
            Node<T>* oldHead = m_Head;
            m_Head = new Node<T>{std::move(elem)};
            oldHead->m_Prev = m_Head;
            m_Head->m_Next = oldHead;
        }
        else
        {
            m_Head = new Node<T>{std::move(elem)};
            m_Tail = m_Head;
        }
        ++m_Size;
    }

    template <typename... Args>
    void emplace_front(Args&&... args)
    {
        if (m_Head)
        {
            Node<T>* oldHead = m_Head;
            m_Head = new Node<T>{std::forward<Args>(args)...};
            oldHead->m_Prev = m_Head;
            m_Head->m_Next = oldHead;
        }
        else
        {
            m_Head = new Node<T>{std::forward<Args>(args)...};
            m_Tail = m_Head;
        }
        ++m_Size;
    }

    void push_back(const T& elem)
    {
        if (m_Tail)
        {
            Node<T>* oldTail = m_Tail;
            m_Tail = new Node<T>{elem};
            oldTail->m_Next = m_Tail;
            m_Tail->m_Prev = oldTail;
        }
        else
        {
            m_Head = new Node<T>{elem};
            m_Tail = m_Head;
        }
        ++m_Size;
    }

    void push_back(T&& elem)
    {
        if (m_Tail)
        {
            Node<T>* oldTail = m_Tail;
            m_Tail = new Node<T>{std::move(elem)};
            oldTail->m_Next = m_Tail;
            m_Tail->m_Prev = oldTail;
        }
        else
        {
            m_Head = new Node<T>{std::move(elem)};
            m_Tail = m_Head;
        }
        ++m_Size;
    }

    template <typename... Args>
    void emplace_back(Args&&... args)
    {
        if (m_Head)
        {
            Node<T>* oldTail = m_Tail;
            m_Tail = new Node<T>{std::forward<Args>(args)...};
            oldTail->m_Next = m_Tail;
            m_Tail->m_Prev = oldTail;
        }
        else
        {
            m_Tail = new Node<T>{std::forward<Args>(args)...};
            m_Head = m_Tail;
        }
        ++m_Size;
    }

    void pop_front()
    {
        // Check if the list has more than 1 element
        if (m_Head->m_Next)
        {
            Node<T>* oldHead = m_Head;
            m_Head = oldHead->m_Next;
            m_Head->m_Prev = nullptr;
            delete oldHead;
            --m_Size;
            return;
        }
        // If not, delete the head and set both the tail and head to nullptr
        delete m_Head;
        m_Head = nullptr;
        m_Tail = nullptr;
        --m_Size;
        // Note: handling calling pop_back/pop_front on an empty list is left to the user, this is UB in the current impl
    }

    void pop_back()
    {
        if (m_Tail->m_Prev)
        {
            Node<T>* oldTail = m_Tail;
            m_Tail = oldTail->m_Prev;
            m_Tail->m_Next = nullptr;
            delete oldTail;
            --m_Size;
            return;
        }
        delete m_Tail;
        m_Head = nullptr;
        m_Tail = nullptr;
        --m_Size;
    }

    void reverse_list()
    {
        m_Tail = m_Head;
        Node<T>* current = m_Head;
        Node<T>* temp = nullptr;
        while (current)
        {
            // Swap pointers
            std::swap(current->m_Prev, current->m_Next);
            temp = current;
            // Move to next node<T> (which is the old previous)
            current = current->m_Prev;
        }
        // After the loop, temp points to the new head
        m_Head = temp;
    }

    void merge(List& other)
    {
        if (this != &other)
        {
            Node<T>* dummy = new Node<T>{};
            Node<T>* node = dummy;
            Node<T>* head1 = m_Head;
            Node<T>* head2 = other.m_Head;
            
            while(head1 && head2)
            {
                if (head1->m_Data < head2->m_Data)
                {
                    node->m_Next = head1;
                    head1->m_Prev = node;
                    head1 = head1->m_Next;
                }
                else 
                {
                    node->m_Next = head2;
                    head2->m_Prev = node;
                    head2 = head2->m_Next;
                }
                node = node->m_Next;
            }

            if (head1)
            {
                node->m_Next = head1;
                head1->m_Prev = node;
            }
            else
            {
                node->m_Next = head2;
                head2->m_Prev = node;
            }

            // Set the new head and tail
            m_Head = dummy->m_Next;
            if(m_Head)  // if merged list is non-empty
                m_Head->m_Prev = nullptr;
            
            // Find tail (or keep node as tail if list is non-empty)
            while (node->m_Next)
            {
                node = node->m_Next;
            }
            m_Tail = node;
            
            delete dummy;
            other.reset();
        }
    }

    void clear()
    {
 
        Node<T>* curr = m_Head;
        while(curr)
        {
            Node<T>* next = curr->m_Next;
            delete curr;
            curr = next;
        }
        m_Head = m_Tail = nullptr;
        m_Size = 0;
    }

    T& front() noexcept
    {
        return m_Head->m_Data;
    }
    T& back() noexcept
    {
        return m_Tail->m_Data;
    }

    const T& front() const noexcept
    {
        return m_Head->m_Data;
    }
    const T& back() const noexcept
    {
        return m_Tail->m_Data;
    }
    bool empty() const noexcept
    {
        return m_Head == nullptr ? true : false;
    }
    size_t size() const noexcept
    {
        return m_Size;
    }
    void print() const
    {
        Node<T>* curr = m_Head;
        while(curr)
        {
            std::cout << curr->m_Data << ',';
            curr = curr->m_Next;
        }
        std::cout << std::endl;
    }
private:
    void reset()
    {
        m_Head = nullptr;
        m_Tail = nullptr;
        m_Size = 0;
    }
    
private:
    Node<T>* m_Head = nullptr;
    Node<T>* m_Tail = nullptr;
    size_t m_Size{};
};