#include "FileSorts.h"

#include "utils/StringUtil.h"
#include "Settings.h"
#include "Log.h"

namespace FileSorts
{

	const FileData::SortType typesArr[] = {
		FileData::SortType(&compareName, true, "name"),
		FileData::SortType(&compareName, false, "name / reverse"),
		
		FileData::SortType(&compareFullSystem, true, "system"),
		FileData::SortType(&compareFullSystem, false, "system / reverse"),
		
		FileData::SortType(&comparePublisher, true, "publisher"),
		FileData::SortType(&comparePublisher, false, "publisher / reverse"),
		
		FileData::SortType(&compareReleaseDate, true, "release date"),
		FileData::SortType(&compareReleaseDate, false, "release date / reverse"),
		
		FileData::SortType(&compareGenre, true, "genre"),
		FileData::SortType(&compareGenre, false, "genre / reverse"),
		
		FileData::SortType(&compareNumPlayers, true, "players"),
		FileData::SortType(&compareNumPlayers, false, "players / reverse"),
		
		FileData::SortType(&compareRegion, true, "region"),
		FileData::SortType(&compareRegion, false, "region / reverse"),
			
		FileData::SortType(&compareTimesPlayed, false, "most played"),
		FileData::SortType(&compareTimesPlayed, true, "most played / reverse"),
		
		FileData::SortType(&compareLastPlayed, false, "last played"),
		FileData::SortType(&compareLastPlayed, true, "last played / reverse"),
	};

	const std::vector<FileData::SortType> SortTypes(typesArr, typesArr + sizeof(typesArr)/sizeof(typesArr[0]));

	//returns if file1 should come before file2
	bool compareName(const FileData* file1, const FileData* file2)
	{
		// we compare the actual metadata name, as collection files have the system appended which messes up the order
		std::string name1 = Utils::String::toUpper(file1->metadata.get("sortname"));
		std::string name2 = Utils::String::toUpper(file2->metadata.get("sortname"));
		if(name1.empty()){
			name1 = Utils::String::toUpper(file1->metadata.get("name"));
		}
		if(name2.empty()){
			name2 = Utils::String::toUpper(file2->metadata.get("name"));
		}

		ignoreLeadingArticles(name1, name2);

		return name1.compare(name2) < 0;
	}
	
	bool compareFullSystem(const FileData* file1, const FileData* file2)
	{
		std::string fullsystem1 = Utils::String::toUpper(file1->metadata.get("fullsystem"));
		std::string fullsystem2 = Utils::String::toUpper(file2->metadata.get("fullsystem"));
		return fullsystem1.compare(fullsystem2) < 0;
	}
	
	bool comparePublisher(const FileData* file1, const FileData* file2)
	{
		std::string publisher1 = Utils::String::toUpper(file1->metadata.get("publisher"));
		std::string publisher2 = Utils::String::toUpper(file2->metadata.get("publisher"));
		return publisher1.compare(publisher2) < 0;
	}
	
	bool compareReleaseDate(const FileData* file1, const FileData* file2)
	{
		// since it's stored as an ISO string (YYYYMMDDTHHMMSS), we can compare as a string
		// as it's a lot faster than the time casts and then time comparisons
		return (file1)->metadata.get("releasedate") < (file2)->metadata.get("releasedate");
	}
	
	bool compareGenre(const FileData* file1, const FileData* file2)
	{
		std::string genre1 = Utils::String::toUpper(file1->metadata.get("genre"));
		std::string genre2 = Utils::String::toUpper(file2->metadata.get("genre"));
		return genre1.compare(genre2) < 0;
	}
	
	bool compareNumPlayers(const FileData* file1, const FileData* file2)
	{
		std::string players1 = Utils::String::toUpper(file1->metadata.get("players"));
		std::string players2 = Utils::String::toUpper(file2->metadata.get("players"));
		return players1.compare(players2) < 0;
	}
	
	bool compareRegion(const FileData* file1, const FileData* file2)
	{
		std::string region1 = Utils::String::toUpper(file1->metadata.get("region"));
		std::string region2 = Utils::String::toUpper(file2->metadata.get("region"));
		return region1.compare(region2) < 0;
	}
	
	bool compareTimesPlayed(const FileData* file1, const FileData* file2)
	{
		//only games have playcount metadata
		if(file1->metadata.getType() == GAME_METADATA && file2->metadata.getType() == GAME_METADATA)
		{
			return (file1)->metadata.getInt("playcount") < (file2)->metadata.getInt("playcount");
		}

		return false;
	}
	
	bool compareLastPlayed(const FileData* file1, const FileData* file2)
	{
		// since it's stored as an ISO string (YYYYMMDDTHHMMSS), we can compare as a string
		// as it's a lot faster than the time casts and then time comparisons
		return (file1)->metadata.get("lastplayed") < (file2)->metadata.get("lastplayed");
	}

	//If option is enabled, ignore leading articles by temporarily modifying the name prior to sorting
	//(Artciles are defined within the settings config file)
	void ignoreLeadingArticles(std::string &name1, std::string &name2) {

		if (Settings::getInstance()->getBool("IgnoreLeadingArticles"))
		{

			std::vector<std::string> articles = Utils::String::delimitedStringToVector(Settings::getInstance()->getString("LeadingArticles"), ",");

			for(Utils::String::stringVector::iterator it = articles.begin(); it != articles.end(); it++)
			{
			
				if (Utils::String::startsWith(Utils::String::toUpper(name1), Utils::String::toUpper(it[0]) + " ")) {
					name1 = Utils::String::replace(Utils::String::toUpper(name1), Utils::String::toUpper(it[0]) + " ", "");
				}

				if (Utils::String::startsWith(Utils::String::toUpper(name2), Utils::String::toUpper(it[0]) + " ")) {
					name2 = Utils::String::replace(Utils::String::toUpper(name2), Utils::String::toUpper(it[0]) + " ", "");
				}

			}

		}

	}

};
