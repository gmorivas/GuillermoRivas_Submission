// Fill out your copyright notice in the Description page of Project Settings.


#include "IndividualScore.h"

void UIndividualScore::NativeConstruct()
{
    Super::NativeConstruct();

    // add listener for char validation
    // had to define a method per each because when text changes we only get the new text as args
    // and I wanted to also be able to send the specific widget to change its context based on the validation
    // workaround: declare 1 listening method per each and a general method to be called, 
    // that method actually receives the new text and the array position of the textbox being validated
    FirstScoreText->OnTextChanged.AddDynamic(this, &UIndividualScore::ValidateFirstTextInput);
    SecondScoreText->OnTextChanged.AddDynamic(this, &UIndividualScore::ValidateSecondTextInput);
    ThirdScoreText->OnTextChanged.AddDynamic(this, &UIndividualScore::ValidateThirdTextInput);

    // setup the elements in an array to permit writing into them only if the previous ones have values already
    TextBoxesArray.Add(FirstScoreText);
    TextBoxesArray.Add(SecondScoreText);
    TextBoxesArray.Add(ThirdScoreText);

    // setup content array
    TextBoxesContentArray.Init("", 3);

    // if size is setup through the editor, native construct will invoke the setup logic
    Setup(ScoreSize, -1);
}

// this methods enables us to disable and enable a third field and change the size of the round's score
void  UIndividualScore::Setup(int32 NewScoreSize, int32 newScoreId)
{

    // update id and score size
    ScoreId = newScoreId;
    ScoreSize = NewScoreSize;
    
    // adjust widgets depending on size of score set (2 or 3)
    if(NewScoreSize == 2)
    {
        // hide third score field
        ThirdScoreText->SetVisibility(ESlateVisibility::Collapsed);
        
        // shrink total score field to fit 2 score fields
        UCanvasPanelSlot* TotalScoreTextGraphicSlot = Cast<UCanvasPanelSlot>(TotalScoreTextGraphic->Slot);
        if (Slot)
        {
            TotalScoreTextGraphicSlot->SetSize(FVector2D(110.0f, 50.0f));
        }
    } 
    else
    {
        // show third score field
        ThirdScoreText->SetVisibility(ESlateVisibility::Visible);
        
        // expand total score field to fit 3 score fields
        UCanvasPanelSlot* TotalScoreTextGraphicSlot = Cast<UCanvasPanelSlot>(TotalScoreTextGraphic->Slot);
        if (Slot)
        {
            TotalScoreTextGraphicSlot->SetSize(FVector2D(170.0f, 50.0f));
        }
    } 
}

void  UIndividualScore:: ValidateFirstTextInput(const FText& InputText)
{
    ValidateAnyTextInput(InputText, 0);
}

void  UIndividualScore:: ValidateSecondTextInput(const FText& InputText)
{
    ValidateAnyTextInput(InputText, 1);
}

void  UIndividualScore:: ValidateThirdTextInput(const FText& InputText)
{
    ValidateAnyTextInput(InputText, 2);
}

void  UIndividualScore::ValidateAnyTextInput(const FText& InputText, int32 TextPosition)
{
    // change to string to easily manipulate chars 
    FString InputString = InputText.ToString();
    UE_LOG(LogTemp, Display, TEXT("Character input: %s"), *InputString);
    UE_LOG(LogTemp, Display, TEXT("String size: %i"), InputString.Len());

    // check for empty strings (actually happened in testing)
    if(InputString.Len() == 0)
        return;

    // get first character in input
    TCHAR FirstChar = InputString[0];

    // get last character in input
    TCHAR LastChar = InputString[InputString.Len() - 1];

    // verify previous text fields have values
    for(int32 CurrentIndex = 0; CurrentIndex < TextPosition; CurrentIndex++)
    {
        if(TextBoxesArray[CurrentIndex]->GetText().ToString().Len() == 0)
        {
            // if any previous text field is not empty don't accept input and return
            UpdateTextAt(TextPosition, FString(""));
            return;
        }        
    }

    // validate input with regex - only numbers and f, F, x, X, / and -
    FRegexPattern Pattern(TEXT("[0-9fFxX/-]")); 
    FRegexMatcher RegexTester(Pattern, FString(1, &LastChar));

    if (RegexTester.FindNext())
    {

        // verify by position:
        // first slot cannot contain a slash,
        // second and third cannot contain an X (unless is size 3 which is the last one)
        if(
            (TextPosition == 0 && LastChar == '/') 
            || (TextPosition > 0 && (LastChar == 'x' || LastChar == 'X') && ScoreSize == 2))
        {
            
            UpdateTextAt(TextPosition, FString(1, &FirstChar));
            return;
        }

        UE_LOG(LogTemp, Display, TEXT("Character is valid"));

        // new character is valid
        UpdateTextAt(TextPosition, FString(1, &LastChar));

        // if new valid value is added update score (broadcast values and expect new total)
        ScoreDelegate.Broadcast();
    }
    else if(InputString.Len() == 1)
    {
        UE_LOG(LogTemp, Display, TEXT("Character is NOT valid"));
        // there was no previous character and this was not valid, set empty string
        UpdateTextAt(TextPosition, FString(""));
    } 
    else 
    {
        UE_LOG(LogTemp, Display, TEXT("new character is not valid, setting old one"));
        // there was a previously validated character, just set it again
        UpdateTextAt(TextPosition, FString(1, &FirstChar));
    }
}

int32 UIndividualScore::GetLastSavedTotal() 
{
    return LastSavedTotal;
}

void UIndividualScore::SetNewTotal(int32 NewTotal) 
{
    // save value
    LastSavedTotal = NewTotal;

    UE_LOG(LogTemp, Display, TEXT("new last saved: %d"), LastSavedTotal);
    
    // display on widget
    TotalScoreText->SetText(FText::FromString(FString::FromInt(LastSavedTotal)));
}

TArray<FString> UIndividualScore::GetContents()
{
    return TextBoxesContentArray;
}

void UIndividualScore::UpdateTextAt(int32 Index, FString NewValue)
{
    // save value on array
    TextBoxesContentArray[Index] = NewValue;

    // set value on GUI
    UEditableTextBox* TextBeingUpdated = TextBoxesArray[Index];
    TextBeingUpdated->SetText(FText::FromString(NewValue));
    
}