#include "docopt/docopt.h"

#include <iostream>
#include <map>
#include <Book.hpp>

static const char USAGE[] =
R"(C.R.U.D. Library Book Tool

   Usage:
     CRUD_Book list  same as 'CRUD_Book read' without parameters
     CRUD_Book create title=<title> author=<author> [isbn=<ISBN>] [year=<year>] [loaned=Yes|No]
     CRUD_Book read [id=<id>] [title=<title>] [author=<author>] [isbn=<ISBN>] [year=<year>] [loaned=Yes|No]
     CRUD_Book update id=<id> [title=<title>] [author=<author>] [isbn=<ISBN>] [year=<year>] [loaned=yes|no]
     CRUD_Book delete (id=<id>|title=<title|isbn=<ISBN>|all)
     CRUD_Book ( -h | --help )
     CRUD_Book --version
     NOTE: You need to quote strings with spaces

   Options:
     -h --help
     --version
     --id <id>
     --title <title>  Use \" \' to make sure spaces are included
     --author <author"  
)";

int main(int argc, const char** argv)
{
    std::map<std::string, docopt::value> args
        = docopt::docopt(USAGE,
                         { argv + 1, argv + argc },
                         true,               // show help if requested
                         "Naval Fate 2.0");  // version string

    for(auto const& arg : args) {
        std::cout << arg.first <<  arg.second << std::endl;
    }

    return 0;
}