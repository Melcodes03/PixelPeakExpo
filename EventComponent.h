#ifndef EVENTCOMPONENT_H
#define EVENTCOMPONENT_H
#include <string>

/**
 * @brief Composite pattern: Component.
 *
 * Common interface for every part of the event, whether it is a single
 * operational unit (Leaf) or a whole area containing other parts (Composite).
 * The client invokes behaviour through this interface without knowing whether
 * it holds one Leaf or an entire subtree.
 *
 * GoF role: Component (Composite pattern).
 */
class EventComponent {
protected:
    std::string name; ///< Human-readable identifier for this part of the event.
public:
    /**
     * @brief Construct a named event component.
     * @param n Display name of this component.
     */
    EventComponent(const std::string& n) : name(n) {}

    /// Virtual destructor: required so deleting through a base pointer
    /// destroys the concrete object (and, for a group, its whole subtree).
    virtual ~EventComponent() {}

    virtual void open() = 0;               ///< Bring this part into operation.
    virtual void close() = 0;              ///< Take this part out of operation.
    virtual void reportStatus() const = 0; ///< Print current status.
        /**
     * @brief Return this component's contribution to overall capacity.
     * @return The capacity value (meaning varies by leaf type — seats, flow rate, stock, staff, etc.).
     */
    virtual int  getCapacity() const = 0;  ///< Return capacity contribution.

    /**
     * @brief Accessor for the component's name.
     * @return The display name.
     */
    const std::string& getName() const { return name; }

    // --- Observer hook ------------------------------------------------------
    // update(...) is intentionally NOT in this base: not every component
    // observes (see spec's open design questions). A component that needs to
    // receive notifications inherits Observer (Observer.h) and implements
    // update(const Notice&). A class that also emits notices inherits Subject
    // (Subject.h). See integration_example.cpp for a class in BOTH roles.
};
#endif
