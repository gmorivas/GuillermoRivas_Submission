// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "FinalScore.generated.h"

/**
 * 
 */
UCLASS()
class GUILLERMORIVAS_API UFinalScore : public UUserWidget
{
	GENERATED_BODY()

	public:

	void SetFinalScore(int32 Score);
	
	private:

		UPROPERTY(meta = (BindWidget))
		UTextBlock* FinalScoreText;
};
