/**
 * \brief Class that represents an event of a Kripke-based Event Model.
 *
 * \details A cem is a representation for actions in epistemic planning
 *
 * @see action and cem.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date June 24, 2021
 */
#pragma once

#include "../../../include/definitions/delphic.h"
#include "../../formulae/belief_formula.h"
#include "../../states/possibilities/pstate.h"
//#include "../action.h"

class cevent {
    friend class cevent_ptr;

private:
    /** \brief The id of *this*.*/
    event_id m_id;

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
     * Only accessible by the \ref cevent_ptr.
     *
     * @return the \ref event_id of *this*.*/
    const event_id get_id() const;
    /** \brief Getter of \ref m_pre.
     *
     * Only accessible by the \ref cevent_ptr.
     *
     * @return the precondition of *this*.*/
    formula_list get_precondition(const pstate & s, const action & act) const;
    /** \brief Getter of \ref m_post.
     *
     * Only accessible by the \ref cevent_ptr.
     *
     * @return the postconditions of *this*.*/
    event_postconditions get_postconditions(const pstate & s, const action & act) const;


    /** \brief Getter of \ref m_meta_pre.
     *
     * Only accessible by the \ref cevent_ptr.
     *
     * @return the meta_precondition of *this*.*/
    const event_metacond & get_meta_precondition() const;
    /** \brief Getter of \ref m_meta_post.
     *
     * Only accessible by the \ref cevent_ptr.
     *
     * @return the meta_postconditions of *this*.*/
    const event_metacond & get_meta_postconditions() const;

    /** \brief Getter of \ref m_ontic_change.
     *
     * Only accessible by the \ref cevent_ptr.
     *
     * @return the \ref m_ontic_change of *this*.*/
    const bool get_ontic_change() const;

public:
    //    /** \brief Empty constructor, call the default constructor of all the fields.*/
    cevent();
    //    /** \brief Constructor with parameters.
    //     * Construct an object with the given info.
    //     *
    //     * @param[in] id: the \ref id to set as \ref m_id.
    //     * @param[ontic_change]: the boolean to set as \ref m_ontic_change.*/
    //    cevent(event_id id, bool ontic_change);
    //    /** \brief Constructor with parameters.
    //     * Construct an object with the given info.
    //     *
    //     * @param[in] id: the \ref id to set as \ref m_id.
    //     * @param[ontic_change]: the boolean to set as \ref m_ontic_change.
    //     * @param[in] pre: the \ref belief_formula to set as \ref m_pre.*/
    //    cevent(event_id id, bool ontic_change, const formula_list & pre);
    //    /** \brief Constructor with parameters.
    //     * Construct an object with the given info.
    //     *
    //     * @param[in] id: the \ref id to set as \ref m_id.
    //     * @param[ontic_change]: the boolean to set as \ref m_ontic_change.
    //     * @param[in] pre: the \ref belief_formula to set as \ref m_pre.
    //     * @param[post]: the \ref event_postconditions to set as \ref m_post.*/
    //    cevent(event_id id, bool ontic_change, const formula_list & pre, const event_postconditions & post);


    /** \brief Constructor with parameters.
     * Construct an object with the given info.
     *
     * @param[in] id: the \ref id to set as \ref m_id.
     * @param[in] meta_pre: the \ref event_metacond to set as \ref m_meta_pre.
     * @param[in] meta_post: the \ref event_metacond to set as \ref m_meta_post.*/
    cevent(event_id id, const formula_list & pre, const event_metacond & meta_pre, const event_postconditions & post, const event_metacond & meta_post);

    /** \brief Constructor with parameters.
     * Construct an object with the given info.
     *
     * @param[in] id: the \ref id to set as \ref m_id.
     * @param[in] meta_pre: the \ref event_metacond to set as \ref m_meta_pre.
     * @param[in] meta_post: the \ref event_metacond to set as \ref m_meta_post.*/
    cevent(event_id id, const event_metacond & meta_pre, const event_metacond & meta_post);
    /** \brief Copy constructor.
     *
     * @param[in] action: the \ref cevent to copy into *this*.*/
    cevent(const cevent & to_copy);

    /** \brief Setter for the field \ref m_id.
     *
     * Only accessible by the \ref cevent_ptr.
     *
     * @param[in] id: the \ref event_id to set as \ref m_id.*/
    void set_id(event_id id);
    /** \brief Setter for the field \ref m_pre.
     *
     * Only accessible by the \ref cevent_ptr.
     *
     * @param[in] pre: the \ref belief_formula to set as \ref m_pre.
    void set_precondition(const formula_list & to_set);*/
    /** \brief Setter for the field \ref m_post.
     *
     * Only accessible by the \ref cevent_ptr.
     *
     * @param[in] post: the \ref event_postconditions to set as \ref m_post.
    void set_postconditions(const event_postconditions & to_set);*/


    /** \brief Setter for the field \ref m_meta_pre.
     *
     * Only accessible by the \ref cevent_ptr.
     *
     * @param[in] pre: the \ref event_metacond to set as \ref m_pre.*/
    void set_meta_precondition(const event_metacond & to_set);
    /** \brief Setter for the field \ref m_meta_post.
     *
     * Only accessible by the \ref cevent_ptr.
     *
     * @param[in] post: the \ref event_metacond to set as \ref m_post.*/
    void set_meta_postconditions(const event_metacond & to_set);

    /**
     *\brief The < operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref cevent (used in \ref cem_store).
     *
     * @see cem_store.
     *
     * @param [in] to_compare: the \ref cevent to compare with *this*.
     * @return true: if \p to_compare is smaller than *this*
     * @return false: otherwise.*/
    bool operator<(const cevent & to_compare) const;
    /**
     *\brief The > operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref cevent (used in \ref cem_store).
     *
     * @see cem_store.
     *
     * @param [in] to_compare: the \ref cevent to compare with *this*.
     * @return true: if \p to_compare is bigger than *this*
     * @return false: otherwise.*/
    bool operator>(const cevent & to_compare) const;
    /**
     *\brief The == operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref cevent (used in \ref cem_store).
     *
     * @see cem_store.
     *
     * @param [in] to_compare: the \ref cevent to compare with *this*.
     * @return true: if \p to_compare is equal to *this*
     * @return false: otherwise.*/
    bool operator==(const cevent & to_compare) const;
    /** \brief The copy operator.
     *
     * @param [in] to_assign: the \ref cevent to assign to *this*.
     * @return true: if \p the assignment went ok.
     * @return false: otherwise.*/
    bool operator=(const cevent & to_copy);

    void print()const;
};

/**
 * \class cevent_ptr
 *
 * \brief A wrapper class for a std::shared_pointer to a \ref cevent usually stored in \ref cem_store.
 *
 * This class is necessary so we can use the < operator in the set of \ref cevent_ptr.
 * If we don't use the wrapper pointers to the same elements are different.
 *
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date April 28, 2021
 */
class cevent_ptr {
private:
    /**\brief the pointer that is wrapped by *this*.*/
    std::shared_ptr <cevent> m_ptr;

public:
    /**\brief Constructor without parameters.*/
    cevent_ptr();

    /**\brief Constructor with parameters.
     *
     * This constructor uses const pointer, this means that the pointer is copied
     * and the counter of the shared pointer is increased (std implementation).
     *
     * @param[in] ptr: the pointer to assign to \ref m_ptr.
     */
    cevent_ptr(const std::shared_ptr <cevent> &ptr);

    /**\brief Constructor with parameters.
     *
     * This constructor uses non-const pointer, this means that the pointer is copied
     * in \ref m_ptr and \p ptr becomes empty (std implementation).
     *
     * @param[in] ptr: the pointer to assign to \ref m_ptr.
     */
    cevent_ptr(std::shared_ptr <cevent> &&ptr);

    /** \brief Constructor with parameters.
     *
     * This constructor build a pointer to the given parameter.
     *
     * @param[in] state: the \ref cevent that *this* (\ref m_ptr) should point.*/
    cevent_ptr(const cevent &state);

    /**\brief Setter for the field \ref m_ptr.
     *
     * This setter uses const parameter, this means that the pointer is copied
     * and the counter of the shared pointer is increased (std implementation).
     *
     * @param[in] ptr: the pointer to assign to \ref m_ptr.*/
    void set_ptr(const std::shared_ptr <cevent> &ptr);

    /**\brief Setter for the field \ref m_ptr (move constructor).
     *
     * This setter uses non-const parameter, this means that the pointer is copied
     * in \ref m_ptr and \p ptr becomes empty (std implementation).
     *
     * @param[in] ptr: the pointer to assign to \ref m_ptr.*/
    void set_ptr(std::shared_ptr <cevent> &&ptr);

    /**\brief Getter for the field \ref m_ptr.
     *
     * @return a copy of the pointer \ref m_ptr.*/
    std::shared_ptr <cevent> get_ptr() const;


    const event_id get_id() const;
    /** \brief Getter of \ref m_pre.
     *
     * @return the precondition of *this*.*/
    formula_list get_precondition(const pstate & s, const action & act) const;

    /** \brief Getter of \ref m_post.
     *
     * @return the postconditions of *this*.*/
    event_postconditions get_postconditions(const pstate & s, const action & act) const;

    /** \brief Getter of \ref m_meta_pre.
     *
     * @return the meta_precondition of *this*.*/
    const event_metacond & get_meta_precondition() const;
    /** \brief Getter of \ref m_meta_post.
     *
     * @return the meta_postconditions of *this*.*/
    const event_metacond & get_meta_postconditions() const;
    /** \brief Function that return the field m_ontic_change of the pointed \ref cevent.
     *
     * @return the \ref m_id of the \ref cevent pointed by \ref m_ptr.*/
    bool get_ontic_change() const;
    /**
     *\brief The < operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref cevent_ptr (used in \ref cem_store).
     *
     * @see cem_store.
     *
     * @param [in] to_compare: the \ref cevent_ptr to compare with *this*.
     * @return true: if \p to_compare is smaller than *this*
     * @return false: otherwise.*/
    bool operator<(const cevent_ptr & to_compare) const;
    /**
     *\brief The > operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref cevent_ptr (used in \ref cem_store).
     *
     * @see cem_store.
     *
     * @param [in] to_compare: the \ref cevent_ptr to compare with *this*.
     * @return true: if \p to_compare is bigger than *this*
     * @return false: otherwise.*/
    bool operator>(const cevent_ptr & to_compare) const;
    /**
     *\brief The == operator based on the field \ref m_id.
     * Implemented to allow the ordering on set of \ref cevent_ptr (used in \ref cem_store).
     *
     * @see cem_store.
     *
     * @param [in] to_compare: the \ref cevent_ptr to compare with *this*.
     * @return true: if \p to_compare is equal to *this*
     * @return false: otherwise.*/
    bool operator==(const cevent_ptr & to_compare) const;
    /** \brief The copy operator.
     *
     * @param [in] to_assign: the \ref cevent_ptr to assign to *this*.
     * @return true: if \p the assignment went ok.
     * @return false: otherwise.*/
    bool operator=(const cevent_ptr & to_copy);

    void print()const;

};