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

////////////////////////////////////////////////////////////////////////////////
//
// This file contains all the strings that should be localized.  If a string is
// *not* in this file, then either:
//   (a) it should not be localized, or
//   (b) you have found a bug - please report it
//
// Strings that should be localized:
//   - text output that the user sees
//
// Strings that should NOT be localized:
//   - .taskrc configuration variable names
//   - command names
//   - extension function names
//   - certain literals associated with parsing
//   - debug strings
//   - attribute names
//   - modifier names
//   - logical operators (and, or, xor)
//
// Rules:
//   - Localized strings should not in general  contain leading or trailing
//     white space, including \n, thus allowing the code to compose strings.
//   - Retain the tense of the original string.
//   - Retain the same degree of verbosity of the original string.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// Translators:
//   1. Copy this file (eng-USA.h) to a new file with the target locale as the
//      file name.  Using German as an example, do this:
//
//        cp eng-USA.h deu-DEU.h
//
//   2. Modify all the strings below.
//        i.e. change "Unknown error." to "Unbekannter Fehler.".
//
//   3. Add your new translation to the task.git/src/i18n.h file, if necessary,
//      by inserting:
//
//        #elif PACKAGE_LANGUAGE == LANGUAGE_DEU_DEU
//        #include <deu-DEU.h>
//
//   4. Add your new language to task.git/CMakeLists.txt, making sure that
//      number is unique:
//
//        set (LANGUAGE_DEU_DEU 3)
//
//   5. Add your new language to task.git/cmake.h.in:
//
//        #define LANGUAGE_DEU_DEU ${LANGUAGE_DEU_DEU}
//
//   6. Build your localized Taskwarrior with these commands:
//
//      cd task.git
//      cmake -D LANGUAGE=3 .
//      make
//
//   7. Submit your translation to support@taskwarrior.org, where it will be
//      shared with others.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_STRINGS
#define INCLUDED_STRINGS

// Note that for English, the following two lines are not displayed.  For all
// other localizations, these lines will appear in the output of the 'version'
// and 'diagnostics' commands.
//
// DO NOT include a copyright in the translation.
//
#define STRING_LOCALIZATION_DESC     "English localization"
#define STRING_LOCALIZATION_AUTHOR   "Translated into English by A. Person."



// Errors
#define STRING_UNKNOWN_ERROR         "Unknown error."

// Color
#define STRING_COLOR_UNRECOGNIZED    "The color '{1}' is not recognized."

// FS
#define STRING_FS_PERMS              "Tasksh does not have the correct permissions for '{1}'."

// Review
#define STRING_REVIEW_MODIFIED       "Modified."
#define STRING_REVIEW_REVIEWED       "Marked as reviewed."
#define STRING_REVIEW_COMPLETED      "Completed."
#define STRING_REVIEW_DELETED        "Deleted."
#define STRING_REVIEW_UNRECOGNIZED   "Command '{1}' is not recognized."
#define STRING_REVIEW_END            "End of review. {1} out of {2} tasks reviewed."

#define STRING_REVIEW_INTRO_1        "The review process is important for keeping your list accurate, so you are working on the right thing."
#define STRING_REVIEW_INTRO_2        "For each task you are shown, look at the metadata. Determine whether the task needs to be changed (enter 'e' to edit), or whether it is accurate (enter 'r' to mark as reviewed). You may skip a task ('enter') but a skipped task is not considered reviewed."
#define STRING_REVIEW_INTRO_3        "You may stop at any time, and resume later, right where you left off. See 'man tasksh' for more details."
#define STRING_REVIEW_PROMPT         "(Enter) Skip, (e)dit, (c)ompleted, (d)eleted, Mark as (r)eviewed, (q)uit"

// Initial prompt
#define STRING_COMMAND_USAGE         "  Commands:\n\
    tasksh> review           Task review session\n\
    tasksh> list             Or any other Taskwarrior command\n\
    tasksh> diagnostics      Tasksh diagnostics\n\
    tasksh> help             Tasksh help\n\
    tasksh> exec ls -al      Any shell command.  May also use '!ls -al'\n\
    tasksh> quit             End of session. May also use 'exit'\n"

// Help
#define STRING_COMMAND_HELP          "Run 'man tasksh' from your shell prompt.\nRun '! man tasksh' from inside tasksh.\n"

// Diagnostics
#define STRING_CMD_DIAG_PLATFORM     "Platform"
#define STRING_CMD_DIAG_UNKNOWN      "Unknown"
#define STRING_CMD_DIAG_COMPILER     "Compiler"
#define STRING_CMD_DIAG_VERSION      "Version"
#define STRING_CMD_DIAG_CAPS         "Caps"
#define STRING_CMD_DIAG_COMPLIANCE   "Compliance"
#define STRING_CMD_DIAG_FEATURES     "Build Features"
#define STRING_CMD_DIAG_BUILT        "Built"
#define STRING_CMD_DIAG_COMMIT       "Commit"
#define STRING_CMD_DIAG_CONFIG       "Configuration"

#endif

