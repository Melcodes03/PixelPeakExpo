#ifndef SUBJECT_H
#define SUBJECT_H
#include "Observer.h"

/**
 * @brief Observer pattern: Subject (abstract contract).
 *
 * Frozen interface every subject must honour. Person B implements the CONCRETE
 * subject: the registration list, duplicate-attach / missing-detach policy
 * (Task 3.1) and the concrete notice-producing behaviour (Task 3.3-3.4).
 * Observer pointers are NON-OWNING: a subject never deletes its observers.
 *
 * GoF role: Subject.
 */
class Subject {
public:
    /** @brief Destroy the object. */
    virtual ~Subject() {}
    /** @brief Register a non-owning observer. @param o Must not be nullptr. */
    virtual void attach(Observer* o) = 0;
    /** @brief Remove a previously registered observer. Safe if not present.  @param o Observer to remove.*/
    virtual void detach(Observer* o) = 0;
    /** @brief Push the current notice to all registered observers. @param n The notice to broadcast.*/
    virtual void notify(const Notice& n) = 0;
};
#endif
