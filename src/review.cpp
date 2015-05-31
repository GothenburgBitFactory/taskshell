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
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>

#ifdef HAVE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

#include <unistd.h>
#include <sys/ioctl.h>

#ifdef SOLARIS
#include <sys/termios.h>
#endif

#include <text.h>
#include <util.h>
#include <i18n.h>

std::string getResponse (const std::string&);

////////////////////////////////////////////////////////////////////////////////
static unsigned int getWidth ()
{
  // Determine window size.
//  int width = config.getInteger ("defaultwidth");
  int width = 0;

  if (width == 0)
  {
    unsigned short buff[4];
    if (ioctl (STDOUT_FILENO, TIOCGWINSZ, &buff) != -1)
      width = buff[1];
  }

  // Ncurses does this, and perhaps we need to as well, to avoid a problem on
  // Cygwin where the display goes right up to the terminal width, and causes
  // an odd color wrapping problem.
//  if (config.getBoolean ("avoidlastcolumn"))
//    --width;

  return width;
}

////////////////////////////////////////////////////////////////////////////////
static void editTask (const std::string& uuid)
{
  std::string command = "task rc.confirmation:no rc.verbose:nothing " + uuid + " edit";
  system (command.c_str ());

  command = "task rc.confirmation:no rc.verbose:nothing " + uuid + " modify reviewed:now";
  system (command.c_str ());
  std::cout << STRING_REVIEW_MODIFIED << "\n";
}

////////////////////////////////////////////////////////////////////////////////
static void reviewTask (const std::string& uuid)
{
  std::string command = "task rc.confirmation:no rc.verbose:nothing " + uuid + " modify reviewed:now";
  system (command.c_str ());
  std::cout << STRING_REVIEW_REVIEWED << "\n";
}

////////////////////////////////////////////////////////////////////////////////
static void completeTask (const std::string& uuid)
{
  std::string command = "task rc.confirmation:no rc.verbose:nothing " + uuid + " done";
  system (command.c_str ());
  std::cout << STRING_REVIEW_COMPLETED << "\n";
}

////////////////////////////////////////////////////////////////////////////////
static void deleteTask (const std::string& uuid)
{
  std::string command = "task rc.confirmation:no rc.verbose:nothing " + uuid + " delete";
  system (command.c_str ());
  std::cout << STRING_REVIEW_DELETED << "\n";
}

////////////////////////////////////////////////////////////////////////////////
static void reviewLoop (const std::vector <std::string>& uuids)
{
  auto tasks = 0;
  auto total = uuids.size ();

  unsigned int current = 0;
  while (current < total)
  {
    auto uuid = uuids[current];
    std::cout << "# [" << current << "] uuid '" << uuid << "'\n";

    // TODO Run 'info' report for task
    std::string command = "task " + uuid + " information";
    std::cout << "# [" << current << "] command '" << command << "'\n";
    system (command.c_str ());

    // TODO Add prompt context '<ID> (<n> of <total>)'
    std::string prompt = "(Enter) Skip, (e)dit, (c)ompleted, (d)eleted, Mark as (r)eviewed, (q)uit ";

    // Display prompt, get input.
    command = getResponse (prompt);

         if (command == "e") { editTask (uuid);          ++current; }
    else if (command == "r") { reviewTask (uuid);        ++current; }
    else if (command == "c") { completeTask (uuid);      ++current; }
    else if (command == "d") { deleteTask (uuid);        ++current; }
    else if (command == "q") { break;                               }
    else if (command == "")  { std::cout << "Skipped\n"; ++current; }
    else
    {
      std::cout << format (STRING_REVIEW_UNRECOGNIZED, command) << "\n";
    }

    // Note that just hitting <Enter> yields an empty command, which does
    // nothing but advance to the next task.

    // TODO Remove prompt context.
  }

  std::cout << format (STRING_REVIEW_END, tasks, total) << "\n";
}

////////////////////////////////////////////////////////////////////////////////
int cmdReview (const std::vector <std::string>& args)
{
  // Configure 'reviewed' UDA, but only if necessary.
  std::string input;
  std::string output;
  auto status = execute ("task",
                         {"_get", "rc.uda.reviewed.type"},
                         input,
                         output);
  if (status || output != "date\n")
  {
    execute ("task", {"rc.confirmation:no", "rc.verbose:nothing", "config", "uda.reviewed.type", "date"},      input, output);
    execute ("task", {"rc.confirmation:no", "rc.verbose:nothing", "config", "uda.reviewed.label", "Reviewed"}, input, output);
    execute ("task", {"rc.confirmation:no", "rc.verbose:nothing", "config", "review.period", "1week"},         input, output);
  }

  // Obtain a list of UUIDs to review.
  // TODO Incorporate filter 'rc.report.review_temp.filter:reviewed.before:now-1week'.
  // TODO Incorporate user supplied filter 'review <filter>'.
  status = execute ("task",
                    {"rc.report.review_temp.columns:uuid",
                     "rc.report.review_temp.sort:reviewed+",
//                     "rc.report.review_temp.filter:reviewed.before:now-1week",
                     "rc.verbose:nothing",
                     "(", "+PENDING", "or", "+WAITING", ")",
                     "review_temp"},
                    input, output);

  // Iterate over each task in the list.
  std::vector <std::string> uuids;
  split (uuids, output, '\n');
  reviewLoop (uuids);
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
