#ifndef LEAVES_H
#define LEAVES_H
#include "EventComponent.h"
#include <iostream>

/**
 * @brief Composite pattern: Leaf (tournament stage).
 * Reacts to open/close by starting/pausing a match. GoF role: Leaf.
 */
class TournamentStage : public EventComponent {
    int seats;       ///< Spectator seats.
    bool live;       ///< Whether a match is currently running.
public:
    /** @param n Name. @param s Spectator seats. */
    TournamentStage(const std::string& n, int s) : EventComponent(n), seats(s), live(false) {}
    void open() override  { live = true;  std::cout << "[Stage] " << name << " starts the match.\n"; }
    void close() override { live = false; std::cout << "[Stage] " << name << " pauses the match.\n"; }
    void reportStatus() const override {
        std::cout << "  Stage " << name << ": " << (live ? "LIVE" : "idle") << ", seats " << seats << "\n";
    }
    int getCapacity() const override { return seats; }
};

/**
 * @brief Composite pattern: Leaf (entry gate).
 * open/close toggles whether attendees are admitted. GoF role: Leaf.
 */
class EntryGate : public EventComponent {
    int flowPerMin;  ///< Admissions per minute.
    bool admitting;  ///< Whether the gate is currently admitting.
public:
    /** @param n Name. @param f Admissions per minute. */
    EntryGate(const std::string& n, int f) : EventComponent(n), flowPerMin(f), admitting(false) {}
    void open() override  { admitting = true;  std::cout << "[Gate] " << name << " now admitting attendees.\n"; }
    void close() override { admitting = false; std::cout << "[Gate] " << name << " stops admitting.\n"; }
    void reportStatus() const override {
        std::cout << "  Gate " << name << ": " << (admitting ? "OPEN" : "CLOSED") << ", flow " << flowPerMin << "/min\n";
    }
    int getCapacity() const override { return flowPerMin; }
};

/**
 * @brief Composite pattern: Leaf (merchandise vendor).
 * open/close starts/suspends service. GoF role: Leaf.
 */
class MerchVendor : public EventComponent {
    int stock;       ///< Units of stock available.
    bool serving;    ///< Whether the vendor is currently serving.
public:
    /** @param n Name. @param st Stock units. */
    MerchVendor(const std::string& n, int st) : EventComponent(n), stock(st), serving(false) {}
    void open() override  { serving = true;  std::cout << "[Vendor] " << name << " opens for service.\n"; }
    void close() override { serving = false; std::cout << "[Vendor] " << name << " suspends service.\n"; }
    void reportStatus() const override {
        std::cout << "  Vendor " << name << ": " << (serving ? "serving" : "closed") << ", stock " << stock << "\n";
    }
    int getCapacity() const override { return stock; }
};

/**
 * @brief Composite pattern: Leaf (medical booth).
 * Deliberately DIFFERENT: stays active on close() because it is a safety unit.
 * GoF role: Leaf.
 */
class MedicBooth : public EventComponent {
    int staff;       ///< Medical staff on duty.
public:
    /** @param n Name. @param s Staff count. */
    MedicBooth(const std::string& n, int s) : EventComponent(n), staff(s) {}
    void open() override  { std::cout << "[Medic] " << name << " on standby.\n"; }
    void close() override { std::cout << "[Medic] " << name << " REMAINS ACTIVE (safety unit).\n"; }
    void reportStatus() const override {
        std::cout << "  Medic " << name << ": always active, staff " << staff << "\n";
    }
    int getCapacity() const override { return staff; }
};

/**
 * @brief Composite pattern: Leaf (shuttle stop).
 * open/close starts/halts transport. GoF role: Leaf.
 */
class ShuttleStop : public EventComponent {
    int busesPerHour; ///< Buses per hour serving this stop.
    bool running;     ///< Whether transport is currently running.
public:
    /** @param n Name. @param b Buses per hour. */
    ShuttleStop(const std::string& n, int b) : EventComponent(n), busesPerHour(b), running(false) {}
    void open() override  { running = true;  std::cout << "[Shuttle] " << name << " begins transport.\n"; }
    void close() override { running = false; std::cout << "[Shuttle] " << name << " halts transport.\n"; }
    void reportStatus() const override {
        std::cout << "  Shuttle " << name << ": " << (running ? "running" : "stopped") << ", " << busesPerHour << " buses/hr\n";
    }
    int getCapacity() const override { return busesPerHour * 40; }
};
#endif
