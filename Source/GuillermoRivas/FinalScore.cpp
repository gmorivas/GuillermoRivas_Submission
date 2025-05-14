// Fill out your copyright notice in the Description page of Project Settings.


#include "FinalScore.h"

void UFinalScore::SetFinalScore(int32 Score)
{
    FinalScoreText->SetText(FText::FromString(FString::FromInt(Score)));
}