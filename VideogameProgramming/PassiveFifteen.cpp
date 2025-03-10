#include "PassiveFifteen.h"
#include "GameManager.h"
#include "InvocationScript.h"
#include "CellScript.h"
#include "PassiveDescriptions.h"
#include <iostream>
#include <vector>

void PassiveFifteen::Activate(GameManager* gameManager, InvocationScript* invocation) {
    // Passive activates when the invocation is summoned
    std::cout << "PassiveFifteen activated: Tracking turns survived." << std::endl;
    turnsSurvived = 0;  // Reset turn counter
}

void PassiveFifteen::OnTurnEnd(GameManager* gameManager, InvocationScript* invocation) {
    if (!gameManager || !invocation) return;

    turnsSurvived++;
    std::cout << "PassiveFifteen: Turn survived " << turnsSurvived << "/3" << std::endl;
}

void PassiveFifteen::ModifyMovableCells(GameManager* gameManager, InvocationScript* invocation, std::vector<CellScript*>& movableCellScripts, std::vector<Entity*>& movableCells) {
    if (!gameManager || !invocation) return;

    // **If the invocation has survived 3 turns, add conquerable cells as possible moves**
    if (turnsSurvived == 3) {
        std::cout << "PassiveFifteen: Adding conquerable cells as possible moves." << std::endl;

        auto allCellScripts = gameManager->getAllCellScripts();

        for (auto* cell : gameManager->getAllCellScripts()) {
            if (cell->getIsConquerable()) {
                movableCellScripts.push_back(cell); 
                movableCells.push_back(cell->getCellEntity()); 

                auto it = std::find(allCellScripts.begin(), allCellScripts.end(), cell);
                if (it != allCellScripts.end()) {
                    int index = std::distance(allCellScripts.begin(), it);
                    gameManager->highlightSprite(index);  
                }
            }
        }

        AnnouncePassiveUsage(gameManager, invocation);
        Reveal();
    }
}

const std::string& PassiveFifteen::GetDescription() const {
    static const std::string description = PassiveDescriptions::PassiveFifteen;
    return description;
}