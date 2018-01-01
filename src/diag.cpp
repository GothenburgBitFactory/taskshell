////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2006 - 2018, Paul Beckingham, Federico Hernandez.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// http://www.opensource.org/licenses/mit-license.php
//
////////////////////////////////////////////////////////////////////////////////

#include <cmake.h>
#include <commit.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <stdlib.h>
#include <FS.h>
#include <Color.h>
#include <shared.h>
#include <format.h>

#ifdef HAVE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

////////////////////////////////////////////////////////////////////////////////
int cmdDiagnostics ()
{
  Color bold ("bold");

  std::cout << "\n"
            << bold.colorize (PACKAGE_STRING)
            << "\n"
            << "   " << "Platform: " << osName ()
            << "\n\n";

  // Compiler.
  std::cout << bold.colorize ("Compiler")
            << "\n"
#ifdef __VERSION__
            << "    " << "Version: "
            << __VERSION__ << "\n"
#endif
            << "       " << "Caps:"
#ifdef __STDC__
            << " +stdc"
#endif
#ifdef __STDC_HOSTED__
            << " +stdc_hosted"
#endif
#ifdef __STDC_VERSION__
            << " +" << __STDC_VERSION__
#endif
#ifdef _POSIX_VERSION
            << " +" << _POSIX_VERSION
#endif
#ifdef _POSIX2_C_VERSION
            << " +" << _POSIX2_C_VERSION
#endif
#ifdef _ILP32
            << " +ILP32"
#endif
#ifdef _LP64
            << " +LP64"
#endif
            << " +c"      << 8 * sizeof (char)
            << " +i"      << 8 * sizeof (int)
            << " +l"      << 8 * sizeof (long)
            << " +vp"     << 8 * sizeof (void*)
            << " +time_t" << 8 * sizeof (time_t)
            << "\n";

  // Compiler compliance level.
  std::cout << " Compliance: "
            << cppCompliance ()
            << "\n\n";

  std::cout << bold.colorize ("Build Features")
            << "\n"

  // Build date.
            << "      " << "Built: " << __DATE__ << " " << __TIME__ << "\n"
#ifdef HAVE_COMMIT
            << "     " << "Commit: " << COMMIT << "\n"
#endif
            << "      CMake: " << CMAKE_VERSION << "\n";

  std::cout << "libreadline: "
#ifdef HAVE_READLINE
#ifdef RL_VERSION_MAJOR
            << RL_VERSION_MAJOR << "." << RL_VERSION_MINOR
#elif defined RL_READLINE_VERSION
            << "0x" << std::hex << RL_READLINE_VERSION
#endif
#else
            << "n/a"
#endif
            << "\n";

  std::cout << " Build type: "
#ifdef CMAKE_BUILD_TYPE
            << CMAKE_BUILD_TYPE
#else
            << "-"
#endif
            << "\n\n";

  std::cout << bold.colorize ("Configuration")
            << "\n";

  auto env = getenv ("TASKRC");
  std::cout << "     TASKRC: "
            << (env ? env : "")
            << "\n";

  env = getenv ("TASKDATA");
  std::cout << "   TASKDATA: "
            << (env ? env : "")
            << "\n";

  // Taskwarrior version + location
  std::string path (getenv ("PATH"));
  std::cout << "       PATH: " << path << "\n";

  for (const auto& i : split (path, ':'))
  {
    File task (i + "/task");
    if (task.exists ())
    {
      std::string input;
      std::string output;
      execute ("task", {"--version"}, input, output);

      std::cout << "Taskwarrior: "
                << i
                << "/task "
                << output; // Still has \n
    }
  }

  std::cout << "\n";
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
