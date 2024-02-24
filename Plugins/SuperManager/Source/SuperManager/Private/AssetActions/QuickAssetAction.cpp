// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetActions/QuickAssetAction.h"
#include "DebugHeader.h"
#include "EditorUtilityLibrary.h"  //info about each asset can determine paths and such
#include "EditorAssetLibrary.h"

/*
* the asset action utility gives us the panel that pops up in the editor
*/
void UQuickAssetAction::DuplicateAssets(int32 NumOfDuplicates)
{
	if (NumOfDuplicates <= 0) //check bad user input
	{
	
		ShowMsgDialog(EAppMsgType::Ok, TEXT("Please enter a valid number"));
		return;
	}

	TArray<FAssetData> SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData(); //creates an array with the asset data for the assets that are selected
	uint32 Counter = 0; //the number of assets have been duplicated

	/*
	* this structure is a nested for loop that gives access to asset data for each duplicate of each selected asset
	* for example you can highlight 2 assets in the content browset and then right click to access the scripted asset actions option, then enter the number of dups
	* for each selected object for each dup the asset data is taken to be able to create the dup using the Duplicate asset function. 
	* 
	* 
	*/
	for (const FAssetData& SelectedAssetData : SelectedAssetsData)  //Range based for loop   //we use a const Reference here to avoid unnecerry copies being created
	{
		for (int32 i = 0; i < NumOfDuplicates; i++)
		{
			const FString SourceAssetPath = SelectedAssetData.ObjectPath.ToString(); //the source asset path default is FName
			const FString NewDuplicatesAssetName = SelectedAssetData.AssetName.ToString() + TEXT("_") + FString::FromInt(i + 1); //giving the duplicate asset the name of the origional asset+(i+1)
			const FString NewPathName = FPaths::Combine(SelectedAssetData.PackagePath.ToString(), NewDuplicatesAssetName);

			if (UEditorAssetLibrary::DuplicateAsset(SourceAssetPath, NewPathName))
			{
				UEditorAssetLibrary::SaveAsset(NewPathName, false);
				++Counter;
			}
			
		}
	}

	if (Counter > 0)
	{
		//Print(TEXT("Successfully duplicated" + FString::FromInt(Counter) + "files"), FColor::Green);
		ShowNotifyInfo(TEXT("Successfully duplicated" + FString::FromInt(Counter) + " files "));
	}
	
}

void UQuickAssetAction::AddPrefixes()
{
	TArray<UObject*>SelecctedObjects = UEditorUtilityLibrary::GetSelectedAssets();
	uint32 Counter = 0;

	for (UObject* SelectedObject : SelecctedObjects) {

		if (!SelectedObject) continue;

		FString* PrefixFound = PrefixMap.Find(SelectedObject->GetClass());

		if (!PrefixFound || PrefixFound->IsEmpty())
		{
			Print(TEXT("Fail to find prefix for class") + SelectedObject->GetClass()->GetName(), FColor::Red);
			continue;
		}
		FString OldName = SelectedObject->GetName();

		if (OldName.StartsWith(*PrefixFound))
		{
			Print(OldName + TEXT(" already has prefix added "), FColor::Red);
			continue;
		}
		//MySolition to remove the M_ and _Inst 
		if (*PrefixFound == "MI_") {
			OldName.RemoveAt(0,2);

			int I = OldName.Find("_Inst");

			OldName.RemoveAt(I, 5);

		}

		/*The instructors solution
		* if(SelectedObject->IsA<UMaterialInstanceConstant>())
		* {
		*	    OldName.RemoveFromStart(TEXT("M_"));
		*		OldName.RemoveFromEnd(TEXT("_Inst));
		* }
		*/

		const FString NewNameWithPrefix = *PrefixFound + OldName;

		

		UEditorUtilityLibrary::RenameAsset(SelectedObject, NewNameWithPrefix);

		++Counter;
			
	}
	if (Counter > 0)
	{
		ShowNotifyInfo(TEXT("Successfully renamed " + FString::FromInt(Counter) + " assets"));
	}
}