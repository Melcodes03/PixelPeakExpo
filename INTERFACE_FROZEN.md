# Frozen shared interface (hand this to B and C — build to it)

Person A owns and has frozen these. Do not rename; build against them.

## Composite (EventComponent.h)
    virtual void open() = 0;
    virtual void close() = 0;
    virtual void reportStatus() const = 0;
    virtual int  getCapacity() const = 0;
    virtual ~EventComponent();

## Observer model: PUSH
The subject pushes a Notice straight to the observer.

### Observer.h
    virtual void update(const Notice& n) = 0;
    virtual ~Observer();

### Subject.h  (B implements the concrete subject: registration list,
###             duplicate-attach / missing-detach policy, 6+ notice types)
    virtual void attach(Observer* o) = 0;
    virtual void detach(Observer* o) = 0;
    virtual void notify(const Notice& n) = 0;
    virtual ~Subject();

### Notice.h
    enum class NoticeType { OPEN, CLOSE, SCHEDULE_CHANGE, CAPACITY_ALERT,
                            WEATHER_ALERT, PAUSE, RESUME, EVACUATE };
    struct Notice { NoticeType type; std::string detail; };

## Ownership rules (already implemented)
- A group OWNS its children; its destructor frees the whole subtree once.
- Observer pointers are NON-OWNING; a subject never deletes an observer.
- Transfers use transferUnit(...), which moves ownership AND registration.

## Design decisions baked in (team should ratify — spec leaves these open)
1. PUSH over pull.
2. update() is NOT in EventComponent; only classes that observe inherit Observer.
3. A unit that both belongs to the tree and observes = multiple inheritance
   (EventComponent + Observer), as in integration_example.cpp.
