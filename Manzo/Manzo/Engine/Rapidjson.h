#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"

#include <string>
#include <array>
#include <vector>
#include <fstream>
#include <iostream> // For logging/debugging
#include "vec2.h"
#include "Engine.h"

using namespace rapidjson;

namespace CS230 {
    class JsonParser : public CS230::Component {
    public:
        JsonParser(std::string path);
        virtual ~JsonParser() = default;

        inline std::string GetBossName() { return boss_name; }
        inline int GetIndex() { return index; }
        inline bool IsBossFight() { return is_boss_fight; }
        inline int GetBPM() { return bpm; }
        inline std::string GetMp3() { return mp3; }
        inline std::array<int, 2> GetMovePosition() { return move_position; }
        inline std::vector<std::vector<std::vector<float>>> GetParttern() { return parttern; }

    private:
        Document document;

        std::string boss_name;
        int index;
        bool is_boss_fight;
        int bpm;
        std::string mp3;
        std::array<int, 2> move_position;
        std::vector<std::vector<std::vector<float>>> parttern;
    };

    JsonParser::JsonParser(std::string path)
        : boss_name(""), index(0), is_boss_fight(false), bpm(0), mp3(""),
        move_position({ 0, 0 }), parttern() {

        FILE* fptr = nullptr;
        errno_t err = fopen_s(&fptr, path.c_str(), "rb");
        if (err != 0 || fptr == nullptr) {
            Engine::GetLogger().LogError("Failed to open JSON file");
            return;
        }

        char readbuffer[65536];
        FileReadStream is(fptr, readbuffer, sizeof(readbuffer));

        document.ParseStream(is);
        fclose(fptr);

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

        if (document.HasMember("Parttern") && document["Parttern"].IsArray()) {
            const auto& partternArray = document["Parttern"].GetArray();
            for (const auto& entry : partternArray) {
                if (entry.IsObject() && entry.HasMember("entry") && entry["entry"].IsArray()) {
                    std::vector<std::vector<float>> entryVec;
                    const auto& entryList = entry["entry"].GetArray();
                    for (const auto& pointsArray : entryList) {
                        if (pointsArray.IsArray()) {
                            std::vector<float> beat_belay;
                            for (const auto& point : pointsArray.GetArray()) {
                                if (point.IsNumber()) {
                                    beat_belay.push_back(point.GetFloat());
                                }
                            }
                            entryVec.push_back(beat_belay);
                        }
                    }
                    parttern.push_back(entryVec);
                }
            }
        }
    }
}
