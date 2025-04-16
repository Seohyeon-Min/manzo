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


JsonParser_save::JsonParser_save()
{
}

std::string JsonParser_save::SerializeGameData(const SaveData& data)
{
    Document doc;
    doc.SetObject();
    Document::AllocatorType& alloc = doc.GetAllocator();

    doc.AddMember("day", data.day, alloc);

    //Value flags(kObjectType);

    Value inventory(kObjectType);
    inventory.AddMember("money", data.money, alloc);
    doc.AddMember("inventory", inventory, alloc);

    //// eventsDone 배열 생성 (이벤트 완료 내역 저장)
    //Value events(kArrayType);
    //for (const auto& evt : data.eventsDone) {       // 각 이벤트 문자열에 대해 반복
    //    Value eventStr;
    //    eventStr.SetString(evt.c_str(), alloc);       // 문자열 값을 할당, alloc을 사용해 문자열의 메모리를 할당
    //    events.PushBack(eventStr, alloc);             // 배열에 추가
    //}
    //doc.AddMember("eventsDone", events, alloc);       // 최상위에 "eventsDone" 배열 추가

    // JSON 객체를 문자열로 변환하는 단계
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);                               // Document의 내용을 Writer를 통해 StringBuffer에 기록

    return buffer.GetString();                        // 최종적으로 생성된 JSON 문자열 반환
}

SaveData JsonParser_save::Deserialize(const std::string& jsonStr)
{
    SaveData data;
    Document doc;
    doc.Parse(jsonStr.c_str());                       // JSON 문자열을 파싱하여 Document 객체로 생성
    if (doc.HasParseError()) {                        // 파싱 에러가 있다면 에러 메시지 출력 후 기본 data 반환
        std::cerr << "Error parsing JSON" << std::endl;
        return data;
    }

    // JSON 객체에서 "day" 값을 읽어서 data.day에 저장
    if (doc.HasMember("day") && doc["day"].IsInt())
        data.day = doc["day"].GetInt();

    //// "flags" 객체 내의 멤버들 읽기
    //if (doc.HasMember("flags") && doc["flags"].IsObject()) {
    //    const Value& flags = doc["flags"];
    //    if (flags.HasMember("tutorialComplete") && flags["tutorialComplete"].IsBool())
    //        data.tutorialComplete = flags["tutorialComplete"].GetBool();
    //    if (flags.HasMember("gotWeedkiller") && flags["gotWeedkiller"].IsBool())
    //        data.gotWeedkiller = flags["gotWeedkiller"].GetBool();
    //}

    // "inventory" 객체 내의 값 읽기
    if (doc.HasMember("inventory") && doc["inventory"].IsObject()) {
        const Value& inv = doc["inventory"];
        if (inv.HasMember("money") && inv["money"].IsInt())
            data.money = inv["money"].GetInt();
    }

    //// "eventsDone" 배열에서 각 문자열을 읽어 data.eventsDone에 push_back
    //if (doc.HasMember("eventsDone") && doc["eventsDone"].IsArray()) {
    //    const Value& events = doc["eventsDone"];
    //    for (SizeType i = 0; i < events.Size(); i++) {
    //        if (events[i].IsString())
    //            data.eventsDone.push_back(events[i].GetString());
    //    }
    //}

    return data;
}

const std::string JsonParser_save::LoadFromFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "";
    }

    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
}