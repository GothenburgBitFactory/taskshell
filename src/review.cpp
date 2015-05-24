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

#ifdef HAVE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

////////////////////////////////////////////////////////////////////////////////
static int reviewLoop (const std::string& uuid)
{
  // TODO Add prompt context '<ID> (<n> of <total>)'
  // TODO Present options '(Enter) Skip, (e)dit, (c)ompleted, (d)eleted, Mark as (r)eviewed, (q)uit'
  std::string prompt = "(Enter) Skip, (e)dit, (c)ompleted, (d)eleted, Mark as (r)eviewed, (q)uit ";

  // TODO Run 'info' report for task

  // Display prompt, get input.
#ifdef HAVE_READLINE
  char *line_read = readline (prompt.c_str ());
  if (! line_read)
  {
    std::cout << "\n";
    return -1;
  }

  // Save history.
  if (*line_read)
    add_history (line_read);

  std::string command (line_read);
  free (line_read);
#else
  std::cout << prompt;
  std::string command;
  std::getline (std::cin, command);
  if (std::cin.eof () == 1)
  {
    std::cout << "\n";
    return -1;
  }
#endif

  int status = 0;
  if (command == "")
  {
    // TODO S --> nop
  }
  else if (command == "e")
  {
    // TODO E --> task edit <UUID>
  }
  else if (command == "r")
  {
    // TODO R --> task <UUID> modify reviewed:now
  }
  else if (command == "c")
  {
    // TODO C --> task <UUID> done
  }
  else if (command == "d")
  {
    // TODO D --> task rc.confirmation:no <UUID> delete
  }
  else if (command == "q")
  {
    // TODO Q --> end review
    status = 1;
  }

  // TODO Remove prompt context.

  return status;
}

////////////////////////////////////////////////////////////////////////////////
int cmdReview (const std::vector <std::string>& args)
{
  // TODO Configure 'reviewed' UDA, if necessary.
  // TODO Generate list of tasks.
  //      - status 'Pending' or 'Waiting'
  //      - 'reviewed' attribute older than 'now - rc.review.period'
  //      - apply <filter>, if specified
  //      - sort by ascending 'reviewed' date
  std::vector <std::string> uuids;
  uuids.push_back ("one");
  uuids.push_back ("two");

  // TODO Iterate over each task in the list.

  for (auto& uuid : uuids)
    while (reviewLoop (uuid) == 0)
      ;

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
