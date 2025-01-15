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
        skill_net = new Skillsys::Skill_Net({ ship_ptr->GetPosition() }, this);
        Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(skill_net);
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

        if (check_ship_ptr == false)
        {
            if (ship_ptr && skill_net)
            {
                skill_net->SetShipPtr(ship_ptr);
                check_ship_ptr = true;
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
            if (Engine::GetInput().KeyJustPressed(Input::Keys::A))
            {
                selected_slot = 0;
                std::cout << "picked slot 1" << std::endl;
                is_slot_selected = true;
            }
            if (Engine::GetInput().KeyJustPressed(Input::Keys::S))
            {
                selected_slot = 1;
                std::cout << "picked slot 2" << std::endl;
                is_slot_selected = true;
            }
            if (Engine::GetInput().KeyJustPressed(Input::Keys::D))
            {
                selected_slot = 2;
                std::cout << "picked slot 3" << std::endl;
                is_slot_selected = true;
            }
        }

        if (is_slot_selected == true)
        {

            if (Engine::GetInput().KeyJustPressed(Input::Keys::Num1))
            {
                selected_skill = Net;
                std::cout << "picked Net" << std::endl;

                ready_to_set = true;
            }
            if (Engine::GetInput().KeyJustPressed(Input::Keys::Num2))
            {
                selected_skill = Light;
                std::cout << "picked Light" << std::endl;
                ready_to_set = true;
            }
            if (Engine::GetInput().KeyJustPressed(Input::Keys::Num3))
            {
                selected_skill = Empty;
                std::cout << "Cleared" << std::endl;
                ready_to_set = true;
            }
        }

        if (ready_to_set)
        {
            bool IsinInven = false;
            for (const auto& skill : inventory)
            {
                if (selected_skill == skill)
                {
                    std::cout << "Skill changed" << std::endl;
                    ready_to_set = false;
                    is_slot_selected = false;
                    SetSkill(selected_slot, selected_skill);
                    IsinInven = true;
                    return;
                }
            }
            std::cout << "It doesn't exist in inventory" << std::endl;
            ready_to_set = false;
            is_slot_selected = false;
        }

        if (Engine::GetInput().KeyJustPressed(Input::Keys::Z))
        {
            SkillPrint();
        }
    }
}

void Skillsys::SkillPrint()
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
            case TEMP1:
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
    ship_ptr = ptr;
}

Ship* Skillsys::GetShipPtr()
{
    return ship_ptr;
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

Skillsys::Skill_Net::Skill_Net(vec2 position, Skillsys* skillsys) : GameObject(position)
{
    ship_ptr = skillsys->GetShipPtr();
    SetPosition({ ship_ptr->GetPosition() });
    SetScale({ ship_ptr->GetScale() });
    AddGOComponent(new Sprite("assets/images/ship.spt", this));
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
    GameObject::Draw();
}