#pragma once
#include "Misc/MessageDialog.h"

#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"

void Print(const FString& Message, const FColor& Color)
{
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 8.f, Color, Message);
	}
}

void PrintLog(const FString& Message)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
}

EAppReturnType::Type ShowMsgDialog(EAppMsgType::Type MsgType, const FString& Message, 
 bool bShowMsgAsWarning = true) 
{
	if (bShowMsgAsWarning) 
	{

		FText MsgTitle = FText::FromString(TEXT("Warning"));

		return FMessageDialog::Open( MsgType, FText::FromString(Message), &MsgTitle); //allows us to use a warning panel box instead of a pritn to screen
	}
	else
	{
		return FMessageDialog::Open(MsgType, FText::FromString(Message));//this is the option to show dialog w/o it being a warning
	}
	

}

void ShowNotifyInfo(const FString& Message) { //this is code that allows us to show notifications in the same panel that the compiled successfully messege pops up in 

	FNotificationInfo NotifyInfo(FText::FromString(Message));
	NotifyInfo.bUseLargeFont = true;
	NotifyInfo.FadeOutDuration = 7.f;

	FSlateNotificationManager::Get().AddNotification(NotifyInfo);
}