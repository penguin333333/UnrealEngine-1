// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "NetworkFileSystemPrivatePCH.h"
#include "TargetDeviceId.h"
#include "ITargetDevice.h"
#include "ITargetPlatformManagerModule.h"
#include "ModuleManager.h"


DEFINE_LOG_CATEGORY(LogFileServer);


/**
 * Implements the NetworkFileSystem module.
 */
class FNetworkFileSystemModule
	: public INetworkFileSystemModule
{
public:

	// INetworkFileSystemModule interface

	virtual INetworkFileServer* CreateNetworkFileServer( bool bLoadTargetPlatforms, int32 Port, const FFileRequestDelegate* InFileRequestDelegate, const FRecompileShadersDelegate* InRecompileShadersDelegate, const ENetworkFileServerProtocol Protocol ) const override
	{
		TArray<ITargetPlatform*> ActiveTargetPlatforms;
		if (bLoadTargetPlatforms)
		{
			ITargetPlatformManagerModule& TPM = GetTargetPlatformManagerRef();

			// if we didn't specify a target platform then use the entire target platform list (they could all be possible!)
			FString Platforms;
			if (FParse::Value(FCommandLine::Get(), TEXT("TARGETPLATFORM="), Platforms))
			{
				ActiveTargetPlatforms =  TPM.GetActiveTargetPlatforms();
			}
			else
			{
				ActiveTargetPlatforms = TPM.GetTargetPlatforms();
			}
		}

		switch ( Protocol )
		{
#if ENABLE_HTTP_FOR_NFS
		case NFSP_Http: 
			return new FNetworkFileServerHttp(Port, InFileRequestDelegate, InRecompileShadersDelegate, ActiveTargetPlatforms);
#endif
		case NFSP_Tcp:
			return new FNetworkFileServer(Port, InFileRequestDelegate, InRecompileShadersDelegate, ActiveTargetPlatforms);
		}
 
		return NULL;
	}
};


IMPLEMENT_MODULE(FNetworkFileSystemModule, NetworkFileSystem);
