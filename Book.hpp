#ifndef _BOOK_HPP_
#define _BOOK_HPP_
#include <string>

class Book
{
    int         id;         // Must be Unique
    std::string Title;      // 
    std::string Author;
    std::string ISBN;
    int         YearPublished;
    bool        LoanedOut;
};

#endif