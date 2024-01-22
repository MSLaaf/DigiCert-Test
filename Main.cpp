#include "docopt/docopt.h"
 // https://github.com/docopt/docopt.cpp/tree/master
#include <iostream>

#include <cstdlib>

#include <map>

#include "Book.hpp"

static const char USAGE[] =
R"(C.R.U.D. Library Book Tool

   Usage:
     CRUD_Book list
     CRUD_Book create --title=<title> --author=<author> [--isbn=<isbn>] [--year=<year>] [--loaned]
     CRUD_Book read [--id=<id>] [--title=<title>] [--author=<author>] [--isbn=<isbn>] [--year=<year] [--loaned]
     CRUD_Book update [--id=<id>] [--ti--tle=<title>] [--author=<author>] [--isbn=<isbn>] [--year=<year>] [(--loaned|--returned)]
     CRUD_Book delete (--id=<id>|--title=<title>|--isbn=<isbn>|--all)
     CRUD_Book ( -h | --help )
     CRUD_Book ( -v | --version )

   Options:
     -h --help
     -v --version
     --id=<id>          RecordID
     --title=<title>    Quote to make sure spaces are included
     --author=<author>  Quote Author Name
     --isbn=<isbn>      ISBN
     --year=<year>      year of release
     --loaned           
     --returned
     --debug            debug output
)";

bool debugFlag = false;

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
         std::cout << "\"" << arg.first << "\"=\"" << arg.second << "\"" << std::endl;
      }
   }

   if (args["list"]) {
      std::cout << "List All records" << std::endl;
   } else if (args["create"]) {
      std::cout << "Create" << std::endl;
   } else if (args["delete"]) {
      std::cout << "Delete" << std::endl;
   } else if (args["read"]) {
      std::cout << "Read" << std::endl;
   } else if (args["update"]) {
      std::cout << "Update" << std::endl;
   } else {
      std::cout << "Invalid command" << std::endl;
      return 1;
   }
   return 0;
}