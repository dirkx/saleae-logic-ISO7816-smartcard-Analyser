//
// Copyright © 2013 Dirk-Willem van Gulik <dirkx@webweaving.org>, all rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at:
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
//

#ifndef ISO7816_ANALYZER_H
#define ISO7816_ANALYZER_H

#include <Analyzer.h>

#include <AnalyzerChannelData.h>
#include <AnalyzerHelpers.h>
#include <AnalyzerResults.h>

#include "iso7816AnalyzerResults.h"
#include "iso7816SimulationDataGenerator.h"

typedef enum { UNK, DIRECT, INVERSE } iso7816_mode_t;

class iso7816AnalyzerSettings;
class ANALYZER_EXPORT iso7816Analyzer : public Analyzer
{
public:
	iso7816Analyzer();
	virtual ~iso7816Analyzer();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	std::auto_ptr< iso7816AnalyzerSettings > mSettings;
	std::auto_ptr< iso7816AnalyzerResults > mResults;

	AnalyzerChannelData* mIo;
	AnalyzerChannelData* mReset;

	iso7816SimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //ISO7816_ANALYZER_H
