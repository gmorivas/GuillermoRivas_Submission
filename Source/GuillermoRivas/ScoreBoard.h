// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IndividualScore.h"
#include "FinalScore.h"
#include "Containers/Array.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Internationalization/Regex.h"
#include "ScoreBoard.generated.h"

/**
 * 
 */
UCLASS()
class GUILLERMORIVAS_API UScoreBoard : public UUserWidget
{
	GENERATED_BODY()

	private:

		virtual void NativeConstruct() override;

		UPROPERTY(meta = (BindWidget))
		UCanvasPanel* ScoreBoardCanvas;

		UPROPERTY(EditAnywhere)
		int32 ScoreBoardSize = 10;

		UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> NameWidgetClass;

		UPROPERTY(EditAnywhere)
		TSubclassOf<UIndividualScore> IndividualScoreClass;

		UPROPERTY(EditAnywhere)
		TSubclassOf<UFinalScore> FinalScoreClass;

		TArray<UIndividualScore*> IndividualScoreArray;

		UFinalScore* FinalScore;

		void AddWidgetToCanvas(UUserWidget* Widget, int32 AnchorX, int32 AnchorY, float PositionX, float PositionY);

		UFUNCTION()
		void ScoreUpdateFromIndividualListener();

		void UpdateFinalScore();

		TArray<FString> GetNextValues(int32 CurrentTurnIndex, int32 AmountOfValues);

		TArray<FString> GetNextValuesForLastTurns(int32 StringSource, int32 FirstIndex, int32 SecondIndex, int32 AmountOfValues);

		void AddIfNotEmpty(FString Value, TArray<FString> Result);

		int32 TranslateBowlingStringIntoInt(FString BowlingString);
};
