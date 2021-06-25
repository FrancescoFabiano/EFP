/**
 * \brief Class that represents a Kripke-based Event Model (KEM).
 *
 * \details  A KEM is a representation for actions in epistemic planning
 *
 * @see action and kevent.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date June 24, 2021
 */

#pragma once

#include "../../utilities/define.h"

class kem
{
    friend class kem_ptr;
private:
    /** \brief The id of *this*.*/
    kem_id m_id;
    /** \brief The map containing the edges of the action.
     *
     * Each edge consists of <\ref agent, \ref event_ptr_set> and link each \ref agent to a set of events.
     *
     * @see kem_store and agent.*/
    kem_edges m_edges;

    /** \brief The id of the pointed pevent of *this*.*/
    event_id m_pointed_id;



    /** \brief Getter of \ref m_id.
     *
     * Only accessible by the \ref kem_ptr.
     *
     * @return the \ref kem_id of *this*.*/
    kem_id get_id() const;


    /** \brief Getter of \ref m_pointed_id.
     *
     * Only accessible by the \ref kem_ptr.
     *
     * @return the \ref m_pointed_id of *this*.*/
    event_id get_pointed_id() const;

    /** \brief Getter of \ref m_information_state.
     *
     * Only accessible by the \ref kem_ptr.
     *
     * @return the map of edges of *this*.*/
    const kem_edges & get_edges() const;

public:
    /** \brief Empty constructor, call the default constructor of all the fields.*/
    kem();
    /** \brief Constructor with parameters.
     * Construct an object with the given info.
     *
     * @param[in] id: the \ref id to set as \ref m_id.*/
    kem(kem_id id);

    /** \brief Copy constructor.
     *
     * @param[in] to_copy: the \ref kem to copy into *this*.*/
    kem(const kem & to_copy);

    /** \brief Setter for the field \ref m_id.
     *
     * Only accessible by the \ref kem_ptr.
     *
     * @param[in] to_set: the \ref kem_id to set as \ref m_id.*/
    void set_id(kem_id to_set);


    /** \brief Setter for the field \ref m_pointed_id.
     *
     * Only accessible by the \ref kem_ptr.
     *
     * @param[in] to_set: the \ref event_id to set as \ref m_pointed_id.*/
    void set_pointed_id(event_id to_set);


    /** \brief Setter for the field \ref m_information_state.
     *
     * Only accessible by the \ref kem_ptr.
     *
     * @param[in] to_set: the map of \ref kem_edges to set as \ref m_information_state.*/
    void set_edges(const kem_edges & to_set);

    /**
     *\brief The < operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref kem (used in \ref kem_store).
     *
     * @see kem_store.
     *
     * @param [in] to_compare: the \ref kem to compare with *this*.
     * @return true: if \p to_compare is smaller than *this*
     * @return false: otherwise.*/
    bool operator<(const kem & to_compare) const;
    /**
     *\brief The > operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref kem (used in \ref kem_store).
     *
     * @see kem_store.
     *
     * @param [in] to_compare: the \ref kem to compare with *this*.
     * @return true: if \p to_compare is bigger than *this*
     * @return false: otherwise.*/
    bool operator>(const kem & to_compare) const;
    /**
     *\brief The == operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref kem (used in \ref kem_store).
     *
     * @see kem_store.
     *
     * @param [in] to_compare: the \ref kem to compare with *this*.
     * @return true: if \p to_compare is equal to *this*
     * @return false: otherwise.*/
    bool operator==(const kem & to_compare) const;
    /** \brief The copy operator.
     *
     * @param [in] to_assign: the \ref kem to assign to *this*.
     * @return true: if \p the assignment went ok.
     * @return false: otherwise.*/
    bool operator=(const kem & to_copy);

    void print()const;
};

/**
 * \class kem_ptr
 * 
 * \brief A wrapper class for a std::shared_pointer to a \ref kem usually stored in \ref kem_store.
 * 
 * This class is necessary so we can use the < operator in the set of \ref kem_ptr.
 * If we don't use the wrapper pointers to the same elements are different.
 *
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date June 24, 2021
 */
class kem_ptr
{
private:
    /**\brief the pointer that is wrapped by *this*.*/
    std::shared_ptr<kem> m_ptr;

public:
    /**\brief Constructor without parameters.*/
    kem_ptr();

    /**\brief Constructor with parameters.
     *
     * This constructor uses const pointer, this means that the pointer is copied
     * and the counter of the shared pointer is increased (std implementation).
     * 
     * @param[in] ptr: the pointer to assign to \ref m_ptr.
     */
    kem_ptr(const std::shared_ptr<kem> &ptr);

    /**\brief Constructor with parameters.
     *
     * This constructor uses non-const pointer, this means that the pointer is copied
     * in \ref m_ptr and \p ptr becomes empty (std implementation).
     *  
     * @param[in] ptr: the pointer to assign to \ref m_ptr.
     */
    kem_ptr(std::shared_ptr<kem> &&ptr);

    /** \brief Constructor with parameters.
     *
     * This constructor build a pointer to the given parameter.
     *  
     * @param[in] action: the \ref kem that *this* (\ref m_ptr) should point.*/
    kem_ptr(const kem &action);

    /**\brief Setter for the field \ref m_ptr.
     *
     * This setter uses const parameter, this means that the pointer is copied
     * and the counter of the shared pointer is increased (std implementation).
     * 
     * @param[in] ptr: the pointer to assign to \ref m_ptr.*/
    void set_ptr(const std::shared_ptr<kem> &ptr);

    /**\brief Setter for the field \ref m_ptr (move constructor).
     * 
     * This setter uses non-const parameter, this means that the pointer is copied
     * in \ref m_ptr and \p ptr becomes empty (std implementation).
     *  
     * @param[in] ptr: the pointer to assign to \ref m_ptr.*/
    void set_ptr(std::shared_ptr<kem> &&ptr);

    /**\brief Getter for the field \ref m_ptr.
     *
     * @return a copy of the pointer \ref m_ptr.*/
    std::shared_ptr<kem> get_ptr() const;

    /**\brief Setter for the field \ref m_id of \ref m_ptr.
     *
     * @param[in] to_set: the \ref kem_id to assign to \ref m_id.*/
    void set_id(kem_id to_set);

    /** \brief Setter for the field \ref m_pointed_id.
     *
     *
     * @param[in] to_set: the \ref event_id to set as \ref m_pointed_id.*/
    void set_pointed_id(event_id to_set);

    /**\brief Setter for the field \ref m_information_state of \ref m_ptr.
     *
     * @param[in] to_set: the \ref kem_edges to assign to \ref m_information_state.*/
    void set_edges(const kem_edges & to_set);

    /** \brief Function that return the field m_id \ref m_ptr.
     *
     * @return the \ref m_id of the \ref kem pointed by \ref m_ptr.*/
    kem_id get_id() const;

    /** \brief Getter of \ref m_pointed_id.
     *
     *
     * @return the \ref kem_id of *this*.*/
    event_id get_pointed_id() const;

    /** \brief Function that return the field m_information_state of the pointed \ref pevent.
     *
     * @return the \ref m_information_state of the \ref pevent pointed by \ref m_ptr.*/
    const kem_edges& get_edges() const;

    /**
     *\brief The < operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref kem_ptr (used in \ref kem_store).
     *
     * @see kem_store.
     *
     * @param [in] to_compare: the \ref kem_ptr to compare with *this*.
     * @return true: if \p to_compare is smaller than *this*
     * @return false: otherwise.*/
    bool operator<(const kem_ptr & to_compare) const;
    /**
     *\brief The > operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref kem_ptr (used in \ref kem_store).
     *
     * @see kem_store.
     *
     * @param [in] to_compare: the \ref kem_ptr to compare with *this*.
     * @return true: if \p to_compare is bigger than *this*
     * @return false: otherwise.*/
    bool operator>(const kem_ptr & to_compare) const;
    /**
     *\brief The == operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref kem_ptr (used in \ref kem_store).
     *
     * @see kem_store.
     *
     * @param [in] to_compare: the \ref kem_ptr to compare with *this*.
     * @return true: if \p to_compare is equal to *this*
     * @return false: otherwise.*/
    bool operator==(const kem_ptr & to_compare) const;
    /** \brief The copy operator.
     *
     * @param [in] to_assign: the \ref kem_ptr to assign to *this*.
     * @return true: if \p the assignment went ok.
     * @return false: otherwise.*/
    bool operator=(const kem_ptr & to_copy);

    void print()const;

};
