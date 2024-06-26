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
#include "Trajectory.h"

#include <stdexcept>

#include "../catch.hpp"

#include "witsml2_1/Wellbore.h"
#include "witsml2/Trajectory.h"

using namespace std;
using namespace witsml2_test;
using namespace COMMON_NS;

const char* Trajectory::defaultUuid = "b4f02547-9fca-49ef-83a9-c96a802c857e";
const char* Trajectory::defaultTitle = "Witsml Trajectory Test";

Trajectory::Trajectory(const string & epcDocPath)
	: AbstractTest(epcDocPath) {
}

void Trajectory::initRepo() {
	WITSML2_NS::Wellbore* wellbore = repo->createPartial<WITSML2_1_NS::Wellbore>("", "");
	WITSML2_NS::Trajectory* traj = repo->createTrajectory(wellbore, defaultUuid, defaultTitle, false);
	traj->pushBackTrajectoryStation(gsoap_eml2_3::witsml21__TrajStationType::unknown, 250, gsoap_eml2_3::eml23__LengthUom::m);
	traj->pushBackTrajectoryStation(gsoap_eml2_3::witsml21__TrajStationType::DLS, 500, gsoap_eml2_3::eml23__LengthUom::ft, "my Uid");
	traj->setTrajectoryStationAzi(1, 15, gsoap_eml2_3::eml23__PlaneAngleUom::dega);
}

void Trajectory::readRepo() {
	WITSML2_NS::Trajectory* traj = repo->getDataObjectByUuid<WITSML2_NS::Trajectory>(defaultUuid);
	REQUIRE(traj != nullptr);
	REQUIRE(traj->getTrajectoryStationCount() == 2);
	REQUIRE(traj->getTrajectoryStationTypeTrajStation(0) == gsoap_eml2_3::witsml21__TrajStationType::unknown);
	REQUIRE(traj->getTrajectoryStationTypeTrajStation(1) == gsoap_eml2_3::witsml21__TrajStationType::DLS);
	REQUIRE(traj->getTrajectoryStationMdValue(0) == 250);
	REQUIRE(traj->getTrajectoryStationMdValue(1) == 500);
	REQUIRE(traj->getTrajectoryStationMdUom(0) == gsoap_eml2_3::eml23__LengthUom::m);
	REQUIRE(traj->getTrajectoryStationMdUom(1) == gsoap_eml2_3::eml23__LengthUom::ft);
	REQUIRE(traj->getTrajectoryStationuid(0) == "0");
	REQUIRE(traj->getTrajectoryStationuid(1) == "my Uid");
	REQUIRE(!traj->hasTrajectoryStationAzi(0));
	REQUIRE(traj->hasTrajectoryStationAzi(1));
	REQUIRE(traj->getTrajectoryStationAziValue(1) == 15);
	REQUIRE(traj->getTrajectoryStationAziUom(1) == gsoap_eml2_3::eml23__PlaneAngleUom::dega);
}
