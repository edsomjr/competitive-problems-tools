#include <iostream>

#include <getopt.h>
#include <unistd.h>

#include "commands/init.h"
#include "defs.h"
#include "dirs.h"
#include "error.h"
#include "message.h"
#include "sh.h"

// Raw strings
static const string help_message{
    R"message(
Initialize an empty directory with template files. If a file already exists, it is not overridden.

    Option          Description

    -h              Generates this help message.
    --help

    -o              Directory to be initialized
    --output

)message"};

namespace cptools::commands::init {

// Global variables
static struct option longopts[] = {{"help", no_argument, NULL, 'h'},
                                   {"output", required_argument, NULL, 'o'},
                                   {0, 0, 0, 0}};

// Auxiliary routines
string usage() { return "Usage: " NAME " init [-h] [-o output_dir]"; }

string help() { return usage() + help_message; }

int copy_template_files(const string &dest, ostream &out, ostream &err) {
  out << message::info("Initializing directory '" + dest + "' ...") << "\n";

  // Cria o diretório, se necessário
  auto res = cptools::sh::make_dir(dest);

  if (res.rc != CP_TOOLS_OK) {
    err << message::failure("Can't create directory '" + dest + "'!") << "\n";
    err << message::trace(res.output) << '\n';
    return res.rc;
  }

  // Copia os templates para o diretório indicado
  res = cptools::sh::copy_dir(dest, CP_TOOLS_TEMPLATES_DIR);

  if (res.rc == CP_TOOLS_OK)
    out << message::success() << "\n";
  else {
    err << message::failure("Directory '" + dest +
                            "' could not be initialized!")
        << "\n";
    err << message::trace(res.output) << '\n';
  }

  return res.rc;
}

// API functions
int run(int argc, char *const argv[], ostream &out, ostream &err) {
  int option = -1;
  string dest{"."};

  while ((option = getopt_long(argc, argv, "ho:", longopts, NULL)) != -1) {
    switch (option) {
    case 'h':
      out << help() << '\n';
      return 0;

    case 'o':
      dest = string(optarg);
      break;

    default:
      err << help() << '\n';
      return CP_TOOLS_ERROR_INIT_INVALID_OPTION;
    }
  }

  return copy_template_files(dest, out, err);
}
} // namespace cptools::commands::init
