/* main.cc */

#include "../include/reader.h"
#include "../include/timer.h"
#include "../include/planner.h"
#include "../include/kstate.h"
#include "../include/kripke.h"
#include <stdio.h>
#include <string.h>

#define VERSION "2.0"
#define DEBUG

// functions
void print_usage (char*);

Reader reader;
Timer timer;
Kripke test;

int
main (int argc, char **argv)
{
  int i;
  Planner planner (&reader, &timer);

  cout << "EFP version " << VERSION <<
          " - Built date: " << BUILT_DATE << endl;

  if (argc < 2)
    print_usage (argv[0]);

  i = 2;
  while (i < argc)
    {
      if (strcmp (argv[i], "-pc") == 0)
        {
          planner.m_semantics = PC;
        }
      else if (strcmp (argv[i], "-debug") == 0)
        {
          cout << " debug is on " << endl;
          planner.debug = true;
        }
      else if (strcmp (argv[i], "-mpg") == 0)
        {
          cout << " search with planning graph heuristic " << endl;
          planner.m_heuristic = PLANNINGGRAPH;
          planner.useHeuristic = true;
        }
      else if (strcmp (argv[i], "-spg") == 0)
        {
          cout << " search with planning graph heuristic " << endl;
          planner.m_heuristic = SUMPLANNINGGRAPH;
          planner.useHeuristic = true;
        }
      else if (strcmp (argv[i], "-h") == 0)
        {
          cout << " search with heuristic " << endl;
          planner.m_semantics = PH;
        }
      else if (strcmp (argv[i], "-p") == 0)
        {
          cout << " search with heuristic " << endl;
          planner.m_series_kripkes = true;
        }
      else if (strcmp (argv[i], "-e") == 0)
        {
          planner.m_task = DOPLAN;

          if (i == argc - 1)
            print_usage (argv[0]);

          i++;

          while (i < argc && argv[i][0] != '-')
            {
              planner.m_plan.push_back (string (argv[i++]));
            }
        }
      else if (strcmp (argv[i], "-p") == 0)
        {
          planner.m_detect_essential = false;
        }
      else if (strcmp (argv[i], "-o") == 0)
        {
          planner.m_output_decisive = true;
        }
      else
        print_usage (argv[0]);
      i++;
    }

  if (freopen (argv[1], "r", stdin) == NULL)
    {
      cerr << argv[0] << ": File " << argv[1] << " cannot be opened.\n";
      exit (1);
    }

  timer.start (READ_TIMER);
  reader.read ();
  if (planner.debug) reader.print ();
  timer.end (READ_TIMER);
  planner.main ();

  exit (0);
}

void
print_usage (char* prog_name)
{
  cout << "USAGE:" << endl;
  cout << "  " << prog_name << " input_domain [options]" << endl << endl;
  cout << "OPTIONS:" << endl;
  cout << "  -e action1 action2 action3 ..." << endl;
  cout << "     Perform a sequence of actions and print out" << endl;
  cout << "     results step by step. The planner does not" << endl;
  cout << "     search for a plan." << endl << endl;
  cout << "  -p" << endl;
  cout << "     Force the planner to use the possible world semantics (CPA*)." << endl;
  cout << "     This option should be used for testing purpose only." << endl;
  cout << "  -o" << endl;
  cout << "     Print out decisive sets. Effective only when option -p" << endl;
  cout << "     is not specified." << endl;
  cout << "EXAMPLES:" << endl;
  cout << "  " << prog_name << " blw.al" << endl;
  cout << "     Find a conformant plan for blw.al. The system will" << endl;
  cout << "     automatically detect necessary fluents to be considered." << endl;
  cout << "  " << prog_name << " blw.al -e \"move(2,table)\" \"move(1,2)\" -p" << endl;
  cout << "     Execute the plan [move(2,table);move(1,2)]." << endl;
  cout << "     The possible world semantics is used." << endl << endl;

  exit (1);
}
