#include <stdexcept>

template <typename T>
struct Node
{
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
    ~List();

    void push_front() 
    {
        m_Head->m_Prev = nullptr;

    }
    void push_back()
    {

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
    
private:
    Node* m_Head;
    Node* m_Tail;
    size_t m_Size;
};