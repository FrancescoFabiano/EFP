/**
 * \brief Class that describes an edge of a Kripke structure (\ref kstate).
 *
 * \details   An edge in a \ref kstate labeled with the \ref agent *ag* represent
 *            the information that the \ref agent *ag* cannot distinguish between the two worlds
 *            that the edge connects.
 * 
 *            Each label is provided with an id to save and retrieve them from \ref kstore.
 * 
 * @see kstate, kworld, and kstore.
 *
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date March 31, 2019
 */
#pragma once

#include "kworld.h"

#include "../../utilities/define.h"

class kedge
{
private:
    /**
     * \brief The \ref kworld_ptr to the world where *this* is from.
     */
    kworld_ptr m_from;
    /**
     * \brief The \ref kworld_ptr to the world where *this* is directed.
     */
    kworld_ptr m_to;
    /**
     * \brief The label of *this*, that is an \ref agent.
     */
    agent m_label;
    /**
     * \brief The unique id of *this* computed with \ref hash_info_into_id().
     */
    kedge_id m_edge_id;

    /**
     * \brief Function used to hash the the info of an edge in a unique id.
     *
     *
     *
     * @param[in] from: the world where the edge is coming from.
     * @param[in] to: the world where the edge is going to.
     * @param[in] label: the \ref agent that is the label of the edge.
     * @return the unique id of the edge.
     * 
     * @warning Useless if not moved to \ref kstore.
     */
    kedge_id hash_info_into_id(kworld_ptr from, kworld_ptr to, agent label);

    /**
     * \brief Function used to hash the the info of *this* in a unique id.
     *
     * @return the unique id of *this*.
     */
    kedge_id hash_info_into_id();

    /** \brief Setter for the field \ref m_from.
     * 
     * It takes a world and set its \ref kworld_ptr, it is private
     * because we want to do it only in the constructor.
     *
     * @param[in] from: the \ref kworld to set as \ref m_from.
     * 
     * \todo is the parameter passing the best one?*/
    void set_from(const kworld & from);
    /** \brief Setter for the field \ref m_to.
     * 
     * It takes a world and set its \ref kworld_ptr, it is private
     * because we want to do it only in the constructor.
     *
     * @param[in] to: the \ref kworld to set as \ref m_to.
     * 
     * \todo is the parameter passing the best one?*/
    void set_to(const kworld & to);


    /** \brief Setter for the field \ref m_from.
     * 
     * It is private because we want to do it only in the constructor.
     *
     * @param[in] from: the \ref kworld_ptr to set as \ref m_from.*/
    void set_from(kworld_ptr from);
    /** \brief Setter for the field \ref m_to.
     * 
     * It is private because we want to do it only in the constructor.
     *
     * @param[in] to: the \ref kworld_ptr to set as \ref m_to.*/
    void set_to(kworld_ptr & to);

    /**
     * \brief Setter for the field \ref m_label.
     * 
     * It is private because we want to do it only in the constructor,
     *
     * @param[in] ag: the \ref agent to set as \ref m_label.
     * 
     */
    void set_label(agent ag);

    /**
     * \brief Function that uses the info of  *this* to set its  \ref m_edge_id.
     */
    void set_id();

public:
    /**
     * \brief Empty constructor, call the default constructor of all the fields.
     */
    kedge();
    /** \brief Constructor with parameters.
     * 
     * Construct an object with the given info and then set the unique id.
     * @param[in] from: the world where *this* is coming from.
     * @param[in] to: the world where *this* is going to.
     * @param[in] label: the \ref agent that is the label of *this*.*/
    kedge(const kworld & from, const kworld & to, agent label);


    /** \brief Constructor with parameters.
     * 
     * Construct an object with the given info and then set the unique id.
     * @param[in] from: the pointer to the world where *this* is coming from.
     * @param[in] to: the pointer to the world where *this* is going to.
     * @param[in] label: the \ref agent that is the label of *this*.*/
    kedge(kworld_ptr from, kworld_ptr to, agent label);

    /**
     *\brief Getter of \ref m_from.
     *     
     * @return the \ref kworld_ptr to the world where *this* is from.*/
    kworld_ptr get_from() const;
    /**
     *\brief Getter of \ref m_to.
     *     
     * @return the \ref kworld_ptr to the world where *this* is directed.*/
    kworld_ptr get_to() const;
    /**
     *\brief Getter of \ref m_label.
     *     
     * @return the \ref agent that is the label of the *this*.*/
    agent get_label() const;
    /**
     *\brief Getter of \ref m_edge_id.
     *     
     * @return the int that is the unique id of *this*.*/
    kedge_id get_id() const;
    /**
     *\brief The < operator based on the field \ref m_edge_id.
     * Implemented to allow the ordering on set of \ref kedge (used in \ref kstore).
     * 
     * @see kstore.
     *     
     * @param [in] to_compare: the \ref kedge to compare with *this*.
     * @return true: if \p to_compare is smaller than *this*
     * @return false: otherwise.*/
    bool operator<(const kedge& to_compare) const;


    /**
     * \brief Function used to print all the information of *this*.
     */
    void print();
};