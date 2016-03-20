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

#include "iso7816AnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "iso7816Analyzer.h"
#include "iso7816AnalyzerSettings.h"
#include <iostream>
#include <fstream>

iso7816AnalyzerResults::iso7816AnalyzerResults( iso7816Analyzer* analyzer, iso7816AnalyzerSettings* settings )
:	AnalyzerResults(),
	mSettings( settings ),
	mAnalyzer( analyzer )
{
}

iso7816AnalyzerResults::~iso7816AnalyzerResults()
{
}
#if 0
void iso7816AnalyzerResults::GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base )
{
	ClearResultStrings();
	Frame frame = GetFrame( frame_index );

	char number_str[128];
	AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );
	AddResultString( number_str );
}
#endif

void iso7816AnalyzerResults::GenerateBubbleText(U64 frame_index, Channel& channel, DisplayBase display_base)  //unrefereced vars commented out to remove warnings.
{
	char number_str[128];
    //we only need to pay attention to 'channel' if we're making bubbles for more than one channel (as set by AddChannelBubblesWillAppearOn)
    ClearResultStrings();
    Frame frame = GetFrame(frame_index);

	if (channel == mSettings->mResetChannel)
	{
		switch (frame.mFlags)
		{
		case ATR:
			AddResultString("A");
			AddResultString("ATR");
			AddResultString("ATR");
			break;

		case PPS:
			AddResultString("P");
			AddResultString("PPS");
			AddResultString("PPS: ", ((iso7816Analyzer*)mAnalyzer)->GetDetails(static_cast<int>(frame.mData1)).c_str());
			break;

		default:
			break;
		}
	}
	else
	{
		switch (frame.mFlags)
		{
		case 0:
			AnalyzerHelpers::GetNumberString(frame.mData1, display_base, 8, number_str, sizeof(number_str));
			AddResultString(number_str);
			break;

		case INVERSE:
			AddResultString("I");
			AddResultString("INV");
			AddResultString("Hdr(INVERSE)");
			break;

		case DIRECT:
			AddResultString("D");
			AddResultString("DIR");
			AddResultString("Hdr(DIRECT)");
			break;

		case ATR:
			break;

		case PPS:
			break;

		default:
			AddResultString("?");
			AnalyzerHelpers::GetNumberString(frame.mFlags, display_base, 8, number_str, sizeof(number_str));
			AddResultString(number_str);
			break;
		}
	}
}

void iso7816AnalyzerResults::GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id )
{
	std::ofstream file_stream( file, std::ios::out );

	U64 trigger_sample = mAnalyzer->GetTriggerSample();
	U32 sample_rate = mAnalyzer->GetSampleRate();

	file_stream << "Time [s],Value" << std::endl;

	U64 num_frames = GetNumFrames();
	for( U32 i=0; i < num_frames; i++ )
	{
		Frame frame = GetFrame( i );
		
		char time_str[128];
		AnalyzerHelpers::GetTimeString( frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128 );

		char number_str[128];
		AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );

		file_stream << time_str << "," << number_str << std::endl;

		if( UpdateExportProgressAndCheckForCancel( i, num_frames ) == true )
		{
			file_stream.close();
			return;
		}
	}

	file_stream.close();
}

void iso7816AnalyzerResults::GenerateFrameTabularText( U64 frame_index, DisplayBase display_base )
{
	Frame frame = GetFrame( frame_index );
	ClearResultStrings();

	char number_str[128];
	AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );
	AddResultString( number_str );
}

void iso7816AnalyzerResults::GeneratePacketTabularText( U64 packet_id, DisplayBase display_base )
{
	ClearResultStrings();
	AddResultString( "not supported" );
}

void iso7816AnalyzerResults::GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base )
{
	ClearResultStrings();
	AddResultString( "not supported" );
}
