#include "kstate.h"
#include "kripke.h"
#include "kedge.h"
#include "planner.h"
#include "define.h"

/*********************************************************************
 Kripke implementation
 **********************************************************************/

Kripke::Kripke () {
 }

Kripke::Kripke (Kripke* k)
{
  states_list = k->get_stateslist ();
  edges_list = k->get_edgeslist ();
  //B edges;
  pId = k->get_pstate ()->get_id ();
}

Kripke::~Kripke () {
 }

Kripke*
Kripke::make_copy ()
{
  Kripke* out = new Kripke ();
  out->set_stateslist_fix_states (states_list);
  out->set_edgeslist_fix_states (edges_list);
  out->set_pId (pId);
  return out;
}

States
Kripke::get_stateslist ()
{
  return states_list;
}

B
Kripke::get_edges ()
{
  return edges;
}

Edges
Kripke::get_edgeslist ()
{
  return edges_list;
}

Kstate*
Kripke::get_pstate ()
{
  return &states_list.at (pId);
}

int
Kripke::get_id ()
{
  return pId;
}

void
Kripke::set_stateslist (States sl)
{
  states_list = sl;
}

void
Kripke::set_edges (B el)
{
  edges = el;
}

void
Kripke::set_edgeslist (Edges el)
{
  edges_list = el;
}

void
Kripke::set_stateslist_fix_states (States sl) ////set the stateslist and clear the hashmap mapEdgelabelNextState in each state as well
{
  for (States::iterator i = sl.begin (); i != sl.end (); i++)
    {
      (*i).reset_map ();
    }
  states_list = sl;

}

void
Kripke::set_edgeslist_fix_states (Edges el) //set the edgeslist and fix the hashmap mapEdgelabelNextState in each state as well
{
  for (Edges::iterator i = el.begin (); i != el.end (); i++)
    {
      add_edge (*i);
    }
  //edges_list = el;
}

void
Kripke::set_pId (int st)
{
  pId = st;
}

bool
Kripke::entail_ff_list (const list<FluentFormula> ffs) const
{
  list<FluentFormula>::const_iterator itff;
  BFNode ff;
  ff.node_type = fluForm;
  for (itff = ffs.begin (); itff != ffs.end (); itff++)
    {
      FluentFormula temp = *itff;
      ff.fluform = &temp;
      if (!entail (ff))
        {
          return false;
        }
    }
  return true;
}

bool
Kripke::entail_BForm (BFNode node, Agent ag, bool atleastone) const
{
  Kripke temp;
  temp.set_stateslist (states_list);
  temp.set_edgeslist (edges_list);

  Kstate pState = states_list.at (pId);
  EdgeList el = pState.get_out ();

  EdgeList::const_iterator it;

  bool exist_label;
  if (atleastone)
    {
      exist_label = false;
    }
  else
    {
      exist_label = true;
    }
  map<int, set<int> > k1map = pState.get_preparedMap ();
  if (k1map.find (ag) != k1map.end ())
    {
      set<int> forcheck = k1map[ag];
      for (int i : forcheck)
        {
          exist_label = true;
          temp.set_pId (i);
          if (atleastone && !temp.entail (node))
            {
              return false;
            }
          else if (!atleastone && !temp.entail1 (node))
            {
              return false;
            }
        }
    }
  if (!exist_label)
    {
      return false;
    }
  return true;

  //    for(it = el.begin(); it != el.end(); it++)
  //    {
  //        Kedge edg = edges_list.at(*it);
  //        if(edg.get_label() == ag)
  //        {
  //            exist_label = true;
  //            temp.set_pId(edg.get_to());
  //            if(atleastone && !temp.entail(node))
  //            {
  //                return false;
  //            }else if(!atleastone && !temp.entail1(node)){
  //                return false;
  //            }
  //        }
  //    }
  //    if(!exist_label){
  //        return false;
  //    }
  //    return true;

}

bool
Kripke::isValid (AgentSet ag)
{
  Kstate p = states_list.at (pId);
  EdgeList el = p.get_out ();
  EdgeList::iterator it;
  for (it = el.begin (); it != el.end (); it++)
    {
      int label = edges_list.at (*it).get_label ();
      ag.erase (label);
      if (ag.size () == 0)
        {
          return true;
        }
    }
  return false;
}

StateList
Kripke::reachableStates (int fromStateID, Agents ags) const
{
  Kstate src = states_list.at (fromStateID);
  StateList out;
  //consider also K^0\varphi = \varphi, then add pointed state as one of reachable state
  //comment below if it is required that (M,w) \models C\varphi if (M,w) \models B^j\varphi where j >=1 and
  // B^1\varphi = B\varphi, B^k\varphi = B(B^{k-1}\varphi)
  //out.insert(fromStateID);

  StateList newlyadd;
  newlyadd.insert (fromStateID);
  while (newlyadd.size () != 0)
    {
      int curId = *newlyadd.begin ();
      newlyadd.erase (newlyadd.begin ());
      Kstate curstate = states_list.at (curId);
      EdgeList el = curstate.get_out ();
      EdgeList::iterator it;
      for (it = el.begin (); it != el.end (); it++)
        {
          Kedge e = edges_list.at (*it);
          Agent ag = e.get_label ();
          //check edge's agent
          Agents::iterator it1;
          it1 = std::find (ags.begin (), ags.end (), ag);
          if (it1 == ags.end ())
            {
              continue;
            }
          //check the state_to is in outlist or not yet
          int to = e.get_to ();
          StateList::iterator islt;
          islt = out.find (to);
          if (islt == out.end ())
            {
              out.insert (to);
              newlyadd.insert (to);
            }
        }
    }
  return out;
}
//Used when it DOES require at least one outgoing edge

bool
Kripke::entail (BFNode node) const
{
  switch (node.node_type)
    {
    case fluForm:
      {
        Kstate s = states_list.at (pId);
        return s.entail_fluform (*node.fluform);
      }
    case BForm:
      {
        Agent ag = node.agentPro2;
        BFNode bf = *node.bfnode1;
        return entail_BForm (bf, ag, true);
      }
    case propForm:
      {
        if (node.bfOperator == BFNOT)
          {
            return !entail (*node.bfnode1);
          }
        else if (node.bfOperator == NONE)
          {
            return entail (*node.bfnode1);
          }
        else if (node.bfOperator == BFOR)
          {
            return entail (*node.bfnode1) || entail (*node.bfnode2);
          }
        else if (node.bfOperator == BFAND)
          {
            return entail (*node.bfnode1) && entail (*node.bfnode2);
          }
        else
          {
            return true;
          }
      }
    case EForm:
      {
        Agents::iterator it;

        for (it = node.agents.begin (); it != node.agents.end (); it++)
          {
            if (!entail_BForm (*(node.bfnode1), *it, true))
              return false;
          }

        return true;
      }
    case CForm:
      {
        Agents ags = node.agents;
        StateList sl = reachableStates (pId, ags);
        if (sl.size () == 0)
          {
            return false;
          }
        StateList::iterator it;
        for (it = sl.begin (); it != sl.end (); it++)
          {
            Kripke temp;
            temp.set_stateslist (states_list);
            temp.set_edgeslist (edges_list);
            temp.set_pId (*it);
            if (!temp.entail (*node.bfnode1))
              {
                return false;
              }
          }
        return true;
      }
    case BFEmpty:
      {
        return true;
      }
    }
}
//Used when it DOES NOT require at least one outgoing edge

bool
Kripke::entail1 (BFNode node) const
{
  switch (node.node_type)
    {
    case fluForm:
      {
        Kstate s = states_list.at (pId);
        return s.entail_fluform (*node.fluform);
      }
    case BForm:
      {
        Agent ag = node.agentPro2;
        BFNode bf = *node.bfnode1;
        return entail_BForm (bf, ag, false);
      }
    case propForm:
      {
        if (node.bfOperator == BFNOT)
          {
            return !entail1 (*node.bfnode1);
          }
        else if (node.bfOperator == NONE)
          {
            return entail1 (*node.bfnode1);
          }
        else if (node.bfOperator == BFOR)
          {
            return entail1 (*node.bfnode1) || entail1 (*node.bfnode2);
          }
        else if (node.bfOperator == BFAND)
          {
            return entail1 (*node.bfnode1) && entail1 (*node.bfnode2);
          }
        else
          {
            return true;
          }
      }
    case EForm:
      {
        Agents::iterator it;

        for (it = node.agents.begin (); it != node.agents.end (); it++)
          {
            if (!entail_BForm (*(node.bfnode1), *it, false))
              return false;
          }

        return true;
      }
    case CForm:
      {
        Agents ags = node.agents;
        StateList sl = reachableStates (pId, ags);
        StateList::iterator it;
        for (it = sl.begin (); it != sl.end (); it++)
          {
            Kripke temp;
            temp.set_stateslist (states_list);
            temp.set_edgeslist (edges_list);
            temp.set_pId (*it);
            if (!temp.entail1 (*node.bfnode1))
              {
                return false;
              }
          }
        return true;
      }
    case BFEmpty:
      return true;
    }
}

void
Kripke::add_kstate (Kstate ks)
{
  states_list.push_back (ks);
}

void
Kripke::print_report ()
{
  cout << "The number of states is: " << states_list.size () << endl;
  cout << "The number of edges is: " << edges_list.size () << endl;
  cout << "s is: " << states_list.at (pId).get_id () << endl << endl;

  States::iterator it;
  for (it = states_list.begin (); it != states_list.end (); it++)
    {
      it->print_info ();
      cout << endl;
      //        cout << "print states into...." << endl;
      //        EdgeList::iterator iee;
      //        for(iee = (*it)->get_in()->begin(); iee!= (*it)->get_in()->end(); iee++){
      //            cout << "id of edge is " << (*iee)->get_id() << endl;
      //        }
    }

  Edges::iterator ite;
  for (ite = edges_list.begin (); ite != edges_list.end (); ite++)
    {
      ite->print_info ();
    }

  B::iterator it3;
  vector<int>::iterator it2;

  for (it3 = edges.begin (); it3 != edges.end (); it3++)
    for (it2 = it3->begin (); it2 != it3->end (); it2++)
      {
        cout << "Edge (" << edges_list.at (*it2).get_exist () << ") with id " << edges_list.at (*it2).get_id () << " has label " << edges_list.at (*it2).get_label ()
                << " and connects worlds " << edges_list.at (*it2).get_from () << " --> "
                << edges_list.at (*it2).get_to () << endl;
      }
  cout << endl << endl;
}

void
Kripke::add_edge2state (int stIndex, int edgeIndex, bool isOut)
{
  Kstate* st = &states_list.at (stIndex);
  if (isOut == true)
    {
      st->add_out (edgeIndex);
    }
  else
    {
      st->add_in (edgeIndex);
    }
}

void
Kripke::add_edge (Kedge k)
{
  edges_list.push_back (k);
  Kstate* stFrom = &states_list.at (k.get_from ());
  Kstate* stTo = &states_list.at (k.get_to ());
  Agent label = k.get_label ();
  //add edge to stFrom
  stFrom->add_out (k.get_id ());
  stFrom->add_to_map (label, k.get_to ());
  //add edge to stTo
  stTo->add_in (k.get_id ());
}

void
Kripke::add_to_edges_list (Kedge k) //add to edges list only, without add to states
{
  edges_list.push_back (k);
}

/*Francesco
The function to correct the behavior of the agents with false beliefs.
Call this function every time an announcemt or a sensing action are performed.
Reference: "mA: A High-Level Action Language for Epistemic Planning." Paper ID # 3815 (called mal.pdf in the references folder).
 */

void
negateFluentFormula (const FluentFormula ff, FluentFormula &not_ff, int increase, Fluents single_clause)
{
  FluentFormula::const_iterator it = ff.begin ();
  std::advance (it, increase);
  if (it == ff.end ())
    {
      not_ff.insert (single_clause);

      return;
    }
  Fluents tmp = *it;
  Fluents::iterator itSingle;
  Fluent neg;
  for (itSingle = tmp.begin (); itSingle != tmp.end (); ++itSingle)
    {
      NEGATE (*itSingle, neg);
      single_clause.insert (neg);
      negateFluentFormula (ff, not_ff, increase + 1, single_clause);
      single_clause.erase (neg);
    }

}

//will be deleted

BFNode
createTheNode_sens (FluentFormula not_ff, Agent ag)
{
  BFNode res;
  res.agentPro2 = ag;
  res.node_type = fluForm;
  FluentFormula copy = not_ff;
  res.fluform = &copy;
  return res;
}

//correcting

BFNode
createTheNode (FluentFormula not_ff, Agent ag)
{
  BFNode res1;
  BFNode res;
  res1.node_type = fluForm;
  res1.fluform = &not_ff;

  res.agentPro2 = ag;
  res.node_type = BForm;
  res.bfnode1 = &res1;
  return res;

}

void
Kripke::get_the_new_edges (Agent ag, int &id_edge, Edges &to_add)
{

  //The number of state (matrix dimension) will be the biggest state id
  int n_of_states = 0;
  //int n_of_states = states_list.size();

  //map<realId, matrixId>
  std::map<int, int> map_statesId;
  //map<matrixId, realId>
  std::map<int, int> reverse_map;


  States::iterator it0;
  for (it0 = states_list.begin (); it0 != states_list.end (); it0++)
    {
      //generate HASH table for <mainID, matrixID>
      map_statesId[it0->get_id ()] = n_of_states;
      reverse_map[n_of_states] = it0->get_id ();
      n_of_states++;
    }





  //Default: everything is false in the 2D boolean array

  //Keep a copy of the original one for final check of which edges we need to add
  bool originalAdj[n_of_states][n_of_states];
  bool transformedAdj[n_of_states][n_of_states];

  //Used for the special case of the reverse function if the agent from the pointed K structure does not have any edge
  bool atleastone = false;
  int i, j, k;

  //Generate the adj Matrix for the Agent ag from the list of edges of the Kripkie structure
  Edges::iterator it1;
  int map_id_to, map_id_from;
  for (it1 = edges_list.begin (); it1 != edges_list.end (); it1++)
    {
      if (it1->get_label () == ag) //If the edge correspond to the agent with false beliefs add it to its Adjacency Matrix
        {
          map_id_from = map_statesId[it1->get_from ()];
          map_id_to = map_statesId[it1->get_to ()];
          //if(it1->get_exist()) Do I need to check it?
          originalAdj[map_id_from][map_id_to] = true;

          //We check if there is at least one connection for the agent from this state for the special case of the Reverse function
          if ((it1->get_from () == pId) || (it1->get_to () == pId))
            {
              atleastone = true;
            }
        }
    }

  //Copy the initial configuation to transform
  for (i = 0; i < n_of_states; i++)
    {
      for (j = 0; j < n_of_states; j++)
        {
          transformedAdj[i][j] = originalAdj[i][j];
        }
    }

  //Reverse function

  //If the agent does not have connections from the pointed K structure we add one communication from the actual state to all the others (even itself)
  //We just need to introduce one way comunnication, the other will be added by the function below.
  //We just add the edges to existing states in the current Kripkie structure
  if (!atleastone)
    {
      States::iterator it2;
      for (it2 = states_list.begin (); it2 != states_list.end (); it2++)
        {
          map_id_to = map_statesId[it2->get_id ()];
          transformedAdj[pId][it2->get_id ()] = true;
        }
    }

  for (i = 0; i < n_of_states; i++)
    {
      for (j = 0; j < n_of_states; j++) //If the edge correspond to the agent with false beliefs add it to its Adjacency Matrix
        {
          if (transformedAdj[i][j])
            {
              transformedAdj[j][i] = true;
            }
        }
    }


  /*Transitive closure with Floyd-Warshall (http://www.geeksforgeeks.org/transitive-closure-of-a-graph/)*/
  for (k = 0; k < n_of_states; k++)
    {
      for (i = 0; i < n_of_states; i++)
        {
          for (j = 0; j < n_of_states; j++)
            {
              transformedAdj[i][j] = transformedAdj[i][j] || (transformedAdj[i][k] && transformedAdj[k][j]);
            }
        }
    }

  //Populate the to_add edge list
  for (i = 0; i < n_of_states; i++)
    {
      for (j = 0; j < n_of_states; j++)
        {
          if (!originalAdj[i][j] && transformedAdj[i][j])
            {
              Kedge temp = new Kedge ();
              temp.set_label (ag);

              temp.set_id (id_edge);
              temp.set_from (reverse_map[i]);
              temp.set_to (reverse_map[j]);
              to_add.push_back (temp);
              id_edge++;
            }
        }
    }
}

/*Since we add all the edges at the end (when we have already checked all the agents) we need to keep track of the id of the edges (Change every time we add an edge), using push_back we don't lose the correspondence*/

/*void Kripke::correctFalseBeliefs_sens(FluentFormula ff, Agent ag, int &id_edge, Edges &to_add) {
        FluentFormula not_ff;
        Fluents single_clause;
        negateFluentFormula(ff, not_ff, 0, single_clause);
        if (entail1(createTheNode_sens(not_ff, ag))) {
                get_the_new_edges(ag, id_edge, to_add);
        }
        return;
}*/

void
Kripke::correctFalseBeliefs (FluentFormula ff, Agent ag, int &id_edge, Edges &to_add)
{

  FluentFormula not_ff;
  Fluents single_clause;
  negateFluentFormula (ff, not_ff, 0, single_clause);

  if (entail1 (createTheNode (not_ff, ag)))
    {
      get_the_new_edges (ag, id_edge, to_add);
    }
  return;
}



