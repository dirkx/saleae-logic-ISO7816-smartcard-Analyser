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

#ifndef ISO7816_ANALYZER_RESULTS
#define ISO7816_ANALYZER_RESULTS

#include <AnalyzerResults.h>

class iso7816Analyzer;
class iso7816AnalyzerSettings;

class iso7816AnalyzerResults : public AnalyzerResults
{
public:
	iso7816AnalyzerResults( iso7816Analyzer* analyzer, iso7816AnalyzerSettings* settings );
	virtual ~iso7816AnalyzerResults();

	virtual void GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base );
	virtual void GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id );

	virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base );
	virtual void GeneratePacketTabularText( U64 packet_id, DisplayBase display_base );
	virtual void GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base );

protected: //functions

protected:  //vars
	iso7816AnalyzerSettings* mSettings;
	iso7816Analyzer* mAnalyzer;
};

#endif //ISO7816_ANALYZER_RESULTS
