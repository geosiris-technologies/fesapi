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

#include "../resqml2/IjkGridNoGeometryRepresentation.h"

namespace RESQML2_0_1_NS
{
	/** @brief	An ijk grid no geometry representation. This class cannot be inherited. */
	class IjkGridNoGeometryRepresentation final : public RESQML2_NS::IjkGridNoGeometryRepresentation
	{
	public:

		/**
		 * Constructor
		 *
		 * @param [in,out]	repo  	If non-null, the repo.
		 * @param 		  	guid  	Unique identifier.
		 * @param 		  	title 	The title.
		 * @param 			iCount	Number of cell in I direction.
		 * @param 			jCount	Number of cell in J direction.
		 * @param 			kCount	Number of cell in K direction.
		 * @param 		  	kGaps	(Optional) Boolean array of length KCellCount-1.
		 *							TRUE if there is a gap after the corresponding layer.
		 *							Won't be freed by FESAPI.
		 * @param [in]		proxy	(Optional) The HDF proxy for writing the @p enabledCells
		 * 							values. If @c nullptr (default), then the default HDF proxy will be
		 * 							used.
		 */
		IjkGridNoGeometryRepresentation(COMMON_NS::DataObjectRepository * repo,
			const std::string & guid, const std::string & title,
			unsigned int iCount, unsigned int jCount, unsigned int kCount, bool* kGaps = nullptr, EML2_NS::AbstractHdfProxy* proxy = nullptr) :
			RESQML2_NS::IjkGridNoGeometryRepresentation(repo, guid, title, iCount, jCount, kCount, kGaps, proxy) {}

		/**
		 * Constructor
		 *
		 * @param [in,out]	interp	If non-null, the interp.
		 * @param 		  	guid  	Unique identifier.
		 * @param 		  	title 	The title.
		 * @param 			iCount	Number of cell in I direction.
		 * @param 			jCount	Number of cell in J direction.
		 * @param 			kCount	Number of cell in K direction.
		 * @param 		  	kGaps	(Optional) Boolean array of length KCellCount-1.
		 *							TRUE if there is a gap after the corresponding layer.
		 *							Won't be freed by FESAPI.
		 * @param [in]		proxy	(Optional) The HDF proxy for writing the @p enabledCells
		 * 							values. If @c nullptr (default), then the default HDF proxy will be
		 * 							used.
		 */
		IjkGridNoGeometryRepresentation(RESQML2_NS::AbstractFeatureInterpretation* interp,
			const std::string & guid, const std::string & title,
			unsigned int iCount, unsigned int jCount, unsigned int kCount, bool* kGaps = nullptr, EML2_NS::AbstractHdfProxy* proxy = nullptr) :
			RESQML2_NS::IjkGridNoGeometryRepresentation(interp, guid, title, iCount, jCount, kCount, kGaps, proxy) {}

		/**
		 * @brief	Creates an instance of this class by wrapping a gSOAP instance.
		 *
		 * @param [in]	fromGsoap	If non-null, the gSOAP instance.
		 */
		IjkGridNoGeometryRepresentation(gsoap_resqml2_0_1::_resqml20__IjkGridRepresentation* fromGsoap) : RESQML2_NS::IjkGridNoGeometryRepresentation(fromGsoap) {}

		/**
		 * @brief	Creates an instance of this class by wrapping a gSOAP instance.
		 *
		 * @param [in]	fromGsoap	If non-null, the gSOAP instance.
		 */
		IjkGridNoGeometryRepresentation(gsoap_resqml2_0_1::_resqml20__TruncatedIjkGridRepresentation* fromGsoap) : RESQML2_NS::IjkGridNoGeometryRepresentation(fromGsoap) {}

		/** Destructor does nothing since the memory is managed by the gsoap context. */
		~IjkGridNoGeometryRepresentation() = default;

		COMMON_NS::DataObjectReference getHdfProxyDor() const final;

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
