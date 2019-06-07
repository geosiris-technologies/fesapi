/*-----------------------------------------------------------------------
Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"; you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.
-----------------------------------------------------------------------*/
#include "resqml2_0_1test/PolylineSetRepresentation.h"

#include "../catch.hpp"
#include "common/EpcDocument.h"
#include "resqml2_0_1/FaultInterpretation.h"
#include "resqml2_0_1/LocalTime3dCrs.h"
#include "resqml2_0_1/PolylineSetRepresentation.h"
#include "common/HdfProxy.h"
#include "resqml2_0_1test/FaultInterpretationTest.h"
#include "resqml2_0_1test/LocalTime3dCrs.h"

#include <stdexcept>

using namespace std;
using namespace resqml2_0_1test;
using namespace COMMON_NS;

const char* PolylineSetRepresentation::defaultUuid = "60b04722-8608-4e92-8f1d-596372dd309e";
const char* PolylineSetRepresentation::defaultTitle = "Polyline represenation (in time)";
unsigned int PolylineSetRepresentation::numNodesPerPolylinePerPatch[] = { 3, 2 };
double PolylineSetRepresentation::polylinePoints[] = { 150, 0, 200, 300, 0, 350, 450, 0, 500, 150, 200, 200, 450, 200, 500 };

PolylineSetRepresentation::PolylineSetRepresentation(const string & epcDocPath)
	: commontest::AbstractObjectTest(epcDocPath) {
}

PolylineSetRepresentation::PolylineSetRepresentation(EpcDocument* epcDoc, bool init)
	: commontest::AbstractObjectTest(epcDoc) {
	if (init)
		initEpcDoc();
	else
		readEpcDoc();
}

void PolylineSetRepresentation::initEpcDocHandler() {
	RESQML2_0_1_NS::FaultInterpretation * interp = epcDoc->getDataObjectByUuid<RESQML2_0_1_NS::FaultInterpretation>(FaultInterpretationTest::defaultUuid);
	if (interp == nullptr) {
		FaultInterpretationTest interpTest(epcDoc, true);
		interp = epcDoc->getDataObjectByUuid<RESQML2_0_1_NS::FaultInterpretation>(FaultInterpretationTest::defaultUuid);
	}

	RESQML2_0_1_NS::LocalTime3dCrs * crs = epcDoc->getDataObjectByUuid<RESQML2_0_1_NS::LocalTime3dCrs>(LocalTime3dCrs::defaultUuid);
	if (crs == nullptr) {
		LocalTime3dCrs crsTest(epcDoc, true);
		crs = epcDoc->getDataObjectByUuid<RESQML2_0_1_NS::LocalTime3dCrs>(LocalTime3dCrs::defaultUuid);
	}

	RESQML2_0_1_NS::PolylineSetRepresentation* rep = epcDoc->createPolylineSetRepresentation(interp, crs, defaultUuid, defaultTitle);
	REQUIRE(rep != nullptr);
	rep->pushBackGeometryPatch(numNodesPerPolylinePerPatch, polylinePoints, 2, false, epcDoc->getHdfProxySet()[0]);
}

void PolylineSetRepresentation::readEpcDocHandler()
{
}
