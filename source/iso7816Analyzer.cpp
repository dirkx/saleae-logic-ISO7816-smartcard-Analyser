//
// Copyright © 2013 Dirk-Willem van Gulik <dirkx@webweaving.org>, all rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at:
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
//


#include "iso7816Analyzer.h"
#include "iso7816AnalyzerSettings.h"

#include <AnalyzerChannelData.h>
#include <AnalyzerHelpers.h>
#include <AnalyzerResults.h>

#include <iostream>
#include <sstream>

static const unsigned char msb2lsb[] = 
{
  0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 
  0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 
  0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 
  0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 
  0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 
  0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
  0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 
  0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
  0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
  0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 
  0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
  0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
  0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 
  0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
  0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 
  0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

static const bool parity[256] = 
{
#   define P2(n) n, n^1, n^1, n
#   define P4(n) P2(n), P2(n^1), P2(n^1), P2(n)
#   define P6(n) P4(n), P4(n^1), P4(n^1), P4(n)
    P6(0), P6(1), P6(1), P6(0)
};

iso7816Analyzer::iso7816Analyzer()
:	Analyzer(),  
	mSettings( new iso7816AnalyzerSettings() ),
	mSimulationInitilized( false )
{
	SetAnalyzerSettings( mSettings.get() );
}

iso7816Analyzer::~iso7816Analyzer()
{
	KillThread();
}

void iso7816Analyzer::WorkerThread()
{
	iso7816_mode_t mode;

	mResults.reset( new iso7816AnalyzerResults( this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );

	mResults->AddChannelBubblesWillAppearOn( mSettings->mIoChannel );
	// mResults->AddChannelBubblesWillAppearOn( mSettings->mResetChannel );

	mIo = GetAnalyzerChannelData( mSettings->mIoChannel );
	mReset= GetAnalyzerChannelData( mSettings->mResetChannel );

	for( ; ; ) {
		// seek for a RESET going high.
		//
		mReset->AdvanceToNextEdge();

		if (mReset->GetBitState() != BIT_HIGH )
			continue;

		// discard all serial data until now.
		//
		mIo->AdvanceToAbsPosition( mReset->GetSampleNumber() );

		// mark the start in pretty much all channels.
		//
		// mResults->AddMarker(mReset->GetSampleNumber(), AnalyzerResults::UpArrow, mSettings->mResetChannel);
		// mResults->AddMarker(mIo->GetSampleNumber(), AnalyzerResults::UpArrow, mSettings->mIoChannel);

		// We know expect a TS (After some 400 to 40k clock cycles of idleness XX).
		// We can use the first up/down dip to measure the baud rate.
		//
		//  S01234567P     S01234567P
		// HLHHLLLLLLH or HLHHLHHHLLH
		//   11000000 1     11011100 1
 		//   xC0            xDC
		// inverse        direct convention
		//
		// search for first stop bit.
		//
		while(mIo->GetBitState() != BIT_HIGH)
			mIo->AdvanceToNextEdge();

		mIo->AdvanceToNextEdge();
	
		U64 a0 = mIo->GetSampleNumber();
		mIo->AdvanceToNextEdge();

		U64 a1 = mIo->GetSampleNumber();
		U64 ea = (a1 - a0);

		if (mIo->GetBitState() != BIT_HIGH) {
			mResults->AddMarker(mIo->GetSampleNumber(), AnalyzerResults::ErrorDot, mSettings->mIoChannel);
			mResults->CommitResults();
			printf("Start bit end error\n");
			continue;
		}

		mResults->AddMarker(a0, AnalyzerResults::DownArrow, mSettings->mIoChannel);
		mResults->AddMarker(a0+ea/2, AnalyzerResults::Start, mSettings->mIoChannel);
		mResults->AddMarker(a1, AnalyzerResults::UpArrow, mSettings->mIoChannel);

		U16 data = 0; 

		for(U32 i = 0; i <= 8; i++) {
			// move to the center of what we're guessing as best as we can
			mIo->AdvanceToAbsPosition(a1 + ea * (0.5 + i));
			U8 bit = mIo->GetBitState() ? 1 : 0;

			mResults->AddMarker(mIo->GetSampleNumber(), bit ? AnalyzerResults::One : AnalyzerResults::Zero, mSettings->mIoChannel);

			data = (data <<1) | bit;
		};
		U8 p = data  & 1;
		data >>= 1;

		if (parity[ data ] != p) {
			mResults->AddMarker(
				a1 + ea * (0.5 + 9), AnalyzerResults::ErrorDot, mSettings->mIoChannel
			);
			mResults->CommitResults();
			printf("Parity error\n");
			continue;
		};
		
		mResults->AddMarker(a1 + ea * (0.5 + 9), AnalyzerResults::Stop, mSettings->mIoChannel);
		if (mIo->GetBitState() != BIT_HIGH) {
			printf("Stop bit not high.");
			mResults->AddMarker(
				a1 + ea * 10, AnalyzerResults::ErrorDot, mSettings->mIoChannel
			);
			mResults->CommitResults();
			continue;
		};

		switch(data & 0xFF) {
		case 0xC0:
			mode = INVERSE;
			printf("Inverse mode msb first trransmitted\n");
			break;
		case 0xDC:
			mode = DIRECT;
			printf("Direct mode lsb first transmitted\n");
			break;
		default:
			printf("unk mode %x\n", data);
			mResults->AddMarker(mIo->GetSampleNumber(),
				AnalyzerResults::ErrorDot, mSettings->mIoChannel);
			mResults->CommitResults();
			continue;
		}

		if (mode == INVERSE) {
			data = ~data;
            if (data != 0x3F)
                AnalyzerHelpers::Assert("Internal calculation issue or something similarly odd. Expected 0x3F for INVERSE mode.");
		};
		if (mode == DIRECT) {
			data = msb2lsb[data & 0xFF];
            if (data != 0x3B)
                AnalyzerHelpers::Assert("Internal calculation issue or something similarly odd. Expected 0x3B for DIRECT mode.");
		};
		{
			Frame frame;
			frame.mData1 = 0;
			frame.mFlags = mode;
			frame.mStartingSampleInclusive = a0;
			frame.mEndingSampleInclusive = mIo->GetSampleNumber();

			mResults->AddFrame( frame );
			mResults->CommitResults();
		}

		// now we keep waiting for the next 'down'; start bit
		// and then read our 10 bits, etc, etc.
		for(;;) {
			mIo->AdvanceToNextEdge(); //falling edge -- beginning of the start bit
			if (mIo->GetBitState() != BIT_LOW) {
				mResults->AddMarker(mIo->GetSampleNumber(),
					AnalyzerResults::ErrorDot, mSettings->mIoChannel);
				mResults->CommitResults();
				printf("Out of sync with start bit.\n");
				break;
			};

			U64 starting_sample = mIo->GetSampleNumber();
			
			mIo->Advance( (U32)(ea / 2));
			mResults->AddMarker(mIo->GetSampleNumber(), AnalyzerResults::Start, mSettings->mIoChannel);

			U16 data = 0; 

			for(U32 i = 0; i <= 8; i++) {
				mIo->Advance((U32)ea);

				U8 bit = mIo->GetBitState() ? 1 : 0;
				mResults->AddMarker(mIo->GetSampleNumber(), bit ? AnalyzerResults::One : AnalyzerResults::Zero, mSettings->mIoChannel);

				data = (data <<1) | bit;
			};

			U8 p = data  & 1;
			data >>= 1;

			if (parity[ data ] != p) {
				mResults->AddMarker(
					mIo->GetSampleNumber(),AnalyzerResults::ErrorDot, mSettings->mIoChannel
				);
				mResults->CommitResults();
				printf("Parity error\n");
				break;
			};

			mIo->Advance((U32)ea);
			if (mIo->GetBitState() != BIT_HIGH) {
				printf("Stop bit not high.");
				mResults->AddMarker(
					mIo->GetSampleNumber(), AnalyzerResults::ErrorDot, mSettings->mIoChannel
				);
				mResults->CommitResults();
				break;
			};

			mResults->AddMarker(mIo->GetSampleNumber(), AnalyzerResults::Stop, mSettings->mIoChannel);

	                if (mode == INVERSE) 
	       	                 data = (!data) & 0xFF;
			else 
                        	data = msb2lsb[data & 0xFF];
		
			Frame frame;
			frame.mData1 = data;
			frame.mFlags = 0;
			frame.mStartingSampleInclusive = starting_sample;
			frame.mEndingSampleInclusive = mIo->GetSampleNumber();

			mResults->AddFrame( frame );
			mResults->CommitResults();
			ReportProgress( frame.mEndingSampleInclusive );
		}
	}
}

bool iso7816Analyzer::NeedsRerun()
{
	return false;
}

U32 iso7816Analyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 iso7816Analyzer::GetMinimumSampleRateHz()
{
	return 0;
}

const char* iso7816Analyzer::GetAnalyzerName() const
{
	return "ISO 7816 SmartCard";
}

const char* GetAnalyzerName()
{
	return "ISO 7816 SmartCard";
}

Analyzer* CreateAnalyzer()
{
	return new iso7816Analyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}
