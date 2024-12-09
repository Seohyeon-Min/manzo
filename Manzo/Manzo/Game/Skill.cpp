#pragma once
#include "Skill.h"
#include "..\Engine\GameObjectManager.h"

void Skillsys::Active_skill(Skill_list skill)
{
    switch (skill)
    {
    case Empty:
        break;
    case Net:
        SkillNet();
        break;
    case Light:
        SkillLight();
        break;
    }
}

void Skillsys::SkillNet()
{
    if (!skill_net)
    {
        skill_net = new Skillsys::Skill_Net({ Ship_ptr->GetPosition() }, this);
        Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Add(skill_net);
    }
}

void Skillsys::SkillLight()
{
    return;
}

void Skillsys::PrintInven()
{
    for (auto i : inventory)
    {
        std::cout << inventory[i] << "  ";
    }
    std::cout << std::endl;
}

// Skillsystem

void Skillsys::Update()
{
    if (Engine::GetGameStateManager().GetStateName() == "Mode1")
    {

        if (Check_ship_ptr == false)
        {
            if (Ship_ptr && skill_net)
            {
                skill_net->SetShipPtr(Ship_ptr);
                Check_ship_ptr = true;
            }
        }

        for (auto i : skillslots)
        {
            if (skillslots[i] != Empty)
            {
                Active_skill(skillslots[i]);
            }
        }
    }

    if (Engine::GetGameStateManager().GetStateName() == "Mode2")
    {

        if (is_slot_selected == false)
        {
            if (Engine::GetInput().KeyJustPressed(CS230::Input::Keys::A))
            {
                Selected_slot = 0;
                std::cout << "picked slot 1" << std::endl;
                is_slot_selected = true;
            }
            if (Engine::GetInput().KeyJustPressed(CS230::Input::Keys::S))
            {
                Selected_slot = 1;
                std::cout << "picked slot 2" << std::endl;
                is_slot_selected = true;
            }
            if (Engine::GetInput().KeyJustPressed(CS230::Input::Keys::D))
            {
                Selected_slot = 2;
                std::cout << "picked slot 3" << std::endl;
                is_slot_selected = true;
            }
        }

        if (is_slot_selected == true)
        {

            if (Engine::GetInput().KeyJustPressed(CS230::Input::Keys::Num1))
            {
                Selected_skill = Net;
                std::cout << "picked Net" << std::endl;

                Ready_to_set = true;
            }
            if (Engine::GetInput().KeyJustPressed(CS230::Input::Keys::Num2))
            {
                Selected_skill = Light;
                std::cout << "picked Light" << std::endl;
                Ready_to_set = true;
            }
            if (Engine::GetInput().KeyJustPressed(CS230::Input::Keys::Num3))
            {
                Selected_skill = Empty;
                std::cout << "Cleared" << std::endl;
                Ready_to_set = true;
            }
        }

        if (Ready_to_set)
        {
            bool IsinInven = false;
            for (const auto& skill : inventory)
            {
                if (Selected_skill == skill)
                {
                    std::cout << "Skill changed" << std::endl;
                    Ready_to_set = false;
                    is_slot_selected = false;
                    setskill(Selected_slot, Selected_skill);
                    IsinInven = true;
                    return;
                }
            }
            std::cout << "It doesn't exist in inventory" << std::endl;
            Ready_to_set = false;
            is_slot_selected = false;
        }

        if (Engine::GetInput().KeyJustPressed(CS230::Input::Keys::Z))
        {
            skillprint();
        }
    }
}

void Skillsys::setskill(int slot, Skill_list skill)
{
    if (slot < 0 || slot >= skillslots.size())
    {
        std::cout << "invalid slot" << std::endl;
        return;
    }
    if (skillslots[slot] != Empty && skillslots[slot] == skill)
    {
        std::cout << "Same skill(didn't changed)" << std::endl;
        return;
    }
    for (int i = 0; i < skillslots.size(); ++i)
    {
        if (skillslots[i] == skill && skillslots[i] != Empty)
        {
            std::cout << "Already set skill" << std::endl;
            return;
        }
    }

    skillslots[slot] = skill;
}

void Skillsys::skillprint()
{
    for (int i = 0; i < skillslots.size(); ++i)
    {
        if (skillslots[i] != 0)
        {
            switch (skillslots[i])
            {
            case Net:
                std::cout << "Slot " << i + 1 << ": Net\n";
                break;
            case Light:
                std::cout << "Slot " << i + 1 << ": Light\n";
                break;
            case GARRY:
                std::cout << "Slot " << i + 1 << ": GARRY\n";
                break;
            }
        }
        else
        {
            std::cout << "Slot " << i + 1 << ": Empty\n";
        }
    }
}

void Skillsys::ClearSkill()
{
    for (int i = 0; i < skillslots.size(); ++i)
    {
        skillslots[i] = Empty;
    }
}

void Skillsys::SetShipPtr(Ship* ptr)
{
    Ship_ptr = ptr;
}

Ship* Skillsys::GetShipPtr()
{
    return Ship_ptr;
}
void Skillsys::AddSkill(Skill_list input, int money)
{
    bool IsPurchase = false;

    for (const auto& skill : inventory)
    {
        if (skill == input)
        {
            IsPurchase = true;
            std::cout << "Already Purchased" << std::endl;
            return;
        }
    }

    if(!IsPurchase)
    {
        if (Engine::GetGameStateManager().GetGSComponent<Fish>()->GetMoney() >= money)
        {
            inventory.push_back(input);
            Engine::GetGameStateManager().GetGSComponent<Fish>()->SetMoney(Engine::GetGameStateManager().GetGSComponent<Fish>()->GetMoney() - money);
            std::cout << "Purchase" << std::endl;
            std::cout << "Left money : " << Engine::GetGameStateManager().GetGSComponent<Fish>()->GetMoney() << std::endl;
        }
        else
        {
            std::cout << "Not enough money" << std::endl;
        }
    }
}
void Skillsys::RemoveSkill(Skill_list input, int money)
{
    bool IsSell = false;
    for (int i = 0; i < inventory.size(); i++)
    {
        if (inventory[i] == input)
        {
            inventory[i] = Empty;
            Engine::GetGameStateManager().GetGSComponent<Fish>()->SetMoney(Engine::GetGameStateManager().GetGSComponent<Fish>()->GetMoney() + money);
            std::cout << "Sell complete" << std::endl;
            std::cout << "Left money: " << Engine::GetGameStateManager().GetGSComponent<Fish>()->GetMoney() << std::endl;
            IsSell = true;
        }
    }
    if (!IsSell)
    {
        std::cout << "Error from sell" << std::endl;
    }

}
Skillsys::Skill_list Skillsys::Change_number_to_list(int input)
{
    return static_cast<Skill_list>(input);
}
;

Skillsys::Skill_Net::Skill_Net(vec2 position, Skillsys* skillsys) : GameObject(position)
{
    ship_ptr = skillsys->GetShipPtr();
    SetPosition({ ship_ptr->GetPosition() });
    SetScale({ ship_ptr->GetScale() });
    AddGOComponent(new CS230::Sprite("assets/images/ship.spt", this));
};


bool Skillsys::Skill_Net::CanCollideWith(GameObjectTypes other_objects)
{
    switch (other_objects)
    {
    case GameObjectTypes::Fish:
        return true;
        break;
    }
    return false;
}

void Skillsys::Skill_Net::Update(double [[maybe_unused]] dt)
{
    GameObject::Update(dt);
    SetPosition(ship_ptr->GetPosition());
    SetScale(ship_ptr->GetScale() * 1.5f);
    SetVelocity(ship_ptr->GetVelocity());
}

void Skillsys::Skill_Net::Draw()
{
    CS230::GameObject::Draw();
}