// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/Border.h"
#include "Internationalization/Regex.h"
#include "Components/CanvasPanelSlot.h"
#include "IndividualScore.generated.h"

/**
 * 
 */

 // this is the delegate that will be broadcasted whenever any new valid value is set on the score
 // the board will listen for this and recalculate values
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FScoreDelegate);


// CLASS THAT REPRESENTES A SINGLE PLAYER TURN WITH 2 OR 3 THROWS.
// EXTENDED BY WBP_IndividualScore BLUEPRINT
UCLASS()
class GUILLERMORIVAS_API UIndividualScore : public UUserWidget
{
	GENERATED_BODY()

	public:

		FScoreDelegate ScoreDelegate;
		void Setup(int32 NewScoreSize, int32 newScoreId);

		int32 GetLastSavedTotal();
		void SetNewTotal(int32 NewTotal);
		TArray<FString> GetContents();

	private:
	
		virtual void NativeConstruct() override;

		UPROPERTY(meta = (BindWidget))
		UEditableTextBox* FirstScoreText;
		
		UPROPERTY(meta = (BindWidget))
		UEditableTextBox* SecondScoreText;

		UPROPERTY(meta = (BindWidget))
		UEditableTextBox* ThirdScoreText;

		UPROPERTY(meta = (BindWidget))
		UTextBlock* TotalScoreText;

		UPROPERTY(meta = (BindWidget))
		UBorder* TotalScoreTextGraphic;

		UPROPERTY(EditAnywhere)
		int32 ScoreSize = 2;

		UPROPERTY(EditAnywhere)
		int32 ScoreId = -1;

		UFUNCTION()
		void ValidateFirstTextInput(const FText& InputText);
		
		UFUNCTION()
		void ValidateSecondTextInput(const FText& InputText);
		
		UFUNCTION()
		void ValidateThirdTextInput(const FText& InputText);

		void ValidateAnyTextInput(const FText& InputText, int32 TextPosition);

		void UpdateTextAt(int32 Index, FString NewValue);

		TArray<UEditableTextBox*> TextBoxesArray;
		
		TArray<FString> TextBoxesContentArray;

		int32 LastSavedTotal = 0;

};
