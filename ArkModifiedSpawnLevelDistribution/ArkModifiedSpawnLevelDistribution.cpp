#include "ArkModifiedSpawnLevelDistribution.h"

#pragma comment(lib, "ArkApi.lib")

DECLARE_HOOK(APrimalDinoCharacter_GetRandomBaseLevel, int, APrimalDinoCharacter*, float);

#define FLT_MATCH(a, b) (fabs((a) - (b)) < 0.00001f)
//#define LOGGING

static int Hook_APrimalDinoCharacter_GetRandomBaseLevel(APrimalDinoCharacter* _APrimalDinoCharacter, float ForceRand_NotUsed)
{
	TArray<FDinoBaseLevelWeightEntry>& baseLevelWeightEntries = _APrimalDinoCharacter->DinoBaseLevelWeightEntriesField();

	// standard creatures
	if (baseLevelWeightEntries.Num() == 4 &&

		baseLevelWeightEntries[0].EntryWeight == 1.0f &&
		(baseLevelWeightEntries[0].BaseLevelMinRange == 1.0f || baseLevelWeightEntries[0].BaseLevelMinRange == 5.0f) && // hyaenodon starts at level 5
		baseLevelWeightEntries[0].BaseLevelMaxRange == 5.0f &&

		FLT_MATCH(baseLevelWeightEntries[1].EntryWeight, 0.5f) &&
		baseLevelWeightEntries[1].BaseLevelMinRange == 6.0f &&
		baseLevelWeightEntries[1].BaseLevelMaxRange == 12.0f &&

		FLT_MATCH(baseLevelWeightEntries[2].EntryWeight, 0.25f) &&
		baseLevelWeightEntries[2].BaseLevelMinRange == 13.0f &&
		baseLevelWeightEntries[2].BaseLevelMaxRange == 20.0f &&

		FLT_MATCH(baseLevelWeightEntries[3].EntryWeight, 0.1f) &&
		baseLevelWeightEntries[3].BaseLevelMinRange == 21.0f  &&
		baseLevelWeightEntries[3].BaseLevelMaxRange == 30.0f)
	{
		// default weights (1.85 sum, 5, 7, 8, 10 level entries)
		// 54.1% chance, 10.8% per level
		// 27.0% chance, 3.86% per level
		// 13.5% chance, 1.69% per level
		//  5.4% chance, 0.54% per level

		FString descriptiveName;
		_APrimalDinoCharacter->ClassField()->GetDescription(&descriptiveName);

		std::string strDino = descriptiveName.ToString();

		if (strDino == "Gigant_Character_BP_C" || strDino == "Quetz_Character_BP_C" || strDino == "BionicQuetz_Character_BP_C")
		{
			// favor even higher spawns for very rare dinos (gigas and quetzals, with only 3-4 spawns on each map)
			// 3.0 sum
			baseLevelWeightEntries[0].EntryWeight = 0.5f;	// 16.7% chance, 3.4% per level
			baseLevelWeightEntries[1].EntryWeight = 0.7f;	// 23.3% chance, 3.3% per level
			baseLevelWeightEntries[2].EntryWeight = 0.8f;	// 26.7% chance, 3.3% per level
			baseLevelWeightEntries[3].EntryWeight = 1.0f;	// 33.3% chance, 3.3% per level
		}
		else
		{
			// re-weight the spawn distribution for more common high-levels
			// 3.0 sum
			baseLevelWeightEntries[0].EntryWeight = 1.0f;	// 33.3% chance, 6.6% per level
			baseLevelWeightEntries[1].EntryWeight = 0.8f;	// 26.7% chance, 3.8% per level
			baseLevelWeightEntries[2].EntryWeight = 0.7f;	// 23.3% chance, 2.92% per level
			baseLevelWeightEntries[3].EntryWeight = 0.5f;	// 16.7% chance, 1.67% per level
		}
	}

	// wyvern/rock drakes
	else if (baseLevelWeightEntries.Num() == 4 &&
		FLT_MATCH(baseLevelWeightEntries[0].EntryWeight, 0.6f) &&
		baseLevelWeightEntries[0].BaseLevelMinRange == 1.0f &&
		baseLevelWeightEntries[0].BaseLevelMaxRange == 5.0f &&

		FLT_MATCH(baseLevelWeightEntries[1].EntryWeight, 0.3f) &&
		baseLevelWeightEntries[1].BaseLevelMinRange == 6.0f &&
		baseLevelWeightEntries[1].BaseLevelMaxRange == 12.0f &&

		FLT_MATCH(baseLevelWeightEntries[2].EntryWeight, 0.3f) &&
		baseLevelWeightEntries[2].BaseLevelMinRange == 13.0f &&
		baseLevelWeightEntries[2].BaseLevelMaxRange == 20.0f &&

		FLT_MATCH(baseLevelWeightEntries[3].EntryWeight, 0.3f) &&
		baseLevelWeightEntries[3].BaseLevelMinRange == 21.0f &&
		baseLevelWeightEntries[3].BaseLevelMaxRange == 38.0f)
	{
		// default weights (1.5 sum, 5, 7, 8, 18 level entries)
		// 40% chance, 8% per level
		// 20% chance, 2.86% per level
		// 20% chance, 2.5% per level
		// 20% chance, 1.11% per level

		// re-weight the spawn distribution for more common high-levels
		// 2.0 sum
		baseLevelWeightEntries[0].EntryWeight = 0.6f;	// 30% chance, 6% per level
		baseLevelWeightEntries[1].EntryWeight = 0.4f;	// 20% chance, 2.86% per level
		baseLevelWeightEntries[2].EntryWeight = 0.4f;	// 20% chance, 2.5% per level
		baseLevelWeightEntries[3].EntryWeight = 0.6f;	// 30% chance, 1.67% per level
	}
	// special lvl 1 dinos (bee, titano, ...)
	else if (baseLevelWeightEntries.Num() == 4 &&

		baseLevelWeightEntries[0].BaseLevelMinRange == 1.0f &&
		baseLevelWeightEntries[0].BaseLevelMaxRange == 1.0f &&

		baseLevelWeightEntries[1].BaseLevelMinRange == 1.0f &&
		baseLevelWeightEntries[1].BaseLevelMaxRange == 1.0f &&

		baseLevelWeightEntries[2].BaseLevelMinRange == 1.0f &&
		baseLevelWeightEntries[2].BaseLevelMaxRange == 1.0f &&

		baseLevelWeightEntries[3].BaseLevelMinRange == 1.0f &&
		baseLevelWeightEntries[3].BaseLevelMaxRange == 1.0f)
	{
		// do nothing to those
	}
	// liopleurodon
	else if (baseLevelWeightEntries.Num() == 1 &&

		baseLevelWeightEntries[0].BaseLevelMinRange == 1.0f &&
		baseLevelWeightEntries[0].BaseLevelMaxRange == 1.0f)
	{
		// do nothing to those
	}
	else {
#ifdef LOGGING
		FString name;
		_APrimalDinoCharacter->GetDinoDescriptiveName(&name);
		Log::GetLog()->warn("Unknown Weight Setup ({}): n {}", name.ToString(), baseLevelWeightEntries.Num());
		for (int i = 0; i < baseLevelWeightEntries.Num(); i++)
			Log::GetLog()->warn("Weight {}: EntryWeight {}, MinRange {}, MaxRange {}", i, baseLevelWeightEntries[i].EntryWeight, baseLevelWeightEntries[i].BaseLevelMinRange, baseLevelWeightEntries[i].BaseLevelMaxRange);
#endif
	}

	return APrimalDinoCharacter_GetRandomBaseLevel_original(_APrimalDinoCharacter, ForceRand_NotUsed);
}

static void Load()
{
#ifdef LOGGING
	Log::Get().Init("ArkModifiedSpawnLevelDistribution");
#endif
	ArkApi::GetHooks().SetHook("APrimalDinoCharacter.GetRandomBaseLevel", &Hook_APrimalDinoCharacter_GetRandomBaseLevel, &APrimalDinoCharacter_GetRandomBaseLevel_original);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Load();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
