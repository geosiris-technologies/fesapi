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
#pragma once

#include "../witsml2/Trajectory.h"

namespace WITSML2_1_NS
{
	/**
	 * @brief	It contains many trajectory stations to capture the information about individual survey points.
	 */
	class Trajectory : public WITSML2_NS::Trajectory
	{
	public:

		/**
		 * Constructor for partial transfer
		 *
		 * @param [in,out]	partialObject	If non-null, the partial object.
		 *
		 * 
		 */
		DLL_IMPORT_OR_EXPORT Trajectory(gsoap_resqml2_0_1::eml20__DataObjectReference* partialObject) : WITSML2_NS::Trajectory(partialObject) {}

		/**
		 * @brief	Constructor
		 *
		 * @exception	std::invalid_argument	If <tt>witsmlWellbore == nullptr</tt>.
		 *
		 * @param [in]	witsmlWellbore	If non-null, the witsml wellbore.
		 * @param 	  	guid		  	Unique identifier.
		 * @param 	  	title		  	The title.
		 * @param 	  	isActive	  	True if is active, false if not.
		 */
		Trajectory(WITSML2_NS::Wellbore* witsmlWellbore,
			const std::string & guid,
			const std::string & title,
			bool isActive);

		/**
		 * Creates an instance of this class by wrapping a gsoap instance.
		 *
		 * @param [in,out]	fromGsoap	If non-null, from gsoap.
		 */
		Trajectory(gsoap_eml2_3::witsml21__Trajectory* fromGsoap) : WITSML2_NS::Trajectory(fromGsoap) {}

		/** Destructor does nothing since the memory is managed by the gsoap context. */
		~Trajectory() = default;

		DLL_IMPORT_OR_EXPORT COMMON_NS::DataObjectReference getWellboreDor() const final;
		DLL_IMPORT_OR_EXPORT void setWellbore(WITSML2_NS::Wellbore* witsmlWellbore) final;

		FINAL_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(time_t, DTimTrajStart)
		FINAL_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(time_t, DTimTrajEnd)
		FINAL_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(gsoap_eml2_3::eml23__NorthReferenceKind, AziRef)

		FINAL_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(bool, Definitive)
		FINAL_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(bool, Memory)
		FINAL_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(bool, FinalTraj)

		//***************************************
		// ******* TRAJECTORY STATIONS **********
		//***************************************

		// Mandatory
		FINAL_GETTER_AND_SETTER_GENERIC_ATTRIBUTE_IN_VECTOR(std::string, TrajectoryStation, uid)
		FINAL_GETTER_AND_SETTER_GENERIC_ATTRIBUTE_IN_VECTOR(gsoap_eml2_3::witsml21__TrajStationType, TrajectoryStation, TypeTrajStation)
		FINAL_GETTER_AND_SETTER_DEPTH_MEASURE_ATTRIBUTE_IN_VECTOR(TrajectoryStation, Md, gsoap_eml2_3::eml23__LengthUom)
		FINAL_GETTER_AND_SETTER_GENERIC_ATTRIBUTE_IN_VECTOR(time_t, TrajectoryStation, Creation)
		FINAL_GETTER_AND_SETTER_GENERIC_ATTRIBUTE_IN_VECTOR(time_t, TrajectoryStation, LastUpdate)


		// Optional bool
		FINAL_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(bool, TrajectoryStation, ManuallyEntered)
		FINAL_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(bool, TrajectoryStation, GravAccelCorUsed)
		FINAL_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(bool, TrajectoryStation, MagXAxialCorUsed)
		FINAL_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(bool, TrajectoryStation, SagCorUsed)
		FINAL_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(bool, TrajectoryStation, MagDrlstrCorUsed)
		FINAL_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(bool, TrajectoryStation, InfieldRefCorUsed)
		FINAL_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(bool, TrajectoryStation, InterpolatedInfieldRefCorUsed)
		FINAL_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(bool, TrajectoryStation, InHoleRefCorUsed)
		FINAL_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(bool, TrajectoryStation, AxialMagInterferenceCorUsed)
		FINAL_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(bool, TrajectoryStation, CosagCorUsed)
		FINAL_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(bool, TrajectoryStation, MSACorUsed)

		// Optional string
		FINAL_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(std::string, TrajectoryStation, MagModelUsed)
		FINAL_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(std::string, TrajectoryStation, MagModelValid)
		FINAL_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(std::string, TrajectoryStation, GeoModelUsed)

		// Optional timestamp
		FINAL_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(time_t, TrajectoryStation, DTimStn)

		// Optional enum
		FINAL_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(gsoap_eml2_3::witsml21__TypeSurveyTool, TrajectoryStation, TypeSurveyTool)
		FINAL_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(gsoap_eml2_3::witsml21__TrajStnCalcAlgorithm, TrajectoryStation, CalcAlgorithm)
		FINAL_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(gsoap_eml2_3::witsml21__TrajStationStatus, TrajectoryStation, StatusTrajStation)

		// Optional Length Measure
		FINAL_GETTER_AND_SETTER_DEPTH_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, Tvd, gsoap_eml2_3::eml23__LengthUom)
		FINAL_GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, DispNs, gsoap_eml2_3::eml23__LengthUom)
		FINAL_GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, DispEw, gsoap_eml2_3::eml23__LengthUom)
		FINAL_GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, VertSect, gsoap_eml2_3::eml23__LengthUom)
		FINAL_GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, MdDelta, gsoap_eml2_3::eml23__LengthUom)
		FINAL_GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, TvdDelta, gsoap_eml2_3::eml23__LengthUom)

		// Optional Plane Angle Measure
		FINAL_GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, Incl, gsoap_eml2_3::eml23__PlaneAngleUom)
		FINAL_GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, Azi, gsoap_eml2_3::eml23__PlaneAngleUom)
		FINAL_GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, Mtf, gsoap_eml2_3::eml23__PlaneAngleUom)
		FINAL_GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, Gtf, gsoap_eml2_3::eml23__PlaneAngleUom)
		FINAL_GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, DipAngleUncert, gsoap_eml2_3::eml23__PlaneAngleUom)
		FINAL_GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, MagDipAngleReference, gsoap_eml2_3::eml23__PlaneAngleUom)

		// Optional Angle per Length Measure
		FINAL_GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, Dls, gsoap_eml2_3::eml23__AnglePerLengthUom)
		FINAL_GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, RateTurn, gsoap_eml2_3::eml23__AnglePerLengthUom)
		FINAL_GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, RateBuild, gsoap_eml2_3::eml23__AnglePerLengthUom)

		// Optional Linear Acceleration Measure
		FINAL_GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, GravTotalUncert, gsoap_eml2_3::eml23__LinearAccelerationUom)
		FINAL_GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, GravTotalFieldReference, gsoap_eml2_3::eml23__LinearAccelerationUom)

		// Optional Magnetic Flux Density Measure
		FINAL_GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, MagTotalUncert, gsoap_eml2_3::eml23__MagneticFluxDensityUom)
		FINAL_GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, MagTotalFieldReference, gsoap_eml2_3::eml23__MagneticFluxDensityUom)

		/**
		 * Push back a minimal trajectory station into the trajectory.
		 *
		 * @param 	kind   	The kind of trajectory station.
		 * @param 	mdValue	The Measured Depth value of the trajectory station.
		 * @param 	uom	   	The Measured Depth uom of the trajectory station.
		 * @param 	uid	   	(Optional) The uid of the trajectory station. Automatically set to its index
		 * 					if empty.
		 */
		DLL_IMPORT_OR_EXPORT void pushBackTrajectoryStation(gsoap_eml2_3::witsml21__TrajStationType kind, double mdValue, gsoap_eml2_3::eml23__LengthUom uom, const std::string & uid = "") final;

		/**
		 * Get the count of trajectory stations in this trajectory
		 *
		 * @returns	the count of trajectory stations in this trajectory.
		 */
		DLL_IMPORT_OR_EXPORT unsigned int getTrajectoryStationCount() const final;

		/**
		* The standard XML namespace for serializing this data object.
		*/
		DLL_IMPORT_OR_EXPORT static constexpr char const* XML_NS = "witsml21";

		/**
		* Get the standard XML namespace for serializing this data object.
		*/
		DLL_IMPORT_OR_EXPORT std::string getXmlNamespace() const final { return XML_NS; }
	};
}
