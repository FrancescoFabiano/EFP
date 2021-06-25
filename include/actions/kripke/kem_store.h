/**
 * \class kem_store
 * \brief Singleton class that stores the unique copy of each \ref kevent created.
 * 
 * \details All the other class only store pointers of \ref kevent and this class takes care of creating new
 * one in case the requested one doesn't exist.
 * 
 * @see kevent.
 * 
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date June 24, 2021
 */

#pragma once

#include <set>
#include "kevent.h"
#include "kem.h"

class kem_store
{
private:
    /** \brief The kevent representing the perspective of oblivious agents.*/
    kevent_ptr m_epsilon;
    /** \brief A set that contains all the generated kems (even the kems that are "internal")*/
    kevent_set m_created_events;

    /** \brief All the kevent that represent an action (given as input).
     *
     * These kems are pointer to the ones in \ref m_created_events.*/
    kem_set m_created_kems;

    std::map<std::string, agent_group_id> m_agent_group_ids;
    std::map<std::string, kem_id> m_kem_ids_map;
    std::map<std::string, event_id> m_events_ids_map;


    //  std::map<agent_group_id, std::string> m_inverse_agent_group_ids;


    /** \brief Private constructor since it is a Singleton class.*/
    kem_store();
public:
    /**Function that populates the list m_created_kems with the kevent model given as input in \ref file*/
    void generate(const std::string & file);

    /** \brief To get always (the same instance of) *this* and the same instantiated fields.*/
    static kem_store& get_instance();
    //    /** \brief Function that inserts a \ref kevent given its id to \ref m_created_kems.
    //     * @param[in] to_add: The kevent model that represent the graph of an action.
    //     * @parma[in] id: The id associated to \ref to_add.
    //     */
    //    void add_action_kem(const kevent & to_add, event_id id);
    //
    //    /** \brief Function that inserts a \ref kevent given its id to \ref m_created_kems.
    //     * @param[in] to_add: The kevent model (pointer) that represent the graph of an action.
    //     * @parma[in] id: The id associated to \ref to_add.
    //     */
    //    void add_action_kem(const kevent_ptr & to_add, event_id id);
    //
    //    /** \brief Function that retrieves a \ref kevent from its \ref event_id.
    //     * @parma[in] id: The id of the \ref kevent that needs to be returned.
    //     * @return: The kevent model that represent the graph of the action associated to \ref id.
    //     */
    //    const kevent_ptr & get_action_kem(event_id id);


    /** \brief Function that return the pointer to the given \ref kevent.
     * 
     * If the \ref kevent didn't exist then it is inserted to \ref m_created_events and then the pointer is returned;
     * if it existed a pointer to the already existing version is returned.
     * 
     * @param[in] to_add: the \ref kevent to add to the collection of created events.
     * 
     * @return the \ref kevent_ptr to \p to_add.*/
    const kevent_ptr add_event(const kevent & to_add);
    /** \brief Function that return the pointer to the given \ref kem.
     *
     * If the \ref kem didn't exist then it is inserted to \ref m_created_kems and then the pointer is returned;
     * if it existed a pointer to the already existing version is returned.
     *
     * @param[in] to_add: the \ref kem to add to the collection of created kems.
     *
     * @return the \ref kem_ptr to \p to_add.*/
    const kem_ptr add_kem(const kem & to_add);


    /** \brief Function that add a new agent group name to the map name->id.
     *
     * @return the \ref string to \p to_add in m_agent_group_ids.*/
    void add_agent_group(const std::string & to_add);


    /** \brief Function that return the pointer to the given \ref kevent from its id.
     * 
     * @return the \ref kevent_ptr to that has ref \id id.*/
    const kevent_ptr get_event(event_id id) const;

    /** \brief Function that return the pointer to the given \ref model from its id.
     * 
     * @return the \ref kem_ptr to that has ref \id id.*/
    const kem_ptr get_kem(kem_id id) const;



    /** \brief Function that returns the id given an agent group name.
     *
     * @return the \ref id of \p to_get.*/
    agent_group_id get_agent_group(const std::string & to_get) const;

    std::string get_agent_group_name(agent_group_id id) const;

    kem_id get_kem_id(const std::string & to_get);
    bool exist_kem(kem_id to_check);

    std::string get_kem_name(kem_id id) const;

    event_id get_event_id(const std::string & to_get);


    short get_agent_group_number() const;


    /** \brief Copy constructor removed since is Singleton class. */
    kem_store(kem_store const&) = delete;
    /** \brief Copy operator removed since Singleton class. */
    void operator=(kem_store const&) = delete;

};

