#pragma once

#include "../Game/SaveData.h"
#include "TextSaveParser.h"

#include <string>
#include <fstream>
#include <iostream>
#include <memory>

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

    std::map<int, int> GetFishCollection() const { return currentSaveData.fishCollection; }
    int GetMoney() const { return currentSaveData.money; }

private:
    std::string saveFilePath;
    SaveData currentSaveData;
    SaveData CreateDefaultSaveData();
    std::unique_ptr<TextSaveParser> text_parser;
};
