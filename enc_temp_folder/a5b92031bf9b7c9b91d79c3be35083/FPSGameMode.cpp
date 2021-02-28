// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "FPSCubeActor.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();

	Http = &FHttpModule::Get();


}

void AFPSGameMode::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, "Plz show up!");

	GetMostWantedData();

}


void AFPSGameMode::GetMostWantedData()
{
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, "Getting data!");
	
	TSharedRef<IHttpRequest, ESPMode::NotThreadSafe> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AFPSGameMode::OnResponseReceived);
	//This is the url on which to process the request
	Request->SetURL("https://api.fbi.gov/wanted/v1/list");
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
	

}

void AFPSGameMode::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, "Got Data");

	//Create a pointer to hold the json srialized data
	TSharedPtr<FJsonObject> JsonObject;

	//Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	//Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		//Get the value of the json object by field name
		TArray<TSharedPtr<FJsonValue>> objArray = JsonObject->GetArrayField("items");

		//Iterate though cubes in the world and assign each a color based on a criminal
		int j = 0;
		for (TActorIterator<AFPSCubeActor> ActorItr(GetWorld()); ActorItr; )
		{
			// Follow iterator object to my actual actor pointer
			AFPSCubeActor* cube = *ActorItr;
			int criminalIndex = j % objArray.Num();
			auto criminal = objArray[criminalIndex]->AsObject();

			FString eyeColor = criminal->GetStringField("eyes");
			int weight_max = criminal->GetNumberField("weight_max");
			int weight_min = criminal->GetNumberField("weight_min");
			int height_max = criminal->GetNumberField("height_max");

			FString sex = criminal->GetStringField("sex");

			TArray<TSharedPtr<FJsonValue>> subjectArray = criminal->GetArrayField("subjects");
			
			
			FString subject;

			

			if(subjectArray.Num() > 0)
				subjectArray[0]->TryGetString(subject);

			if (subject != "Kidnappings and Missing Persons") {
				if (MaterialsMap.Contains(eyeColor)) {
					//Eye color exists in map
					cube->SetColor(MaterialsMap[eyeColor]);
					++ActorItr;
				}
				
			}
			
			j++;
			
		}


	}
	
	
}
