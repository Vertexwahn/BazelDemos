// This file has been copied from
// https://github.com/nelhage/rules_boost/blob/master/test/program_options_test.cc
// It is available under the Apache-2.0 license
// and Copyright belongs to the Contributors of rules_boost

#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main()
{
  int ac = 3;
  const char *const av[] = {"", "--compression", "7"};

  po::options_description desc("Allowed options");
  desc.add_options()
      ("compression", po::value<int>(), "set compression level")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(ac, av, desc), vm);
  po::notify(vm);

  if (!vm.count("compression") || vm["compression"].as<int>() != 7) {
      return 1;
  }

  return 0;
}
