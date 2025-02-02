// UniquePtr
#include <utility>
#include <string>
#include <iostream>

using namespace std;
template <typename T> 
class UniquePtr{
private:
    T * data;
public:
    UniquePtr() :data{nullptr} {}
    UniquePtr(T* ptr) : data{ptr} {}
    ~UniquePtr() {
        delete data; // free the pointer
        // Other case: if T is array typed, not implementing for now
    };

    // lvalue: on the left of expression, has addr in mem &
    // rvalue: a temporary, no address in memory
    // Move constructor
    UniquePtr(UniquePtr&& from) : data{from.data}{
        // data = std::move(from.data);
        // We dont need the move
        from.data = nullptr;
    };

    // Move assignment
    UniquePtr& operator = (UniquePtr&& from) {
        if (&from != this) {
            data = from.data;
            from.data = nullptr;
        }
        return *this; // remember to dereference
    }
    
    // Delete the copy constructor and copy assignment
    // Unique pointers take ownership of heap alloc object
    // Construct, delete object. Cannot have 2 copies of unique ptr
    UniquePtr(const UniquePtr& other) = delete;
    UniquePtr & operator = (const UniquePtr & other) = delete;

    // Operations
    T* release() {
        // releases control over pointer
        T * temp = this.data;
        this->data= nullptr;
        return temp;
    }; 
    void reset(T* replacement){
        // delete one currently used by shared ptr, takes new one
        if (replacement != this->data){
            delete data; // delete the object managed
            this->data = replacement;
        }
    }; 

    void swap(UniquePtr& other) {
        // std::swap(this->data, &other.data);
        // std::exchange(this->data, other.data); works too
        T * other_temp = other.data;
        other.data = this->data;
        this->data = other_temp;
    };

    T* get() const {
        // const T* would make it a const object
        return this->data;
    }; 

    T& operator* () {
        // return a T reference
        return *(this->data);
    }; 


};

// Make Unique
template <typename T, typename ... Args>
UniquePtr<T> make_unique(Args&&... args) {
    // ... is the parameter pack syntax
    // in std::forward, the ... after means to unpack the params
    return UniquePtr<T>(new T{std::forward<Args>(args)...});
};
