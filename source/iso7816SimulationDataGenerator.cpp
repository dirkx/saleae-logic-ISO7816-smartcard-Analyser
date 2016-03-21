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

#include "iso7816SimulationDataGenerator.h"
#include "iso7816AnalyzerSettings.h"

#include <AnalyzerHelpers.h>
#include <stdlib.h>
#include <ctime>

iso7816SimulationDataGenerator::iso7816SimulationDataGenerator()
{
}

iso7816SimulationDataGenerator::~iso7816SimulationDataGenerator()
{
}

void iso7816SimulationDataGenerator::Initialize( U32 simulation_sample_rate, iso7816AnalyzerSettings* settings )
{
	mSimulationSampleRateHz = simulation_sample_rate;
	mSettings = settings;
    srand(rand() ^ static_cast<int>(time(NULL)));

	mSerialSimulationData.SetInitialBitState( BIT_HIGH );
}

U32 iso7816SimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel )
{
	U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample( largest_sample_requested, sample_rate, mSimulationSampleRateHz );

	while( mSerialSimulationData.GetCurrentSampleNumber() < adjusted_largest_sample_requested )
	{
        U32 samples_per_bit = 100; // mSimulationSampleRateHz / mSettings->mBitRate;
        
        U8 byte = rand() & 0xFF;

        mSerialSimulationData.Advance( samples_per_bit * 20 );
        
        mSerialSimulationData.Transition();  //low-going edge for start bit
        mSerialSimulationData.Advance( samples_per_bit );  //add start bit time
        mSerialSimulationData.Transition();  // end of start bit.
        mSerialSimulationData.Advance( samples_per_bit );  //add start bit time
        
        U8 mask = 0x1 << 7;
        for( U32 i=0; i<8; i++ )
        {
            if( ( byte & mask ) != 0 )
                mSerialSimulationData.TransitionIfNeeded( BIT_HIGH );
            else
                mSerialSimulationData.TransitionIfNeeded( BIT_LOW );
            
            mSerialSimulationData.Advance( samples_per_bit );
            mask = mask >> 1;
        }
        
        mSerialSimulationData.TransitionIfNeeded( BIT_HIGH ); //we need to end high
        mSerialSimulationData.Advance( samples_per_bit );
        
	}

	*simulation_channel = &mSerialSimulationData;
	return 1;
}

