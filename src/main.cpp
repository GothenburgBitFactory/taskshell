////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2006 - 2014, Paul Beckingham, Federico Hernandez.
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
#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <i18n.h>
#include <text.h>

#ifdef HAVE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

// TODO These conflict with tw commands. This needs to be resolved.
//      Perhaps an escape, such as '-- help' could invoke local help, or using
//      a 'task' prefix could disambiguate.

// tasksh commands.
int cmdHelp ();
int cmdDiagnostics ();
std::string composePrompt ();
std::string findTaskwarrior ();

////////////////////////////////////////////////////////////////////////////////
static int commandLoop ()
{
  // TODO Local data: timer, context stack.  No globals.

  // Compose the prompt.
  std::string prompt = composePrompt ();

  // Display prompt, get input.
#ifdef HAVE_READLINE
  char *line_read = readline (prompt.c_str ());
  if (! line_read)
  {
    std::cout << "\n";
    return 1;
  }

  // Save history.
  if (*line_read)
    add_history (line_read);

  std::string command (line_read);
  free (line_read);
#else
  // TODO When Ctrl-D is pressed, it closeѕ std::cin, which causes this loop
  //      to cycle very quickly with no input. Perhaps reopen std::cin?
  std::cout << prompt;
  std::string command;
  std::getline (std::cin, command);
#endif

  // Dispatch command
  int status = 0;
       if (closeEnough ("exit",        command, 3)) status = 1;
  else if (closeEnough ("quit",        command, 3)) status = 1;
  else if (closeEnough ("help",        command, 3)) status = cmdHelp ();
  else if (closeEnough ("diagnostics", command, 3)) status = cmdDiagnostics ();
  else
  {
    std::cout << "[task " << command << "]\n";
    command = "task " + command;
    system (command.c_str ());

    // Deliberately ignoreѕ taskwarrior exit status, otherwise empty filters
    // cause the shell to terminate.
  }

  return status;
}

////////////////////////////////////////////////////////////////////////////////
int main (int argc, const char** argv)
{
  int status = 0;

  // Lightweight version checking that doesn't require initialization or any I/O.
  if (argc == 2 && !strcmp (argv[1], "--version"))
  {
    std::cout << VERSION << "\n";
  }
  else
  {
    try
    {
      while ((status = commandLoop ()) == 0)
        ;
    }

    catch (const std::string& error)
    {
      std::cerr << error << "\n";
      status = -1;
    }

    catch (...)
    {
      std::cerr << "Unknown error." << "\n";
      status = -2;
    }
  }

  return status;
}

////////////////////////////////////////////////////////////////////////////////
