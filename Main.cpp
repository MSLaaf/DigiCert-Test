#include "docopt/docopt.h"
// https://github.com/docopt/docopt.cpp/tree/master
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <map>
#include "Book.hpp"
#include <sys/stat.h>
#include <unistd.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>


static const char USAGE[] =
R"(C.R.U.D. Library Book Tool

   Usage:
     CRUD_Book list
     CRUD_Book create --title=<title> [--author=<author>] [--isbn=<isbn>] [--year=<year>] [--loaned] [--returned]
     CRUD_Book read [--id=<id>] [--title=<title>] [--author=<author>] [--isbn=<isbn>] [--year=<year>]
     CRUD_Book update --id=<id> [--title=<title>] [--author=<author>] [--isbn=<isbn>] [--year=<year>]
     CRUD_Book delete (--id=<id>|--title=<title>|--isbn=<isbn>|--all)
     CRUD_Book ( -h | --help )
     CRUD_Book ( -v | --version )

   Options:
     -h --help
     -v --version
     --id=<id>          RecordID - selector
     --title=<title>    Quote to make sure spaces are included
     --author=<author>  Quote Author Name
     --isbn=<isbn>      ISBN
     --year=<year>      year of release
     --loaned           set loaned flag
     --returned         clear loaned flag
)";

bool debugFlag = false;

std::string BookDbFile = "Book.db";
           std::string SqlCreate = "CREATE TABLE IF NOT EXISTS LibraryBooks ("
                                      "title TEXT NOT NULL UNIQUE,"
                                      "author TEXT,"
                                      "isbn TEXT,"
                                      "year INT DEFAULT 1980,"
                                      "loaned INT DEFAULT FALSE )";


// return true if the file specified
// by the filename exists
bool file_exists(const char *filename)
{
    struct stat buffer;
    return stat(filename, &buffer) == 0 ? true : false;
}

int ListBooks(void)
{
    std::cout << "List" << std::endl;
    try
    {
        SQLite::Database BookDb(BookDbFile,SQLite::OPEN_READONLY);  // Open database
        SQLite::Statement query(BookDb, "SELECT rowid,* from LibraryBooks");

        while (query.executeStep())
        {
            int id             = query.getColumn(0);
            const char *title  = query.getColumn(1);
            const char *author = query.getColumn(2);
            const char *isbn   = query.getColumn(3);
            int year           = query.getColumn(4);
            int loanedOut      = query.getColumn(5);
            std::cout << "Record:   " << id << std::endl
                      << "  Title:  " << title << std::endl
                      << "  Author: " << author << std::endl
                      << "  isbn:   " << isbn << std::endl
                      << "  loaned: " << (loanedOut?"Yes":"No") << std::endl << std::endl;
        }
    }
    catch (std::exception& e)
    {
        std::cout << "SQLite exception: " << e.what() << std::endl;
        return EXIT_FAILURE; // unexpected error : exit the example program
    }
    return 0;
}

// CreateBook creates a book entry.The database does _NOT_ have to exist, and it will be created
// also the table will be added
int CreateBook(std::map < std::string, docopt::value > args)
{
    int MaxId = 0;
    try
    {
        bool didFileExist = file_exists(BookDbFile.c_str());
        std::stringstream fields;
        std::stringstream values;
        std::stringstream query;
        SQLite::Database BookDb(BookDbFile,
                                (didFileExist ? 0: SQLite::OPEN_CREATE)|SQLite::OPEN_READWRITE);
        if (!didFileExist) BookDb.exec(SqlCreate);
        fields << "(title";  // Title is ALWAYS there
        values << "(" << args["--title"];
        if (args["--author"]) {
            fields << ",author";
            values << "," << args["--author"];
        }
        if (args["--isbn"]) {
            fields << ",isbn";
            values << "," << args["--isbn"];
        }
        if (args["--year"]) {
            fields << ",year";
            values << "," << args["--year"].asLong();
        }
        if (args["--loaned"].asBool()) {
            fields << ",loaned";
            values << ",1";
        }
        else if (args["--returned"].asBool()) {
            fields << ",loaned";
            values << ",0";
        }
        fields << ")";
        values << ")";
        query << "INSERT INTO LibraryBooks " << fields.str() << " VALUES " << values.str();
        if (debugFlag) std::cout << query.str() << std::endl;

        BookDb.exec(query.str());
    }
    catch (std::exception& e)
    {
        std::cout << "CREATE SQLite exception: " << e.what() << std::endl;
        return EXIT_FAILURE; // unexpected error : exit the example program
    }
    return 0;
}

// The parameters to readbook are where clauses
int ReadBook(std::map < std::string, docopt::value > args)
{
    try
    {
        SQLite::Database BookDb(BookDbFile,SQLite::OPEN_READONLY);  // Open database
        std::stringstream baseQuery;
        std::stringstream whereClause;
        baseQuery << "SELECT rowid,* from LibraryBooks";
        // In database id is the automatic rowid
        if (args["--id"]) {
            whereClause << " WHERE rowid=" << args["--id"].asLong();
        }
        if (args["--title"]) {
            if (whereClause.tellp() == std::streampos(0))
                whereClause << " WHERE ";
            else
                whereClause << " AND ";
            whereClause << "title=" << args["--title"];
        }
        if (args["--author"]) {
            if (whereClause.tellp() == std::streampos(0))
                whereClause << " WHERE ";
            else
                whereClause << " AND ";
            whereClause << "author=" << args["--author"];
        }
        if (args["--isbn"]) {
            if (whereClause.tellp() == std::streampos(0))
                whereClause << " WHERE ";
            else
                whereClause << " AND ";
            whereClause << "isbn=" << args["--isbn"];
        }
        if (args["--year"]) {
            if (whereClause.tellp() == std::streampos(0))
                whereClause << " WHERE ";
            else
                whereClause << " AND ";
            whereClause << "year=" << args["--year"];
        }

        std::cout << whereClause.str() << std::endl;
        if (whereClause.tellp() != std::streampos(0))
        {
            baseQuery << whereClause.str();
        }
        SQLite::Statement query(BookDb,baseQuery.str());
        while (query.executeStep())
        {
            int id             = query.getColumn(0);
            const char *title  = query.getColumn(1);
            const char *author = query.getColumn(2);

            const char *isbn   = query.getColumn(3);
            int year           = query.getColumn(4);
            int loanedOut      = query.getColumn(5);
            std::cout << "Record:   " << id << std::endl
                      << "  Title:  " << title << std::endl
                      << "  Author: " << author << std::endl
                      << "  isbn:   " << isbn << std::endl
                      << "  loaned: " << (loanedOut?"Yes":"No") << std::endl << std::endl;
        }
    }
    catch (std::exception& e)
    {
        std::cout << "READ SQLite exception: " << e.what() << std::endl;
        return EXIT_FAILURE; // unexpected error : exit the example program
    }
    return 0;
}


int UpdateBook(std::map < std::string, docopt::value > args)
{
    std::cout << "Update" << std::endl;
    try
    {
        SQLite::Database BookDb(BookDbFile,SQLite::OPEN_READONLY);  // Open database
        std::stringstream baseQuery;
        std::stringstream whereClause;
        std::stringstream fields;
        std::stringstream values;
        baseQuery << "SELECT rowid,* from LibraryBooks";
        // In database id is the automatic rowid
        // rowid / id is UNMUTABLE and it is the key to
        // locate the record.
        if (args["--id"]) {
            whereClause << " WHERE rowid=" << args["--id"].asLong();
        }
        if (args["--title"]) {
            fields << "(title";
            values << args["--title"];
        }
        if (args["--author"]) {
            if (fields.tellp() == std::streampos(0))
            {
                fields << "(";
                values << "(";
            }
            else
            {
                fields << ",";
                values << ",";
            }
            fields << "author";
            values << args["--author"];
        }
        if (args["--isbn"]) {
            if (fields.tellp() == std::streampos(0))
            {
                fields << "(";
                values << "(";
            }
            else
            {
                fields << ",";
                values << ",";
            }
            fields << "isbn";
            values << args["--isbn"];
        }
        if (args["--year"]) {
            if (fields.tellp() == std::streampos(0))
            {
                fields << "(";
                values << "(";
            }
            else
            {
                fields << ",";
                values << ",";
            }
            fields << "year";
            values << args["--year"];
        }
        if (args["--loaned"]) {
            if (fields.tellp() == std::streampos(0))
            {
                fields << "(";
                values << "(";
            }
            else
            {
                fields << ",";
                values << ",";
            }
            fields << "loaned";
            values << 1;
        }
        else if (args["--returned"]) {
            if (fields.tellp() == std::streampos(0))
            {
                fields << "(";
                values << "(";
            }
            else
            {
                fields << ",";
                values << ",";
            }
            fields << "loaned";
            values << 0;
        }
        if (fields.tellp() == std::streampos(0))
        {
            // we got here - NO updates
            std::cout << "No fields to update" << std::endl;
            return 0;
        }
        else
        {
            // close field and value clauses
            fields << ")";
            values << ")";
        }
        std::stringstream updateQuery;
        updateQuery << "UPDATE LibraryBooks " << std::endl
                    << "SET " << fields.str() << " = " << values.str()
                    << whereClause.str();

        std::cout << updateQuery.str() << std::endl;
        SQLite::Statement query(BookDb,updateQuery.str());
    }
    catch (std::exception& e)
    {
        std::cout << "UPDATE SQLite exception: " << e.what() << std::endl;
        return EXIT_FAILURE; // unexpected error : exit the example program
    }
    return 0;
}

int DeleteBook(std::map < std::string, docopt::value > args)
{
    int rc = -1;
    std::cout << "Delete" << std::endl;
    // --all zaps the database - deletes it
    if (args["--all"].asBool()) {
        std::cout << "Deleting database file" << std::endl;
    } else {

    }
    return rc;
}


int main(int argc,
         const char ** argv) {
    std::map < std::string, docopt::value > args = docopt::docopt(USAGE, {
        argv + 1,
        argv + argc
    },
    true, // show help if requested
    "CRUD_Book 1.0"); // version stri
    debugFlag = std::getenv("DEBUG_CRUD") != NULL;

    if (debugFlag) {
        std::cout << "Argument List:" << std::endl;
        for (auto
                const & arg: args) {
            std::cout << arg.first << "=" << arg.second << std::endl;
        }
    }

    if (args["list"].asBool()) {
        std::cout << "List All records" << std::endl;
        return ListBooks();   // Basically a wrapper for ReadBook(all)
    } else if (args["create"].asBool()) {
        return CreateBook(args);
    } else if (args["read"].asBool()) {
        return ReadBook(args);
    } else if (args["update"].asBool()) {
        return UpdateBook(args);
    } else if (args["delete"].asBool()) {
        DeleteBook(args);
    } else {
        std::cout << "Invalid command" << std::endl;
        return 1;
    }
    return 0;
}