#ifndef EVENTCONTROL_H
#define EVENTCONTROL_H
#include "Subject.h"
#include "Observer.h"
#include "Notice.h"
#include <vector>
#include <algorithm>
#include <iostream>

/**
 * @brief Observer pattern: concrete Subject (also the spec's EventControl).
 *
 * EventControl is the event-level coordinator: the single point where notices
 * such as weather alerts, capacity warnings or evacuation instructions
 * originate. It implements the frozen Subject contract declared in Subject.h.
 *
 * Design decisions (Task 3.1 / Task 11 open questions):
 *  - Observer pointers are NON-OWNING. EventControl never deletes an observer;
 *    observers are owned by the Composite tree (see EventGroup) and must be
 *    detached before they are destroyed, to avoid dangling registrations.
 *  - attach() on an already-registered observer is a no-op (idempotent). This
 *    avoids a subject calling update() twice per notice for the same observer.
 *  - detach() of an observer that is not registered is a safe no-op; it does
 *    not throw or assert, because callers cannot always know registration
 *    state in advance (e.g. during teardown).
 *  - notify() iterates over a SNAPSHOT (copy) of the registration list rather
 *    than the live list. This makes it safe for an observer's update(...) to
 *    attach or detach observers -- including itself -- while notification for
 *    the current notice is still in progress. Any change made mid-notify is
 *    only reflected starting from the NEXT call to notify().
 *
 * GoF role: Subject (concrete). Also fills the spec's named "EventControl" role.
 */
class EventControl : public Subject {
    std::vector<Observer*> observers; // Non-owning: registered observers.
public:
    EventControl() {}
    ~EventControl() override {}

    /**
     * @brief Register a non-owning observer. Duplicate registration is ignored.
     * @param o Observer to register. Must not be nullptr.
     */
    void attach(Observer* o) override {
        if (!o) return;
        if (std::find(observers.begin(), observers.end(), o) != observers.end()) {
            std::cout << "[EventControl] attach ignored: already registered.\n";
            return;
        }
        observers.push_back(o);
    }

    /**
     * @brief Remove a previously registered observer. Safe if not present.
     * @param o Observer to remove.
     */
    void detach(Observer* o) override {
        auto it = std::find(observers.begin(), observers.end(), o);
        if (it == observers.end()) {
            std::cout << "[EventControl] detach ignored: not registered.\n";
            return;
        }
        observers.erase(it);
    }

    /**
     * @brief Push a notice to every currently registered observer (PUSH model).
     * @param n The notice to broadcast.
     */
    void notify(const Notice& n) override {
        std::vector<Observer*> snapshot = observers; // see class doc: re-entrancy safety
        for (Observer* o : snapshot) o->update(n);
    }

    /**
     * @brief Convenience helper: build a Notice and push it in one call.
     * @param type   The kind of notice.
     * @param detail Human-readable detail shown in demo output.
     */
    void issueNotice(NoticeType type, const std::string& detail = "") {
        std::cout << "[EventControl] issuing notice: " << detail << "\n";
        notify(Notice(type, detail));
    }

    /** @return Number of currently registered observers (for demos/tests). */
    size_t observerCount() const { return observers.size(); }
};
#endif
