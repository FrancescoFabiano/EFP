/**
 * \class pworld
 * 
 * \brief Class that describes a possible interpretation of the world and of the agents' beliefs.
 *
 * \details   A pworld represent a consistent set of \ref fluent (AKA a \ref fluent_set), alongside
 *            with an information set which is the set of the possibilities that the agents consider true.
 * 
 * @see pstate, and pstore.
 *
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date September 14, 2019
 */
#pragma once

#include <set>

#include "../../../include/definitions/possibilities.h"
#include "../../utilities/printer.h"
#include "../../actions/custom_event_models/cevent.h"

class pworld {
private:
    /** \brief The set of  \ref fluent that describes how these are interpreted in *this*.*/
    const fluent_ptr_set *m_fluent_set;

    const cevent *m_event;

    /** \brief The unique id of *this*.*/
    const unsigned long m_id;

public:
    pworld();
    /** \brief Constructor with parameters.
     *Construct an object with the given info and then set the unique id.
     * the set of \ref fluent to set as \ref m_fluent_set.
     *
     * @param[in] description: the set of \ref fluent to set as \ref m_fluent_set.
     *
     * \todo is the parameter passing the best one? Copy?*/
    pworld(const fluent_ptr_set *fluent_set, const cevent *event, const unsigned long id = 0);


    /** \brief Getter of \ref m_fluent_set.
     *
     * Only accessible by the \ref pworld_ptr.
     *
     * @return the \ref pworld_ptr to the world where *this* is from.*/
    const fluent_ptr_set * get_fluent_set() const;

    /** \brief Getter of \ref m_id.
     *
     * Only accessible by the \ref pworld_ptr.
     *
     * @return the int that is the unique id of *this*.*/
    unsigned long get_id() const;

    /**
     *\brief The < operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref pworld (used in \ref kstore).
     * 
     * @see kstore.
     *     
     * @param [in] to_compare: the \ref pworld to compare with *this*.
     * @return true: if \p to_compare is smaller than *this*
     * @return false: otherwise.*/
    bool operator<(const pworld& to_compare) const;

    /**
     *\brief The > operator based on the field \ref m_id. 
     * @see kstore.
     *     
     * @param [in] to_compare: the \ref pworld to compare with *this*.
     * @return true: if \p to_compare is bigger than *this*
     * @return false: otherwise.*/
    bool operator>(const pworld& to_compare) const;

    /** \brief The == operator based on the field \ref m_id.
     *     
     * @param [in] to_compare: the \ref pworld to compare with *this*.
     * @return true: if \p to_compare is equal to *this*
     * @return false: otherwise.*/
    bool operator==(const pworld& to_compare) const;

    pworld &operator=(const pworld &world);

    /** \brief Function used to print all the information of *this*.*/
    void print() const;
};

/**
 * \class pworld_ptr
 * 
 * \brief A wrapper class for a std::shared_pointer to a \ref pworld usually stored in \ref pstore.
 * 
 * This class is necessary so we can use the < operator in the set of \ref pworld_ptr.
 * If we dom't use the wrapper pointers to the same elements are different.
 *
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date April 29, 2019
 */
//class pworld_ptr
//{
//private:
//    /**\brief the pointer that is wrapped by *this*.*/
//    std::shared_ptr<const pworld> m_ptr;
//
//    /** \brief The number of repetition of *this* in a \ref pstate due to oblivious obsv.*/
//    unsigned short m_repetition = 0;
//
//public:
//    /**\brief Constructor without parameters.*/
//    pworld_ptr();
//    /**\brief Constructor with parameters.
//     *
//     * This constructor uses const pointer, this means that the pointer is copied
//     * and the counter of the shared pointer is increased (std implementation).
//     * 
//     * @param[in] ptr: the pointer to assign to \ref m_ptr.
//     * @param[in] repetition: the value to give to \ref m_repetition, default to 0.
//     */
//    pworld_ptr(const std::shared_ptr<const pworld> & ptr, unsigned short repetition = 0);
//    /**\brief Constructor with parameters.
//     *
//     * This constructor uses non-const pointer, this means that the pointer is copied
//     * in \ref m_ptr and \p ptr becomes empty (std implementation).
//     *  
//     * @param[in] ptr: the pointer to assign to \ref m_ptr.
//     * @param[in] repetition: the value to give to \ref m_repetition, default to 0.
//     */
//    pworld_ptr(std::shared_ptr<const pworld>&& ptr, unsigned short repetition = 0);
//
//    /** \brief Constructor with parameters.
//     *
//     * This constructor build a pointer to the given parameter.
//     *  
//     * @param[in] world: the \ref pworld that *this* (\ref m_ptr) should point.
//     * @param[in] repetition: the value to give to \ref m_repetition, default to 0.*/
//    pworld_ptr(const pworld & world, unsigned short repetition = 0);
//
//    /**\brief Getter for the field \ref m_ptr.
//     *  
//     * @return a copy of the pointer \ref m_ptr.*/
//    std::shared_ptr<const pworld> get_ptr() const;
//
//    /**\brief Setter for the field \ref m_ptr.
//     *
//     * This setter uses const parameter, this means that the pointer is copied
//     * and the counter of the shared pointer is increased (std implementation).
//     * 
//     * @param[in] ptr: the pointer to assign to \ref m_ptr.*/
//    void set_ptr(const std::shared_ptr<const pworld> & ptr);
//    /**\brief Setter for the field \ref m_ptr (move constructor).
//     * 
//     * This setter uses non-const parameter, this means that the pointer is copied
//     * in \ref m_ptr and \p ptr becomes empty (std implementation).
//     *  
//     * @param[in] ptr: the pointer to assign to \ref m_ptr.*/
//    void set_ptr(std::shared_ptr<const pworld>&& ptr);
//
//    /** \brief Setter for the field \ref m_repetition.
//     * 
//     * @param[in] repetition: the value to give to \ref m_repetition.*/
//    void set_repetition(unsigned short repetition);
//
//    /** \brief Function that increases the value for the field \ref m_repetition.
//     * 
//     * @param[in] increase: the value to add to \ref m_repetition.*/
//    void increase_repetition(unsigned short increase);
//
//    /** \brief Getter of the field \ref m_repetition.
//     * 
//     * @return the value to of \ref m_repetition.*/
//    unsigned short get_repetition() const;
//
//    /** \brief Function that return the field m_fluent_set of the pointed \ref pworld.
//     *     
//     * @return the \ref fluent_set that is the description of the \ref pworld pointed by \ref m_ptr.*/
//    const fluent_set & get_fluent_set() const;
//
//    /** \brief Function that return the field m_id of the pointed \ref pworld + \ref m_repetition.
//     *     
//     * @return the \ref pworld_id that is the id of the \ref pworld pointed by \ref m_ptr + \ref m_repetition.*/
//    pworld_id get_id() const;
//
//    /** \brief Function that return the field m_id of \ref pworld.
//     *     
//     * @return the \ref pworld_id that is the id of the \ref pworld pointed by \ref m_ptr.*/
//    pworld_id get_numerical_id() const;
//
//
//    /** \brief Function that return the field m_id of the pointed \ref pworld.
//     *     
//     * @return the \ref pworld_id that is the id of the \ref pworld pointed by \ref m_ptr.*/
//    pworld_id get_fluent_based_id() const;
//
//    /** \brief Function that check the entailment of a single \ref fluent in \ref m_ptr.
//     * 
//     * 
//     * @param[in] to_check: the \ref fluent that has to be checked if entailed by \ref m_ptr (if is present in \ref m_ptr).
//     *
//     * @return true: \p to_check is entailed;
//     * @return false: \p -to_check is entailed.*/
//    bool entails(fluent to_check) const;
//    /**
//     *\brief Function that check the entailment of a conjunctive set of \ref fluent in \ref m_ptr.
//     * 
//     * @param[in] to_check: the conjunctive set of \ref fluent that has to be checked if entailed by \ref m_ptr.
//     *
//     * @return true: \p to_check is entailed;
//     * @return false: \p -to_check is entailed.*/
//    bool entails(const fluent_set& to_check) const;
//    /**
//     *\brief Function that check the entailment of a DNF \ref fluent_formula in \ref m_ptr.
//     * 
//     * @param[in] to_check: the DNF formula that has to be checked if entailed by \ref m_ptr.
//     *
//     * @return true: \p to_check is entailed;
//     * @return false: \p -to_check is entailed.*/
//    bool entails(const fluent_formula & to_check) const;
//
//    /**\brief The operator =.
//     *
//     * This operator assign the parameter without destroying \p to_copy.
//     * 
//     * @param[in] to_copy: the \ref pworld_ptr to assign to *this*.
//     * @return true: if the assignment went through.
//     * @return false: otherwise.*/
//    bool operator=(const pworld_ptr & to_copy);
//    /**\brief The operator < for set operation.
//     *
//     * The ordering is based on the pointed object and not on the pointer itself so we have one
//     * copy of each pointed object.
//     * 
//     * @param[in] to_compare: the \ref pworld_ptr to check for odering.
//     * 
//     * @return true: if *this* is smaller than \p to_compare.
//     * @return false: otherwise.*/
//
//    bool operator<(const pworld_ptr & to_compare) const;
//    /**\brief The operator > for \ref pstate < comparison.
//     *
//     * The ordering is based on the pointed object and not on the pointer itself so we have one
//     * copy of each pointed object.
//     * 
//     * @param[in] to_compare: the \ref pworld_ptr to check for odering.
//     * 
//     * @return true: if *this* is bigger than \p to_compare.
//     * @return false: otherwise.*/
//
//    bool operator>(const pworld_ptr & to_compare) const;
//    /**\brief The operator ==.
//     * 
//     * @param[in] to_compare: the \ref pworld_ptr to confront with *this*.
//     * @return true: if *this* is equal to \p to_compare.
//     * @return false: otherwise.*/
//    bool operator==(const pworld_ptr & to_compare) const;
//};
