//
// Copyright © 2013 Dirk-Willem van Gulik <dirkx@webweaving.org>, all rights reserved.
// Copyright © 2016 Adam Augustyn <adam@augustyn.net>, all rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at:
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
//

#include "iso7816AnalyzerSettings.h"
#include <AnalyzerHelpers.h>

iso7816AnalyzerSettings::iso7816AnalyzerSettings()
:	mVccChannel( UNDEFINED_CHANNEL ),
	mResetChannel( UNDEFINED_CHANNEL ),
	mClkChannel( UNDEFINED_CHANNEL ),
	mIoChannel( UNDEFINED_CHANNEL )
{
	mVccChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mClkChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mResetChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mIoChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );

	mVccChannelInterface->SetTitleAndTooltip( "VCC/C1", "C1" );
	mResetChannelInterface->SetTitleAndTooltip( "RST/C2", "C2 - Reset" );
	mClkChannelInterface->SetTitleAndTooltip( "CLK/C3", "C3 - SCL or CLK" );
	mIoChannelInterface->SetTitleAndTooltip( "IO/C7", "C7 - SDA or IO" );

	mVccChannelInterface->SetChannel( mVccChannel );
	mResetChannelInterface->SetChannel( mResetChannel );
	mClkChannelInterface->SetChannel( mClkChannel );
	mIoChannelInterface->SetChannel( mIoChannel );

	AddInterface( mVccChannelInterface.get() );
	AddInterface( mResetChannelInterface.get() );
	AddInterface( mClkChannelInterface.get() );
	AddInterface( mIoChannelInterface.get() );

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );

	ClearChannels();
	AddChannel( mVccChannel, "VCC", false );
	AddChannel( mResetChannel, "RST", false );
	AddChannel( mClkChannel, "SCL/CLK", false );
	AddChannel( mIoChannel, "SDA/IO", false );
}

iso7816AnalyzerSettings::~iso7816AnalyzerSettings()
{
}

bool iso7816AnalyzerSettings::SetSettingsFromInterfaces()
{
	mVccChannel = mVccChannelInterface->GetChannel();
	mResetChannel = mResetChannelInterface->GetChannel();
	mClkChannel = mClkChannelInterface->GetChannel();
	mIoChannel = mIoChannelInterface->GetChannel();

	ClearChannels();
	AddChannel( mVccChannel, "VCC", true );
	AddChannel( mResetChannel, "RST", true );
	AddChannel( mClkChannel, "SCL/CLK", true );
	AddChannel( mIoChannel, "SDA/IO", true );

	return true;
}

void iso7816AnalyzerSettings::UpdateInterfacesFromSettings()
{
	mVccChannelInterface->SetChannel( mVccChannel );
	mResetChannelInterface->SetChannel( mResetChannel );
	mClkChannelInterface->SetChannel( mClkChannel );
	mIoChannelInterface->SetChannel( mIoChannel );
}

void iso7816AnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mVccChannel;
	text_archive >> mResetChannel;
	text_archive >> mClkChannel;
	text_archive >> mIoChannel;

	ClearChannels();
	AddChannel( mVccChannel, "VCC", true);
	AddChannel( mResetChannel, "RST", true);
	AddChannel( mClkChannel, "SCL/CLK", true);
	AddChannel( mIoChannel, "SDA/IO", true);

	UpdateInterfacesFromSettings();
}

const char* iso7816AnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mVccChannel;
	text_archive << mResetChannel;
	text_archive << mClkChannel;
	text_archive << mIoChannel;

	return SetReturnString( text_archive.GetString() );
}
