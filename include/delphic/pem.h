/**
 * \brief Class that represents a Possibility-based Event Model (PEM).
 *
 * \details  A PEM is a representation for actions in epistemic planning
 *
 * @see action and pstate.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 28, 2021
 */
#pragma once

#include "../utilities/define.h"
#include "../formulae/belief_formula.h"

class pem
{
    private:
    /** \brief The type of *this*.*/
    event_type m_type;
    /** \brief The precondition of *this*.*/
    belief_formula m_pre;
    /** \brief The map that associates a belief formula (postcondition) to each \ref fluent.*/
    postconditions m_post;
    /** \brief The map containing the edges of the action.
     *
     * Each edge consists of <\ref agent, \ref pem_ptr_set> and link each \ref agent to a set of pems.
     *
     * @see pem_store and agent.*/
    pem_edges m_edges;

    /** \brief Setter for the field \ref m_event.
     *
     * Only accessible by the \ref pem_ptr.
     *
     * @param[in] type: the \ref event_type to set as \ref m_type.*/
    void set_type(const event_type type);
    /** \brief Setter for the field \ref m_pre.
     *
     * Only accessible by the \ref pem_ptr.
     *
     * @param[in] pre: the \ref belief_formula to set as \ref m_pre.*/
    void set_precondition(const belief_formula & pre);
    /** \brief Setter for the field \ref m_post.
     *
     * Only accessible by the \ref pem_ptr.
     *
     * @param[in] post: the \ref postconditions to set as \ref m_post.*/
    void set_postconditions(const postconditions & post);
    /** \brief Setter for the field \ref m_edges.
     *
     * Only accessible by the \ref pem_ptr.
     *
     * @param[in] edges: the map of \ref edges to set as \ref m_edges.*/
    void set_edges(const pem_edges & edges);

    /** \brief Getter of \ref m_type.
     *
     * Only accessible by the \ref pem_ptr.
     *
     * @return the \ref event_type of *this*.*/
    const event_type get_type() const;
    /** \brief Getter of \ref m_pre.
     *
     * Only accessible by the \ref pem_ptr.
     *
     * @return the precondition of *this*.*/
    void get_precondition();
    /** \brief Getter of \ref m_post.
     *
     * Only accessible by the \ref pem_ptr.
     *
     * @return the postconditions of *this*.*/
    void get_postconditions();
    /** \brief Getter of \ref m_edges.
     *
     * Only accessible by the \ref pem_ptr.
     *
     * @return the map of edges of *this*.*/
    const pem_edges& get_edges() const;

    public:
    /** \brief Empty constructor, call the default constructor of all the fields.*/
    pem();
    /** \brief Constructor with parameters.
     * Construct an object with the given info.
     *
     * @param[in] type: the \ref event_type to set as \ref m_type.
     * @param[in] pre: the \ref belief_formula to set as \ref m_pre.
     * @param[in] post: the \ref postconditions to set as \ref m_post.
     * @param[in] edges: the map of the edges of *this*.*/
    pem(const event_type type, const belief_formula & pre, const postconditions & post, const pedges_opt & edges);
    /** \brief Copy constructor.
     *
     * @param[in] action: the \ref pem to copy into *this*.*/
    pem(const pem & action);
};

/**
 * \class pem_ptr
 * 
 * \brief A wrapper class for a std::shared_pointer to a \ref pem usually stored in \ref pem_store.
 * 
 * This class is necessary so we can use the < operator in the set of \ref pem_ptr.
 * If we don't use the wrapper pointers to the same elements are different.
 *
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 28, 2021
 */
class pem_ptr {
private:
    /**\brief the pointer that is wrapped by *this*.*/
    std::shared_ptr <pem> m_ptr;

public:
    /**\brief Constructor without parameters.*/
    pem_ptr();

    /**\brief Constructor with parameters.
     *
     * This constructor uses const pointer, this means that the pointer is copied
     * and the counter of the shared pointer is increased (std implementation).
     * 
     * @param[in] ptr: the pointer to assign to \ref m_ptr.
     */
    pem_ptr(const std::shared_ptr <pem> &ptr);

    /**\brief Constructor with parameters.
     *
     * This constructor uses non-const pointer, this means that the pointer is copied
     * in \ref m_ptr and \p ptr becomes empty (std implementation).
     *  
     * @param[in] ptr: the pointer to assign to \ref m_ptr.
     */
    pem_ptr(std::shared_ptr <pem> &&ptr);

    /** \brief Constructor with parameters.
     *
     * This constructor build a pointer to the given parameter.
     *  
     * @param[in] state: the \ref pem that *this* (\ref m_ptr) should point.*/
    pem_ptr(const pem &state);

    /**\brief Setter for the field \ref m_ptr.
     *
     * This setter uses const parameter, this means that the pointer is copied
     * and the counter of the shared pointer is increased (std implementation).
     * 
     * @param[in] ptr: the pointer to assign to \ref m_ptr.*/
    void set_ptr(const std::shared_ptr <pem> &ptr);

    /**\brief Setter for the field \ref m_ptr (move constructor).
     * 
     * This setter uses non-const parameter, this means that the pointer is copied
     * in \ref m_ptr and \p ptr becomes empty (std implementation).
     *  
     * @param[in] ptr: the pointer to assign to \ref m_ptr.*/
    void set_ptr(std::shared_ptr <pem> &&ptr);

    /**\brief Getter for the field \ref m_ptr.
     *
     * @return a copy of the pointer \ref m_ptr.*/
    std::shared_ptr <pem> get_ptr() const;

    /** \brief Function that return the field m_type of the pointed \ref pem.
     *
     * @return the \ref m_type of the \ref pem pointed by \ref m_ptr.*/
    const event_type get_type() const;
    /** \brief Function that return the field m_precondition of the pointed \ref pem.
     *
     * @return the \ref m_precondition of the \ref pem pointed by \ref m_ptr.*/
    void get_precondition();
    /** \brief Function that return the field m_postconditions of the pointed \ref pem.
     *
     * @return the \ref m_postconditions of the \ref pem pointed by \ref m_ptr.*/
    void get_postconditions();
    /** \brief Function that return the field m_edges of the pointed \ref pem.
     *
     * @return the \ref m_edges of the \ref pem pointed by \ref m_ptr.*/
    const pem_edges& get_edges() const;
};