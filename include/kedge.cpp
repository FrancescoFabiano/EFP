#include "define.h"
#include "kstate.h"
#include "kedge.h"

/*********************************************************************
 Kedge implementation
**********************************************************************/

Kedge::Kedge()
{

}

Kedge::Kedge(Kedge* e)
{
	from = e->get_from();
	to = e->get_to();
	label = e->get_label();
	id = e->get_id();
}
Kedge::~Kedge()
{

}

int Kedge::get_from()
{
	return from;
}

int Kedge::get_to()
{
	return to;
}

Agent Kedge::get_label()
{
	return label;
}

int Kedge::get_id()
{
	return id;
}

bool Kedge::get_exist()
{
	return exists;
}

void Kedge::set_from(int ks)
{
	from = ks;
}

void Kedge::set_to(int ks)
{
	to = ks;
}

void Kedge::set_label(Agent agent)
{
	label = agent;
}

void Kedge::set_id(int ident)
{
	id = ident;
}

void Kedge::set_exist(bool ex)
{
	exists = ex;
}

void Kedge::print_info()
{
	cout << "edge id " << id << " label: " << label << " from: " << from << " to " << to << endl;	
}

