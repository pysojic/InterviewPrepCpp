#include <stdexcept>

template <typename T>
struct Node
{
    Node(const T& val)
        : m_Data{val}, m_Prev{nullptr}, m_Next{nullptr}
    {}
    Node(T&& val)
        : m_Data{std::move(val)}, m_Prev{nullptr}, m_Next{nullptr}
    {}
    Node(Node&& other) noexcept
    {
        m_Data = std::move(other.m_Data);
        m_Prev = std::exchange(other.m_Prev, nullptr);
        m_Next = std::exchange(other.m_Next, nullptr);
    }

    T m_Data{};
    Node* m_Prev = nullptr;
    Node* m_Next = nullptr;
};

template <typename T>
class List
{   
    List()
        : m_Head{nullptr}, m_Tail{nullptr}, m_Size{}
    {}
    List(size_t size) 
        : m_Size{size}
    {
        if (size > 1)
        {
            m_Head = new Node{};
            Node* curr = m_Head;
            for (size_t i = 1; i < m_Size; ++i)
            {
                Node* newNode = new Node{};
                curr->m_Next = newNode;
                newNode->m_Prev = curr;
                curr = newNode;
            }
            m_Tail = curr;
        }
        else if (size == 1)
        {
            m_Head = new Node{};
            m_Tail = m_Head;
        }
        else
        {
            m_Head = nullptr;
            m_Tail = nullptr;
        }
    }
    List(const List& other)
        : m_Size{other.m_Size}
    {
        // Check that the other list is not empty
        if (other.m_Head)
        {
            m_Head = new Node{other.m_Head->m_Data};
            Node* curr = m_Head;
            Node* otherCurr = other.m_Head->m_Next;
            while(otherCurr)
            {
                Node* next = new Node{otherCurr->m_Data};
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
                m_Head = new Node{other.m_Head->m_Data};
                Node* curr = m_Head;
                Node* otherCurr = other.m_Head->m_Next;
                while(otherCurr)
                {
                    Node* next = new Node{otherCurr->m_Data};
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
            Node* oldHead = m_Head;
            m_Head = new Node{elem};
            oldHead->m_Prev = m_Head;
            m_Head->m_Next = oldHead;
        }
        else
        {
            m_Head = new Node{elem};
            m_Tail = m_Head;
        }
        ++m_Size;
    }

    void push_front(T&& elem) 
    {
        if (m_Head)
        {
            Node* oldHead = m_Head;
            m_Head = new Node{std::move(elem)};
            oldHead->m_Prev = m_Head;
            m_Head->m_Next = oldHead;
        }
        else
        {
            m_Head = new Node{std::move(elem)};
            m_Tail = m_Head;
        }
        ++m_Size;
    }

    void push_back(const T& elem)
    {
        if (m_Tail)
        {
            Node* oldTail = m_Tail;
            m_Tail = new Node{elem};
            oldTail->m_Next = m_Tail;
            m_Tail->m_Prev = oldTail;
        }
        else
        {
            m_Head = new Node{elem};
            m_Tail = m_Head;
        }
        ++m_Size;
    }

    void push_back(T&& elem)
    {
        if (m_Tail)
        {
            Node* oldTail = m_Tail;
            m_Tail = new Node{std::move(elem)};
            oldTail->m_Next = m_Tail;
            m_Tail->m_Prev = oldTail;
        }
        else
        {
            m_Head = new Node{std::move(elem)};
            m_Tail = m_Head;
        }
        ++m_Size;
    }

    void pop_front()
    {
        // Check if the list has more than 1 element
        if (m_Head->m_Next)
        {
            Node* oldHead = m_Head;
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
            Node* oldTail = m_Tail;
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

    void clear()
    {
 
        Node* curr = m_Head;
        while(curr)
        {
            Node* next = curr->m_Next;
            delete curr;
            curr = next;
        }
        m_Head = m_Tail = nullptr;
        m_Size = 0;
    }
    
private:
    Node* m_Head = nullptr;
    Node* m_Tail = nullptr;
    size_t m_Size{};
};