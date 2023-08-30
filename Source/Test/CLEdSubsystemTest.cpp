// CubyLab Games™ All Rights Reserved


#include "CLEdSubsystemTest.h"

void UCLEdSubsystemTest::WillThisWork()
{

}

void UCLEdSubsystemTest::GetAllEditorSubsystems(TArray<UEditorSubsystem*>& EditorSubsystem)
{
	// Get all subsystem instances
	EditorSubsystem = GEditor->GetEditorSubsystemArray<UEditorSubsystem>();
}
