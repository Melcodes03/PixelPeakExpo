// integration_example.cpp
// A+B SEAM ILLUSTRATION ONLY. Proves the frozen Observer contract compiles and
// that transferUnit() moves ownership AND registration together.
// Person B owns the REAL Subject (registration list, duplicate/missing policy,
// 6+ notice types, cascading) as Task 3. The stub below is throwaway.
#include "EventGroup.h"
#include "Leaves.h"
#include "Observer.h"
#include "Subject.h"
#include <iostream>
#include <vector>
#include <algorithm>

// --- DEMO STUB subject (B replaces with the real one) ----------------------
class StubSubject : public Subject {
    std::vector<Observer*> obs;               // non-owning
public:
    void attach(Observer* o) override {
        if (o && std::find(obs.begin(), obs.end(), o) == obs.end()) obs.push_back(o);
    }
    void detach(Observer* o) override {
        obs.erase(std::remove(obs.begin(), obs.end(), o), obs.end());
    }
    void notify(const Notice& n) override { for (Observer* o : obs) o->update(n); }
};

// A vendor that is BOTH an EventComponent (Leaf) AND an Observer.
class ObservingVendor : public MerchVendor, public Observer {
public:
    ObservingVendor(const std::string& n, int st) : MerchVendor(n, st) {}
    void update(const Notice& n) override {
        if (n.type == NoticeType::WEATHER_ALERT)
            std::cout << "  [" << getName() << "] weather alert -> covering stock.\n";
        else if (n.type == NoticeType::EVACUATE)
            std::cout << "  [" << getName() << "] evacuate -> closing till.\n";
    }
};

int main() {
    EventGroup* zoneA = new EventGroup("Zone A");
    EventGroup* zoneB = new EventGroup("Zone B");
    StubSubject subA, subB;

    ObservingVendor* v = new ObservingVendor("Roaming Stall", 120);
    zoneA->add(v);
    subA.attach(v);                          // registered with Zone A's subject

    std::cout << "-- Zone A notifies --\n";
    subA.notify(Notice(NoticeType::WEATHER_ALERT, "storm"));

    std::cout << "-- transfer Roaming Stall: Zone A -> Zone B (ownership + registration) --\n";
    transferUnit(zoneA, zoneB, v, &subA, &subB, v);

    std::cout << "-- Zone A notifies again (v should NOT react) --\n";
    subA.notify(Notice(NoticeType::WEATHER_ALERT, "storm"));
    std::cout << "-- Zone B notifies (v SHOULD react) --\n";
    subB.notify(Notice(NoticeType::EVACUATE, "clear the area"));

    delete zoneA;   // v is no longer here
    delete zoneB;   // v freed here, exactly once
    return 0;
}
