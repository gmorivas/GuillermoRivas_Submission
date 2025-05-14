// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreBoard.h"

void UScoreBoard::NativeConstruct()
{
    Super::NativeConstruct();

    // spawn name 
    UUserWidget* NameWidget = CreateWidget<UUserWidget>(GetWorld(), NameWidgetClass);
    AddWidgetToCanvas(NameWidget, 0, 0, 50.0f, 50.0f);
    
    // spawn all the individual score sets
    for(int32 CurrentIndex = 0; CurrentIndex < ScoreBoardSize; CurrentIndex++)
    {
        // create individual score widget
        UIndividualScore* ScoreWidget = CreateWidget<UIndividualScore>(GetWorld(), IndividualScoreClass);
        AddWidgetToCanvas(ScoreWidget, 0, 0, 360.0f + CurrentIndex * 120, 50.0f);

        // subscribe to each individual score
        ScoreWidget->ScoreDelegate.AddDynamic(this, &UScoreBoard::ScoreUpdateFromIndividualListener);
        
        // setup size and index values
        ScoreWidget->Setup(2, CurrentIndex);

        // add to array
        IndividualScoreArray.Add(ScoreWidget);
    }

    // update the last one to have size 3 
    IndividualScoreArray[ScoreBoardSize - 1]->Setup(3, ScoreBoardSize - 1);
    
    // spawn final result
    FinalScore = CreateWidget<UFinalScore>(GetWorld(), FinalScoreClass);
    AddWidgetToCanvas(FinalScore, 0, 0, 420.0f + ScoreBoardSize * 120, 50.0f);
    
}

void UScoreBoard::AddWidgetToCanvas(UUserWidget* Widget,int32 AnchorX, int32 AnchorY, float PositionX, float PositionY)
{

    if(Widget)
    {
        UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(ScoreBoardCanvas->AddChild(Widget));
        if(PanelSlot)
        {
            // set anchor to top left
            PanelSlot->SetAnchors(FAnchors(AnchorX, AnchorY));

            // position newly created widget
            PanelSlot->SetOffsets(FMargin(PositionX, PositionY));
        }
        
    }
}

void UScoreBoard::ScoreUpdateFromIndividualListener()
{
    // originally had extra logic here, simplified after refactor and left it as is 
    // because of time concerns
    UpdateFinalScore();
}

void UScoreBoard::UpdateFinalScore()
{
    int32 AccumulatedScore = 0;

    // traverse all individual scores and add values
    for(int32 CurrentIndex = 0; CurrentIndex < IndividualScoreArray.Num(); CurrentIndex++)
    {
        UE_LOG(LogTemp, Display, TEXT("**** ITERATING %d"), CurrentIndex);

        // Get strings for current frame
        TArray<FString> CurrentStringValues = IndividualScoreArray[CurrentIndex]->GetContents();

        // calculate the score for this frame

        // 3 cases - 

        // strike - this frame + next 2 throws
        if(CurrentStringValues[0].Equals("x") || CurrentStringValues[0].Equals("X"))
        {

            TArray<FString> NextValues = GetNextValues(CurrentIndex, 2);
            if(NextValues.Num() < 2)
            {
                // not enough values to calculate yet
                UE_LOG(LogTemp, Display, TEXT("CANNOT CALCULATE VALUE YET FOR STRIKE"));
                break;
            }
            else
            {
                AccumulatedScore += 10 + TranslateBowlingStringIntoInt(NextValues[0]) + TranslateBowlingStringIntoInt(NextValues[1]);
                IndividualScoreArray[CurrentIndex]->SetNewTotal(AccumulatedScore);
            }
        }
        
        // spare - this frame + next throw
        else if(CurrentStringValues[1].Equals("/"))
        {

            TArray<FString> NextValues = GetNextValues(CurrentIndex, 1);
            if(NextValues.Num() < 1)
            {
                // not enough values to calculate yet
                UE_LOG(LogTemp, Display, TEXT("CANNOT CALCULATE VALUE YET FOR SPARE"));
                break;
            }
            else
            {
                AccumulatedScore += 10 + TranslateBowlingStringIntoInt(NextValues[0]);
                IndividualScoreArray[CurrentIndex]->SetNewTotal(AccumulatedScore);
            }
        }

        // 2 other values
        else
        {
            if(CurrentStringValues[0].Equals("") || CurrentStringValues[1].Equals(""))
                break;

            AccumulatedScore += TranslateBowlingStringIntoInt(CurrentStringValues[0]) + TranslateBowlingStringIntoInt(CurrentStringValues[1]) + TranslateBowlingStringIntoInt(CurrentStringValues[2]);
            
            IndividualScoreArray[CurrentIndex]->SetNewTotal(AccumulatedScore);
        }
    }

    // calculate values for last one

    // set accumulated score on final score widget
    FinalScore->SetFinalScore(AccumulatedScore);
}

int32 UScoreBoard::TranslateBowlingStringIntoInt(FString BowlingString)
{
    // if numbers return the numeric value
    if (BowlingString.IsNumeric())
        return FCString::Atoi(*BowlingString);

    // test for -
    if (BowlingString.Equals("-") ||BowlingString.Equals(""))
        return 0;

    // test for f, F
    if (BowlingString.Equals("f") || BowlingString.Equals("F"))
        return 0;

    // test for x, X
    if (BowlingString.Equals("x") || BowlingString.Equals("X"))
        return 10;

    // test for /
    if (BowlingString.Equals("/"))
        return 10;

    // any other string
    return -1;
}



// method that returns the values that are next on the scoreboard
// originally written as a recursive method, refactored for simplicity and because 
// we only travel to the next 2 score turns at most (if we have 2 consecutive strikes)
TArray<FString> UScoreBoard::GetNextValues(int32 CurrentTurnIndex, int32 AmountOfValues)
{
    // if position is the last return next locations in same set
    if(CurrentTurnIndex == ScoreBoardSize - 1)
    {
        return GetNextValuesForLastTurns(CurrentTurnIndex, AmountOfValues % 2 + 1, AmountOfValues, AmountOfValues);
    }

    // if position is the second to last return next locations in last set
    if(CurrentTurnIndex == ScoreBoardSize - 2)
    {
        return GetNextValuesForLastTurns(CurrentTurnIndex + 1, 0, 1, AmountOfValues);
    }

    // any other case
    // get first value of next turn
    TArray<FString> Result;

    TArray<FString> NextStringValues = IndividualScoreArray[CurrentTurnIndex + 1]->GetContents();

    if(!NextStringValues[0].Equals(""))
        Result.Add(NextStringValues[0]);

    if(AmountOfValues > 1)
    {
        // if we want 2 check for next one
        if(NextStringValues[0].Equals("x") || NextStringValues[0].Equals("X"))
        {
            // if next value is a strike, look for the next one
            TArray<FString> SecondStringValues = IndividualScoreArray[CurrentTurnIndex + 2]->GetContents();
            if(!SecondStringValues[0].Equals(""))
                Result.Add(SecondStringValues[0]);
        }
        else
        {
            if(!NextStringValues[1].Equals(""))
                Result.Add(NextStringValues[1]);
        }
    }

    return Result;
}


TArray<FString> UScoreBoard::GetNextValuesForLastTurns(int32 StringSource, int32 FirstIndex, int32 SecondIndex, int32 AmountOfValues)
{

    TArray<FString> Result;

    // Get strings for last frame
    TArray<FString> CurrentStringValues = IndividualScoreArray[StringSource]->GetContents();
    
    if(!CurrentStringValues[FirstIndex].Equals(""))
        Result.Add(CurrentStringValues[FirstIndex]);
        
    if(AmountOfValues > 1 && !CurrentStringValues[SecondIndex].Equals(""))
        Result.Add(CurrentStringValues[SecondIndex]);

    return Result;
}

void UScoreBoard::AddIfNotEmpty(FString Value, TArray<FString> Result)
{
    if(!Value.Equals(""))
        Result.Add(Value);
}

