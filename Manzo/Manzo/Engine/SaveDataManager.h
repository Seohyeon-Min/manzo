#pragma once

#include "../Game/SaveData.h"  // SaveData ����ü �� ���� �Լ� ���� ����

#include <string>
#include <fstream>
#include <iostream>

class JsonParser_save;

// ���� ���� ���� Ȯ�� �Լ� (inline �Լ��� ����� ����)
inline bool FileExists(const std::string& filePath) {
    std::ifstream file(filePath);
    return file.good();
}

class SaveDataManager {
public:
    explicit SaveDataManager(const std::string& path);
    ~SaveDataManager();
    bool Load();
    bool Save();
    bool SaveToFile(const std::string& filePath, const SaveData& data) const;
    SaveData& GetSaveData();
    void UpdateSaveData(const SaveData& newData);

private:
    std::string saveFilePath;
    SaveData currentSaveData;
    SaveData CreateDefaultSaveData();
    JsonParser_save* json_reader;
};


// ���� ����
//int main() {
//    // ���� ��δ� ������ ���� (��: assets/images/jsons/save.json)
//    std::string saveFilePath = "assets/images/jsons/save.json";
//
//    SaveDataManager saveManager(saveFilePath);
//
//    // �̹� ����� �����͸� �ҷ��� ��, �ʿ�� ���� �������� ���
//    SaveData& data = saveManager.GetSaveData();
//    std::cout << "Current Day: " << data.day << "\nMoney: " << data.money << std::endl;
//
//    // ���� ���, �̺�Ʈ�� �Ϸ����� �� ������ ���� �� ������Ʈ:
//    data.money += 500;
//    data.fish += 2;
//    saveManager.UpdateSaveData(data);
//
//    return 0;
//}
