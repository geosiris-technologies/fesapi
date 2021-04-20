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
#include "FluidCharacterization.h"

#include <limits>
#include <stdexcept>

#include "FluidSystem.h"
#include "FrictionTheorySpecification.h"
#include "CorrelationViscosityBubblePointSpecification.h"

#include "../resqml2/RockFluidUnitInterpretation.h"

using namespace std;
using namespace PRODML2_1_NS;
using namespace gsoap_eml2_2;
using namespace COMMON_NS;

const char* FluidCharacterization::XML_TAG = "FluidCharacterization";
const char* FluidCharacterization::XML_NS = "prodml21";

FluidCharacterization::FluidCharacterization(COMMON_NS::DataObjectRepository * repo,
	const std::string & guid,
	const std::string & title)
{
	if (repo == nullptr) {
		throw invalid_argument("A repo must exist.");
	}

	gsoapProxy2_2 = soap_new_prodml21__FluidCharacterization(repo->getGsoapContext());

	initMandatoryMetadata();
	setMetadata(guid, title, "", -1, "", "", -1, "");

	repo->addDataObject(this);
}

SETTER_OPTIONAL_ATTRIBUTE_IMPL(FluidCharacterization, prodml21__FluidCharacterization, gsoapProxy2_2, FluidCharacterizationType, std::string, soap_new_std__string)
SETTER_OPTIONAL_ATTRIBUTE_IMPL(FluidCharacterization, prodml21__FluidCharacterization, gsoapProxy2_2, IntendedUsage, std::string, soap_new_std__string)
SETTER_OPTIONAL_ATTRIBUTE_IMPL(FluidCharacterization, prodml21__FluidCharacterization, gsoapProxy2_2, Remark, std::string, soap_new_std__string)

void FluidCharacterization::setStandardConditions(double temperatureValue, gsoap_eml2_2::eml22__ThermodynamicTemperatureUom temperatureUom,
	double pressureValue, gsoap_eml2_2::eml22__PressureUom pressureUom)
{
	prodml21__FluidCharacterization* fc = static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2);

	fc->StandardConditions = soap_new_eml22__TemperaturePressure(getGsoapContext());
	static_cast<eml22__TemperaturePressure*>(fc->StandardConditions)->Temperature = soap_new_eml22__ThermodynamicTemperatureMeasure(getGsoapContext());
	static_cast<eml22__TemperaturePressure*>(fc->StandardConditions)->Temperature->__item = temperatureValue;
	static_cast<eml22__TemperaturePressure*>(fc->StandardConditions)->Temperature->uom = temperatureUom;
	static_cast<eml22__TemperaturePressure*>(fc->StandardConditions)->Pressure = soap_new_eml22__PressureMeasure(getGsoapContext());
	static_cast<eml22__TemperaturePressure*>(fc->StandardConditions)->Pressure->__item = pressureValue;
	static_cast<eml22__TemperaturePressure*>(fc->StandardConditions)->Pressure->uom = soap_eml22__PressureUom2s(getGsoapContext(), pressureUom);
}

bool FluidCharacterization::hasStandardConditions() const
{
	return static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->StandardConditions != nullptr;
}

double FluidCharacterization::getStandardTemperatureValue() const
{
	if (!hasStandardConditions()) {
		throw std::logic_error("This fluid characterization has not got standard conditions");
	}

	prodml21__FluidCharacterization* fc = static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2);
	if (fc->StandardConditions->soap_type() == SOAP_TYPE_gsoap_eml2_2_eml22__TemperaturePressure) {
		return static_cast<eml22__TemperaturePressure*>(fc->StandardConditions)->Temperature->__item;
	}
	else if (fc->StandardConditions->soap_type() == SOAP_TYPE_gsoap_eml2_2_eml22__ReferenceTemperaturePressure) {
		istringstream iss(*static_cast<eml22__ReferenceTemperaturePressure*>(fc->StandardConditions)->union_ReferenceTemperaturePressure_.ReferenceTempPres);
		double result;
		iss >> result;
		return result;
	}

	throw logic_error("Unrecognized datatype for StandardConditions.");
}

gsoap_eml2_2::eml22__ThermodynamicTemperatureUom FluidCharacterization::getStandardTemperatureUom() const
{
	if (!hasStandardConditions()) {
		throw std::logic_error("This fluid characterization has not got standard conditions");
	}

	prodml21__FluidCharacterization* fc = static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2);
	if (fc->StandardConditions->soap_type() == SOAP_TYPE_gsoap_eml2_2_eml22__TemperaturePressure) {
		return static_cast<eml22__TemperaturePressure*>(fc->StandardConditions)->Temperature->uom;
	}
	else if (fc->StandardConditions->soap_type() == SOAP_TYPE_gsoap_eml2_2_eml22__ReferenceTemperaturePressure) {
		const std::string str = *static_cast<eml22__ReferenceTemperaturePressure*>(fc->StandardConditions)->union_ReferenceTemperaturePressure_.ReferenceTempPres;
		if (str.find("degC") != std::string::npos) return gsoap_eml2_2::eml22__ThermodynamicTemperatureUom::degC;
		if (str.find("degF") != std::string::npos) return gsoap_eml2_2::eml22__ThermodynamicTemperatureUom::degF;
		throw logic_error("Unrecognized temperature uom in " + str);
	}

	throw logic_error("Unrecognized datatype for StandardConditions.");
}

double FluidCharacterization::getStandardPressureValue() const
{
	if (!hasStandardConditions()) {
		throw std::logic_error("This fluid characterization has not got standard conditions");
	}

	prodml21__FluidCharacterization* fc = static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2);
	if (fc->StandardConditions->soap_type() == SOAP_TYPE_gsoap_eml2_2_eml22__TemperaturePressure) {
		return static_cast<eml22__TemperaturePressure*>(fc->StandardConditions)->Pressure->__item;
	}
	else if (fc->StandardConditions->soap_type() == SOAP_TYPE_gsoap_eml2_2_eml22__ReferenceTemperaturePressure) {
		const std::string str = *static_cast<eml22__ReferenceTemperaturePressure*>(fc->StandardConditions)->union_ReferenceTemperaturePressure_.ReferenceTempPres;
		istringstream iss(str);
		int tempValue;
		iss >> tempValue;
		std::string  tempUom;
		iss >> tempUom;
		int pressureValue;
		iss >> pressureValue;
		return pressureValue;
	}

	throw logic_error("Unrecognized datatype for StandardConditions.");
}

gsoap_eml2_2::eml22__PressureUom FluidCharacterization::getStandardPressureUom() const
{
	if (!hasStandardConditions()) {
		throw std::logic_error("This fluid characterization has not got standard conditions");
	}

	prodml21__FluidCharacterization* fc = static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2);
	if (fc->StandardConditions->soap_type() == SOAP_TYPE_gsoap_eml2_2_eml22__TemperaturePressure) {
		gsoap_eml2_2::eml22__PressureUom result;
		int conversion = soap_s2eml22__PressureUom(getGsoapContext(), static_cast<eml22__TemperaturePressure*>(fc->StandardConditions)->Pressure->uom.c_str(), &result);
		if (conversion != SOAP_OK) {
			throw invalid_argument("The pressure uom " + static_cast<eml22__TemperaturePressure*>(fc->StandardConditions)->Pressure->uom  + " is not recognized yet");
		}
		return result;
	}
	else if (fc->StandardConditions->soap_type() == SOAP_TYPE_gsoap_eml2_2_eml22__ReferenceTemperaturePressure) {
		const std::string str = *static_cast<eml22__ReferenceTemperaturePressure*>(fc->StandardConditions)->union_ReferenceTemperaturePressure_.ReferenceTempPres;
		if (str.find("bar") != std::string::npos) return gsoap_eml2_2::eml22__PressureUom::bar;
		if (str.find("atm") != std::string::npos) return gsoap_eml2_2::eml22__PressureUom::atm;
		if (str.find("degF") != std::string::npos && str.find("in Hg") != std::string::npos) return gsoap_eml2_2::eml22__PressureUom::inHg_x005b60degF_x005d;
		throw logic_error("Unrecognized pressure uom in " + str);
	}

	throw logic_error("Unrecognized datatype for StandardConditions.");
}

void FluidCharacterization::setRockFluidUnit(RESQML2_NS::RockFluidUnitInterpretation* rockFluidUnit)
{
	if (rockFluidUnit == nullptr) {
		throw std::invalid_argument("Cannot set a null rockFluidUnit");
	}
	static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->RockFluidUnitFeature = rockFluidUnit->newEml22Reference();

	getRepository()->addRelationship(this, rockFluidUnit);
}

COMMON_NS::DataObjectReference FluidCharacterization::getRockFluidUnitDor() const
{
	return static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->RockFluidUnitFeature;
}

RESQML2_NS::RockFluidUnitInterpretation* FluidCharacterization::getRockFluidUnit() const
{
	return getRepository()->getDataObjectByUuid<RESQML2_NS::RockFluidUnitInterpretation>(getRockFluidUnitDor().getUuid());
}

void FluidCharacterization::setFluidSystem(FluidSystem* fluidSystem)
{
	if (fluidSystem == nullptr) {
		throw std::invalid_argument("Cannot set a null fluidSystem");
	}
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidSystem != nullptr) {
		throw std::invalid_argument("There is already a fluid system set for this fluid characterization.");
	}
	static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidSystem = fluidSystem->newEml22Reference();

	getRepository()->addRelationship(this, fluidSystem);
}

COMMON_NS::DataObjectReference FluidCharacterization::getFluidSystemDor() const
{
	return static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidSystem;
}

FluidSystem* FluidCharacterization::getFluidSystem() const
{
	return getRepository()->getDataObjectByUuid<FluidSystem>(getFluidSystemDor().getUuid());
}

#define SETTER_FLUID_CATALOG_COMPONENT_OPTIONAL_ATTRIBUTE_IMPL(vectorName, attributeName, attributeDatatype, constructor)\
void FluidCharacterization::set##vectorName##attributeName(unsigned int index, const attributeDatatype& value)\
{\
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog == nullptr || index >= static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog->vectorName.size()) {throw std::out_of_range("This index is out of range.");}\
	static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog->vectorName[index]->attributeName = constructor(getGsoapContext());\
	*static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog->vectorName[index]->attributeName = value;\
}

#define SETTER_FLUID_CATALOG_COMPONENT_MEASURE_ATTRIBUTE_IMPL(vectorName, attributeName, uomDatatype, constructor)\
void FluidCharacterization::set##vectorName##attributeName(unsigned int index, double value, uomDatatype uom)\
{\
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog == nullptr || index >= static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog->vectorName.size()) {throw std::out_of_range("This index is out of range.");}\
	static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog->vectorName[index]->attributeName = constructor(getGsoapContext());\
	static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog->vectorName[index]->attributeName->__item = value;\
	static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog->vectorName[index]->attributeName->uom = uom;\
}

#define SETTER_FLUID_CATALOG_COMPONENT_COMMON_ATTRIBUTES_IMPL(vectorName)\
SETTER_FLUID_CATALOG_COMPONENT_OPTIONAL_ATTRIBUTE_IMPL(vectorName, Remark, std::string, soap_new_std__string)\
SETTER_FLUID_CATALOG_COMPONENT_MEASURE_ATTRIBUTE_IMPL(vectorName, MassFraction, eml22__MassPerMassUom, soap_new_eml22__MassPerMassMeasure)\
SETTER_FLUID_CATALOG_COMPONENT_MEASURE_ATTRIBUTE_IMPL(vectorName, MoleFraction, eml22__AmountOfSubstancePerAmountOfSubstanceUom, soap_new_eml22__AmountOfSubstancePerAmountOfSubstanceMeasure)

unsigned int FluidCharacterization::getFormationWaterCount() const
{
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog == nullptr) {
		return 0;
	}

	size_t count = static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog->FormationWater.size();
	if (count > (std::numeric_limits<unsigned int>::max)()) {
		throw out_of_range("There are too much FormationWater");
	}
	return static_cast<unsigned int>(count);
}
void FluidCharacterization::pushBackFormationWater(const std::string & uid)
{
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog == nullptr) {
		static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog = soap_new_prodml21__FluidComponentCatalog(getGsoapContext());
	}

	prodml21__FormationWater* const result = soap_new_prodml21__FormationWater(getGsoapContext());
	result->uid = uid;
	static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog->FormationWater.push_back(result);
}
SETTER_FLUID_CATALOG_COMPONENT_COMMON_ATTRIBUTES_IMPL(FormationWater)
SETTER_FLUID_CATALOG_COMPONENT_MEASURE_ATTRIBUTE_IMPL(FormationWater, Salinity, gsoap_eml2_2::eml22__MassPerMassUom, gsoap_eml2_2::soap_new_eml22__MassPerMassMeasure)
SETTER_FLUID_CATALOG_COMPONENT_OPTIONAL_ATTRIBUTE_IMPL(FormationWater, SpecificGravity, double, soap_new_double)

unsigned int FluidCharacterization::getPureFluidComponentCount() const
{
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog == nullptr) {
		return 0;
	}

	size_t count = static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog->PureFluidComponent.size();
	if (count > (std::numeric_limits<unsigned int>::max)()) {
		throw out_of_range("There are too much PureFluidComponent");
	}
	return static_cast<unsigned int>(count);
}
void FluidCharacterization::pushBackPureFluidComponent(const std::string & uid, gsoap_eml2_2::prodml21__PureComponentEnum kind, bool hydrocarbonFlag)
{
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog == nullptr) {
		static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog = soap_new_prodml21__FluidComponentCatalog(getGsoapContext());
	}

	prodml21__PureFluidComponent* const result = soap_new_prodml21__PureFluidComponent(getGsoapContext());
	result->uid = uid;
	result->Kind = gsoap_eml2_2::soap_prodml21__PureComponentEnum2s(getGsoapContext(), kind);
	result->HydrocarbonFlag = hydrocarbonFlag;
	static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog->PureFluidComponent.push_back(result);
}
SETTER_FLUID_CATALOG_COMPONENT_COMMON_ATTRIBUTES_IMPL(PureFluidComponent)
SETTER_FLUID_CATALOG_COMPONENT_MEASURE_ATTRIBUTE_IMPL(PureFluidComponent, MolecularWeight, gsoap_eml2_2::eml22__MolecularWeightUom, gsoap_eml2_2::soap_new_eml22__MolecularWeightMeasure)

unsigned int FluidCharacterization::getPlusFluidComponentCount() const
{
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog == nullptr) {
		return 0;
	}

	size_t count = static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog->PlusFluidComponent.size();
	if (count > (std::numeric_limits<unsigned int>::max)()) {
		throw out_of_range("There are too much PlusFluidComponent");
	}
	return static_cast<unsigned int>(count);
}
void FluidCharacterization::pushBackPlusFluidComponent(const std::string & uid, gsoap_eml2_2::prodml21__PlusComponentEnum kind)
{
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog == nullptr) {
		static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog = soap_new_prodml21__FluidComponentCatalog(getGsoapContext());
	}

	prodml21__PlusFluidComponent* const result = soap_new_prodml21__PlusFluidComponent(getGsoapContext());
	result->uid = uid;
	result->Kind = gsoap_eml2_2::soap_prodml21__PlusComponentEnum2s(getGsoapContext(), kind);
	static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog->PlusFluidComponent.push_back(result);
}
SETTER_FLUID_CATALOG_COMPONENT_COMMON_ATTRIBUTES_IMPL(PlusFluidComponent)
SETTER_FLUID_CATALOG_COMPONENT_OPTIONAL_ATTRIBUTE_IMPL(PlusFluidComponent, SpecificGravity, double, soap_new_double)
SETTER_FLUID_CATALOG_COMPONENT_OPTIONAL_ATTRIBUTE_IMPL(PlusFluidComponent, StartingCarbonNumber, uint64_t, soap_new_ULONG64)
SETTER_FLUID_CATALOG_COMPONENT_MEASURE_ATTRIBUTE_IMPL(PlusFluidComponent, StartingBoilingPoint, gsoap_eml2_2::eml22__ThermodynamicTemperatureUom, gsoap_eml2_2::soap_new_eml22__ThermodynamicTemperatureMeasure)
SETTER_FLUID_CATALOG_COMPONENT_MEASURE_ATTRIBUTE_IMPL(PlusFluidComponent, AvgDensity, std::string, gsoap_eml2_2::soap_new_eml22__MassPerVolumeMeasure)
SETTER_FLUID_CATALOG_COMPONENT_MEASURE_ATTRIBUTE_IMPL(PlusFluidComponent, AvgMolecularWeight, gsoap_eml2_2::eml22__MolecularWeightUom, gsoap_eml2_2::soap_new_eml22__MolecularWeightMeasure)

unsigned int FluidCharacterization::getStockTankOilCount() const
{
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog == nullptr) {
		return 0;
	}

	size_t count = static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog->StockTankOil.size();
	if (count > (std::numeric_limits<unsigned int>::max)()) {
		throw out_of_range("There are too much StockTankOil");
	}
	return static_cast<unsigned int>(count);
}
void FluidCharacterization::pushBackStockTankOil(const std::string & uid)
{
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog == nullptr) {
		static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog = soap_new_prodml21__FluidComponentCatalog(getGsoapContext());
	}

	prodml21__StockTankOil* const result = soap_new_prodml21__StockTankOil(getGsoapContext());
	result->uid = uid;
	static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog->StockTankOil.push_back(result);
}
SETTER_FLUID_CATALOG_COMPONENT_COMMON_ATTRIBUTES_IMPL(StockTankOil)
SETTER_FLUID_CATALOG_COMPONENT_MEASURE_ATTRIBUTE_IMPL(StockTankOil, APIGravity, gsoap_eml2_2::eml22__APIGravityUom, gsoap_eml2_2::soap_new_eml22__APIGravityMeasure)
SETTER_FLUID_CATALOG_COMPONENT_MEASURE_ATTRIBUTE_IMPL(StockTankOil, MolecularWeight, gsoap_eml2_2::eml22__MolecularWeightUom, gsoap_eml2_2::soap_new_eml22__MolecularWeightMeasure)
SETTER_FLUID_CATALOG_COMPONENT_MEASURE_ATTRIBUTE_IMPL(StockTankOil, GrossEnergyContentPerUnitMass, gsoap_eml2_2::eml22__EnergyPerMassUom, gsoap_eml2_2::soap_new_eml22__EnergyPerMassMeasure)
SETTER_FLUID_CATALOG_COMPONENT_MEASURE_ATTRIBUTE_IMPL(StockTankOil, NetEnergyContentPerUnitMass, gsoap_eml2_2::eml22__EnergyPerMassUom, gsoap_eml2_2::soap_new_eml22__EnergyPerMassMeasure)
SETTER_FLUID_CATALOG_COMPONENT_MEASURE_ATTRIBUTE_IMPL(StockTankOil, GrossEnergyContentPerUnitVolume, gsoap_eml2_2::eml22__EnergyPerVolumeUom, gsoap_eml2_2::soap_new_eml22__EnergyPerVolumeMeasure)
SETTER_FLUID_CATALOG_COMPONENT_MEASURE_ATTRIBUTE_IMPL(StockTankOil, NetEnergyContentPerUnitVolume, gsoap_eml2_2::eml22__EnergyPerVolumeUom, gsoap_eml2_2::soap_new_eml22__EnergyPerVolumeMeasure)

unsigned int FluidCharacterization::getNaturalGasCount() const
{
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog == nullptr) {
		return 0;
	}

	size_t count = static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog->NaturalGas.size();
	if (count > (std::numeric_limits<unsigned int>::max)()) {
		throw out_of_range("There are too much NaturalGas");
	}
	return static_cast<unsigned int>(count);
}
void FluidCharacterization::pushBackNaturalGas(const std::string & uid)
{
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog == nullptr) {
		static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog = soap_new_prodml21__FluidComponentCatalog(getGsoapContext());
	}

	prodml21__NaturalGas* const result = soap_new_prodml21__NaturalGas(getGsoapContext());
	result->uid = uid;
	static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog->NaturalGas.push_back(result);
}
SETTER_FLUID_CATALOG_COMPONENT_COMMON_ATTRIBUTES_IMPL(NaturalGas)
SETTER_FLUID_CATALOG_COMPONENT_OPTIONAL_ATTRIBUTE_IMPL(NaturalGas, GasGravity, double, soap_new_double)
SETTER_FLUID_CATALOG_COMPONENT_MEASURE_ATTRIBUTE_IMPL(NaturalGas, MolecularWeight, gsoap_eml2_2::eml22__MolecularWeightUom, gsoap_eml2_2::soap_new_eml22__MolecularWeightMeasure)
SETTER_FLUID_CATALOG_COMPONENT_MEASURE_ATTRIBUTE_IMPL(NaturalGas, GrossEnergyContentPerUnitMass, gsoap_eml2_2::eml22__EnergyPerMassUom, gsoap_eml2_2::soap_new_eml22__EnergyPerMassMeasure)
SETTER_FLUID_CATALOG_COMPONENT_MEASURE_ATTRIBUTE_IMPL(NaturalGas, NetEnergyContentPerUnitMass, gsoap_eml2_2::eml22__EnergyPerMassUom, gsoap_eml2_2::soap_new_eml22__EnergyPerMassMeasure)
SETTER_FLUID_CATALOG_COMPONENT_MEASURE_ATTRIBUTE_IMPL(NaturalGas, GrossEnergyContentPerUnitVolume, gsoap_eml2_2::eml22__EnergyPerVolumeUom, gsoap_eml2_2::soap_new_eml22__EnergyPerVolumeMeasure)
SETTER_FLUID_CATALOG_COMPONENT_MEASURE_ATTRIBUTE_IMPL(NaturalGas, NetEnergyContentPerUnitVolume, gsoap_eml2_2::eml22__EnergyPerVolumeUom, gsoap_eml2_2::soap_new_eml22__EnergyPerVolumeMeasure)

unsigned int FluidCharacterization::getPseudoFluidComponentCount() const
{
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog == nullptr) {
		return 0;
	}

	size_t count = static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog->PseudoFluidComponent.size();
	if (count > (std::numeric_limits<unsigned int>::max)()) {
		throw out_of_range("There are too much PseudoFluidComponent");
	}
	return static_cast<unsigned int>(count);
}
void FluidCharacterization::pushBackPseudoFluidComponent(const std::string & uid, gsoap_eml2_2::prodml21__PseudoComponentEnum kind)
{
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog == nullptr) {
		static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog = soap_new_prodml21__FluidComponentCatalog(getGsoapContext());
	}

	prodml21__PseudoFluidComponent* const result = soap_new_prodml21__PseudoFluidComponent(getGsoapContext());
	result->uid = uid;
	result->Kind = gsoap_eml2_2::soap_prodml21__PseudoComponentEnum2s(getGsoapContext(), kind);
	static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidComponentCatalog->PseudoFluidComponent.push_back(result);
}
SETTER_FLUID_CATALOG_COMPONENT_COMMON_ATTRIBUTES_IMPL(PseudoFluidComponent)
SETTER_FLUID_CATALOG_COMPONENT_OPTIONAL_ATTRIBUTE_IMPL(PseudoFluidComponent, SpecificGravity, double, soap_new_double)
SETTER_FLUID_CATALOG_COMPONENT_OPTIONAL_ATTRIBUTE_IMPL(PseudoFluidComponent, StartingCarbonNumber, uint64_t, soap_new_ULONG64)
SETTER_FLUID_CATALOG_COMPONENT_OPTIONAL_ATTRIBUTE_IMPL(PseudoFluidComponent, EndingCarbonNumber, uint64_t, soap_new_ULONG64)
SETTER_FLUID_CATALOG_COMPONENT_MEASURE_ATTRIBUTE_IMPL(PseudoFluidComponent, StartingBoilingPoint, gsoap_eml2_2::eml22__ThermodynamicTemperatureUom, gsoap_eml2_2::soap_new_eml22__ThermodynamicTemperatureMeasure)
SETTER_FLUID_CATALOG_COMPONENT_MEASURE_ATTRIBUTE_IMPL(PseudoFluidComponent, EndingBoilingPoint, gsoap_eml2_2::eml22__ThermodynamicTemperatureUom, gsoap_eml2_2::soap_new_eml22__ThermodynamicTemperatureMeasure)
SETTER_FLUID_CATALOG_COMPONENT_MEASURE_ATTRIBUTE_IMPL(PseudoFluidComponent, AvgBoilingPoint, gsoap_eml2_2::eml22__ThermodynamicTemperatureUom, gsoap_eml2_2::soap_new_eml22__ThermodynamicTemperatureMeasure)
SETTER_FLUID_CATALOG_COMPONENT_MEASURE_ATTRIBUTE_IMPL(PseudoFluidComponent, AvgDensity, std::string, gsoap_eml2_2::soap_new_eml22__MassPerVolumeMeasure)
SETTER_FLUID_CATALOG_COMPONENT_MEASURE_ATTRIBUTE_IMPL(PseudoFluidComponent, AvgMolecularWeight, gsoap_eml2_2::eml22__MolecularWeightUom, gsoap_eml2_2::soap_new_eml22__MolecularWeightMeasure)

unsigned int FluidCharacterization::getModelCount() const
{
	size_t count = static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel.size();
	if (count > (std::numeric_limits<unsigned int>::max)()) {
		throw out_of_range("There are too much models.");
	}
	return static_cast<unsigned int>(count);
}

void FluidCharacterization::pushBackModel(const std::string & uid)
{
	prodml21__FluidCharacterizationModel* const result = soap_new_prodml21__FluidCharacterizationModel(getGsoapContext());
	result->uid = uid.empty() 
		? std::to_string(static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel.size())
		: uid;
	static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel.push_back(result);

	modelSpecifications.push_back(nullptr);
}

PvtSpecification* FluidCharacterization::initModelSpecification(unsigned int modelIndex, ModelSpecification kind)
{
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel.size() <= modelIndex) {
		throw std::out_of_range("The index of the model is out of range");
	}

	switch (kind) {
		case CompositionalThermal:
			modelSpecifications[modelIndex] = new CompositionalSpecification(soap_new_prodml21__CompositionalThermalModel(getGsoapContext()));
			break;
		case SrkEos:
			modelSpecifications[modelIndex] = new CompositionalSpecification(soap_new_prodml21__Srk_USCOREEOS(getGsoapContext()));
			break;
		case PengRobinson76Eos:
			modelSpecifications[modelIndex] = new CompositionalSpecification(soap_new_prodml21__PengRobinson76_USCOREEOS(getGsoapContext()));
			break;
		case PengRobinson78Eos:
			modelSpecifications[modelIndex] = new CompositionalSpecification(soap_new_prodml21__PengRobinson78_USCOREEOS(getGsoapContext()));
			break;
		case LohrenzBrayClarkCorrelation:
			modelSpecifications[modelIndex] = new CompositionalViscositySpecification(soap_new_prodml21__Lohrenz_Bray_ClarkCorrelation(getGsoapContext()));
			break;
		case CSPedersen84:
			modelSpecifications[modelIndex] = new CompositionalViscositySpecification(soap_new_prodml21__CSPedersen84(getGsoapContext()));
			break;
		case CSPedersen87: 
			modelSpecifications[modelIndex] = new CompositionalViscositySpecification(soap_new_prodml21__CSPedersen87(getGsoapContext()));
			break;
		case FrictionTheory:
			modelSpecifications[modelIndex] = new FrictionTheorySpecification(soap_new_prodml21__FrictionTheory(getGsoapContext()));
			break;
		case CorrelationThermal:
			modelSpecifications[modelIndex] = new PvtSpecification(soap_new_prodml21__CorrelationThermalModel(getGsoapContext()));
			break;
		case BergmanSuttonBubblePoint:
			modelSpecifications[modelIndex] = new CorrelationViscosityBubblePointSpecification(soap_new_prodml21__BergmanSutton_BubblePoint(getGsoapContext()));
			break;
		case DeGhettoBubblePoint:
			modelSpecifications[modelIndex] = new CorrelationViscosityBubblePointSpecification(soap_new_prodml21__DeGhetto_BubblePoint(getGsoapContext()));
			break;
		case StandingBubblePoint:
			modelSpecifications[modelIndex] = new CorrelationViscosityBubblePointSpecification(soap_new_prodml21__Standing_BubblePoint(getGsoapContext()));
			break;
		case DindorukChristmanBubblePoint:
			modelSpecifications[modelIndex] = new CorrelationViscosityBubblePointSpecification(soap_new_prodml21__DindorukChristman_BubblePoint(getGsoapContext()));
			break;
		case PetroskyFarshadBubblePoint:
			modelSpecifications[modelIndex] = new CorrelationViscosityBubblePointSpecification(soap_new_prodml21__PetroskyFarshad_BubblePoint(getGsoapContext()));
			break;
		default: throw std::invalid_argument("This kind of specification is not supported yet");
	}
	static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel[modelIndex]->ModelSpecification = modelSpecifications[modelIndex]->gsoapProxy;

	return modelSpecifications[modelIndex];
}

SETTER_OPTIONAL_ATTRIBUTE_IN_VECTOR_IMPL(FluidCharacterization, gsoap_eml2_2::prodml21__FluidCharacterization, gsoapProxy2_2, FluidCharacterizationModel, Name, std::string, gsoap_eml2_2::soap_new_std__string)
SETTER_OPTIONAL_ATTRIBUTE_IN_VECTOR_IMPL(FluidCharacterization, gsoap_eml2_2::prodml21__FluidCharacterization, gsoapProxy2_2, FluidCharacterizationModel, Remark, std::string, gsoap_eml2_2::soap_new_std__string)
SETTER_MEASURE_ATTRIBUTE_IN_VECTOR_IMPL(FluidCharacterization, gsoap_eml2_2::prodml21__FluidCharacterization, gsoapProxy2_2, FluidCharacterizationModel, ReferenceTemperature, gsoap_eml2_2::eml22__ThermodynamicTemperatureUom, gsoap_eml2_2::soap_new_eml22__ThermodynamicTemperatureMeasure)
SETTER_MEASURE_ATTRIBUTE_IN_VECTOR_IMPL(FluidCharacterization, gsoap_eml2_2::prodml21__FluidCharacterization, gsoapProxy2_2, FluidCharacterizationModel, ReferenceStockTankTemperature, gsoap_eml2_2::eml22__ThermodynamicTemperatureUom, gsoap_eml2_2::soap_new_eml22__ThermodynamicTemperatureMeasure)

void FluidCharacterization::loadTargetRelationships()
{
	COMMON_NS::DataObjectReference dor = getRockFluidUnitDor();
	if (!dor.isEmpty()) {
		RESQML2_NS::RockFluidUnitInterpretation* rockFluidUnit = getRepository()->getDataObjectByUuid<RESQML2_NS::RockFluidUnitInterpretation>(dor.getUuid());
		if (rockFluidUnit == nullptr) {
			convertDorIntoRel<RESQML2_NS::RockFluidUnitInterpretation>(dor);
			rockFluidUnit = getRepository()->getDataObjectByUuid<RESQML2_NS::RockFluidUnitInterpretation>(dor.getUuid());
		}
		getRepository()->addRelationship(this, rockFluidUnit);
	}

	dor = getFluidSystemDor();
	if (!dor.isEmpty()) {
		FluidSystem* fluidSystem = getRepository()->getDataObjectByUuid<FluidSystem>(dor.getUuid());
		if (fluidSystem == nullptr) {
			convertDorIntoRel<FluidSystem>(dor);
			fluidSystem = getRepository()->getDataObjectByUuid<FluidSystem>(dor.getUuid());
		}
		getRepository()->addRelationship(this, fluidSystem);
	}

	std::vector<prodml21__FluidCharacterizationModel *> models = static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel;
	for (size_t modelIndex = 0; modelIndex < models.size(); ++modelIndex) {
		if (models[modelIndex]->ModelSpecification != nullptr) {
			switch (models[modelIndex]->ModelSpecification->soap_type()) {
				case SOAP_TYPE_gsoap_eml2_2_prodml21__CompositionalThermalModel:
					modelSpecifications.push_back(new CompositionalSpecification(static_cast<prodml21__CompositionalThermalModel*>(models[modelIndex]->ModelSpecification)));
					break;
				case SOAP_TYPE_gsoap_eml2_2_prodml21__Srk_USCOREEOS:
					modelSpecifications.push_back(new CompositionalSpecification(static_cast<prodml21__Srk_USCOREEOS*>(models[modelIndex]->ModelSpecification)));
					break;
				case SOAP_TYPE_gsoap_eml2_2_prodml21__PengRobinson76_USCOREEOS:
					modelSpecifications.push_back(new CompositionalSpecification(static_cast<prodml21__PengRobinson76_USCOREEOS*>(models[modelIndex]->ModelSpecification)));
					break;
				case SOAP_TYPE_gsoap_eml2_2_prodml21__PengRobinson78_USCOREEOS:
					modelSpecifications.push_back(new CompositionalSpecification(static_cast<prodml21__PengRobinson78_USCOREEOS*>(models[modelIndex]->ModelSpecification)));
					break;
				case SOAP_TYPE_gsoap_eml2_2_prodml21__Lohrenz_Bray_ClarkCorrelation:
					modelSpecifications.push_back(new CompositionalViscositySpecification(static_cast<prodml21__Lohrenz_Bray_ClarkCorrelation*>(models[modelIndex]->ModelSpecification)));
					break;
				case SOAP_TYPE_gsoap_eml2_2_prodml21__CSPedersen84:
					modelSpecifications.push_back(new CompositionalViscositySpecification(static_cast<prodml21__CSPedersen84*>(models[modelIndex]->ModelSpecification)));
					break;
				case SOAP_TYPE_gsoap_eml2_2_prodml21__CSPedersen87:
					modelSpecifications.push_back(new CompositionalViscositySpecification(static_cast<prodml21__CSPedersen87*>(models[modelIndex]->ModelSpecification)));
					break;
				case SOAP_TYPE_gsoap_eml2_2_prodml21__FrictionTheory:
					modelSpecifications.push_back(new FrictionTheorySpecification(static_cast<prodml21__FrictionTheory*>(models[modelIndex]->ModelSpecification)));
					break;
				case SOAP_TYPE_gsoap_eml2_2_prodml21__CorrelationThermalModel:
					modelSpecifications.push_back(new PvtSpecification(static_cast<prodml21__CorrelationThermalModel*>(models[modelIndex]->ModelSpecification)));
					break;
				case SOAP_TYPE_gsoap_eml2_2_prodml21__BergmanSutton_BubblePoint:
					modelSpecifications.push_back(new CorrelationViscosityBubblePointSpecification(static_cast<prodml21__BergmanSutton_BubblePoint*>(models[modelIndex]->ModelSpecification)));
					break;
				case SOAP_TYPE_gsoap_eml2_2_prodml21__DeGhetto_BubblePoint:
					modelSpecifications.push_back(new CorrelationViscosityBubblePointSpecification(static_cast<prodml21__DeGhetto_BubblePoint*>(models[modelIndex]->ModelSpecification)));
					break;
				case SOAP_TYPE_gsoap_eml2_2_prodml21__Standing_BubblePoint:
					modelSpecifications.push_back(new CorrelationViscosityBubblePointSpecification(static_cast<prodml21__Standing_BubblePoint*>(models[modelIndex]->ModelSpecification)));
					break;
				case SOAP_TYPE_gsoap_eml2_2_prodml21__DindorukChristman_BubblePoint:
					modelSpecifications.push_back(new CorrelationViscosityBubblePointSpecification(static_cast<prodml21__DindorukChristman_BubblePoint*>(models[modelIndex]->ModelSpecification)));
					break;
				case SOAP_TYPE_gsoap_eml2_2_prodml21__PetroskyFarshad_BubblePoint:
					modelSpecifications.push_back(new CorrelationViscosityBubblePointSpecification(static_cast<prodml21__PetroskyFarshad_BubblePoint*>(models[modelIndex]->ModelSpecification)));
					break;
				default: throw std::invalid_argument("This kind of specification is not supported yet");
			}
		}
		else {
			modelSpecifications.push_back(nullptr);
		}
	}
}

unsigned int FluidCharacterization::getTableFormatCount() const
{
	size_t count = static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationTableFormat.size();
	if (count > (std::numeric_limits<unsigned int>::max)()) {
		throw out_of_range("There are too much FluidCharacterizationTableFormat");
	}
	return static_cast<unsigned int>(count);
}

uint64_t FluidCharacterization::getTableFormatColumnCount(const std::string & tableFormatUid) const
{
	for (const auto& tableFormat : static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationTableFormat) {
		if (tableFormat->uid == tableFormatUid) {
			return tableFormat->TableColumn.size();
		}
	}

	throw out_of_range("The table format uid \"" + tableFormatUid + "\" has not been found.");
}

void FluidCharacterization::pushBackTableFormat(const std::string & uid)
{
	prodml21__FluidCharacterizationTableFormat* const result = soap_new_prodml21__FluidCharacterizationTableFormat(getGsoapContext());
	result->Delimiter = soap_new_prodml21__TableDelimiter(getGsoapContext());
	result->Delimiter->asciiCharacters = " ";
	result->uid = uid.empty()
		? std::to_string(static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationTableFormat.size())
		: uid;
	static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationTableFormat.push_back(result);
}

std::string FluidCharacterization::getTableFormatColumnUom(const std::string & tableFormatUid, unsigned int columnIndex) const
{
	for (const auto& tableFormat : static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationTableFormat) {
		if (tableFormat->uid == tableFormatUid) {
			return tableFormat->TableColumn[columnIndex]->uom;
		}
	}

	throw out_of_range("The table format uid \"" + tableFormatUid + "\" has not been found.");
}

std::string FluidCharacterization::getTableFormatColumnProperty(const std::string & tableFormatUid, unsigned int columnIndex) const
{
	for (const auto& tableFormat : static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationTableFormat) {
		if (tableFormat->uid == tableFormatUid) {
			return tableFormat->TableColumn[columnIndex]->Property;
		}
	}

	throw out_of_range("The table format uid \"" + tableFormatUid + "\" has not been found.");
}

void FluidCharacterization::pushBackTableFormatColumn(unsigned int tableFormatIndex, gsoap_eml2_2::eml22__UnitOfMeasure uom, gsoap_eml2_2::prodml21__OutputFluidProperty fluidProperty)
{
	pushBackTableFormatColumn(tableFormatIndex,
		gsoap_eml2_2::soap_eml22__UnitOfMeasure2s(getGsoapContext(), uom),
		gsoap_eml2_2::soap_prodml21__OutputFluidProperty2s(getGsoapContext(), fluidProperty));
}

void FluidCharacterization::pushBackTableFormatColumn(unsigned int tableFormatIndex, const std::string & uom, const std::string & fluidProperty)
{
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationTableFormat.size() <= tableFormatIndex) {
		throw std::out_of_range("The table format index is out of range.");
	}

	prodml21__FluidCharacterizationTableColumn* const result = soap_new_prodml21__FluidCharacterizationTableColumn(getGsoapContext());
	result->uom = uom;
	result->Property = fluidProperty;
	static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationTableFormat[tableFormatIndex]->TableColumn.push_back(result);
}

unsigned int FluidCharacterization::getTableCount(unsigned int modelIndex) const
{
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel.size() <= modelIndex) {
		throw std::out_of_range("The model index is out of range.");
	}

	size_t count = static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel[modelIndex]->FluidCharacterizationTable.size();
	if (count > (std::numeric_limits<unsigned int>::max)()) {
		throw out_of_range("There are too much FluidCharacterizationTable in this model");
	}
	return static_cast<unsigned int>(count);
}

std::string FluidCharacterization::getTableName(unsigned int modelIndex, unsigned int tableIndex) const
{
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel.size() <= modelIndex) {
		throw std::out_of_range("The model index is out of range.");
	}
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel[modelIndex]->FluidCharacterizationTable.size() <= tableIndex) {
		throw std::out_of_range("The table index is out of range.");
	}

	return static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel[modelIndex]->FluidCharacterizationTable[tableIndex]->name;
}

std::string FluidCharacterization::getTableFormatUid(unsigned int modelIndex, unsigned int tableIndex) const
{
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel.size() <= modelIndex) {
		throw std::out_of_range("The model index is out of range.");
	}
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel[modelIndex]->FluidCharacterizationTable.size() <= tableIndex) {
		throw std::out_of_range("The table index is out of range.");
	}

	return static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel[modelIndex]->FluidCharacterizationTable[tableIndex]->tableFormat;
}

void FluidCharacterization::pushBackTable(unsigned int modelIndex, const std::string & name, const std::string & tableFormatUid, const std::string & uid)
{
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel.size() <= modelIndex) {
		throw std::out_of_range("The model index is out of range.");
	}

	prodml21__FluidCharacterizationTable* const result = soap_new_prodml21__FluidCharacterizationTable(getGsoapContext());
	result->uid = uid.empty()
		? std::to_string(static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel[modelIndex]->FluidCharacterizationTable.size())
		: uid;

	result->name = name;
	result->tableFormat = tableFormatUid;
	static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel[modelIndex]->FluidCharacterizationTable.push_back(result);
}

unsigned int FluidCharacterization::getTableRowCount(unsigned int modelIndex, unsigned int tableIndex) const
{
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel.size() <= modelIndex) {
		throw std::out_of_range("The model index is out of range.");
	}
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel[modelIndex]->FluidCharacterizationTable.size() <= tableIndex) {
		throw std::out_of_range("The table index is out of range.");
	}

	size_t count = static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel[modelIndex]->FluidCharacterizationTable[tableIndex]->TableRow.size();
	if (count > (std::numeric_limits<unsigned int>::max)()) {
		throw out_of_range("There are too much row in this table");
	}
	return static_cast<unsigned int>(count);
}

std::string FluidCharacterization::getTableRowContent(unsigned int modelIndex, unsigned int tableIndex, unsigned int rowIndex) const
{
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel.size() <= modelIndex) {
		throw std::out_of_range("The model index is out of range.");
	}
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel[modelIndex]->FluidCharacterizationTable.size() <= tableIndex) {
		throw std::out_of_range("The table index is out of range.");
	}
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel[modelIndex]->FluidCharacterizationTable[tableIndex]->TableRow.size() <= rowIndex) {
		throw std::out_of_range("The row index is out of range.");
	}

	return static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel[modelIndex]->FluidCharacterizationTable[tableIndex]->TableRow[rowIndex]->__item;
}

void FluidCharacterization::pushBackTableRow(unsigned int modelIndex, unsigned int tableIndex, const std::vector<double> & rowContent)
{
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel.size() <= modelIndex) {
		throw std::out_of_range("The model index is out of range.");
	}

	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel[modelIndex]->FluidCharacterizationTable.size() <= tableIndex) {
		throw std::out_of_range("The table index is out of range.");
	}

	auto tableFormatUid = getTableFormatUid(modelIndex, tableIndex);
	if (rowContent.size() != getTableFormatColumnCount(tableFormatUid)) {
		throw std::out_of_range("you did not give the same number of values than the expected one.");
	}

	prodml21__FluidCharacterizationTableRow* const result = soap_new_prodml21__FluidCharacterizationTableRow(getGsoapContext());
	result->row = std::to_string(static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel[modelIndex]->FluidCharacterizationTable[tableIndex]->TableRow.size());

	std::ostringstream oss;
	std::copy(rowContent.begin(), rowContent.end() - 1, std::ostream_iterator<double>(oss, " "));
	oss << rowContent.back();	
	result->__item = oss.str();

	static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel[modelIndex]->FluidCharacterizationTable[tableIndex]->TableRow.push_back(result);
}

void FluidCharacterization::pushBackTableRow(unsigned int modelIndex, unsigned int tableIndex, const std::vector<double> & rowContent, bool isSaturated)
{
	pushBackTableRow(modelIndex, tableIndex, rowContent);
	static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel[modelIndex]->FluidCharacterizationTable[tableIndex]->TableRow.back()->kind = (gsoap_eml2_2::prodml21__saturationKind*)soap_malloc(getGsoapContext(), sizeof(gsoap_eml2_2::prodml21__saturationKind));
	*static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel[modelIndex]->FluidCharacterizationTable[tableIndex]->TableRow.back()->kind = isSaturated
		? prodml21__saturationKind::saturated
		: prodml21__saturationKind::undersaturated;
}

void FluidCharacterization::pushBackParameter(unsigned int modelIndex, double value, gsoap_eml2_2::eml22__UnitOfMeasure uom, gsoap_eml2_2::prodml21__OutputFluidProperty fluidProperty)
{
	if (static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel.size() <= modelIndex) {
		throw std::out_of_range("The model index is out of range.");
	}

	// Creates a parameter set if not already present
	auto* model = static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel[modelIndex];
	if (model->FluidCharacterizationParameterSet == nullptr) {
		model->FluidCharacterizationParameterSet = soap_new_prodml21__FluidCharacterizationParameterSet(getGsoapContext());
	}

	auto* param = soap_new_prodml21__FluidCharacterizationParameter(getGsoapContext());
	param->value = value;
	param->uom = gsoap_eml2_2::soap_eml22__UnitOfMeasure2s(getGsoapContext(), uom);
	param->Property = gsoap_eml2_2::soap_prodml21__OutputFluidProperty2s(getGsoapContext(), fluidProperty);

	model->FluidCharacterizationParameterSet->FluidCharacterizationParameter.push_back(param);
}

void FluidCharacterization::pushBackParameter(unsigned int modelIndex, double value, gsoap_eml2_2::eml22__UnitOfMeasure uom, gsoap_eml2_2::prodml21__OutputFluidProperty fluidProperty, gsoap_eml2_2::prodml21__ThermodynamicPhase phase)
{
	pushBackParameter(modelIndex, value, uom, fluidProperty);
	static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel[modelIndex]->FluidCharacterizationParameterSet->FluidCharacterizationParameter.back()->Phase = (gsoap_eml2_2::prodml21__ThermodynamicPhase*)soap_malloc(getGsoapContext(), sizeof(gsoap_eml2_2::prodml21__ThermodynamicPhase));
	*static_cast<prodml21__FluidCharacterization*>(gsoapProxy2_2)->FluidCharacterizationModel[modelIndex]->FluidCharacterizationParameterSet->FluidCharacterizationParameter.back()->Phase = phase;
}
