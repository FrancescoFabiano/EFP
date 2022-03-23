/**
 * \brief Class that represents a Kripke-based Event Model (CEM).
 *
 * \details  A CEM is a representation for actions in epistemic planning
 *
 * @see action and cevent.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date June 24, 2021
 */

#pragma once

#include "../../../include/definitions/delphic.h"

class cem
{
    friend class cem_ptr;
private:
    /** \brief The id of *this*.*/
    cem_id m_id;
    /** \brief The map containing the edges of the action.
     *
     * Each edge consists of <\ref agent, \ref event_ptr_set> and link each \ref agent to a set of events.
     *
     * @see cem_store and agent.*/
    cem_edges m_edges;

    /** \brief The id of the pointed cevent of *this*.*/
    event_id m_pointed_id;



    /** \brief Getter of \ref m_id.
     *
     * Only accessible by the \ref cem_ptr.
     *
     * @return the \ref cem_id of *this*.*/
    cem_id get_id() const;


    /** \brief Getter of \ref m_pointed_id.
     *
     * Only accessible by the \ref cem_ptr.
     *
     * @return the \ref m_pointed_id of *this*.*/
    const event_id get_pointed_id() const;

    /** \brief Getter of \ref m_information_state.
     *
     * Only accessible by the \ref cem_ptr.
     *
     * @return the map of edges of *this*.*/
    const cem_edges & get_edges() const;

public:
    /** \brief Empty constructor, call the default constructor of all the fields.*/
    cem();
    /** \brief Constructor with parameters.
     * Construct an object with the given info.
     *
     * @param[in] id: the \ref id to set as \ref m_id.*/
    cem(cem_id id);

    /** \brief Copy constructor.
     *
     * @param[in] to_copy: the \ref cem to copy into *this*.*/
    cem(const cem & to_copy);

    /** \brief Setter for the field \ref m_id.
     *
     * Only accessible by the \ref cem_ptr.
     *
     * @param[in] to_set: the \ref cem_id to set as \ref m_id.*/
    void set_id(cem_id to_set);


    /** \brief Setter for the field \ref m_pointed_id.
     *
     * Only accessible by the \ref cem_ptr.
     *
     * @param[in] to_set: the \ref event_id to set as \ref m_pointed_id.*/
    void set_pointed_id(event_id to_set);


    /** \brief Setter for the field \ref m_information_state.
     *
     * Only accessible by the \ref cem_ptr.
     *
     * @param[in] to_set: the map of \ref cem_edges to set as \ref m_information_state.*/
    void set_edges(const cem_edges & to_set);

    /**
     *\brief The < operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref cem (used in \ref cem_store).
     *
     * @see cem_store.
     *
     * @param [in] to_compare: the \ref cem to compare with *this*.
     * @return true: if \p to_compare is smaller than *this*
     * @return false: otherwise.*/
    bool operator<(const cem & to_compare) const;
    /**
     *\brief The > operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref cem (used in \ref cem_store).
     *
     * @see cem_store.
     *
     * @param [in] to_compare: the \ref cem to compare with *this*.
     * @return true: if \p to_compare is bigger than *this*
     * @return false: otherwise.*/
    bool operator>(const cem & to_compare) const;
    /**
     *\brief The == operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref cem (used in \ref cem_store).
     *
     * @see cem_store.
     *
     * @param [in] to_compare: the \ref cem to compare with *this*.
     * @return true: if \p to_compare is equal to *this*
     * @return false: otherwise.*/
    bool operator==(const cem & to_compare) const;
    /** \brief The copy operator.
     *
     * @param [in] to_assign: the \ref cem to assign to *this*.
     * @return true: if \p the assignment went ok.
     * @return false: otherwise.*/
    bool operator=(const cem & to_copy);

    void print()const;
};

/**
 * \class cem_ptr
 * 
 * \brief A wrapper class for a std::shared_pointer to a \ref cem usually stored in \ref cem_store.
 * 
 * This class is necessary so we can use the < operator in the set of \ref cem_ptr.
 * If we don't use the wrapper pointers to the same elements are different.
 *
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date June 24, 2021
 */
class cem_ptr
{
private:
    /**\brief the pointer that is wrapped by *this*.*/
    std::shared_ptr<cem> m_ptr;

public:
    /**\brief Constructor without parameters.*/
    cem_ptr();

    /**\brief Constructor with parameters.
     *
     * This constructor uses const pointer, this means that the pointer is copied
     * and the counter of the shared pointer is increased (std implementation).
     * 
     * @param[in] ptr: the pointer to assign to \ref m_ptr.
     */
    cem_ptr(const std::shared_ptr<cem> &ptr);

    /**\brief Constructor with parameters.
     *
     * This constructor uses non-const pointer, this means that the pointer is copied
     * in \ref m_ptr and \p ptr becomes empty (std implementation).
     *  
     * @param[in] ptr: the pointer to assign to \ref m_ptr.
     */
    cem_ptr(std::shared_ptr<cem> &&ptr);

    /** \brief Constructor with parameters.
     *
     * This constructor build a pointer to the given parameter.
     *  
     * @param[in] action: the \ref cem that *this* (\ref m_ptr) should point.*/
    cem_ptr(const cem &action);

    /**\brief Setter for the field \ref m_ptr.
     *
     * This setter uses const parameter, this means that the pointer is copied
     * and the counter of the shared pointer is increased (std implementation).
     * 
     * @param[in] ptr: the pointer to assign to \ref m_ptr.*/
    void set_ptr(const std::shared_ptr<cem> &ptr);

    /**\brief Setter for the field \ref m_ptr (move constructor).
     * 
     * This setter uses non-const parameter, this means that the pointer is copied
     * in \ref m_ptr and \p ptr becomes empty (std implementation).
     *  
     * @param[in] ptr: the pointer to assign to \ref m_ptr.*/
    void set_ptr(std::shared_ptr<cem> &&ptr);

    /**\brief Getter for the field \ref m_ptr.
     *
     * @return a copy of the pointer \ref m_ptr.*/
    std::shared_ptr<cem> get_ptr() const;

    /**\brief Setter for the field \ref m_id of \ref m_ptr.
     *
     * @param[in] to_set: the \ref cem_id to assign to \ref m_id.*/
    void set_id(cem_id to_set);

    /** \brief Setter for the field \ref m_pointed_id.
     *
     *
     * @param[in] to_set: the \ref event_id to set as \ref m_pointed_id.*/
    void set_pointed_id(event_id to_set);

    /**\brief Setter for the field \ref m_information_state of \ref m_ptr.
     *
     * @param[in] to_set: the \ref cem_edges to assign to \ref m_information_state.*/
    void set_edges(const cem_edges & to_set);

    /** \brief Function that return the field m_id \ref m_ptr.
     *
     * @return the \ref m_id of the \ref cem pointed by \ref m_ptr.*/
    cem_id get_id() const;

    /** \brief Getter of \ref m_pointed_id.
     *
     *
     * @return the \ref cem_id of *this*.*/
    const event_id get_pointed_id() const;

    /** \brief Function that return the field m_information_state of the pointed \ref cevent.
     *
     * @return the \ref m_information_state of the \ref cevent pointed by \ref m_ptr.*/
    const cem_edges& get_edges() const;

    /**
     *\brief The < operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref cem_ptr (used in \ref cem_store).
     *
     * @see cem_store.
     *
     * @param [in] to_compare: the \ref cem_ptr to compare with *this*.
     * @return true: if \p to_compare is smaller than *this*
     * @return false: otherwise.*/
    bool operator<(const cem_ptr & to_compare) const;
    /**
     *\brief The > operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref cem_ptr (used in \ref cem_store).
     *
     * @see cem_store.
     *
     * @param [in] to_compare: the \ref cem_ptr to compare with *this*.
     * @return true: if \p to_compare is bigger than *this*
     * @return false: otherwise.*/
    bool operator>(const cem_ptr & to_compare) const;
    /**
     *\brief The == operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref cem_ptr (used in \ref cem_store).
     *
     * @see cem_store.
     *
     * @param [in] to_compare: the \ref cem_ptr to compare with *this*.
     * @return true: if \p to_compare is equal to *this*
     * @return false: otherwise.*/
    bool operator==(const cem_ptr & to_compare) const;
    /** \brief The copy operator.
     *
     * @param [in] to_assign: the \ref cem_ptr to assign to *this*.
     * @return true: if \p the assignment went ok.
     * @return false: otherwise.*/
    bool operator=(const cem_ptr & to_copy);

    void print()const;

};
