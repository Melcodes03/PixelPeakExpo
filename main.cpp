/**
 * @file main.cpp
 * @brief EventFlow submission entry point (Task 8). Integrates the
 *        Composite subsystem (Person A) and the Observer subsystem
 *        (Person B) into one coherent PixelPeak Expo simulation.
 *
 * Demonstrates, per Task 8.1: Composite construction; Observer
 * registration; three or more distinct notices; a cascading
 * notification; a registration change at runtime; a Composite
 * traversal/query; a runtime reorganisation; and clean shutdown.
 *
 * The capacity-threshold check below is the exact condition modelled
 * in SD3's alt fragment (Task 5.3) - this is real code, not a
 * diagram-only decoration, per the practical's rule that diagrams and
 * code must agree.
 */
#include "EventGroup.h"
#include "Leaves.h"
#include "EventControl.h"
#include "ObservingZone.h"
#include "ObservingLeaves.h"
#include <iostream>

/**
 * @brief EventFlow submission entry point.
 * @return 0 on successful completion.
 */
int main() {
    const int EVACUATION_THRESHOLD = 5000; //matches SD3's alt guard exactly

    std::cout << "===== PixelPeak Expo: EventFlow simulation =====\n\n";

    //Composite construction: 3 levels of nesting-
    EventGroup* expo = new EventGroup("PixelPeak Expo");

    ObservingZone* arenaZone = new ObservingZone("Arena Zone");
    ObservingTournamentStage* mainArena = new ObservingTournamentStage("Main Arena", 5000);
    ObservingEntryGate* westGate  = new ObservingEntryGate("West Gate", 300);
    MedicBooth* medic = new MedicBooth("Medic Booth A", 8);
    arenaZone->add(mainArena);
    arenaZone->add(westGate);
    arenaZone->add(medic);

    EventGroup* merchCourt = new EventGroup("Merch Court");         // level 3
    ObservingMerchVendor* figurines = new ObservingMerchVendor("Figurine Stall", 200);
    ObservingMerchVendor* apparel   = new ObservingMerchVendor("Apparel Stall", 150);
    merchCourt->add(figurines);
    merchCourt->add(apparel);
    arenaZone->add(merchCourt);

    EventGroup* lanZone = new EventGroup("LAN Zone");
    ObservingShuttleStop* bus = new ObservingShuttleStop("Shuttle Bay", 6);
    lanZone->add(new TournamentStage("LAN Arena", 2000));
    lanZone->add(bus);

    expo->add(arenaZone);
    expo->add(lanZone);

    //Observer registration: separate relationship from ownership --
    EventControl control;
    control.attach(arenaZone);//zone hears notices from control
    arenaZone->attach(mainArena); //leaves hear notices relayed by their zone
    arenaZone->attach(westGate);
    arenaZone->attach(figurines);
    arenaZone->attach(apparel);

    std::cout << "--- initial state ---\n";
    expo->reportStatus(); //Composite traversal/query
    std::cout << "\n";

    //Rule: schedule notices only affect TournamentStage-
    std::cout << "--- notice 1/3: SCHEDULE_CHANGE (only the stage reacts) ---\n";
    control.issueNotice(NoticeType::SCHEDULE_CHANGE, "Main Arena final delayed 30 min");
    std::cout << "\n";

    //Rule: weather alert -> adaptive, not shutdown, response
    std::cout << "--- notice 2/3: WEATHER_ALERT (adaptive response, cascades 3 levels) ---\n";
    control.issueNotice(NoticeType::WEATHER_ALERT, "storm approaching");
    //cascade: EventControl -> ObservingZone::update() -> ObservingZone::notify()
    // -> mainArena/westGate/figurines/apparel each react via their own update()
    std::cout << "\n";

    //Registration change at runtime
    std::cout << "--- registration change: figurine stall detaches mid-run ---\n";
    arenaZone->detach(figurines);
    std::cout << "\n";

    //SD3: capacity threshold decides evacuate vs status report-
    std::cout << "--- SD3 scenario: capacity check ---\n";
    int total = arenaZone->getCapacity();               //real Composite operation
    std::cout << "arenaZone total capacity: " << total << "\n";
    if (total >= EVACUATION_THRESHOLD) {
        std::cout << "--- notice 3/3: EVACUATE (threshold breached, cascades) ---\n";
        control.issueNotice(NoticeType::EVACUATE, "capacity critical");
        //figurines will NOT react: detached above. apparel and the leaves still will.

        std::cout << "\n--- opt: full physical shutdown follows the evacuation ---\n";
        arenaZone->close(); //second real Composite operation
        //medic remains active regardless - same message, different behaviour,
        //proven through polymorphism, no type-checking anywhere in this call.
    } else {
        arenaZone->reportStatus();
    }
    std::cout << "\n";

    //Runtime reorganisation: move apparel stall to the LAN zone
    std::cout << "--- runtime reorganisation: Apparel Stall moves Arena -> LAN Zone ---\n";
    transferUnit(merchCourt, lanZone, apparel, arenaZone, /*newSubject=*/nullptr, apparel);
    //Composite ownership AND Observer registration both move together;
    //apparel is not re-attached to a new subject here, matching the
    //team's policy that not every transferred unit must keep observing.
    expo->reportStatus();
    std::cout << "\n";

    //Clean shutdown
    std::cout << "--- clean shutdown ---\n";
    control.detach(arenaZone); //detach before the owning tree is released
    delete expo; //cascades: frees every remaining owned child exactly once
    std::cout << "Shutdown complete.\n";
    return 0;
}