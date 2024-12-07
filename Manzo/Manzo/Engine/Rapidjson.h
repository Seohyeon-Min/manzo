#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"

#include "vec2.h"
#include "Engine.h"
#include "Polygon.h"

#include <string>
#include <array>
#include <vector>
#include <fstream>
#include <iostream> 

using namespace rapidjson;


namespace CS230 {
    class JsonParser_boss : public CS230::Component {
    public:
        JsonParser_boss(std::string path);
        virtual ~JsonParser_boss() = default;

        inline std::string GetBossName() { return boss_name; }
        inline int GetIndex() { return index; }
        inline bool IsBossFight() { return is_boss_fight; }
        inline int GetBPM() { return bpm; }
        inline std::string GetMp3() { return mp3; }
        inline std::array<int, 2> GetMovePosition() { return move_position; }
        inline std::vector<std::vector<EntryData>> GetParttern() { return parttern; }
        inline std::vector<int> GetTotalEntry() { return total_entry; }

    private:
       
        std::string boss_name;
        int index;
        bool is_boss_fight;
        int bpm;
        std::string mp3;
        std::array<int, 2> move_position;
        std::vector<std::vector<EntryData>> parttern;
        std::vector<int> total_entry;

        Document document;
    };
}

namespace CS230 {
    class JsonParser_dialog : public CS230::Component {
    public:
        bool LoadLanguage_dialog(const std::string& language);
    private:


    };
}