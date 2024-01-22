#ifndef _BOOK_HPP_
#define _BOOK_HPP_
#include <string>
#include <iostream>

struct Book
{
    public:
        int         id;         // Must be Unique
        std::string Title;      // 
        std::string Author;
        std::string Isbn;
        int         YearPublished;
        bool        LoanedOut;
};

std::ostream &operator<<(std::ostream &os, Book  const &b) { 
   return os << "ID: " << b.id << std::endl  
             << "Title: " << b.Title << std::endl 
             << "Author: " << b.Author << std::endl
             << "ISBN: " << b.Isbn << "   Year: " << b.YearPublished << std::endl
             << "On Loan: " << (b.LoanedOut ? "Yes" : "No") << std::endl;
};
#endif