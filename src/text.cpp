////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2006 - 2016, Paul Beckingham, Federico Hernandez.
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
#include <text.h>
#include <algorithm>
#include <sstream>
#include <vector>
#include <string>
#include <strings.h>
#include <utf8.h>

static void replace_positional (std::string&, const std::string&, const std::string&);

///////////////////////////////////////////////////////////////////////////////
void wrapText (
  std::vector <std::string>& lines,
  const std::string& text,
  const int width,
  bool hyphenate)
{
  std::string line;
  unsigned int offset = 0;
  while (extractLine (line, text, width, hyphenate, offset))
    lines.push_back (line);
}

////////////////////////////////////////////////////////////////////////////////
// Walk the input text looking for a break point.  A break point is one of:
//   - EOS
//   - \n
//   - last space before 'length' characters
//   - last punctuation (, ; . :) before 'length' characters, even if not
//     followed by a space
//   - first 'length' characters
//
// text       "one two three\n  four"
// bytes       0123456789012 3456789
// characters  1234567890a23 4567890
//
// leading_ws
// ws             ^   ^       ^^
// punct
// break                     ^
bool extractLine (
  std::string& line,
  const std::string& text,
  int width,
  bool hyphenate,
  unsigned int& offset)
{
  // Terminate processing.
  // Note: bytes vs bytes.
  if (offset >= text.length ())
    return false;

  std::string::size_type last_last_bytes = offset;
  std::string::size_type last_bytes = offset;
  std::string::size_type bytes = offset;
  unsigned int last_ws = 0;
  int character;
  int char_width = 0;
  int line_width = 0;
  while (1)
  {
    last_last_bytes = last_bytes;
    last_bytes = bytes;
    character = utf8_next_char (text, bytes);

    if (character == 0 ||
        character == '\n')
    {
      line = text.substr (offset, last_bytes - offset);
      offset = bytes;
      break;
    }
    else if (character == ' ')
      last_ws = last_bytes;

    char_width = mk_wcwidth (character);
    if (line_width + char_width > width)
    {
      int last_last_character = text[last_last_bytes];
      int last_character = text[last_bytes];

      // [case 1] one| two --> last_last != 32, last == 32, ws == 0
      if (last_last_character != ' ' &&
          last_character      == ' ')
      {
        line = text.substr (offset, last_bytes - offset);
        offset = last_bytes + 1;
        break;
      }

      // [case 2] one |two --> last_last == 32, last != 32, ws != 0
      else if (last_last_character == ' ' &&
               last_character      != ' ' &&
               last_ws             != 0)
      {
        line = text.substr (offset, last_bytes - offset - 1);
        offset = last_bytes;
        break;
      }

      else if (last_last_character != ' ' &&
               last_character      != ' ')
      {
        // [case 3] one t|wo --> last_last != 32, last != 32, ws != 0
        if (last_ws != 0)
        {
          line = text.substr (offset, last_ws - offset);
          offset = last_ws + 1;
          break;
        }
        // [case 4] on|e two --> last_last != 32, last != 32, ws == 0
        else
        {
          if (hyphenate)
          {
            line = text.substr (offset, last_bytes - offset - 1) + "-";
            offset = last_last_bytes;
          }
          else
          {
            line = text.substr (offset, last_bytes - offset);
            offset = last_bytes;
          }
        }

        break;
      }
    }

    line_width += char_width;
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////
void split (
  std::vector<std::string>& results,
  const std::string& input,
  const char delimiter)
{
  results.clear ();
  std::string::size_type start = 0;
  std::string::size_type i;
  while ((i = input.find (delimiter, start)) != std::string::npos)
  {
    results.push_back (input.substr (start, i - start));
    start = i + 1;
  }

  if (input.length ())
    results.push_back (input.substr (start));
}

////////////////////////////////////////////////////////////////////////////////
void join (
  std::string& result,
  const std::string& separator,
  const std::vector<std::string>& items)
{
  std::stringstream s;
  unsigned int size = items.size ();
  for (unsigned int i = 0; i < size; ++i)
  {
    s << items[i];
    if (i < size - 1)
      s << separator;
  }

  result = s.str ();
}

////////////////////////////////////////////////////////////////////////////////
std::string trimLeft (const std::string& in, const std::string& t /*= " "*/)
{
  std::string out = in;
  return out.erase (0, in.find_first_not_of (t));
}

////////////////////////////////////////////////////////////////////////////////
std::string trimRight (const std::string& in, const std::string& t /*= " "*/)
{
  std::string out = in;
  return out.erase (out.find_last_not_of (t) + 1);
}

////////////////////////////////////////////////////////////////////////////////
std::string trim (const std::string& in, const std::string& t /*= " "*/)
{
  std::string out = in;
  return trimLeft (trimRight (out, t), t);
}

////////////////////////////////////////////////////////////////////////////////
std::string lowerCase (const std::string& input)
{
  std::string output = input;
  std::transform (output.begin (), output.end (), output.begin (), tolower);
  return output;
}

////////////////////////////////////////////////////////////////////////////////
bool compare (
  const std::string& left,
  const std::string& right,
  bool sensitive /*= true*/)
{
  // Use strcasecmp if required.
  if (!sensitive)
    return strcasecmp (left.c_str (), right.c_str ()) == 0 ? true : false;

  // Otherwise, just use std::string::operator==.
  return left == right;
}

////////////////////////////////////////////////////////////////////////////////
bool closeEnough (
  const std::string& reference,
  const std::string& attempt,
  unsigned int minLength /* = 0 */)
{
  if (compare (reference, attempt, false))
    return true;

  if (attempt.length () < reference.length () &&
      attempt.length () >= minLength)
    return compare (reference.substr (0, attempt.length ()), attempt, false);

  return false;
}

////////////////////////////////////////////////////////////////////////////////
const std::string format (int value)
{
  std::stringstream s;
  s << value;
  return s.str ();
}

////////////////////////////////////////////////////////////////////////////////
static void replace_positional (
  std::string& fmt,
  const std::string& from,
  const std::string& to)
{
  std::string::size_type pos = 0;
  while ((pos = fmt.find (from, pos)) != std::string::npos)
  {
    fmt.replace (pos, from.length (), to);
    pos += to.length ();
  }
}

////////////////////////////////////////////////////////////////////////////////
const std::string format (
  const std::string& fmt,
  const std::string& arg1)
{
  std::string output = fmt;
  replace_positional (output, "{1}", arg1);
  return output;
}

////////////////////////////////////////////////////////////////////////////////
const std::string format (
  const std::string& fmt,
  int arg1,
  int arg2)
{
  std::string output = fmt;
  replace_positional (output, "{1}", format (arg1));
  replace_positional (output, "{2}", format (arg2));
  return output;
}

////////////////////////////////////////////////////////////////////////////////
