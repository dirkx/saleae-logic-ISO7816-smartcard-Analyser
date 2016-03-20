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

#ifndef ISO7816_SIMULATION_DATA_GENERATOR
#define ISO7816_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class iso7816AnalyzerSettings;

class iso7816SimulationDataGenerator
{
public:
	iso7816SimulationDataGenerator();
	~iso7816SimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, iso7816AnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	iso7816AnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	SimulationChannelDescriptor mSerialSimulationData;

};
#endif //ISO7816_SIMULATION_DATA_GENERATOR