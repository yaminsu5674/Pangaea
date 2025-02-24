// Fill out your copyright notice in the Description page of Project Settings.


#include "PangaeaGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"




void UPangaeaGameInstance::StartListenServer()
{
	auto world = GEngine->GetCurrentPlayWorld();
	UGameplayStatics::OpenLevel(world, "TopDownMap", true, "?listen");

}


void UPangaeaGameInstance::JoinAsClient(FString IPAddress)
{
	auto world = GEngine->GetCurrentPlayWorld();
	UGameplayStatics::OpenLevel(world, *IPAddress, true, "?join");
}


void UPangaeaGameInstance::LeaveGame()
{
	auto world = GEngine->GetCurrentPlayWorld();
	UGameplayStatics::OpenLevel(world, "LobbyMap");
}

void UPangaeaGameInstance::Init()
{
    Super::Init();

    // UWorld�� ���� GetNetMode() ȣ��
    UWorld* World = GetWorld();
    if (World)
    {
        ENetMode NetMode = World->GetNetMode();
        FString NetModeStr;

        switch (NetMode)
        {
        case NM_Standalone:
            NetModeStr = TEXT("Standalone");
            break;
        case NM_DedicatedServer:
            NetModeStr = TEXT("DedicatedServer");
            break;
        case NM_ListenServer:
            NetModeStr = TEXT("ListenServer");
            break;
        case NM_Client:
            NetModeStr = TEXT("Client");
            break;
        default:
            NetModeStr = TEXT("Unknown");
            break;
        }

        // ������ ���� �޽��� ���
        if (NetMode == NM_ListenServer )
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, FString::Printf(TEXT("This is Server: %s"), *NetModeStr));
        }
        // Ŭ���̾�Ʈ�� ���� �޽��� ���
        else if (NetMode == NM_Client)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("This is Client: %s"), *NetModeStr));
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("This is Client: %s"), *NetModeStr));
        }
    }
}
