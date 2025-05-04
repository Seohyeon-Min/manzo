#pragma once

#include "../Game/SaveData.h"

#include <string>
#include <fstream>
#include <iostream>

class JsonParser_save;

inline bool FileExists(const std::string& filePath) {
    std::ifstream file(filePath);
    return file.good();
}

class SaveDataManager {
public:
    explicit SaveDataManager();
    explicit SaveDataManager(const std::string& path);
    SaveDataManager(const SaveDataManager&) = delete;
    SaveDataManager& operator=(const SaveDataManager&) = delete;
    ~SaveDataManager();
    bool Load();
    bool Save();
    bool SaveToFile(const std::string& filePath, const SaveData& data) const;
    SaveData& GetSaveData();
    void SetFishData(int money, const std::map<int, int>& fishCollection);
    void SetModuleData(const ModuleData& m1, const ModuleData& m2);
    void UpdateSaveData(const SaveData& newData);

    std::map<int, int> GetFishCollection() { return currentSaveData.fishCollection; }
    int GetMoney() { return currentSaveData.money; }

private:
    std::string saveFilePath;
    SaveData currentSaveData;
    SaveData CreateDefaultSaveData();
    std::unique_ptr<JsonParser_save>json_reader;
};


    //bool BuyModule1() { return currentSaveData.module1.buy; }
    //bool BuyModule2() { return module2.buy; }
    //bool GetModule1() { return module1.set; }
    //bool GetModule2() { return module2.set; }

    //float GetModule1XPos() { return module1Pos; }
    //float GetModule2XPos() { return module2Pos; }   