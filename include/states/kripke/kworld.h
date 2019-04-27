/**
 * \brief Class that describes a world (node) of a Kripke structure (\ref kstate).
 *
 * \details   A world in a \ref kstate represent a consistent set of \ref fluent (AKA a \ref fluent_set);
 *            that is a possible configuration of the real world (only based on the \ref fluent).
 * 
 * @see kstate, kedge, and kstore.
 *
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date March 31, 2019
 */
#pragma once

#include <set>

#include "kedge.h"

#include "../../utilities/define.h"
#include "../../domain/grounder.h"

class kworld
{
private:
    /**
     * \brief The set of  \ref fluent that describes how these are interpreted in *this*.
     */
    fluent_set m_fluent_set;
    /**
     * \brief The unique id of *this* computed with \ref hash_fluents_into_id().
     */
    kworld_id m_id;

    /**
     * \brief Function used to hash the the info of an edge in a unique id.
     *
     *
     *
     * @param[in] description: the interpretation of the \ref fluent in the world.
     * @return the unique id of the world.
     * 
     * @warning Useless if not moved to \ref kstore.
     */
    kworld_id hash_fluents_into_id(const fluent_set& description);

    /**
     * \brief Function used to hash the the info of *this* in a unique id.
     *
     * @return the unique id of *this*.
     */
    kworld_id hash_fluents_into_id();

    /**
     * \brief Setter for the field \ref m_fluent_set.
     * 
     * This function also guarantees consistency.
     * 
     * @param[in] description: the set of \ref fluent to set as \ref m_fluent_set.
     * 
     * \todo is the parameter passing the best one? Copy?
     * \todo is the consistency useless?
     */
    void set_fluent_set(const fluent_set & description);

    /**
     * \brief Function that checks if the given \ref fluent_set is consistent.
     * 
     * @param[in] to_check: the set of \ref fluent to check.
     * 
     * @return true if \p to_check is consistent.
     * @return false if \p to_check is not consistent.
     * 
     * \todo Add \ref static_law as static class and use it to check.
     */
    bool consistent(const fluent_set & to_check);

    /**
     * \brief Function that uses the info of  *this* to set its  \ref m_id.
     */
    void set_id();

public:
    /**
     * \brief Empty constructor, call the default constructor of all the fields.
     */
    kworld();

    /** \brief Constructor with parameters.
     *Construct an object with the given info and then set the unique id.
     * the set of \ref fluent to set as \ref m_fluent_set.
     * 
     * @param[in] description: the set of \ref fluent to set as \ref m_fluent_set.
     * 
     * \todo is the parameter passing the best one? Copy?
     */
    kworld(const fluent_set & description);

    /**
     *\brief Getter of \ref m_fluent_set.
     *     
     * @return the \ref kworld_ptr to the world where *this* is from.*/
    const fluent_set & get_fluent_set() const;
    /**
     *\brief Getter of \ref m_id.
     *     
     * @return the int that is the unique id of *this*.*/
    kworld_id get_id() const;

    /**
     *\brief Function that check the entailment of a single \ref fluent in *this*.
     * 
     * A single \ref fluent is entailed if is present in *this*. Given that the each kworld
     * should only admit consistent set of \ref fluent if the \ref fluent itself is presented in *this*.
     * 
     * @param[in] to_check: the \ref fluent that has to be checked if entailed by *this* (if is present in *this*).
     *
     * @return true: \p to_check is entailed;
     * @return false: \p -to_check is entailed.
     * 
     * \todo To implement also whit \ref kworld_ptr to the \ref kworld?
     * \todo check consistency on constructor?
     */
    bool entails(fluent to_check) const;
    /**
     *\brief Function that check the entailment of a conjunctive set of \ref fluent in *this*.
     * 
     * This is checked by calling recursively \ref entails(fluent) const on all the element
     * of \p to_check and returning true only if all the elements are entailed (conjunctive set of \ref fluent).
     *  
     * @param[in] to_check: the conjunctive set of \ref fluent that has to be checked if entailed by *this*.
     * 
     *
     * @return true: \p to_check is entailed;
     * @return false: \p -to_check is entailed.*/
    bool entails(const fluent_set& to_check) const;
    /**
     *\brief Function that check the entailment of a DNF \ref fluent_formula in *this*.
     * 
     * This is checked by calling recursively \ref entails(const fluent_set &) const on all the element
     * of \p to_check and returning true only if at least one element is entailed (DNF).
     *  
     * @param[in] to_check: the DNF formula that has to be checked if entailed by *this*.
     * 
     *
     * @return true: \p to_check is entailed;
     * @return false: \p -to_check is entailed.*/
    bool entails(const fluent_formula & to_check) const;

    /**
     *\brief The < operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref kworld (used in \ref kstore).
     * 
     * @see kstore.
     *     
     * @param [in] to_compare: the \ref kworld to compare with *this*.
     * @return true: if \p to_compare is smaller than *this*
     * @return false: otherwise.*/
    bool operator<(const kworld& to_compare) const;

    /**
     * \brief Function used to print all the information of *this*.
     * 
     * @param [in] gr: the \ref grounder to retrive the std::string associated with the \ref fluent.
     */
    void print_info(const grounder& gr);


};