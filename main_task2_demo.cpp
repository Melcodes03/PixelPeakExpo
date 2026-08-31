#include "EventGroup.h"
#include "Leaves.h"

/**
 * @file main.cpp
 * @brief Person A test driver for the Composite half of EventFlow.
 *        (The real submission main.cpp is Person C's.)
 */
int main() {
    // Root -> zone -> merch court : three levels of nesting.
    EventGroup* expo = new EventGroup("PixelPeak Expo");

    EventGroup* arenaZone = new EventGroup("Arena Zone");
    TournamentStage* mainArena = new TournamentStage("Main Arena", 5000);
    EntryGate*       westGate  = new EntryGate("West Gate", 300);
    MedicBooth*      medic     = new MedicBooth("Medic Booth A", 8);
    arenaZone->add(mainArena);
    arenaZone->add(westGate);
    arenaZone->add(medic);

    EventGroup* merchCourt = new EventGroup("Merch Court"); // level 3
    merchCourt->add(new MerchVendor("Figurine Stall", 200));
    MerchVendor* apparel = new MerchVendor("Apparel Stall", 150);
    merchCourt->add(apparel);
    arenaZone->add(merchCourt);

    EventGroup* lanZone = new EventGroup("LAN Zone");
    lanZone->add(new TournamentStage("LAN Arena", 2000));
    lanZone->add(new ShuttleStop("Shuttle Bay", 6));

    expo->add(arenaZone);
    expo->add(lanZone);

    expo->open();
    std::cout << "\n--- status ---\n";
    expo->reportStatus();
    std::cout << "\nTotal expo capacity: " << expo->getCapacity() << "\n\n";

    // Runtime reorganisation (Task 4.2): move Apparel Stall to the LAN Zone.
    std::cout << "--- transfer: Apparel Stall -> LAN Zone ---\n";
    transferUnit(merchCourt, lanZone, apparel);
    expo->reportStatus();
    std::cout << "\n";

    expo->close();

    delete expo; // frees the entire remaining tree exactly once.
    return 0;
}
