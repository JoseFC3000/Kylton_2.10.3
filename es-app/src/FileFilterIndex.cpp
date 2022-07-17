#include "FileFilterIndex.h"

#include "utils/StringUtil.h"
#include "views/UIModeController.h"
#include "FileData.h"
#include "Log.h"
#include "Settings.h"

#define UNKNOWN_LABEL "UNKNOWN"
#define INCLUDE_UNKNOWN false;

FileFilterIndex::FileFilterIndex()
	: filterByFavorites(false), filterByGenre(false), filterByPlayers(false), filterByPublisher(false), filterByFullSystem(false), filterByRegion(false)
{
	clearAllFilters();
	FilterDataDecl filterDecls[] = {
		//type 			//allKeys 			//filteredBy 		//filteredKeys 			//primaryKey 		//hasSecondaryKey 	//secondaryKey 		//menuLabel
		{ FULLSYSTEM_FILTER, 	&fullsystemIndexAllKeys,	&filterByFullSystem,	&fullsystemIndexFilteredKeys, 	"fullsystem",		true,			"fullsystem",		"SYSTEM" },
		{ PUBLISHER_FILTER, 	&publisherIndexAllKeys, 	&filterByPublisher,	&publisherIndexFilteredKeys, 	"publisher",		true,			"publisher",		"PUBLISHER" },
		{ GENRE_FILTER, 	&genreIndexAllKeys, 		&filterByGenre,		&genreIndexFilteredKeys, 	"genre",		true,			"genre",		"GENRE"	},
		{ PLAYER_FILTER, 	&playersIndexAllKeys, 		&filterByPlayers,	&playersIndexFilteredKeys, 	"players",		true,			"players",		"PLAYERS" },
		{ REGION_FILTER, 	&regionIndexAllKeys, 		&filterByRegion,	&regionIndexFilteredKeys, 	"region",		true,			"region",		"REGION" },
		{ FAVORITES_FILTER, 	&favoritesIndexAllKeys, 	&filterByFavorites,	&favoritesIndexFilteredKeys,	"favorite",		false,			"",			"FAVORITES" },
	};

	filterDataDecl = std::vector<FilterDataDecl>(filterDecls, filterDecls + sizeof(filterDecls) / sizeof(filterDecls[0]));
}

FileFilterIndex::~FileFilterIndex()
{
	resetIndex();
}

std::vector<FilterDataDecl>& FileFilterIndex::getFilterDataDecls()
{
	return filterDataDecl;
}

void FileFilterIndex::importIndex(FileFilterIndex* indexToImport)
{
	struct IndexImportStructure
	{
		std::map<std::string, int>* destinationIndex;
		std::map<std::string, int>* sourceIndex;
	};

	IndexImportStructure indexStructDecls[] = {
		{ &genreIndexAllKeys, &(indexToImport->genreIndexAllKeys) },
		{ &playersIndexAllKeys, &(indexToImport->playersIndexAllKeys) },
		{ &publisherIndexAllKeys, &(indexToImport->publisherIndexAllKeys) },
		{ &fullsystemIndexAllKeys, &(indexToImport->fullsystemIndexAllKeys) },
		{ &regionIndexAllKeys, &(indexToImport->regionIndexAllKeys) },
		{ &favoritesIndexAllKeys, &(indexToImport->favoritesIndexAllKeys) },
	};

	std::vector<IndexImportStructure> indexImportDecl = std::vector<IndexImportStructure>(indexStructDecls, indexStructDecls + sizeof(indexStructDecls) / sizeof(indexStructDecls[0]));

	for (std::vector<IndexImportStructure>::const_iterator indexesIt = indexImportDecl.cbegin(); indexesIt != indexImportDecl.cend(); ++indexesIt )
	{
		for (std::map<std::string, int>::const_iterator sourceIt = (*indexesIt).sourceIndex->cbegin(); sourceIt != (*indexesIt).sourceIndex->cend(); ++sourceIt )
		{
			if ((*indexesIt).destinationIndex->find((*sourceIt).first) == (*indexesIt).destinationIndex->cend())
			{
				// entry doesn't exist
				(*((*indexesIt).destinationIndex))[(*sourceIt).first] = (*sourceIt).second;
			}
			else
			{
				(*((*indexesIt).destinationIndex))[(*sourceIt).first] += (*sourceIt).second;
			}
		}
	}
}
void FileFilterIndex::resetIndex()
{
	clearAllFilters();
	clearIndex(genreIndexAllKeys);
	clearIndex(playersIndexAllKeys);
	clearIndex(publisherIndexAllKeys);
	clearIndex(fullsystemIndexAllKeys);
	clearIndex(regionIndexAllKeys);
	clearIndex(favoritesIndexAllKeys);
}

std::string FileFilterIndex::getIndexableKey(FileData* game, FilterIndexType type, bool getSecondary)
{
	std::string key = "";
	switch(type)
	{
		case GENRE_FILTER:
		{
			key = Utils::String::toUpper(game->metadata.get("genre"));
			key = Utils::String::trim(key);
			if (getSecondary && !key.empty()) {
				std::istringstream f(key);
				std::string newKey;
				getline(f, newKey, '/');
				if (!newKey.empty() && newKey != key)
				{
					key = newKey;
				}
				else
				{
					key = std::string();
				}
			}
			break;
		}
		case PLAYER_FILTER:
		{
			key = Utils::String::toUpper(game->metadata.get("players"));
			key = Utils::String::trim(key);
			if (getSecondary && !key.empty()) {
				std::istringstream f(key);
				std::string newKey;
				getline(f, newKey, '/');
				if (!newKey.empty() && newKey != key)
				{
					key = newKey;
				}
				else
				{
					key = std::string();
				}
			}		
			break;
		}
		case PUBLISHER_FILTER:
		{
			key = Utils::String::toUpper(game->metadata.get("publisher"));
			key = Utils::String::trim(key);
			if (getSecondary && !key.empty()) {
				std::istringstream f(key);
				std::string newKey;
				getline(f, newKey, '/');
				if (!newKey.empty() && newKey != key)
				{
					key = newKey;
				}
				else
				{
					key = std::string();
				}
			}		
			break;
		}
		case FULLSYSTEM_FILTER:
		{
			key = Utils::String::toUpper(game->metadata.get("fullsystem"));
			key = Utils::String::trim(key);
			if (getSecondary && !key.empty()) {
				std::istringstream f(key);
				std::string newKey;
				getline(f, newKey, '/');
				if (!newKey.empty() && newKey != key)
				{
					key = newKey;
				}
				else
				{
					key = std::string();
				}
			}					
			break;
		}
		case REGION_FILTER:
		{
			key = Utils::String::toUpper(game->metadata.get("region"));
			key = Utils::String::trim(key);
			if (getSecondary && !key.empty()) {
				std::istringstream f(key);
				std::string newKey;
				getline(f, newKey, '/');
				if (!newKey.empty() && newKey != key)
				{
					key = newKey;
				}
				else
				{
					key = std::string();
				}
			}					
			break;
		}
		case FAVORITES_FILTER:
		{
			if (game->getType() != GAME)
				return "FALSE";
			key = Utils::String::toUpper(game->metadata.get("favorite"));
			break;
		}
	}
}

void FileFilterIndex::addToIndex(FileData* game)
{
	manageGenreEntryInIndex(game);
	managePlayerEntryInIndex(game);
	managePublisherEntryInIndex(game);
	manageFullSystemEntryInIndex(game);
	manageRegionEntryInIndex(game);
	manageFavoritesEntryInIndex(game);
}

void FileFilterIndex::removeFromIndex(FileData* game)
{
	manageGenreEntryInIndex(game, true);
	managePlayerEntryInIndex(game, true);
	managePublisherEntryInIndex(game, true);
	manageFullSystemEntryInIndex(game, true);
	manageRegionEntryInIndex(game, true);
	manageFavoritesEntryInIndex(game, true);
}

void FileFilterIndex::setFilter(FilterIndexType type, std::vector<std::string>* values)
{
	// test if it exists before setting
	if(type == NONE)
	{
		clearAllFilters();
	}
	else
	{
		for (std::vector<FilterDataDecl>::const_iterator it = filterDataDecl.cbegin(); it != filterDataDecl.cend(); ++it ) {
			if ((*it).type == type)
			{
				FilterDataDecl filterData = (*it);
				*(filterData.filteredByRef) = values->size() > 0;
				filterData.currentFilteredKeys->clear();
				for (std::vector<std::string>::const_iterator vit = values->cbegin(); vit != values->cend(); ++vit ) {
					// check if exists
					if (filterData.allIndexKeys->find(*vit) != filterData.allIndexKeys->cend()) {
						filterData.currentFilteredKeys->push_back(std::string(*vit));
					}
				}
			}
		}
	}
	return;
}

void FileFilterIndex::clearAllFilters()
{
	for (std::vector<FilterDataDecl>::const_iterator it = filterDataDecl.cbegin(); it != filterDataDecl.cend(); ++it )
	{
		FilterDataDecl filterData = (*it);
		*(filterData.filteredByRef) = false;
		filterData.currentFilteredKeys->clear();
	}
	return;
}

void FileFilterIndex::resetFilters()
{
	clearAllFilters();
	setUIModeFilters();
}

void FileFilterIndex::debugPrintIndexes()
{
	LOG(LogInfo) << "Printing Indexes...";
	for (auto x: playersIndexAllKeys) {
		LOG(LogInfo) << "Multiplayer Index: " << x.first << ": " << x.second;
	}
	for (auto x: genreIndexAllKeys) {
		LOG(LogInfo) << "Genre Index: " << x.first << ": " << x.second;
	}
	for (auto x: publisherIndexAllKeys) {
		LOG(LogInfo) << "Publisher Index: " << x.first << ": " << x.second;
	}
	for (auto x: fullsystemIndexAllKeys) {
		LOG(LogInfo) << "FullSystem Index: " << x.first << ": " << x.second;
	}
	for (auto x: regionIndexAllKeys) {
		LOG(LogInfo) << "Region Index: " << x.first << ": " << x.second;
	}
	for (auto x: favoritesIndexAllKeys) {
		LOG(LogInfo) << "Favorites Index: " << x.first << ": " << x.second;
	}
}

bool FileFilterIndex::showFile(FileData* game)
{
	// this shouldn't happen, but just in case let's get it out of the way
	if (!isFiltered())
		return true;

	// if folder, needs further inspection - i.e. see if folder contains at least one element
	// that should be shown
	if (game->getType() == FOLDER) {
		std::vector<FileData*> children = game->getChildren();
		// iterate through all of the children, until there's a match

		for (std::vector<FileData*>::const_iterator it = children.cbegin(); it != children.cend(); ++it ) {
			if (showFile(*it))
			{
				return true;
			}
		}
		return false;
	}

	bool keepGoing = false;

	for (std::vector<FilterDataDecl>::const_iterator it = filterDataDecl.cbegin(); it != filterDataDecl.cend(); ++it ) {
		FilterDataDecl filterData = (*it);
		if(*(filterData.filteredByRef))
		{
			// try to find a match
			std::string key = getIndexableKey(game, filterData.type, false);
			keepGoing = isKeyBeingFilteredBy(key, filterData.type);

			// if we didn't find a match, try for secondary keys - i.e. publisher and dev, or first genre
			if (!keepGoing)
			{
				if (!filterData.hasSecondaryKey)
				{
					return false;
				}
				std::string secKey = getIndexableKey(game, filterData.type, true);
				if (secKey != UNKNOWN_LABEL)
				{
					keepGoing = isKeyBeingFilteredBy(secKey, filterData.type);
				}
			}
			// if still nothing, then it's not a match
			if (!keepGoing)
				return false;

		}

	}

	return keepGoing;
}

bool FileFilterIndex::isKeyBeingFilteredBy(std::string key, FilterIndexType type)
{
	const FilterIndexType filterTypes[6] = { FAVORITES_FILTER, GENRE_FILTER, PLAYER_FILTER, PUBLISHER_FILTER, FULLSYSTEM_FILTER, REGION_FILTER };
	std::vector<std::string> filterKeysList[6] = { favoritesIndexFilteredKeys, genreIndexFilteredKeys, playersIndexFilteredKeys, publisherIndexFilteredKeys, fullsystemIndexFilteredKeys, regionIndexFilteredKeys };

	for (int i = 0; i < 6; i++)
	{
		if (filterTypes[i] == type)
		{
			for (std::vector<std::string>::const_iterator it = filterKeysList[i].cbegin(); it != filterKeysList[i].cend(); ++it )
			{
				if (key == (*it))
				{
					return true;
				}
			}
			return false;
		}
	}

	return false;
}

void FileFilterIndex::manageGenreEntryInIndex(FileData* game, bool remove)
{

	std::string key = getIndexableKey(game, GENRE_FILTER, false);

	// flag for including unknowns
	bool includeUnknown = INCLUDE_UNKNOWN;

	// only add unknown in pubdev IF both dev and pub are empty
	if (!includeUnknown && (key == UNKNOWN_LABEL || key == "BIOS")) {
		// no valid genre info found
		return;
	}

	manageIndexEntry(&genreIndexAllKeys, key, remove);

	key = getIndexableKey(game, GENRE_FILTER, true);
	if (!includeUnknown && key == UNKNOWN_LABEL)
	{
		manageIndexEntry(&genreIndexAllKeys, key, remove);
	}
}

void FileFilterIndex::managePlayerEntryInIndex(FileData* game, bool remove)
{
	std::string key = getIndexableKey(game, PLAYER_FILTER, false);

	// flag for including unknowns
	bool includeUnknown = INCLUDE_UNKNOWN;

	// only add unknown in pubdev IF both dev and pub are empty
	if (!includeUnknown && (key == UNKNOWN_LABEL || key == "BIOS")) {
		// no valid player info found
		return;
	}

	manageIndexEntry(&playersIndexAllKeys, key, remove);

	key = getIndexableKey(game, PLAYER_FILTER, true);
	if (!includeUnknown && key == UNKNOWN_LABEL)
	{
		manageIndexEntry(&playersIndexAllKeys, key, remove);
	}	
}

void FileFilterIndex::managePublisherEntryInIndex(FileData* game, bool remove)
{
	std::string key = getIndexableKey(game, PUBLISHER_FILTER, false);

	// flag for including unknowns
	bool includeUnknown = INCLUDE_UNKNOWN;

	// only add unknown in pubdev IF both dev and pub are empty
	if (!includeUnknown && (key == UNKNOWN_LABEL || key == "BIOS")) {
		// no valid publisher info found
		return;
	}

	manageIndexEntry(&publisherIndexAllKeys, key, remove);

	key = getIndexableKey(game, PUBLISHER_FILTER, true);
	if (!includeUnknown && key == UNKNOWN_LABEL)
	{
		manageIndexEntry(&publisherIndexAllKeys, key, remove);
	}	
}

void FileFilterIndex::manageFullSystemEntryInIndex(FileData* game, bool remove)
{
	std::string key = getIndexableKey(game, FULLSYSTEM_FILTER, false);

	// flag for including unknowns
	bool includeUnknown = INCLUDE_UNKNOWN;

	// only add unknown in pubdev IF both dev and pub are empty
	if (!includeUnknown && (key == UNKNOWN_LABEL || key == "BIOS")) {
		// no valid fullsystem info found
		return;
	}

	manageIndexEntry(&fullsystemIndexAllKeys, key, remove);

	key = getIndexableKey(game, FULLSYSTEM_FILTER, true);
	if (!includeUnknown && key == UNKNOWN_LABEL)
	{
		manageIndexEntry(&fullsystemIndexAllKeys, key, remove);
	}	
}

void FileFilterIndex::manageRegionEntryInIndex(FileData* game, bool remove)
{
	std::string key = getIndexableKey(game, REGION_FILTER, false);

	// flag for including unknowns
	bool includeUnknown = INCLUDE_UNKNOWN;

	// only add unknown in pubdev IF both dev and pub are empty
	if (!includeUnknown && (key == UNKNOWN_LABEL || key == "BIOS")) {
		// no valid region info found
		return;
	}

	manageIndexEntry(&regionIndexAllKeys, key, remove);

	key = getIndexableKey(game, REGION_FILTER, true);
	if (!includeUnknown && key == UNKNOWN_LABEL)
	{
		manageIndexEntry(&regionIndexAllKeys, key, remove);
	}	
}

void FileFilterIndex::manageFavoritesEntryInIndex(FileData* game, bool remove)
{
	// flag for including unknowns
	bool includeUnknown = INCLUDE_UNKNOWN;
	std::string key = getIndexableKey(game, FAVORITES_FILTER, false);
	if (!includeUnknown && key == UNKNOWN_LABEL) {
		// no valid favorites info found
		return;
	}

	manageIndexEntry(&favoritesIndexAllKeys, key, remove);
}

void FileFilterIndex::manageIndexEntry(std::map<std::string, int>* index, std::string key, bool remove) {
	bool includeUnknown = INCLUDE_UNKNOWN;
	if (!includeUnknown && key == UNKNOWN_LABEL)
		return;
	if (remove) {
		// removing entry
		if (index->find(key) == index->cend())
		{
			// this shouldn't happen
			LOG(LogInfo) << "Couldn't find entry in index! " << key;
		}
		else
		{
			(index->at(key))--;
			if(index->at(key) <= 0) {
				index->erase(key);
			}
		}
	}
	else
	{
		// adding entry
		if (index->find(key) == index->cend())
		{
			(*index)[key] = 1;
		}
		else
		{
			(index->at(key))++;
		}
	}
}

void FileFilterIndex::clearIndex(std::map<std::string, int> indexMap)
{
	indexMap.clear();
}
