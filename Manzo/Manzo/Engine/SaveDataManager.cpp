#include "SaveDataManager.h"
#include "Rapidjson.h"

SaveDataManager::SaveDataManager()
{
    json_reader = std::make_unique<JsonParser_save>();
}

SaveDataManager::SaveDataManager(const std::string& path)
    : saveFilePath(path)
{
    json_reader = std::make_unique<JsonParser_save>();
}

SaveDataManager::~SaveDataManager()
{
    json_reader = nullptr;
}

bool SaveDataManager::Load() {
    if (FileExists(saveFilePath)) {
        const std::string jsonfile = json_reader->LoadFromFile(saveFilePath);
        currentSaveData = json_reader->Deserialize(jsonfile);
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
    std::string jsonStr = json_reader->SerializeGameData(data);
    std::ofstream ofs(filePath, std::ios::binary);
    if (!ofs) {
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
        return false;
    }
    ofs << jsonStr;
    return true;
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

void SaveDataManager::UpdateSaveData(const SaveData& newData) {
    currentSaveData = newData;
    Save();
}

SaveData SaveDataManager::CreateDefaultSaveData() {
    SaveData defaultData;
    defaultData.day = 1;
    defaultData.money = 0;
    defaultData.eventsDone = {};
    //defaultData.fish = 0;
    //defaultData.inventory = {};
    return defaultData;
}
