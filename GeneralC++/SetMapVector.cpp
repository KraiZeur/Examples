#include <iostream>
#include <string>

//C++98

// Map
#include <map>

void map()
{

}

// Multimap

void multimap()
{

}

// Set
#include <set>

void set()
{
    std::set<std::string> my_set;
    my_set.insert("Hello");
    my_set.insert("Hello2");

    std::set<std::string>::const_iterator it = my_set.find("Hello");

    if (it != my_set.end())
    {
        std::cout << "set :: FOUND with find !!" << std::endl;
    }

    if (my_set.count("Hello"))
    {
        std::cout << "set :: FOUND with count" << std::endl;
    }
}

// Multiset

void multiset()
{
    std::multiset<std::string> my_set;
    my_set.insert("Hello");
    my_set.insert("Hello");
    my_set.insert("Hello2");

    std::set<std::string>::const_iterator it = my_set.find("Hello");

    if (it != my_set.end())
    {
        std::cout << "multiset :: FOUND !!" << std::endl;
    }

    if (my_set.count("Hello"))
    {
        std::cout << "set :: FOUND with count occurrences :: " << my_set.count("Hello") << std::endl;
    }
}

// Ordered_set


// C++11

// unordered_set
#include <unordered_set>

void unordered_set() // C++11
{
    std::unordered_set<std::string> my_set;
    my_set.insert("Hello");
    my_set.insert("Hello!");

    std::unordered_set<std::string>::const_iterator it=my_set.find("Hello");

    if (it!=my_set.end())
    {
        std::cout << "unordered_set :: FOUND !!" << std::endl;
    }
}

int main(int argc, char const *argv[])
{

    set();

    unordered_set();

    return 0;
}
