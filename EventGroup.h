#ifndef EVENTGROUP_H
#define EVENTGROUP_H
#include "EventComponent.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include "Subject.h"   // for the registration-aware transfer

/**
 * @brief Composite pattern: Composite.
 *
 * An area of the event that contains child components. Children may be Leaves
 * or other EventGroups, so the tree can nest to any depth. The required
 * operations are implemented by recursing over the children.
 *
 * Ownership policy: an EventGroup OWNS the children added to it. Its destructor
 * deletes them, so destroying the root releases the entire owned subtree
 * exactly once. To move a child WITHOUT deleting it (a runtime transfer),
 * use release() to hand ownership to another group.
 *
 * GoF role: Composite.
 */
class EventGroup : public EventComponent {
    std::vector<EventComponent*> children; ///< Owning pointers to child components.
public:
    /** @param n Name of this area/group. */
    EventGroup(const std::string& n) : EventComponent(n) {}

    /// Deletes every owned child, releasing the whole subtree exactly once.
    ~EventGroup() override {
        for (EventComponent* c : children) delete c;
    }

    /**
     * @brief Add a child; this group takes ownership of it.
     * @param c Child component. Must not be nullptr. Ownership transfers here.
     */
    void add(EventComponent* c) { children.push_back(c); }

    /**
     * @brief Detach a child WITHOUT deleting it, for transfer to another group.
     * @param c The child to release.
     * @return The released child (caller now owns it), or nullptr if not found.
     */
    EventComponent* release(EventComponent* c) {
        auto it = std::find(children.begin(), children.end(), c);
        if (it == children.end()) return nullptr;
        children.erase(it);
        return c;
    }

    void open() override {
        std::cout << "== opening group " << name << " ==\n";
        for (EventComponent* c : children) c->open();
    }
    void close() override {
        std::cout << "== closing group " << name << " ==\n";
        for (EventComponent* c : children) c->close();
    }
    void reportStatus() const override {
        std::cout << "[Group " << name << "] capacity " << getCapacity() << ":\n";
        for (EventComponent* c : children) c->reportStatus();
    }
    int getCapacity() const override {
        int total = 0;
        for (EventComponent* c : children) total += c->getCapacity();
        return total;
    }
};

/**
 * @brief Move a unit from one group to another (runtime reorganisation, Task 4.2).
 *
 * Moves BOTH relationships correctly, with no double-free:
 *  - Composite ownership: release() detaches without deleting; add() re-owns.
 *  - Observer registration: the unit is detached from its old subject and
 *    attached to its new one. These parameters are optional (nullptr) so the
 *    same function serves units that are not observers.
 *
 * @param from       Group currently owning the unit.
 * @param to         Group that should own the unit afterwards.
 * @param unit       The unit to move.
 * @param oldSubject Subject the unit currently observes, or nullptr.
 * @param newSubject Subject the unit should observe afterwards, or nullptr.
 * @param unitAsObs  The unit viewed as an Observer, or nullptr if it observes
 *                   nothing. (A unit may be an EventComponent and an Observer.)
 * @return true if the unit was found in @p from and moved; false otherwise.
 */
inline bool transferUnit(EventGroup* from, EventGroup* to, EventComponent* unit,
                         Subject* oldSubject = nullptr,
                         Subject* newSubject = nullptr,
                         Observer* unitAsObs = nullptr) {
    EventComponent* moved = from->release(unit);
    if (!moved) return false;          // not owned by 'from' -> do not touch registrations
    to->add(moved);
    if (oldSubject && unitAsObs) oldSubject->detach(unitAsObs);
    if (newSubject && unitAsObs) newSubject->attach(unitAsObs);
    return true;
}
#endif
