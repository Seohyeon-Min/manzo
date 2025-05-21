#include "SaveDataManager.h"
#include "TextSaveParser.h"  // <- 새 파서 사용
#include "Engine.h"

SaveDataManager::SaveDataManager() {
    text_parser = std::make_unique<TextSaveParser>();
}

SaveDataManager::SaveDataManager(const std::string& path)
    : saveFilePath(path) {
    text_parser = std::make_unique<TextSaveParser>();
}

SaveDataManager::~SaveDataManager() {
    text_parser = nullptr;
}

bool SaveDataManager::Load() {
    if (FileExists(saveFilePath)) {
        const std::string encodedText = text_parser->LoadFromFile(saveFilePath);
        currentSaveData = text_parser->Deserialize(encodedText);
        std::cout << "Save data loaded successfully." << std::endl;
        return true;
    }
    else {
        std::cout << "Save file does not exist. Creating default save data." << std::endl;
        currentSaveData = CreateDefaultSaveData();
        Save();
        return false;
    }
}

bool SaveDataManager::SaveToFile(const std::string& filePath, const SaveData& data) const {
    std::string encoded = text_parser->Serialize(data);
    return text_parser->SaveToFile(filePath, encoded);
}

bool SaveDataManager::Save() {
    if (SaveToFile(saveFilePath, currentSaveData)) {
        std::cout << "Save data saved successfully." << std::endl;
        return true;
    }
    else {
        std::cerr << "Failed to save data." << std::endl;
        return false;
    }
}

SaveData& SaveDataManager::GetSaveData() {
    return currentSaveData;
}

void SaveDataManager::SetFishData(int money, const std::map<int, int>& fishCollection) {
    currentSaveData.money = money;
    currentSaveData.fishCollection = fishCollection;
}

void SaveDataManager::SetModuleData(const ModuleData& m1, const ModuleData& m2) {
    currentSaveData.module1 = m1;
    currentSaveData.module2 = m2;
}

void SaveDataManager::UpdateSaveData(const SaveData& newData) {
    currentSaveData = newData;
    std::cout << "[SaveDataManager] UpdateSaveData called\n";
    Save();
}

SaveData SaveDataManager::CreateDefaultSaveData() {
    SaveData defaultData;
    defaultData.day = 1;
    defaultData.money = 0;
    defaultData.eventsDone = {};
    return defaultData;
}
