#ifndef OBSERVINGLEAVES_H
#define OBSERVINGLEAVES_H
#include "Leaves.h"
#include "Observer.h"
#include <iostream>

/**
 * @file ObservingLeaves.h
 * @brief Concrete Leaf classes that ALSO implement Observer (Task 3.2 / 4.1).
 *
 * Each class inherits its Composite behaviour (open/close/reportStatus/
 * getCapacity) unchanged from its Leaves.h base, and adds an Observer role
 * through multiple inheritance -- the same pattern already proven in
 * integration_example.cpp. Reactions differ per concrete type purely through
 * polymorphism: nothing here inspects a class type; each object only
 * interprets NoticeType values it is already responsible for, using its own
 * data and its own open()/close() behaviour.
 *
 * Not every Leaf needs to observe (see spec Task 11, "do all Leaves implement
 * Observer?"): MedicBooth deliberately has no Observer version here, because
 * a medical unit's "always active" behaviour already comes entirely from its
 * own open()/close() overrides in Leaves.h and does not depend on receiving
 * notices at all.
 */

/** @brief An EntryGate that reacts to capacity and safety notices. */
class ObservingEntryGate : public EntryGate, public Observer {
public:
    /**
     * @brief Construct an observing entry gate.
     * @param n Name of the gate.
     * @param f Admissions per minute.
     */
    ObservingEntryGate(const std::string& n, int f) : EntryGate(n, f) {}
    /** @brief Destroy the object. */
    ~ObservingEntryGate() override {}

    /** @param n The pushed notice. Reacts to CAPACITY_ALERT and EVACUATE. */
    void update(const Notice& n) override {
        if (n.type == NoticeType::CAPACITY_ALERT) {
            std::cout << "  [" << getName() << "] capacity alert -> stops admitting.\n";
            close();
        } else if (n.type == NoticeType::EVACUATE) {
            std::cout << "  [" << getName() << "] evacuate -> gate reversed for exit only.\n";
            close();
        }
    }
};

/** @brief A ShuttleStop that reacts to weather and evacuation notices. */
class ObservingShuttleStop : public ShuttleStop, public Observer {
public:
    /**
     * @brief Construct an observing shuttle stop.
     * @param n Name of the shuttle stop.
     * @param b Number of buses available.
     */
    ObservingShuttleStop(const std::string& n, int b) : ShuttleStop(n, b) {}
    /** @brief Destroy the object. */
    ~ObservingShuttleStop() override {}

    /** @param n The pushed notice. Reacts to WEATHER_ALERT and EVACUATE. */
    void update(const Notice& n) override {
        if (n.type == NoticeType::WEATHER_ALERT) {
            std::cout << "  [" << getName() << "] weather alert -> rerouting to covered stop.\n";
        } else if (n.type == NoticeType::EVACUATE) {
            std::cout << "  [" << getName() << "] evacuate -> running extra buses.\n";
            open();
        }
    }
};

/** @brief A MerchVendor that reacts to weather and evacuation notices. */
class ObservingMerchVendor : public MerchVendor, public Observer {
public:
    /**
     * @brief Construct an observing merchandise vendor.
     * @param n Name of the vendor stall.
     * @param st Stock level.
     */
    ObservingMerchVendor(const std::string& n, int st) : MerchVendor(n, st) {}
    /** @brief Destroy the object. */
    ~ObservingMerchVendor() override {}

    /** @param n The pushed notice. Reacts to WEATHER_ALERT and EVACUATE. */
    void update(const Notice& n) override {
        if (n.type == NoticeType::WEATHER_ALERT) {
            std::cout << "  [" << getName() << "] weather alert -> covering stock.\n";
        } else if (n.type == NoticeType::EVACUATE) {
            std::cout << "  [" << getName() << "] evacuate -> closing till.\n";
            close();
        }
    }
};

/** @brief A TournamentStage that reacts to schedule and pause/resume notices. */
class ObservingTournamentStage : public TournamentStage, public Observer {
public:
    /**
     * @brief Construct an observing tournament stage.
     * @param n Name of the stage.
     * @param s Number of seats.
     */
    ObservingTournamentStage(const std::string& n, int s) : TournamentStage(n, s) {}
    /** @brief Destroy the object. */
    ~ObservingTournamentStage() override {}

    /** @param n The pushed notice. Reacts to PAUSE, RESUME and SCHEDULE_CHANGE. */
    void update(const Notice& n) override {
        if (n.type == NoticeType::PAUSE) {
            std::cout << "  [" << getName() << "] pause -> match paused.\n";
            close();
        } else if (n.type == NoticeType::RESUME) {
            std::cout << "  [" << getName() << "] resume -> match restarted.\n";
            open();
        } else if (n.type == NoticeType::SCHEDULE_CHANGE) {
            std::cout << "  [" << getName() << "] schedule change: " << n.detail << "\n";
        }
    }
};
#endif