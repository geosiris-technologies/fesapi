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
#include "witsml2_0/Well.h"

#include <stdexcept>
#include <sstream>

using namespace std;
using namespace WITSML2_0_NS;
using namespace gsoap_eml2_1;
using namespace epc;
using namespace COMMON_NS;

const char* Well::XML_TAG = "Well";

Well::Well(soap* soapContext,
			const std::string & guid,
			const std::string & title):resqmlWellboreFeature(nullptr)
{
	if (soapContext == nullptr) throw invalid_argument("A soap context must exist.");

	gsoapProxy2_1 = soap_new_witsml2__Well(soapContext, 1);

	initMandatoryMetadata();
	setMetadata(guid, title, "", -1, "", "", -1, "");
}

Well::Well(soap* soapContext,
		const std::string & guid,
		const std::string & title,
		const std::string & operator_,
		eml21__WellStatus statusWell,
		witsml2__WellDirection directionWell
	):resqmlWellboreFeature(nullptr)
{
	if (soapContext == nullptr) throw invalid_argument("A soap context must exist.");

	gsoapProxy2_1 = soap_new_witsml2__Well(soapContext, 1);

	initMandatoryMetadata();
	setMetadata(guid, title, "", -1, "", "", -1, "");

	setOperator(operator_);

	witsml2__Well* well = static_cast<witsml2__Well*>(gsoapProxy2_1);

	well->StatusWell = (eml21__WellStatus *)soap_malloc(soapContext, sizeof(eml21__WellStatus));
	*well->StatusWell = statusWell;

	well->DirectionWell = (witsml2__WellDirection *)soap_malloc(soapContext, sizeof(witsml2__WellDirection));
	*well->DirectionWell = directionWell;
}

void Well::setOperator(const string & operator_) 
{
	if (operator_.empty()) throw invalid_argument("You must set a non empty operator.");

	witsml2__Well* well = static_cast<witsml2__Well*>(gsoapProxy2_1);

	if (well->Operator == nullptr) {
		well->Operator = soap_new_std__string(gsoapProxy2_1->soap, 1);
	}
	well->Operator->assign(operator_);
}

double Well::getLocationProjectedX(unsigned int locationIndex)
{
	witsml2__Well* well = static_cast<witsml2__Well*>(gsoapProxy2_1);

	if (well->WellLocation.size() <= locationIndex) {
		throw range_error("The well location index is out of range.");
	}
	if (well->WellLocation[locationIndex]->soap_type() != SOAP_TYPE_gsoap_eml2_1_witsml2__ProjectedWellLocation){
		throw range_error("The well location is not a projected one.");
	}

	return static_cast<witsml2__ProjectedWellLocation*>(well->WellLocation[locationIndex])->Coordinate1;
}

double Well::getLocationProjectedY(unsigned int locationIndex)
{
	witsml2__Well* well = static_cast<witsml2__Well*>(gsoapProxy2_1);

	if (well->WellLocation.size() <= locationIndex) {
		throw range_error("The well location index is out of range.");
	}
	if (well->WellLocation[locationIndex]->soap_type() != SOAP_TYPE_gsoap_eml2_1_witsml2__ProjectedWellLocation){
		throw range_error("The well location is not a projected one.");
	}

	return static_cast<witsml2__ProjectedWellLocation*>(well->WellLocation[locationIndex])->Coordinate2;
}

void Well::pushBackLocation(
	const std::string & guid,
	double projectedX,
	double projectedY,
	unsigned int projectedCrsEpsgCode)
{
	witsml2__Well* well = static_cast<witsml2__Well*>(gsoapProxy2_1);

	witsml2__ProjectedWellLocation* location = soap_new_witsml2__ProjectedWellLocation(gsoapProxy2_1->soap, 1);
	if (guid.size() == 0) {
		ostringstream oss;
		oss << well->WellLocation.size();
		location->uid = oss.str();
	}
	else {
		location->uid = guid;
	}
	location->Coordinate1 = projectedX;
	location->Coordinate2 = projectedY;
	location->Crs = soap_new_eml21__ProjectedEpsgCrs(gsoapProxy2_1->soap, 1);
	static_cast<eml21__ProjectedEpsgCrs*>(location->Crs)->EpsgCode = projectedCrsEpsgCode;

	well->WellLocation.push_back(location);
}

unsigned int Well::geLocationCount() const {
	return static_cast<witsml2__Well*>(gsoapProxy2_1)->WellLocation.size();
}

void Well::pushBackDatum(
	const std::string & guid,
	const std::string & title,
	eml21__WellboreDatumReference code,
	const std::string & datum,
	eml21__LengthUom elevationUnit,
	double elevation,
	unsigned int verticalCrsEpsgCode)
{
	witsml2__Well* well = static_cast<witsml2__Well*>(gsoapProxy2_1);

	witsml2__WellDatum* wellDatum = soap_new_witsml2__WellDatum(gsoapProxy2_1->soap, 1);
	if (guid.empty()) {
		ostringstream oss;
		oss << well->WellDatum.size();
		wellDatum->uid = oss.str();
	}
	else {
		wellDatum->uid = guid;
	}
	wellDatum->Name = title.size() == 0 ? "unknown" : title;

	wellDatum->Code = (eml21__WellboreDatumReference *)soap_malloc(gsoapProxy2_1->soap, sizeof(eml21__WellboreDatumReference));
	*wellDatum->Code = code;

	wellDatum->Crs = soap_new_eml21__VerticalEpsgCrs(gsoapProxy2_1->soap, 1);
	static_cast<eml21__VerticalEpsgCrs*>(wellDatum->Crs)->EpsgCode = verticalCrsEpsgCode;

	wellDatum->Elevation = soap_new_witsml2__WellElevationCoord(gsoapProxy2_1->soap, 1);
	wellDatum->Elevation->datum = datum;
	wellDatum->Elevation->uom = elevationUnit;
	wellDatum->Elevation->__item = elevation;

	well->WellDatum.push_back(wellDatum);
}

unsigned int Well::getDatumCount() const
{
	return static_cast<witsml2__Well*>(gsoapProxy2_1)->WellDatum.size();
}

vector<Relationship> Well::getAllEpcRelationships() const
{
	vector<Relationship> result;

	// XML backward relationship
	if (resqmlWellboreFeature != nullptr)
	{
		Relationship rel(resqmlWellboreFeature->getPartNameInEpcDocument(), "", resqmlWellboreFeature->getUuid());
		rel.setSourceObjectType();
		result.push_back(rel);
	}

	for (size_t i = 0; i < wellboreSet.size(); ++i)
	{
		Relationship relWellbore(wellboreSet[i]->getPartNameInEpcDocument(), "", wellboreSet[i]->getUuid());
		relWellbore.setSourceObjectType();
		result.push_back(relWellbore);
	}

	for (size_t i = 0; i < wellCompletionSet.size(); ++i)
	{
		Relationship relWellCompletion(wellCompletionSet[i]->getPartNameInEpcDocument(), "", wellCompletionSet[i]->getUuid());
		relWellCompletion.setSourceObjectType();
		result.push_back(relWellCompletion);
	}

	return result;
}

void Well::importRelationshipSetFromEpc(COMMON_NS::EpcDocument*)
{}
