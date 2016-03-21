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

#ifndef ISO7816_ANALYZER_SETTINGS
#define ISO7816_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class iso7816AnalyzerSettings : public AnalyzerSettings
{
public:
	iso7816AnalyzerSettings();
	virtual ~iso7816AnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	Channel mVccChannel, mResetChannel, mClkChannel, mIoChannel;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mVccChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mResetChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mClkChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mIoChannelInterface;
};

#endif //ISO7816_ANALYZER_SETTINGS
