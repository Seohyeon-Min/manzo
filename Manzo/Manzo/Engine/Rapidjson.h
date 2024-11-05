#pragma once
//for rapidjson header
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "filereadstream.h"

#include <string>
#include <array>
#include <vector>
#include <fstream>
#include "vec2.h"
#include "Engine.h"

using namespace rapidjson;

namespace CS230{
	class JsonParser : public CS230::Component{
	public:
		JsonParser(std::string path);
		virtual ~JsonParser() = default;
		inline std::string Get_bossname() { return boss_name; }
		inline std::array <int, 2> Get_moveposition() { return move_position; }
		void append_employee(std::string name);
		//we can add more if we need

	private:
		Document document;

		std::string boss_name;
		std::array <int, 2> move_position;
	};

	JsonParser::JsonParser(std::string path) {
		FILE* fptr = fopen(path.c_str(), "rb");
		if (fptr == nullptr) {
			Engine::GetLogger().LogError("Json File is not exist");
			return;
		}
		char readbuffer[65536];
		FileReadStream is(fptr, readbuffer, sizeof(readbuffer));

		document.ParseStream(is);

		if (document.HasMember("boss name")) {
			boss_name = document["boss name"].GetString();
		}

		if (document.HasMember("move position") && document["move position"].IsArray()) {
			const auto& posArray = document["move position"].GetArray();
			if (posArray.Size() == 2) { 
				move_position[0] = posArray[0].GetInt();
				move_position[1] = posArray[1].GetInt();
			}
		}
		//we can add more
	}
};