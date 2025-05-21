#pragma once

#include <string>
#include <map>
#include "../Game/SaveData.h"

class TextSaveParser {
public:
    TextSaveParser() = default;
    std::string Serialize(const SaveData& data);
    SaveData Deserialize(const std::string& encodedStr);
    bool SaveToFile(const std::string& path, const std::string& encodedText);

    std::string LoadFromFile(const std::string& filePath);

private:
    std::string EncodeBase64(const std::string& input);
    std::string DecodeBase64(const std::string& input);

    std::map<std::string, std::string> ParseLines(const std::string& text);
};
