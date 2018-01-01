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
#include <vector>
#include <string>
#include <Color.h>

static std::vector <std::string> contextColors = {
  "bold white on red",
  "bold white on blue",
  "bold white on green",
  "bold white on magenta",
  "black on cyan",
  "black on yellow",
  "black on white",
};

static std::vector <std::string> contexts;

std::string composeContexts (bool pretty = false);

////////////////////////////////////////////////////////////////////////////////
int promptClear ()
{
  contexts.clear ();
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
int promptRemove ()
{
  if (contexts.size ())
    contexts.pop_back ();

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
int promptAdd (const std::string& context)
{
  contexts.push_back (context);
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
std::string composeContexts (bool pretty /* = false */)
{
  std::string combined;
  for (unsigned int i = 0; i < contexts.size (); i++)
    if (pretty)
      combined += (combined != "" ? " " : "")
                + std::string ("\001")
                + Color::colorize ("\002 " + contexts[i] + " \001", contextColors[i % contextColors.size ()])
                + "\002";
    else
      combined += (combined != "" ? " " : "") + contexts[i];

  if (combined != "")
    combined += ' ';

  return combined;
}

////////////////////////////////////////////////////////////////////////////////
std::string promptCompose ()
{
  // TODO The prompt may be composed of different elements:
  // TODO - The configurable text
  // TODO - The accumulated context, as colored tokens.
  // TODO - sync status
  // TODO - time
  auto decoration = composeContexts (true);
  if (decoration.length ())
    return "task " + decoration + "> ";

  return "tasksh> ";
}

////////////////////////////////////////////////////////////////////////////////
