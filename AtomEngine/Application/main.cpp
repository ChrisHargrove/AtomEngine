#include "Engine.h"

#include "LoadState.h"
#include "StateManager.h"


int main(int argc, char** argv) {

    Engine* game = new Engine();
    game->Initialize(1024, 768, "Atom Engine v2.0", "AtomEngine.log");

    if (StateMachine::Instance()->AddState("LOAD:", new LoadState())) {
        StateMachine::Instance()->PushState("LOAD:");
    }

    return game->Run();
}