#include <iostream>
#include <exception>
#include <random>
#include <cstddef>

std::mt19937 engine()
{
    std::random_device dev;
    std::seed_seq seed{dev(), dev(), dev()};
    return std::mt19937 (seed);
}
class Box {
public:
    Box()
    {
        counter++;
        std::cout << "I am being constructed! My number is: " << getCount() << std::endl;
        name = __func__;
    }

    void sayHello()
    {
        std::cout << "My name is: "                           << name
        << " and my measurements are as follows length: "     << length
                                             << ", width: "   << width
                                             << ", height: "  << height << std::endl;
    }

    static int getCount()
    {
        return counter;
    }

    std::mt19937 eng = engine();

    // Seed random numbers in range 0 to 100;
    int seedFunc()
    {
        return std::uniform_int_distribution<>(0, 100)(eng);
    }

    struct Iterator
    {
        using iterator_category [[maybe_unused]] = std::forward_iterator_tag;
        using difference_type   [[maybe_unused]] = std::ptrdiff_t;
        using value_type        [[maybe_unused]] = int;
        using pointer           = int*;  // or also value_type*
        using reference         = int&;  // or also value_type&

        explicit Iterator(pointer ptr) : m_ptr(ptr) {}

        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }

        // Prefix increment
        Iterator& operator++() { m_ptr++; return *this; }
        // Postfix increment
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

        friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

    private:
        pointer m_ptr;
    };
    Iterator begin() { return Iterator(&m_data[0]); }
    Iterator end() { return Iterator(&m_data[20]); }

private:
    // initialising private members with random seeded numbers in range 0 - 100
    int length{seedFunc()}, width{seedFunc()}, height{seedFunc()};

    const char* name;
    static int counter;
    int m_data[20]{};
};

int Box::counter {0};

template <typename T>
class Unique_ptr_class
{
public:
    Unique_ptr_class() : ptr_mem(nullptr) // DEFAULT CONSTRUCTOR
    {
    }

    explicit Unique_ptr_class(T* ptr) : ptr_mem(ptr) // PARAMETRISED CONSTRUCTOR
    {
    }

    Unique_ptr_class(const Unique_ptr_class& obj) = delete; // DELETED COPY CONSTRUCTOR

    Unique_ptr_class& operator=(const Unique_ptr_class& obj) = delete; // DELETED COPY ASSIGNMENT OPERATOR


    Unique_ptr_class(Unique_ptr_class&& dying_obj)  noexcept // MOVE CONSTRUCTOR
    {
        // transfer ownership of memory pointed by dying_obj
        this->ptr_mem = dying_obj.ptr_mem;
        dying_obj.ptr_mem = nullptr;
    }

    Unique_ptr_class& operator= (Unique_ptr_class&& dying_obj)  noexcept // MOVE ASSIGNMENT OPERATOR
    {
        _performCleanup_(); // cleanup any existing data

        this->ptr_mem = dying_obj.ptr_mem; // transfer ownership of memory pointed
        dying_obj.ptr_mem = nullptr;
    }

    T* operator->() // OBTAINING POINTER USING ARROW OPERATOR
    {
        return this->ptr_mem;
    }

    T& operator*() // DEREFERENCING UNDERLYING POINTER
    {
        return *(this->ptr_mem);
    }

    ~Unique_ptr_class() // DESTRUCTOR
    {
        _performCleanup_(); // cleanup any existing data
    }

private:
    T* ptr_mem = nullptr;

    void _performCleanup_() // IMPLEMENTED CLEANER FUNCTION
    {
        delete ptr_mem;
    }
};

template <typename T>
class Unique_ptr_class<T[]>
{
public:
    Unique_ptr_class() : ptr_mem(nullptr) // DEFAULT CONSTRUCTOR
    {
    }

    explicit Unique_ptr_class(T* ptr) : ptr_mem(ptr) // PARAMETRISED CONSTRUCTOR
    {
    }

    Unique_ptr_class(const Unique_ptr_class& obj) = delete; // DELETED COPY CONSTRUCTOR

    Unique_ptr_class& operator= (const Unique_ptr_class& obj) = delete; // DELETED COPY ASSIGNMENT OPERATOR

    Unique_ptr_class (Unique_ptr_class&& dying_obj) noexcept // MOVE CONSTRUCTOR
    {
        _performCleanup_(); // cleanup any existing data

        // transfer ownership of the memory pointed by dying_obj to this object
        this->ptr_mem = dying_obj.ptr_mem;
        dying_obj.ptr_mem = nullptr;
    }

    Unique_ptr_class& operator=(Unique_ptr_class&& dying_obj) noexcept // MOVE ASSIGNMENT OPERATOR
    {
        _performCleanup_(); // cleanup any existing data

        this->ptr_mem = dying_obj.ptr_mem;
        dying_obj.ptr_mem = nullptr;
    }

    T* operator->() // OBTAINING POINTER USING ARROW OPERATOR
    {
        return this->ptr_mem;
    }

    T& operator*() // DEREFERENCING UNDERLYING POINTER
    {
        return *(this->ptr_mem);
    }

    T& operator[](int index)
    {
        if (index < 0)
        {
            throw std::invalid_argument("Negative index exception!");
        }
        return this->ptr_mem[index];
    }

    ~Unique_ptr_class() // DESTRUCTOR
    {
        _performCleanup_(); // cleanup any existing data
    }

private:
    T* ptr_mem = nullptr;

    void _performCleanup_() // IMPLEMENTED CLEANER FUNCTION
    {
        delete[] ptr_mem;
    }
};

int main() {
    // CREATE A UNIQUE_PTR_CLASS OBJECT HOLDING A BOX OBJECT
    // Unique_ptr_class<Box> box1(new Box); // creating an object works
    // Unique_ptr_class<Box> box2(std::move(box1)); // move constructor works, std::move casts box1 to r-value ref
    // Unique_ptr_class<Box> box3(box2); // call to deleted copy constructor!

    // CREATE A UNIQUE_PTR_CLASS OBJECT HOLDING AN ARRAY OF BOX OBJECTS
    // Unique_ptr_class<Box[]> boxArr1(new Box[5]);

    // index based assignment
//    Box b1 = boxArr1[0];
//    Box b2 = boxArr1[1];
//    Box b3 = boxArr1[2];
//    Box b4 = boxArr1[3];
//    Box b5 = boxArr1[4];
    // Box b0 = boxArr1[-1]; // Negative index exception is thrown!

    // Unique_ptr_class<Box[]> boxArr2(std::move(boxArr1));

    std::vector<Unique_ptr_class<Box>> vec;

    Box* b11 = new Box;
    Box* b12 = new Box;
    Box* b13 = new Box;
    Box* b14 = new Box;
    Box* b15 = new Box;

    vec.emplace_back(b11);
    vec.emplace_back(b12);
    vec.emplace_back(b13);
    vec.emplace_back(b14);
    vec.emplace_back(b15);


    for (auto& elem : vec)
    {
        elem->sayHello();
    }

    std::cout << Box::getCount() << std::endl;

    return 0;
}
