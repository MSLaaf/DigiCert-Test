#include "docopt/docopt.h"
 // https://github.com/docopt/docopt.cpp/tree/master
#include <iostream>
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
     CRUD_Book create --title=<title> --author=<author> [--isbn=<isbn>] [--year=<year>] [--loaned]
     CRUD_Book read --id=<id> [--title=<title>] [--author=<author>] [--isbn=<isbn>] [--year=<year] [--loaned]
     CRUD_Book update --id=<id> [--title=<title>] [--author=<author>] [--isbn=<isbn>] [--year=<year>] [(--loaned|--returned)]
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


// return true if the file specified
// by the filename exists
bool file_exists(const char *filename)
{
    struct stat buffer;
    return stat(filename, &buffer) == 0 ? true : false;
}

int CreateBook(SQLite::Database &BookDb, std::map < std::string, docopt::value > args)
{
     std::cout << "Create" << std::endl;
     return 1;
}


int ReadBook(SQLite::Database &BookDb, std::map < std::string, docopt::value > args)
{
     std::cout << "Read" << std::endl;
     return 1;
}

int UpdateBook(SQLite::Database &BookDb, std::map < std::string, docopt::value > args)
{
     std::cout << "Update" << std::endl;
     return 1;
}

int DeleteBook(SQLite::Database &BookDb, std::map < std::string, docopt::value > args)
{
    int rc = -1;
    std::cout << "Delete" << std::endl;
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
         bool didFileExist = file_exists(BookDbFile.c_str());
           SQLite::Database BookDb(BookDbFile,
                                   (didFileExist ? 0: SQLite::OPEN_CREATE)|SQLite::OPEN_READWRITE);
           std::string SqlCreate = "CREATE TABLE IF NOT EXISTS LibraryBooks "
                                       "(id INTEGER PRIMARY KEY,"
                                      "title TEXT NOT NULL UNIQUE,"
                                       "author TEXT,"
                                       "loaned INT DEFAULT FALSE )";
          if (!didFileExist) BookDb.exec(SqlCreate);
   if (args["list"].asBool()) {
      std::cout << "List All records" << std::endl;
      ReadBook(BookDb, args);   // Basically a wrapper for ReadBook(all)
   } else if (args["create"].asBool()) {
      CreateBook(BookDb, args);
   } else if (args["read"].asBool()) {
      ReadBook(BookDb, args);
   } else if (args["update"].asBool()) {
      UpdateBook(BookDb, args);
   } else if (args["delete"].asBool()) {
      DeleteBook(BookDb, args);
   } else {
      std::cout << "Invalid command" << std::endl;
      return 1;
   }
   return 0;
}