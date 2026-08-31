#ifndef NOTICE_H
#define NOTICE_H
#include <string>

/**
 * @brief The information a Subject pushes to Observers (PUSH model).
 *
 * Minimal shared representation so every team member codes to the same
 * update(...) signature. Person B expands the catalogue to the 6+ required
 * notice types (Task 3.3), e.g. OPEN, CLOSE, SCHEDULE_CHANGE, CAPACITY_ALERT,
 * WEATHER_ALERT, PAUSE, RESUME, EVACUATE.
 */
enum class NoticeType {
    OPEN, CLOSE, SCHEDULE_CHANGE, CAPACITY_ALERT,
    WEATHER_ALERT, PAUSE, RESUME, EVACUATE
};

/** @brief A single event notice carried to observers under the push model. */
struct Notice {
    NoticeType  type;    ///< What kind of change occurred.
    std::string detail;  ///< Human-readable detail (e.g. "storm approaching").
    /**
     * @brief Construct a notice.
     * @param t The kind of change that occurred.
     * @param d Human-readable detail (optional).
     */
    Notice(NoticeType t, const std::string& d = "") : type(t), detail(d) {}
};
#endif