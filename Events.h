//
// Created by Григорий on 27.11.2023.
//

#ifndef VILLAGE_EVENTS_H
#define VILLAGE_EVENTS_H

#include <vector>

enum class EventType {
    RaidStart,
    RaidEnd,
};

struct EventData {
    EventType type;
};

class Events {
public:
    static void HandleEvent(const EventData& eventData);

    // Function to broadcast events to game objects
    static void BroadcastEvent(const EventData& eventData);

    // Add more event-handling functions as needed
};

#endif //VILLAGE_EVENTS_H
