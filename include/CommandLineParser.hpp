#pragma once

#include <boost/optional.hpp>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

///
/// \brief Parse the program command lines into a variables_map
///
inline auto parseCommandLine(const int argc, char** const argv) -> boost::optional<po::variables_map> {

    // declare supported options
    po::options_description desc("Simulate the detection of ultra-high energy neutrinos by ANITA");
    desc.add_options()
        ("help", "Print help messages")

        ("no-vis",
         po::bool_switch()->default_value(false),
         "Disable all visualization - this will improve execution speed.")

        ("macro",
         po::value<std::string>()->default_value(""),
         "The macro to load and run.")

        ("num-threads",
         po::value<int>()->default_value(4),
         "The number of threads to use.");

    // create variable map
    po::variables_map args;
    try {

        // store command line options into variable map
        po::store(po::parse_command_line(argc, argv, desc), args);

        // print the help description if the user didn't provide any arguments
        if (args.count("help")) {
            std::cout << desc << std::endl;
            return boost::none;
        }

        // throw exceptions if there are any problems (i.e. we didn't get required values)
        po::notify(args);
    }
    // catch required option exception
    catch(po::required_option& e) {
        std::cout << e.what() << std::endl;
        return boost::none;
    }
    // catch unknown option exception
    catch(po::unknown_option& e) {
        std::cerr << e.what() << std::endl;
        return boost::none;
    }

    return args;

} // END: parseCommandLine
