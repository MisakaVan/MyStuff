//
// Created by Misakavan on 2023/7/8.
//

#include "UserPlayer.h"
#include "Session.h"

UserPlayer::UserPlayer() : Player()
{
    typeID = player_T::user;
}

/*void UserPlayer::checkOkAndReplaceActingPet()
{
    Player::checkOkAndReplaceActingPet();
}*/

UserPlayer::UserPlayer(const Player::nameType &name, const Player::nameType &nameMy) : Player(name, nameMy) {
    typeID = player_T::user;
}

UserPlayer::~UserPlayer()
= default;

void UserPlayer::makeIntention()
{
    /*
     * Select your action (0 for escape, 1 for skill, 2 for change):
     * Select the skill (0 for back, 1 for Tackle, 2 for Leaf, 3 for Synthesis):
     * Select your next pet (0 for back, 2 for L-HP80, 3 for G-HP82):
     *
     * */
    using namespace std;
    enum uiState
    {
        selectAction,
        selectSkill,
        selectChange,
        end
    };

    bool canChange = hasOkPet();
    /* Prepare for selecting action */
    vector<string> selectActionOptions = {"escape", "skill"};
    vector<int> selectActionOrderNums = {0, 1};
    if (canChange) { selectActionOrderNums.push_back(2), selectActionOptions.emplace_back("change"); }
    std::string promptSelectAction = "Select your action "
                                     + makeEnumOptionsWithBracket(selectActionOrderNums, selectActionOptions)
                                     + ": ";

    /* Prepare for selecting skill */
    const int skillCount = static_cast<int>(getActingPet()->getSkillListSize());
    vector<string> selectSkillOptions{"back"};
    vector<int> selectSkillOrderNums = makeRange(0, skillCount + 1);
    for (int skillIndex = 0; skillIndex < skillCount; ++skillIndex) {
        selectSkillOptions.emplace_back(getActingPet()->getSkill(skillIndex)->name());
    }
    std::string promptSelectSkill = "Select the skill "
                                    + makeEnumOptionsWithBracket(selectSkillOrderNums,
                                                                 selectSkillOptions)
                                    + ": ";

    /* Prepare for selecting change */
    vector<string> selectChangeOptions{"back"};
    vector<int> selectChangeOrderNums{0};
    const int petCount = static_cast<int>(_petList.size());
    for (int petIndex = 0; petIndex < petCount; ++petIndex) {
        if (_actingIndex == petIndex) {
            continue;
        }
        selectChangeOptions.emplace_back(asStrHpPair(getPet(petIndex)));
        selectChangeOrderNums.emplace_back(petIndex + 1);
    }
    std::string promptSelectChange = "Select your next pet "
                                     + makeEnumOptionsWithBracket(selectChangeOrderNums, selectChangeOptions)
                                     + ": ";

    /* Interact with user input */
    uiState state = selectAction;
    int userInput = -1;
    while (state != end) {
        switch (state) {
            case selectAction: {
                userInput = selectFrom(promptSelectAction, selectActionOrderNums);
                if (userInput == 0) {
                    // escape
                    setNextIntention(makeEscapeIntention(this));
                    state = end;
                    break;
                }
                state = (userInput == 1 ? selectSkill : selectChange);
            }
                break;

            case selectSkill: {
                userInput = selectFrom(promptSelectSkill, selectSkillOrderNums);
                if (userInput == 0) {
                    // back
                    state = selectAction;
                    break;
                }
                // have selected a skill.
                setNextIntention(makeSkillIntention(
                        this,
                        getActingPet(),
                        getActingPet()->getSkill(userInput - 1)));
                state = end;
            }
                break;

            case selectChange: {
                userInput = selectFrom(promptSelectChange, selectChangeOrderNums);
                if (userInput == 0) {
                    state = selectAction;
                    break;
                }
                // have selected a pet.
                setNextIntention(makeChangeIntention(
                        this,
                        getPet(userInput - 1)));
                state = end;
            }
                break;

            case end:
                break;
        }
    }
}

/*
actIntentionResult_T UserPlayer::actIntention()
{
    return Player::actIntention();
}
*/

void UserPlayer::selectInitPet()
{
    /* You can select three from five (1 for W, 2 for L, 3 for G, 4 for Q, 5 for D).
     * Select your 1st pet: 6
     * Select your 1st pet: 4
     * Select your 2nd pet: 4
     * Select your 2nd pet: 5
     * Select your 3rd pet: 2
     * You have Q, D and L.
     */
    using namespace std;
    auto &is = getSession()->is;
    auto &os = getSession()->os;
    os << "You can select three from five "
       << makeEnumOptionsWithBracket({1, 2, 3, 4, 5}, PET_NAME_LIST) << ".\n";

    vector<string> selection{};
    vector<string> num_th{"1st", "2nd", "3rd"};
    std::set<int> options{1, 2, 3, 4, 5};
    int selecting = 0;
    while (selecting < 3) {
        auto select1 = readInt("Select your " + num_th[selecting] + " pet: ", options);
        selection.push_back(PET_NAME_LIST.at(select1 - 1));
        options.erase(options.find(select1));
        ++selecting;
    }
    // selection complete
    os << "You have " << makeListWithAnd(selection) << ".\n";

    // Make pets and set acting pet as the first chosen
    for (const auto &name: selection) {
        appendPet(makePet(name));
    }
    setActingPet(0);
}

/// "Select your next pet (2 for L-HP**, 3 for G-HP**): *"
Player::petIndex UserPlayer::getNextPetPreference()
{
    /* Basically copied from makeIntention() */
    using namespace std;
    vector<string> selectChangeOptions{};
    vector<int> selectChangeOrderNums{};
    const int petCount = static_cast<int>(_petList.size());
    for (int petIndex = 0; petIndex < petCount; ++petIndex) {
        auto thisPet = getPet(petIndex);
        if (_actingIndex == petIndex || !thisPet->isOK()) {
            continue;
        }
        selectChangeOptions.emplace_back(asStrHpPair(thisPet));
        selectChangeOrderNums.emplace_back(petIndex + 1);
    }

    if (selectChangeOptions.empty()) {
        //todo
        return -1;
    }

    std::string promptSelectChange = "Select your next pet "
                                     + makeEnumOptionsWithBracket(selectChangeOrderNums, selectChangeOptions)
                                     + ": ";
    auto userInput = selectFrom(promptSelectChange, selectChangeOrderNums);
    return userInput - 1;
}


