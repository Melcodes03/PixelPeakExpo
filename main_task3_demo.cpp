// main_task3_demo.cpp
// Person B's standalone proof of Task 3 (Observer notification system).
// This is NOT the submission main.cpp -- per INTERFACE_FROZEN.md, Person C
// owns the real main.cpp that integrates everyone's work. This file exists
// so Task 3 can be built, run and reviewed independently before that merge.
#include "EventControl.h"
#include "ObservingZone.h"
#include "ObservingLeaves.h"
#include <iostream>

int main() {
    EventControl control;

    ObservingZone* arenaZone = new ObservingZone("Arena Zone");
    ObservingZone* lanZone   = new ObservingZone("LAN Zone");

    ObservingEntryGate*       gate   = new ObservingEntryGate("West Gate", 300);
    ObservingTournamentStage* stage  = new ObservingTournamentStage("Main Arena", 5000);
    ObservingMerchVendor*     vendor = new ObservingMerchVendor("Apparel Stall", 150);
    ObservingShuttleStop*     bus    = new ObservingShuttleStop("Shuttle Bay", 6);

    // Composite ownership: these calls make the zones the OWNERS of the units.
    arenaZone->add(gate);
    arenaZone->add(stage);
    arenaZone->add(vendor);
    lanZone->add(bus);

    // Observer registration is a SEPARATE relationship from ownership above.
    control.attach(arenaZone);   // zone hears notices from central control
    control.attach(lanZone);
    arenaZone->attach(gate);     // gate hears notices relayed by its zone
    arenaZone->attach(stage);
    arenaZone->attach(vendor);
    lanZone->attach(bus);

    std::cout << "--- duplicate attach is ignored ---\n";
    control.attach(arenaZone);                 // no-op, already registered
    std::cout << "zones registered at control: " << control.observerCount() << "\n\n";

    std::cout << "--- cascade through 3 runtime levels: WEATHER_ALERT ---\n";
    control.issueNotice(NoticeType::WEATHER_ALERT, "storm approaching");
    // EventControl::notify() -> ObservingZone::update() -> ObservingZone::notify()
    // -> ObservingMerchVendor::update() / ObservingShuttleStop::update()

    std::cout << "\n--- detach at runtime, then notify again ---\n";
    control.detach(lanZone);
    control.issueNotice(NoticeType::EVACUATE, "clear the venue");
    // lanZone (and its shuttle) should NOT react this time: it is detached.

    std::cout << "\n--- detach of an already-unregistered observer is safe ---\n";
    control.detach(lanZone);                   // already detached: no crash

    std::cout << "\n--- operational / capacity / safety notice categories ---\n";
    arenaZone->notify(Notice(NoticeType::CAPACITY_ALERT, "arena at 95%"));  // capacity
    arenaZone->notify(Notice(NoticeType::PAUSE, "lightning delay"));        // safety
    arenaZone->notify(Notice(NoticeType::RESUME, "all clear"));            // operational

    delete arenaZone;  // frees gate, stage, vendor (owned children) exactly once each
    delete lanZone;    // frees bus exactly once
    return 0;
}