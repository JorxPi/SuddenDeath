#pragma once
#include <string>
#include <iostream>

// Forward declarations
class GameManager;
class InvocationScript;

class PassiveAbility {
public:
    virtual ~PassiveAbility() = default;

    //Called when the creature is summoned.
    virtual void Activate(GameManager* gameManager, InvocationScript* invocation);

    //Called to react to specific game events.
    virtual void OnEvent(GameManager* gameManager, InvocationScript* invocation) {}

    //Used for any cleanup if the creature is destroyed.
    virtual void OnInvocationDeath() {}

    // Description for debugging and revealing
    virtual const std::string& GetDescription() const;

    virtual const std::string& GetName() const;

    // Reveal logic for the hidden state
    void Reveal() { revealed = true; }
    bool IsRevealed() const { return revealed; }

protected:
    // Announce the passive ability usage
    void AnnouncePassiveUsage(GameManager* gameManager);

    InvocationScript* invocationRef = nullptr;
private:
    bool revealed = false;
};
