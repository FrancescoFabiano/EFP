/**
 * \class cem_store
 * \brief Singleton class that stores the unique copy of each \ref cevent created.
 * 
 * \details All the other class only store pointers of \ref cevent and this class takes care of creating new
 * one in case the requested one doesn't exist.
 * 
 * @see cevent.
 * 
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date June 24, 2021
 */

#pragma once

#include <set>
#include "cem.h"

typedef act_type cem_id;
typedef std::set<cevent> cevent_set; /**< \brief A set of \ref cevent, used to store all the created ones.*/
typedef std::set<cem> cem_set; /**< \brief A set of \ref cem, used to store all the created ones.*/

class cem_store {
private:

    /** \brief The cevent representing the perspective of oblivious agents.*/
    cevent_ptr m_epsilon;
    /** \brief A set that contains all the generated cems (even the cems that are "internal")*/
    cevent_set m_created_events;

    /** \brief All the cevent that represent an action (given as input).
     *
     * These cems are pointer to the ones in \ref m_created_events.*/
    cem_set m_created_cems;

    std::map<std::string, agent_group_id> m_agent_group_ids;
    std::map<std::string, cem_id> m_cem_ids_map;
    std::map<std::string, event_id> m_events_ids_map;


    //  std::map<agent_group_id, std::string> m_inverse_agent_group_ids;


    /** \brief Private constructor since it is a Singleton class.*/
    cem_store();
public:
    /**Function that populates the list m_created_cems with the cevent model given as input in \ref file*/
    void generate(const std::string & file);

    /** \brief To get always (the same instance of) *this* and the same instantiated fields.*/
    static cem_store& get_instance();
    //    /** \brief Function that inserts a \ref cevent given its id to \ref m_created_cems.
    //     * @param[in] to_add: The cevent model that represent the graph of an action.
    //     * @parma[in] id: The id associated to \ref to_add.
    //     */
    //    void add_action_cem(const cevent & to_add, event_id id);
    //
    //    /** \brief Function that inserts a \ref cevent given its id to \ref m_created_cems.
    //     * @param[in] to_add: The cevent model (pointer) that represent the graph of an action.
    //     * @parma[in] id: The id associated to \ref to_add.
    //     */
    //    void add_action_cem(const cevent_ptr & to_add, event_id id);
    //
    //    /** \brief Function that retrieves a \ref cevent from its \ref event_id.
    //     * @parma[in] id: The id of the \ref cevent that needs to be returned.
    //     * @return: The cevent model that represent the graph of the action associated to \ref id.
    //     */
    //    const cevent_ptr & get_action_cem(event_id id);


    /** \brief Function that return the pointer to the given \ref cevent.
     * 
     * If the \ref cevent didn't exist then it is inserted to \ref m_created_events and then the pointer is returned;
     * if it existed a pointer to the already existing version is returned.
     * 
     * @param[in] to_add: the \ref cevent to add to the collection of created events.
     * 
     * @return the \ref cevent_ptr to \p to_add.*/
    const cevent_ptr add_event(const cevent & to_add);
    /** \brief Function that return the pointer to the given \ref cem.
     *
     * If the \ref cem didn't exist then it is inserted to \ref m_created_cems and then the pointer is returned;
     * if it existed a pointer to the already existing version is returned.
     *
     * @param[in] to_add: the \ref cem to add to the collection of created cems.
     *
     * @return the \ref cem_ptr to \p to_add.*/
    const cem_ptr add_cem(const cem & to_add);


    /** \brief Function that add a new agent group name to the map name->id.
     *
     * @return the \ref string to \p to_add in m_agent_group_ids.*/
    void add_agent_group(const std::string & to_add);


    /** \brief Function that return the pointer to the given \ref cevent from its id.
     * 
     * @return the \ref cevent_ptr to that has ref \id id.*/
    const cevent_ptr get_event(event_id id) const;

    /** \brief Function that return the pointer to the given \ref model from its id.
     * 
     * @return the \ref cem_ptr to that has ref \id id.*/
    const cem_ptr get_cem(cem_id id) const;



    /** \brief Function that returns the id given an agent group name.
     *
     * @return the \ref id of \p to_get.*/
    agent_group_id get_agent_group(const std::string & to_get) const;

    std::string get_agent_group_name(agent_group_id id) const;

    cem_id get_cem_id(const std::string & to_get);
    bool exist_cem(cem_id to_check);

    std::string get_cem_name(cem_id id) const;

    event_id get_event_id(const std::string & to_get);

    short get_agent_group_number() const;


    /** \brief Copy constructor removed since is Singleton class. */
    cem_store(cem_store const&) = delete;
    /** \brief Copy operator removed since Singleton class. */
    void operator=(cem_store const&) = delete;


    /*******FOR MAL OPTIMIZATION******/

    /** \brief Function that add a new cem name to the map name->id.
     *
     * This function has been added just for the mAl parsing*/
    void add_cem_name(const std::string & to_add);
};

