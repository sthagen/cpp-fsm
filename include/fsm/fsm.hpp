#pragma once

#include <tuple>
#include <variant>
#include <functional>

template <typename... States>
class State_machine {
public:
    template <typename State>
    void transitionTo() {
        currentState = &std::get<State>(states);
    }

    template <typename Event>
    void handle(const Event& event) {
        auto passEventToState = [this, &event] (auto statePtr) {
            statePtr->handle(event).execute(*this);
        };
        std::visit(passEventToState, currentState);
    }

private:
    std::tuple<States...> states;
    std::variant<States*...> currentState{ &std::get<0>(states) };
};

template <typename State>
struct TransitionTo {
    template <typename Machine>
    void execute(Machine& machine) {
        machine.template transitionTo<State>();
    }
};

struct Nothing {
    template <typename Machine>
    void execute(Machine&) {}
};

/*
struct EventAB{}; struct EventBC{}; struct EventCA{};
struct StateA; struct StateB; struct StateC;
struct StateA {
    TransitionTo<StateB> handle(const EventAB&) const { std::cout << "A->B" << std::endl; return {}; }
    Nothing handle(const EventBC&) const { std::cout << "Ignored B->C" << std::endl; return {}; }
    Nothing handle(const EventCA&) const { std::cout << "Ignored C->A" << std::endl; return {}; }
};
struct StateB {
    Nothing handle(const EventAB&) const { std::cout << "Ignored A->B" << std::endl; return {}; }
    TransitionTo<StateC> handle(const EventBC&) const { std::cout << "B->C" << std::endl; return {}; }
    Nothing handle(const EventCA&) const { std::cout << "Ignored C->A" << std::endl; return {}; }
};
struct StateC {
    Nothing handle(const EventAB&) const { std::cout << "Ignored A->B" << std::endl; return {}; }
    Nothing handle(const EventBC&) const { std::cout << "Ignored B->C" << std::endl; return {}; }
    TransitionTo<StateA> handle(const EventCA&) const { std::cout << "C->A" << std::endl; return {}; }
};
using Ring = State_machine<StateA, StateB, StateC>;
#include <iostream>
int main()
{
    Ring ring;
    ring.handle(EventAB{});
    ring.handle(EventBC{});
    ring.handle(EventCA{});
    ring.handle(EventAB{});
    ring.handle(EventBC{});
    ring.handle(EventBC{});
    ring.handle(EventAB{});
    return 0;
}
*/
