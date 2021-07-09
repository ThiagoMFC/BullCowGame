// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"


void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    

    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordList/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);
    BuildIsogramList(Words);
    
    SetupGame();

    //PrintLine(TEXT("the word is %s"), *HiddenWord);//debug line
 
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver) {
        ClearScreen();
        
        SetupGame();
    }
    else {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame() {
    HiddenWord = SelectRandomWord(Isograms);
    Lives = HiddenWord.Len();
    bGameOver = false;
    //PrintLine(TEXT("the word is %s"), *HiddenWord);//debug line
    PrintLine(TEXT("Welcome to the bull cows game"));
    PrintLine(TEXT("Press 'Tab' to enter the terminal"));
    PrintLine(TEXT("Guess the word. It is %i characters long"), HiddenWord.Len());
    PrintLine(TEXT("The first letter is %c, the last is %c"), HiddenWord[0], HiddenWord[HiddenWord.Len()-1]);
    //PrintLine(TEXT("The number of possible words is %i "), Isograms.Num());
    PrintLine(TEXT("You have %i lives"), Lives);
}

void UBullCowCartridge::EndGame() {
    bGameOver = true;
    
    PrintLine(TEXT("\nPress enter to play again"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess) {
    if (Guess == HiddenWord) {
        ClearScreen();
        PrintLine(TEXT("You won"));
        EndGame();
        return;
    }

    --Lives;
    PrintLine(TEXT("you lost a life"));

    if (Lives <= 0) {
        ClearScreen();
        PrintLine(TEXT("You have no lives left"));
        PrintLine(TEXT("\nThe hidden word was %s"), *HiddenWord);
        EndGame();
        return;
       
    }

    if (!IsIsogram(Guess)) {
        PrintLine(TEXT("No repeating letters"));
    }


    FBullCowCount Score = GetBullCows(Guess);
    PrintLine(TEXT("You have %i bulls and %i cows"), Score.Bulls, Score.Cows);
    PrintLine(TEXT("Try again, you have %i lives remaining"), Lives);
    
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const{

    for (int32 Index = 0; Index < Word.Len(); Index++) {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++) {
            if (Word[Index] == Word[Comparison]) {
                return false;
            }
        }
    }

    return true;
}

void UBullCowCartridge::BuildIsogramList(const TArray<FString>& Words) {

    for (FString Isogram : Words) {
        if (Isogram.Len() >= 4 && IsIsogram(Isogram)) {
            Isograms.Emplace(Isogram);
        }
    }

}

FString UBullCowCartridge::SelectRandomWord(const TArray<FString>& IsogramList) {
    int32 RandomNumber = FMath::RandRange(0, IsogramList.Num()-1);
    return IsogramList[RandomNumber];
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    //BullCount = 0;
    //CowCount = 0;

    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++) {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex]) {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++) {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex]) {
                Count.Cows++;
                break;
            }
        }
    }

    return Count;
}

