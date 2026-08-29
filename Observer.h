#ifndef OBSERVER_H
#define OBSERVER_H
#include "Notice.h"

/**
 * @brief Observer pattern: Observer (abstract).
 *
 * PUSH model: the subject passes the Notice directly to update(...).
 * A class implements this interface only if it is interested in notifications;
 * not every Leaf must be an Observer.
 *
 * GoF role: Observer.
 */
class Observer {
public:
    virtual ~Observer() {}
    /**
     * @brief Receive a pushed notice from a subject the observer is attached to.
     * @param n The notice describing what changed.
     */
    virtual void update(const Notice& n) = 0;
};
#endif
