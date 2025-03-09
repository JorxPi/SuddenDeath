#pragma once
#include <string>
#include <iostream>

// Forward declarations
class GameManager;
class InvocationScript;

class PassiveAbility {
public:
    virtual ~PassiveAbility() = default;

    // Called to activate the passive when triggered
    virtual void Activate(GameManager* gameManager, InvocationScript* invocation) = 0;

    // Optional: Called to react to specific game events
    virtual void OnEvent(GameManager* gameManager, InvocationScript* invocation) {}

    // Add virtual to enable overriding in derived classes
    virtual void OnInvocationDeath() {}

    // Description for debugging and revealing
    virtual const std::string& GetDescription() const;

    // Reveal logic for the hidden state
    void Reveal() { revealed = true; }
    bool IsRevealed() const { return revealed; }

protected:
    // Announce the passive ability usage
    void AnnouncePassiveUsage(GameManager* gameManager, InvocationScript* invocation);

private:
    bool revealed = false;
};
