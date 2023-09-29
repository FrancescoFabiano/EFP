#include "kstate.h"
#include "define.h"
#include "kedge.h"
#include <map>
#include <list>

/*********************************************************************
 Kstate implementation
**********************************************************************/

Kstate::Kstate()
{
    edge_out = EdgeList();
    edge_in = EdgeList();
    mapEdgelabelNextState = map<int, set<int> >();
}

Kstate::~Kstate()
{

}

EdgeList Kstate::get_in()
{
	return edge_in;
}

EdgeList Kstate::get_out()
{
	return edge_out;
}

Literals Kstate::get_lits()
{
	return lits;
}

int Kstate::get_id()
{
	return id;
}

bool Kstate::get_exist()
{
	return exists;
}

void Kstate::set_in(EdgeList in)
{
	edge_in = in;
}

void Kstate::set_out(EdgeList out)
{
	edge_out = out;
}

void Kstate::set_lits(Literals ls)
{
	lits = ls;
}

void Kstate::set_id(int ident)
{
	id = ident;
}

void Kstate::set_exist(bool ex)
{
	exists = ex;
}

map<int, set<int> > Kstate::get_preparedMap(){
    return mapEdgelabelNextState;
}

void Kstate::reset_map(){
    mapEdgelabelNextState = map<int, set<int> >();
}

void Kstate::add_to_map(int ag, int des){
    
    set<int> isAdding;
    if(mapEdgelabelNextState.find(ag) != mapEdgelabelNextState.end()){
        isAdding = mapEdgelabelNextState[ag];
    }else{
        isAdding = set<int>();
    }
    isAdding.insert(des);
    mapEdgelabelNextState[ag] = isAdding;
}

bool Kstate::entail_lit(Literal l) const
{
    Literals::const_iterator it;
    it = lits.find(l);
    if(it == lits.end())
    {
        return false;
    }
    return true;
}

bool Kstate::entail_lits(Literals ls) const
{
    if(ls.size()==0) // Literal = [] then return true;
    {
        return true;
    }
    Literals::const_iterator it;
    for(it = ls.begin(); it!= ls.end();it++)
    {
        if(!entail_lit(*it))
        {
            return false;
        }
    }
    return true;
}

bool Kstate::entail_fluform(FluentFormula ff) const
{
    FluentFormula::const_iterator it;
    for(it = ff.begin(); it!= ff.end();it++)
    {
        if(entail_lits(*it))
        {
            return true;
        }
    }
    return false;
}


Kstate* Kstate::copy()
{
	Kstate* k1 = new Kstate();
    k1->set_lits(lits);
    k1->set_id(id); //keep the same id
    k1->set_out(edge_out);
    k1->set_in(edge_in);
    return k1;
}

void Kstate::add_out(int e)
{
	edge_out.push_back(e);
}

void Kstate::add_in(int e)
{
	edge_in.push_back(e);
}

void Kstate::print_info()
{	
	cout << "State " << id << " specs:" << endl;
        //cout << "Does exist: " << exists << endl;
		cout << "--The number of edges out: " << edge_out.size() << endl;
		EdgeList::iterator ite;
		for(ite = edge_out.begin();ite!=edge_out.end();ite++)
		{
			cout << *ite << " ";
		}
		cout << endl;
		cout << "--The number of edges in: " << edge_in.size() << endl;
		for(ite = edge_in.begin();ite!=edge_in.end();ite++)
		{
			cout << *ite << " ";
		}
		cout << endl;
        cout << "--contains literals: ";
	Literals::iterator it;

	
	cout << "Literals contained in state " << id << ":";
	for(it = lits.begin(); it != lits.end(); it++)
		cout << *it << ", ";
	cout << endl;

}
