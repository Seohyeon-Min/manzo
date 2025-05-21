#include "TextSaveParser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

namespace Base64 {
    static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

    std::string Encode(const std::string& input) {
        std::string encoded;
        int val = 0, valb = -6;
        for (uint8_t c : input) {
            val = (val << 8) + c;
            valb += 8;
            while (valb >= 0) {
                encoded.push_back(base64_chars[(val >> valb) & 0x3F]);
                valb -= 6;
            }
        }
        if (valb > -6)
            encoded.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
        while (encoded.size() % 4)
            encoded.push_back('=');
        return encoded;
    }

    std::string Decode(const std::string& input) {
        std::vector<int> T(256, -1);
        for (int i = 0; i < 64; i++) T[base64_chars[i]] = i;
        std::string decoded;
        int val = 0, valb = -8;
        for (uint8_t c : input) {
            if (T[c] == -1) break;
            val = (val << 6) + T[c];
            valb += 6;
            if (valb >= 0) {
                decoded.push_back(char((val >> valb) & 0xFF));
                valb -= 8;
            }
        }
        return decoded;
    }
}



//TextSaveParser::TextSaveParser() {}

std::string TextSaveParser::Serialize(const SaveData& data) {
    std::ostringstream oss;

    oss << "day=" << data.day << "\n";
    oss << "user_calibration=" << data.user_calibration << "\n";
    oss << "money=" << data.money << "\n";

    for (const auto& [type, count] : data.fishCollection) {
        oss << "fish_" << type << "=" << count << "\n";
    }

    oss << "module1_buy=" << data.module1.buy << "\n";
    oss << "module1_set=" << data.module1.set << "\n";
    oss << "module1_pos=" << data.module1.pos << "\n";

    oss << "module2_buy=" << data.module2.buy << "\n";
    oss << "module2_set=" << data.module2.set << "\n";
    oss << "module2_pos=" << data.module2.pos << "\n";

    for (const auto& e : data.eventsDone) {
        oss << "event=" << e << "\n";
    }

    std::string plain = oss.str();
    return Base64::Encode(plain);
}

SaveData TextSaveParser::Deserialize(const std::string& encodedText) {
    SaveData data;
    std::string decoded = Base64::Decode(encodedText);
    std::istringstream iss(decoded);
    std::string line;

    while (std::getline(iss, line)) {
        auto sep = line.find('=');
        if (sep == std::string::npos) continue;

        std::string key = line.substr(0, sep);
        std::string value = line.substr(sep + 1);

        if (key == "day") data.day = std::stoi(value);
        else if (key == "user_calibration") data.user_calibration = std::stod(value);
        else if (key == "money") data.money = std::stoi(value);
        else if (key.starts_with("fish_")) {
            int type = std::stoi(key.substr(5));
            int count = std::stoi(value);
            data.fishCollection[type] = count;
        }
        else if (key == "module1_buy") data.module1.buy = (value == "1");
        else if (key == "module1_set") data.module1.set = (value == "1");
        else if (key == "module1_pos") data.module1.pos = std::stof(value);
        else if (key == "module2_buy") data.module2.buy = (value == "1");
        else if (key == "module2_set") data.module2.set = (value == "1");
        else if (key == "module2_pos") data.module2.pos = std::stof(value);
        else if (key == "event") data.eventsDone.push_back(value);
    }

    return data;
}

std::string TextSaveParser::LoadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "[LoadFromFile] Failed to open: " << path << std::endl;
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

bool TextSaveParser::SaveToFile(const std::string& path, const std::string& encodedText) {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "[SaveToFile] Failed to write: " << path << std::endl;
        return 0;
    }
    file << encodedText;
    return 1;
}
