#pragma once
#include "Skill.h"

void Skillsys::Active_skill(Skill_list skill)
{
    switch (skill)
    {
    case Empty:
        break;
    case Net:
        Skill_Net();
        break;
    case Light:
        Skill_Light();
        break;
    }
}

void Skillsys::Skill_Net()
{

}

void Skillsys::Skill_Light()
{

}

// Skillsystem

void Skillsys::Update(double dt)
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
            std::cout << "picked Empty" << std::endl;
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

void Skillsys::setskill(int slot, Skill_list skill)
{
    if (slot < 0 || slot >= skillslots.size())
    {
        std::cout << "invalid slot" << std::endl;
        return;
    }
    if (skillslots[slot] != 0 && skillslots[slot] == skill)
    {
        std::cout << "Same skill(didn't changed)" << std::endl;
        return;
    }
    for (int i = 0; i < skillslots.size(); ++i)
    {
        if (skillslots[i] == skill)
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