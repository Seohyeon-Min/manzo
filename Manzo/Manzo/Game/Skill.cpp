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
    skill_net = new Skillsys::Skill_Net({ Ship_ptr->GetPosition() }, this);
    Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Add(skill_net);
}

void Skillsys::SkillLight()

{
    return;
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
        if (!skill_is_activted)
        {

            for (auto i : skillslots)
            {
                if (skillslots[i] != Empty)
                {
                    Active_skill(skillslots[i]);
                }
            }

        }
        skill_is_activted = true;
    }

    if (Engine::GetGameStateManager().GetStateName() == "Mode2")
    {
        skill_is_activted = false;

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
            std::cout << "Skill changed" << std::endl;
            Ready_to_set = false;
            is_slot_selected = false;
            setskill(Selected_slot, Selected_skill);
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
};


//list of Skill


// Net

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

void Skillsys::Skill_Net::Update(double [[maybe_unused]]dt)
{
        GameObject::Update(dt);
        SetPosition(ship_ptr->GetPosition());
        SetScale(ship_ptr->GetScale() * 1.5f);
        SetVelocity( ship_ptr->GetVelocity());
}

void Skillsys::Skill_Net::Draw()
{
    CS230::GameObject::Draw();
}

// End of Net
