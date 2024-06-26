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

#include "../resqml2/FluidBoundaryInterpretation.h"

namespace RESQML2_2_NS
{
	/** A Fluid boundary interpretation. */
	class FluidBoundaryInterpretation final : public RESQML2_NS::FluidBoundaryInterpretation
	{
	public:

		/**
		 * Only to be used in partial transfer context
		 *
		 * @param [in,out]	partialObject	If non-null, the partial object.
		 */
		DLL_IMPORT_OR_EXPORT FluidBoundaryInterpretation(gsoap_resqml2_0_1::eml20__DataObjectReference* partialObject) : RESQML2_NS::FluidBoundaryInterpretation(partialObject) {}

		/**
		 * @brief	Creates an instance of this class in a gsoap context.
		 *
		 * @exception	std::invalid_argument	If <tt>fault == nullptr</tt>.
		 *
		 * @param [in]	feature			The feature the instance interprets.
		 * @param 	  	guid 			The guid to set to the new instance. If empty then a new guid will be
		 * 								generated.
		 * @param 	  	title			A title for the instance to create.
		 * @param 	  	fluidContact	The fluid contact this boundary is.
		 */
		FluidBoundaryInterpretation(RESQML2_NS::BoundaryFeature * feature, const std::string & guid, const std::string & title, gsoap_eml2_3::resqml22__FluidContact fluidContact);

		/**
		 * Creates an instance of this class by wrapping a gsoap instance.
		 *
		 * @param [in,out]	fromGsoap	If non-null, from gsoap.
		 */
		FluidBoundaryInterpretation(gsoap_eml2_3::_resqml22__FluidBoundaryInterpretation* fromGsoap): RESQML2_NS::FluidBoundaryInterpretation(fromGsoap) {}

		/** Destructor does nothing since the memory is managed by the gsoap context. */
		~FluidBoundaryInterpretation() = default;

		/**
		* The standard XML namespace for serializing this data object.
		*/
		DLL_IMPORT_OR_EXPORT static const char* XML_NS;

		/**
		* Get the standard XML namespace for serializing this data object.
		*/
		DLL_IMPORT_OR_EXPORT std::string getXmlNamespace() const final { return XML_NS; }
	};
}
