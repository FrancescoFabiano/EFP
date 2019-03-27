#ifndef KEDGE_H
#define KEDGE_H
#include "define.h"
#include "kstate.h"


class Kstate;


class Kedge{
   protected:
	int from;
	int to;
	Agent label;
	int id;
	bool exists;

   public:
	Kedge();
	Kedge(Kedge*);
	~Kedge();

	/* operators */
	int get_from();
	int get_to();
	Agent get_label();
	int get_id();
	bool get_exist();
	void set_from(int);
	void set_to(int);
	void set_label(Agent);
	void set_id(int);
	void set_exist(bool);
	
	void print_info();


};


#endif
