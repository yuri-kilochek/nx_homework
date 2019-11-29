#include <nx_homework/hasher_microservice/options.hpp>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/errors.hpp>

#include <cstdlib>
#include <iostream>

namespace nx_homework::hasher_microservice {
///////////////////////////////////////////////////////////////////////////////

options::options(int argc, char const* const* argv) {
    using namespace boost::program_options;

    bool show_help = false;
    bool show_version = false;

    boost::program_options::options_description desc("Options");
    desc.add_options()
        ("help", bool_switch(&show_help)
         , "Show this help message.")
        ("version", bool_switch(&show_version)
         , "Show version.")
        ("port,p", value<std::uint_least16_t>(&port)
            ->value_name("PORT")
            ->default_value(port)
         , "Port on which to listen.")
    ;

    variables_map vars;
    try {
        store(parse_command_line(argc, argv, desc), vars);
    } catch (boost::program_options::error const& exception) {
        std::cerr << exception.what() << "\n";
        std::exit(EXIT_FAILURE);
    }
    notify(vars);

    if (show_help) {
        std::cerr << "nx_homework_hasher_microservice v"
                  << NX_HOMEWORK_HASHER_MICROSERVICE_VERSION_STRING << "\n";
        std::cerr << "\n";
        std::cerr << desc;
        std::exit(0);
    }

    if (show_version) {
        std::cerr << NX_HOMEWORK_HASHER_MICROSERVICE_VERSION_STRING << "\n";
        std::exit(0);
    }
}

///////////////////////////////////////////////////////////////////////////////
}
