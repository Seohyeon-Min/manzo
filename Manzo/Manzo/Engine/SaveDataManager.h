#pragma once

#include "../Game/SaveData.h"  // SaveData 구조체 및 관련 함수 선언 포함

#include <string>
#include <fstream>
#include <iostream>

class JsonParser_save;

// 파일 존재 여부 확인 함수 (inline 함수로 헤더에 정의)
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

    //bool BuyModule1() { return currentSaveData.module1.buy; }
    //bool BuyModule2() { return module2.buy; }
    //bool GetModule1() { return module1.set; }
    //bool GetModule2() { return module2.set; }

    //float GetModule1XPos() { return module1Pos; }
    //float GetModule2XPos() { return module2Pos; }   

private:
    std::string saveFilePath;
    SaveData currentSaveData;
    SaveData CreateDefaultSaveData();
    std::unique_ptr<JsonParser_save>json_reader;
};


// 예시 사용법
//int main() {
//    // 파일 경로는 적절히 지정 (예: assets/images/jsons/save.json)
//    std::string saveFilePath = "assets/images/jsons/save.json";
//
//    SaveDataManager saveManager(saveFilePath);
//
//    // 이미 저장된 데이터를 불러온 후, 필요시 게임 로직에서 사용
//    SaveData& data = saveManager.GetSaveData();
//    std::cout << "Current Day: " << data.day << "\nMoney: " << data.money << std::endl;
//
//    // 예를 들어, 이벤트를 완료했을 때 데이터 수정 후 업데이트:
//    data.money += 500;
//    data.fish += 2;
//    saveManager.UpdateSaveData(data);
//
//    return 0;
//}
