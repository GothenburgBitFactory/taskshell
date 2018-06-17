////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2006 - 2017, Paul Beckingham, Federico Hernandez.
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
#include <vector>
#include <string>
#include <cstring>
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <shared.h>

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
int cmdReview (const std::vector <std::string>&, bool);
int cmdShell (const std::vector <std::string>&);
std::string promptCompose ();
std::string findTaskwarrior ();

////////////////////////////////////////////////////////////////////////////////
static void welcome ()
{
  std::cout << PACKAGE_STRING << "\n";
  cmdHelp ();
}

////////////////////////////////////////////////////////////////////////////////
const std::string getResponse (const std::string& prompt)
{
  std::string response {""};

  // Display prompt, get input.
#ifdef HAVE_READLINE
  char *line_read = readline (prompt.c_str ());
  if (! line_read)
  {
    std::cout << "\n";
    response = "<EOF>";
  }
  else
  {
    // Save history.
    if (*line_read)
      add_history (line_read);

    response = std::string (line_read);
    free (line_read);
  }
#else
  std::cout << prompt;
  std::getline (std::cin, response);
  if (std::cin.eof () == 1)
  {
    std::cout << "\n";
    response = "<EOF>";
  }
#endif

  return response;
}

////////////////////////////////////////////////////////////////////////////////
static int commandLoop (bool autoClear)
{
  // Compose the prompt.
  auto prompt = promptCompose ();

  // Display prompt, get input.
  auto command = getResponse (prompt);

  // Obey Taskwarrior's rc.tasksh.autoclear.
  if (autoClear)
    std::cout << "\033[2J\033[0;0H";

  int status = 0;
  if (! isatty (fileno (stdin)) && command == "")
  {
    status = -1;
  }
  else if (command != "")
  {
    std::vector<std::string> args;
    std::string n;
    for (const auto& s : split (command, ' '))
    {
       if ((n = trim(s)) != "")
	 args.push_back(n);
    }

    // Dispatch command.
         if (args[0] == "<EOF>")                      status = -1;
    else if (closeEnough ("exit",        args[0], 3)) status = -1;
    else if (closeEnough ("quit",        args[0], 3)) status = -1;
    else if (closeEnough ("help",        args[0], 3)) status = cmdHelp ();
    else if (closeEnough ("diagnostics", args[0], 3)) status = cmdDiagnostics ();
    else if (closeEnough ("review",      args[0], 3)) status = cmdReview (args, autoClear);
    else if (closeEnough ("exec",        args[0], 3) ||
             args[0][0] == '!')                       status = cmdShell (args);
    else if (command != "")
    {
      command = "task " + command;
      std::cout << "[" << command << "]\n";
      system (command.c_str ());

      // Deliberately ignoreѕ taskwarrior exit status, otherwise empty filters
      // cause the shell to terminate.
    }
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
      // Get the Taskwarrior rc.tasksh.autoclear Boolean setting.
      bool autoClear = false;
      std::string input;
      std::string output;
      execute ("task", {"_get", "rc.tasksh.autoclear"}, input, output);
      output = lowerCase (output);
      autoClear = (output == "true\n" ||
                   output == "1\n"    ||
                   output == "y\n"    ||
                   output == "yes\n"  ||
                   output == "on\n");

      if (isatty (fileno (stdin)))
        welcome ();

      while ((status = commandLoop (autoClear)) == 0)
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

  // Returning -1 drops out of the command loop, but gets translated to 0 here,
  // so that there is a clean way to exit.
  return status == -1 ? 0 : status;
}

////////////////////////////////////////////////////////////////////////////////
