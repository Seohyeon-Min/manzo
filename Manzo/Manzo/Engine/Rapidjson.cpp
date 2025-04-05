#include "Rapidjson.h"
#include "Polygon.h"

// For rapidjson header
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"

#include <string>
#include <vector>
#include <fstream>

// Namespace for rapidjson
using namespace rapidjson;

JsonParser_boss::JsonParser_boss(std::string path)
    : boss_name(""), index(0), is_boss_fight(false), bpm(0), mp3(""),
    move_position({ 0, 0 }), parttern(), total_entry() {

    FILE* fptr = nullptr;
    errno_t err = fopen_s(&fptr, path.c_str(), "rb");
    if (err != 0 || fptr == nullptr) {
        Engine::GetLogger().LogError("Failed to open " + path + " file");
        return;
    }

    char readbuffer[65536];
    FileReadStream is(fptr, readbuffer, sizeof(readbuffer));

    // Ensure `document` is initialized
    Document document;
    document.ParseStream(is);
    fclose(fptr);

    if (document.HasParseError()) {
        Engine::GetLogger().LogError("JSON parse error");
        return;
    }

    if (document.HasMember("Boss name") && document["Boss name"].IsString()) {
        boss_name = document["Boss name"].GetString();
    }

    if (document.HasMember("Index") && document["Index"].IsInt()) {
        index = document["Index"].GetInt();
    }

    if (document.HasMember("Isbossfight") && document["Isbossfight"].IsBool()) {
        is_boss_fight = document["Isbossfight"].GetBool();
    }

    if (document.HasMember("BPM") && document["BPM"].IsInt()) {
        bpm = document["BPM"].GetInt();
    }

    if (document.HasMember("Mp3") && document["Mp3"].IsString()) {
        mp3 = document["Mp3"].GetString();
    }

    if (document.HasMember("Position") && document["Position"].IsArray()) {
        const auto& posArray = document["Position"].GetArray();
        if (posArray.Size() == 2) {
            move_position[0] = posArray[0].GetInt();
            move_position[1] = posArray[1].GetInt();
        }
    }

    if (document.HasMember("Total entry") && document["Total entry"].IsArray()) {
        const auto& totalEntryArray = document["Total entry"].GetArray();
        for (const auto& entry : totalEntryArray) {
            if (entry.IsInt()) {
                total_entry.push_back(entry.GetInt());
            }
        }
    }

    if (document.HasMember("Parttern") && document["Parttern"].IsArray()) {
        const auto& partternArray = document["Parttern"].GetArray();
        for (const auto& entry : partternArray) {
            if (entry.IsObject() && entry.HasMember("entry") && entry["entry"].IsArray()) {
                std::vector<EntryData> entryVec;
                const auto& entryList = entry["entry"].GetArray();
                for (const auto& pointsArray : entryList) {
                    if (pointsArray.IsArray() && pointsArray.Size() == 4) {
                        EntryData entryData;
                        entryData.attacktype = pointsArray[0].GetFloat();
                        entryData.position = vec2(pointsArray[1].GetFloat(), pointsArray[2].GetFloat());
                        entryData.delay = pointsArray[3].GetFloat();
                        entryVec.push_back(entryData);
                    }
                }
                parttern.push_back(entryVec);
            }
        }
    }
}


JsonParser_dialog::JsonParser_dialog(const std::string& language) {
    std::string filePath = "assets/jsons/dialog/" + language + ".json";
    FILE* fptr = nullptr;
    errno_t err = fopen_s(&fptr, filePath.c_str(), "rb");
    if (err != 0 || fptr == nullptr) {
        std::cout << "Could not open JSON file.\n";
        return;
    }

    char buffer[65536];
    rapidjson::FileReadStream is(fptr, buffer, sizeof(buffer));
    rapidjson::Document doc;
    doc.ParseStream(is);
    fclose(fptr);

    if (doc.HasParseError() || !doc.IsObject()) {
        std::cout << "JSON parse error.\n";
        return;
    }

    translations.clear();
    characters.clear();
    groupedDialog.clear();

    for (auto it = doc.MemberBegin(); it != doc.MemberEnd(); ++it) {
        const std::string group_id = it->name.GetString();
        const auto& dialogArray = it->value;

        if (!dialogArray.IsArray()) continue;

        std::vector<std::pair<std::string, std::string>> groupLines;

        for (auto& entry : dialogArray.GetArray()) {
            const std::string character = entry["character"].GetString();

            std::string text;
            if (entry["text"].IsArray()) {
                const auto& textArray = entry["text"];
                int randIndex = rand() % textArray.Size();
                text = textArray[randIndex].GetString();
            }
            else {
                text = entry["text"].GetString();
            }

            groupLines.emplace_back(character, text);
        }

        groupedDialog[group_id] = groupLines;
    }
}

const std::vector<std::pair<std::string, std::string>>& JsonParser_dialog::GetDialogGroup(const std::string& group_id) const {
    static const std::vector<std::pair<std::string, std::string>> empty;
    auto it = groupedDialog.find(group_id);
    return it != groupedDialog.end() ? it->second : empty;
}

const std::string& JsonParser_dialog::GetText(const std::string& id)const {

    static const std::string empty = "";
    auto it = translations.find(id);
    return it != translations.end() ? it->second : empty;
}

const std::string& JsonParser_dialog::GetCharacter(const std::string& id)const {
    static const std::string empty = "";
    auto it = characters.find(id);
    return it != characters.end() ? it->second : empty;
}