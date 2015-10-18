////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2006 - 2015, Paul Beckingham, Federico Hernandez.
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
#include <i18n.h>
#include <text.h>
#include <util.h>

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
            << "\n";

  std::cout << "   " << STRING_CMD_DIAG_PLATFORM << ": "
            <<
#if defined (DARWIN)
               "Darwin"
#elif defined (SOLARIS)
               "Solaris"
#elif defined (CYGWIN)
               "Cygwin"
#elif defined (HAIKU)
               "Haiku"
#elif defined (OPENBSD)
               "OpenBSD"
#elif defined (FREEBSD)
               "FreeBSD"
#elif defined (NETBSD)
               "NetBSD"
#elif defined (LINUX)
               "Linux"
#elif defined (KFREEBSD)
TRING_CMD_DIAG_COMPILER
#elif defined (GNUHURD)
               "GNU/Hurd"
#else
               STRING_CMD_DIAG_UNKNOWN
#endif
            << "\n\n";

  // Compiler.
  std::cout << bold.colorize (STRING_CMD_DIAG_COMPILER)
            << "\n"
#ifdef __VERSION__
            << "    " << STRING_CMD_DIAG_VERSION << ": "
            << __VERSION__ << "\n"
#endif
            << "       " << STRING_CMD_DIAG_CAPS << ":"
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
  std::string compliance = "non-compliant";
#ifdef __cplusplus
  int level = __cplusplus;
  if (level == 199711)
    compliance = "C++98/03";
  else if (level == 201103)
    compliance = "C++11";
  else
    compliance = format (level);
#endif
  std::cout << " " << STRING_CMD_DIAG_COMPLIANCE
            << ": "
            << compliance
            << "\n\n";

  std::cout << bold.colorize (STRING_CMD_DIAG_FEATURES)
            << "\n"

  // Build date.
            << "      " << STRING_CMD_DIAG_BUILT << ": " << __DATE__ << " " << __TIME__ << "\n"
#ifdef HAVE_COMMIT
            << "     " << STRING_CMD_DIAG_COMMIT << ": " << COMMIT << "\n"
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

  std::cout << bold.colorize (STRING_CMD_DIAG_CONFIG)
            << "\n";

  char* env = getenv ("TASKRC");
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

  std::vector <std::string> paths;
  split (paths, path, ':');

  std::vector <std::string>::iterator i;
  for (i = paths.begin (); i != paths.end (); ++i)
  {
    File task (*i + "/task");
    if (task.exists ())
    {
      std::string input;
      std::string output;
      execute ("task", {"--version"}, input, output);

      std::cout << "Taskwarrior: "
                << (*i + "/task")
                << " "
                << output; // Still has \n
    }
  }

  std::cout << "\n";
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
