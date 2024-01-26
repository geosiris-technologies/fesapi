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
#include "PointSetRepresentation.h"

#include <numeric>

#include "../resqml2/AbstractFeatureInterpretation.h"
#include "../eml2/AbstractLocal3dCrs.h"
#include "../eml2/AbstractHdfProxy.h"

using namespace std;
using namespace RESQML2_2_NS;
using namespace gsoap_eml2_3;

const char* PointSetRepresentation::XML_NS = "resqml22";

PointSetRepresentation::PointSetRepresentation(COMMON_NS::DataObjectRepository* repo,
	const std::string & guid, const std::string & title)
{
	if (repo == nullptr) {
		throw invalid_argument("The repo cannot be null.");
	}

	gsoapProxy2_3 = soap_new_resqml22__PointSetRepresentation(repo->getGsoapContext());

	initMandatoryMetadata();
	setMetadata(guid, title, "", -1, "", "", -1, "");

	repo->addDataObject(this);
}

PointSetRepresentation::PointSetRepresentation(RESQML2_NS::AbstractFeatureInterpretation* interp,
	const std::string & guid, const std::string & title)
{
	if (interp == nullptr) {
		throw invalid_argument("You must provide an interpretation");
	}

	gsoapProxy2_3 = soap_new_resqml22__PointSetRepresentation(interp->getGsoapContext());

	initMandatoryMetadata();
	setMetadata(guid, title, "", -1, "", "", -1, "");

	interp->getRepository()->addDataObject(this);
	setInterpretation(interp);
}

void PointSetRepresentation::pushBackGeometryPatch(
	unsigned int xyzPointCount, double const * xyzPoints,
	EML2_NS::AbstractHdfProxy * proxy, EML2_NS::AbstractLocal3dCrs * localCrs)
{
	if (localCrs == nullptr) {
		localCrs = getRepository()->getDefaultCrs();
		if (localCrs == nullptr) {
			throw std::invalid_argument("A (default) CRS must be provided.");
		}
	}

	// XYZ points
	uint64_t pointCountDims = xyzPointCount;
	static_cast<_resqml22__PointSetRepresentation*>(gsoapProxy2_3)->NodePatchGeometry.push_back(
		createPointGeometryPatch2_2(static_cast<_resqml22__PointSetRepresentation*>(gsoapProxy2_3)->NodePatchGeometry.size(), xyzPoints, localCrs, &pointCountDims, 1, proxy)
	);
	getRepository()->addRelationship(this, localCrs);
}

COMMON_NS::DataObjectReference PointSetRepresentation::getHdfProxyDor() const
{
	return getHdfProxyDorFromPointGeometryPatch(getPointGeometry2_2(0));
}

resqml22__PointGeometry* PointSetRepresentation::getPointGeometry2_2(uint64_t patchIndex) const
{
	if (patchIndex >= getPatchCount()) {
		throw range_error("The index of the patch is not in the allowed range of patch.");
	}
	
	return static_cast<_resqml22__PointSetRepresentation*>(gsoapProxy2_3)->NodePatchGeometry[patchIndex];
}

uint64_t PointSetRepresentation::getXyzPointCountOfPatch(unsigned int patchIndex) const
{
	if (patchIndex >= getPatchCount()) {
		throw range_error("The index of the patch is not in the allowed range of patch.");
	}
	auto const* ptArray = static_cast<_resqml22__PointSetRepresentation*>(gsoapProxy2_3)->NodePatchGeometry[patchIndex]->Points;
	resqml22__Point3dExternalArray const* externalPtArray = dynamic_cast<resqml22__Point3dExternalArray const*>(ptArray);
	if (externalPtArray == nullptr) {
		throw range_error("Does only support point set where points are in a resqml22__Point3dExternalArray");
	}

	uint64_t result = 0;
	for (auto* dataArrayPart : externalPtArray->Coordinates->ExternalDataArrayPart) {
		result += std::accumulate(std::begin(dataArrayPart->Count), std::end(dataArrayPart->Count), static_cast<LONG64>(1), std::multiplies<LONG64>());
	}
	return result / 3;
}

void PointSetRepresentation::getXyzPointsOfPatch(unsigned int patchIndex, double * xyzPoints) const
{
	if (patchIndex >= getPatchCount())
		throw range_error("The index of the patch is not in the allowed range of patch.");

	resqml22__PointGeometry* pointGeom = getPointGeometry2_2(patchIndex);
	if (pointGeom != nullptr && pointGeom->Points->soap_type() == SOAP_TYPE_gsoap_eml2_3_resqml22__Point3dExternalArray)
	{
		auto const* daPart = static_cast<resqml22__Point3dExternalArray*>(pointGeom->Points)->Coordinates->ExternalDataArrayPart[0];
		getOrCreateHdfProxyFromDataArrayPart(daPart)->readArrayNdOfDoubleValues(daPart->PathInExternalFile, xyzPoints);
	}
	else
		throw invalid_argument("The geometry of the representation either does not exist or it is not an explicit one.");
}

uint64_t PointSetRepresentation::getPatchCount() const
{
	return static_cast<_resqml22__PointSetRepresentation*>(gsoapProxy2_3)->NodePatchGeometry.size();
}
