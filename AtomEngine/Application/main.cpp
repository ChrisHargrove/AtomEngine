//#include "InputManager.h"
#include "Engine.h"

#include "LoadState.h"
#include "StateManager.h"
#include "GUIManager.h"

int main(int argc, char** argv) {

    Engine* game = new Engine();

    //Setup GUI callbacks into engine

    game->SetGUIInitializeCallback([] { GUI::Instance()->Initialize(); });
    game->SetGUIStartFrameCallback([] { GUI::Instance()->StartFrame(); });
    game->SetGUIRenderFrameCallback([] { GUI::Instance()->Render(); });
    game->SetGUIShutdownCallback([] { GUI::Instance()->Shutdown(); });

    game->Initialize(1330, 768, "Atom Engine v2.0", "AtomEngine.log");

    if (StateMachine::Instance()->AddState("LOAD:", new LoadState())) {
        StateMachine::Instance()->PushState("LOAD:");
    }

    return game->Run();
}