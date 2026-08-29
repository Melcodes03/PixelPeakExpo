#ifndef OBSERVINGZONE_H
#define OBSERVINGZONE_H
#include "EventGroup.h"
#include "Subject.h"
#include "Observer.h"
#include <vector>
#include <algorithm>
#include <iostream>

/**
 * @brief A Composite-level class that is BOTH Observer and Subject (Task 3.4).
 *
 * ObservingZone is a genuine EventGroup: it owns child components and
 * supports open/close/reportStatus/getCapacity like any other group
 * (Composite role, inherited unchanged). It ALSO participates in Observer
 * twice, in two different collaborations:
 *   - as an Observer, it can be attach()ed to an upstream Subject (typically
 *     EventControl) to RECEIVE notices from above;
 *   - as a Subject, it maintains its OWN registration list of interested
 *     children/units and RE-NOTIFIES them when it receives an update.
 *
 * This is what makes a notice cascade through multiple runtime levels:
 * EventControl::notify() -> ObservingZone::update() -> ObservingZone::notify()
 * -> a leaf's Observer::update(). Being both roles is not a pattern misuse:
 * the "upstream" relationship and the "downstream" relationship are two
 * separate one-to-many dependencies with two separate observer lists that
 * happen to live on the same object for different reasons.
 *
 * Ownership: like any EventGroup, this class OWNS its Composite children
 * (deleted once by ~EventGroup()). The separate downstream-observer list is
 * NON-OWNING, exactly like any other Subject: a child being an interested
 * observer does not imply this zone deletes it a second time.
 *
 * GoF roles: Composite (via EventGroup), Observer, Subject (concrete).
 */
class ObservingZone : public EventGroup, public Observer, public Subject {
    std::vector<Observer*> downstream; ///< Non-owning: this zone's own registrants.
public:
    /** @param n Name of this zone. */
    ObservingZone(const std::string& n) : EventGroup(n) {}
    ~ObservingZone() override {}

    // --- Subject side: who this zone notifies downstream --------------------
    void attach(Observer* o) override {
        if (!o) return;
        if (std::find(downstream.begin(), downstream.end(), o) != downstream.end()) return;
        downstream.push_back(o);
    }
    void detach(Observer* o) override {
        auto it = std::find(downstream.begin(), downstream.end(), o);
        if (it != downstream.end()) downstream.erase(it);
    }
    void notify(const Notice& n) override {
        std::vector<Observer*> snapshot = downstream; // re-entrancy safety, see EventControl
        for (Observer* o : snapshot) o->update(n);
    }

    // --- Observer side: what this zone does when notified from above --------
    /**
     * @brief Relay an upstream notice to this zone's own registered observers.
     *
     * This is the cascade step: the zone does not need to know what concrete
     * leaves sit below it -- it just re-broadcasts through its own Subject
     * role. A zone could also react itself before relaying (log, change its
     * own internal state, filter the notice); this base version relays
     * unconditionally and prints a trace line for demo/debug purposes.
     * @param n The notice pushed by the upstream subject.
     */
    void update(const Notice& n) override {
        std::cout << "[Zone " << getName() << "] relaying notice downstream.\n";
        notify(n);
    }
};
#endif