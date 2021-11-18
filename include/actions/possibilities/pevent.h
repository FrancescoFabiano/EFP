/**
 * \brief Class that represents an pevent of a Possibility-based Event Model (PEM).
 *
 * \details A PEM is a representation for actions in epistemic planning
 *
 * @see action and pem.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 28, 2021
 */
#pragma once

#include "../../utilities/define.h"
#include "../../formulae/belief_formula.h"
#include "../action.h"

class pevent
{
    friend class pevent_ptr;

private:
    /** \brief The id of *this*.*/
    event_id m_id;

    /** \brief The edges of *this*.*/
    event_information_state m_information_state;

    /** \brief The 'meta' precondition of *this*.*/
    event_metacond m_meta_pre;
    /** \brief The 'meta' postcondition of *this*.*/
    event_metacond m_meta_post;

    /** \brief Flag that is true iff *this* has postconditions.*/
    bool m_ontic_change;


    /** \brief Setter for the field \ref m_ontic_change looking at the postconditions.*/
    void set_ontic_change();

    /** \brief Setter for the field \ref m_ontic_change.*/
    void set_ontic_change(bool to_set);

    /** \brief Getter of \ref m_id.
     *
     * Only accessible by the \ref pevent_ptr.
     *
     * @return the \ref event_id of *this*.*/
    const event_id get_id() const;
    /** \brief Getter of \ref m_information_state.
     *
     * Only accessible by the \ref pevent_ptr.
     *
     * @return the \ref event_information_state of *this*.*/
    const event_information_state get_information_state() const;
    /** \brief Getter of \ref m_pre.
     *
     * Only accessible by the \ref pevent_ptr.
     *
     * @return the precondition of *this*.*/
    const formula_list & get_precondition(const pstate & s, const action & act) const;
    /** \brief Getter of \ref m_post.
     *
     * Only accessible by the \ref pevent_ptr.
     *
     * @return the postconditions of *this*.*/
    const event_postconditions & get_postconditions(const pstate & s, const action & act) const;


    /** \brief Getter of \ref m_meta_pre.
     *
     * Only accessible by the \ref pevent_ptr.
     *
     * @return the meta_precondition of *this*.*/
    const event_metacond & get_meta_precondition() const;
    /** \brief Getter of \ref m_meta_post.
     *
     * Only accessible by the \ref pevent_ptr.
     *
     * @return the meta_postconditions of *this*.*/
    const event_metacond & get_meta_postconditions() const;

    /** \brief Getter of \ref m_ontic_change.
     *
     * Only accessible by the \ref pevent_ptr.
     *
     * @return the \ref m_ontic_change of *this*.*/
    const bool get_ontic_change() const;

public:
    //    /** \brief Empty constructor, call the default constructor of all the fields.*/
    pevent();
    //    /** \brief Constructor with parameters.
    //     * Construct an object with the given info.
    //     *
    //     * @param[in] id: the \ref id to set as \ref m_id.
    //     * @param[ontic_change]: the boolean to set as \ref m_ontic_change.*/
    //    pevent(event_id id, bool ontic_change);
    //    /** \brief Constructor with parameters.
    //     * Construct an object with the given info.
    //     *
    //     * @param[in] id: the \ref id to set as \ref m_id.
    //     * @param[ontic_change]: the boolean to set as \ref m_ontic_change.
    //     * @param[in] pre: the \ref belief_formula to set as \ref m_pre.*/
    //    pevent(event_id id, bool ontic_change, const formula_list & pre);
    //    /** \brief Constructor with parameters.
    //     * Construct an object with the given info.
    //     *
    //     * @param[in] id: the \ref id to set as \ref m_id.
    //     * @param[ontic_change]: the boolean to set as \ref m_ontic_change.
    //     * @param[in] pre: the \ref belief_formula to set as \ref m_pre.
    //     * @param[post]: the \ref event_postconditions to set as \ref m_post.*/
    //    pevent(event_id id, bool ontic_change, const formula_list & pre, const event_postconditions & post);

    /** \brief Constructor with parameters.
     * Construct an object with the given info.
     *
     * @param[in] id: the \ref id to set as \ref m_id.
     * @param[in] meta_pre: the \ref event_metacond to set as \ref m_meta_pre.
     * @param[in] meta_post: the \ref event_metacond to set as \ref m_meta_post.*/
    pevent(event_id id, const event_metacond & meta_pre, const event_metacond & meta_post);
    /** \brief Copy constructor.
     *
     * @param[in] action: the \ref pevent to copy into *this*.*/
    pevent(const pevent & to_copy);

    /** \brief Setter for the field \ref m_id.
     *
     * Only accessible by the \ref pevent_ptr.
     *
     * @param[in] id: the \ref event_id to set as \ref m_id.*/
    void set_id(event_id id);
    /** \brief Setter for the field \ref m_pre.
     *
     * Only accessible by the \ref pevent_ptr.
     *
     * @param[in] pre: the \ref belief_formula to set as \ref m_pre.*/
    void set_precondition(const formula_list & to_set);
    /** \brief Setter for the field \ref m_post.
     *
     * Only accessible by the \ref pevent_ptr.
     *
     * @param[in] post: the \ref event_postconditions to set as \ref m_post.*/
    void set_postconditions(const event_postconditions & to_set);


    /** \brief Setter for the field \ref m_meta_pre.
     *
     * Only accessible by the \ref pevent_ptr.
     *
     * @param[in] pre: the \ref event_metacond to set as \ref m_pre.*/
    void set_meta_precondition(const event_metacond & to_set);
    /** \brief Setter for the field \ref m_meta_post.
     *
     * Only accessible by the \ref pevent_ptr.
     *
     * @param[in] post: the \ref event_metacond to set as \ref m_post.*/
    void set_meta_postconditions(const event_metacond & to_set);

    /**
     *\brief The < operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref pevent (used in \ref pem_store).
     *
     * @see pem_store.
     *
     * @param [in] to_compare: the \ref pevent to compare with *this*.
     * @return true: if \p to_compare is smaller than *this*
     * @return false: otherwise.*/
    bool operator<(const pevent & to_compare) const;
    /**
     *\brief The > operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref pevent (used in \ref pem_store).
     *
     * @see pem_store.
     *
     * @param [in] to_compare: the \ref pevent to compare with *this*.
     * @return true: if \p to_compare is bigger than *this*
     * @return false: otherwise.*/
    bool operator>(const pevent & to_compare) const;
    /**
     *\brief The == operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref pevent (used in \ref pem_store).
     *
     * @see pem_store.
     *
     * @param [in] to_compare: the \ref pevent to compare with *this*.
     * @return true: if \p to_compare is equal to *this*
     * @return false: otherwise.*/
    bool operator==(const pevent & to_compare) const;
    /** \brief The copy operator.
     *
     * @param [in] to_assign: the \ref pevent to assign to *this*.
     * @return true: if \p the assignment went ok.
     * @return false: otherwise.*/
    bool operator=(const pevent & to_copy);

    void print()const;
};

/**
 * \class pevent_ptr
 *
 * \brief A wrapper class for a std::shared_pointer to a \ref pevent usually stored in \ref pem_store.
 *
 * This class is necessary so we can use the < operator in the set of \ref pevent_ptr.
 * If we don't use the wrapper pointers to the same elements are different.
 *
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 28, 2021
 */
class pevent_ptr
{
private:
    /**\brief the pointer that is wrapped by *this*.*/
    std::shared_ptr <pevent> m_ptr;

public:
    /**\brief Constructor without parameters.*/
    pevent_ptr();

    /**\brief Constructor with parameters.
     *
     * This constructor uses const pointer, this means that the pointer is copied
     * and the counter of the shared pointer is increased (std implementation).
     *
     * @param[in] ptr: the pointer to assign to \ref m_ptr.
     */
    pevent_ptr(const std::shared_ptr <pevent> &ptr);

    /**\brief Constructor with parameters.
     *
     * This constructor uses non-const pointer, this means that the pointer is copied
     * in \ref m_ptr and \p ptr becomes empty (std implementation).
     *
     * @param[in] ptr: the pointer to assign to \ref m_ptr.
     */
    pevent_ptr(std::shared_ptr <pevent> &&ptr);

    /** \brief Constructor with parameters.
     *
     * This constructor build a pointer to the given parameter.
     *
     * @param[in] state: the \ref pevent that *this* (\ref m_ptr) should point.*/
    pevent_ptr(const pevent &state);

    /**\brief Setter for the field \ref m_ptr.
     *
     * This setter uses const parameter, this means that the pointer is copied
     * and the counter of the shared pointer is increased (std implementation).
     *
     * @param[in] ptr: the pointer to assign to \ref m_ptr.*/
    void set_ptr(const std::shared_ptr <pevent> &ptr);

    /**\brief Setter for the field \ref m_ptr (move constructor).
     *
     * This setter uses non-const parameter, this means that the pointer is copied
     * in \ref m_ptr and \p ptr becomes empty (std implementation).
     *
     * @param[in] ptr: the pointer to assign to \ref m_ptr.*/
    void set_ptr(std::shared_ptr <pevent> &&ptr);

    /**\brief Getter for the field \ref m_ptr.
     *
     * @return a copy of the pointer \ref m_ptr.*/
    std::shared_ptr <pevent> get_ptr() const;

    /** \brief Getter of \ref m_id.
     *
     * @return the id of *this*.*/
    const event_id get_id() const;
    /** \brief Getter of \ref m_information_state.
     *
     * @return the information state of *this*.*/
    const event_information_state get_information_state() const;
    /** \brief Getter of \ref m_pre.
     *
     * @return the precondition of *this*.*/
    const formula_list & get_precondition(const pstate & s, const action & act) const;

    /** \brief Getter of \ref m_post.
     *
     * @return the postconditions of *this*.*/
    const event_postconditions & get_postconditions(const pstate & s, const action & act) const;

    /** \brief Getter of \ref m_meta_pre.
     *
     * @return the meta_precondition of *this*.*/
    const event_metacond & get_meta_precondition() const;
    /** \brief Getter of \ref m_meta_post.
     *
     * @return the meta_postconditions of *this*.*/
    const event_metacond & get_meta_postconditions() const;
    /** \brief Function that return the field m_ontic_change of the pointed \ref pevent.
     *
     * @return the \ref m_id of the \ref pevent pointed by \ref m_ptr.*/
    bool get_ontic_change() const;
    /**
     *\brief The < operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref pevent_ptr (used in \ref pem_store).
     *
     * @see pem_store.
     *
     * @param [in] to_compare: the \ref pevent_ptr to compare with *this*.
     * @return true: if \p to_compare is smaller than *this*
     * @return false: otherwise.*/
    bool operator<(const pevent_ptr & to_compare) const;
    /**
     *\brief The > operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref pevent_ptr (used in \ref pem_store).
     *
     * @see pem_store.
     *
     * @param [in] to_compare: the \ref pevent_ptr to compare with *this*.
     * @return true: if \p to_compare is bigger than *this*
     * @return false: otherwise.*/
    bool operator>(const pevent_ptr & to_compare) const;
    /**
     *\brief The == operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref pevent_ptr (used in \ref pem_store).
     *
     * @see pem_store.
     *
     * @param [in] to_compare: the \ref pevent_ptr to compare with *this*.
     * @return true: if \p to_compare is equal to *this*
     * @return false: otherwise.*/
    bool operator==(const pevent_ptr & to_compare) const;
    /** \brief The copy operator.
     *
     * @param [in] to_assign: the \ref pevent_ptr to assign to *this*.
     * @return true: if \p the assignment went ok.
     * @return false: otherwise.*/
    bool operator=(const pevent_ptr & to_copy);

    void print()const;

};