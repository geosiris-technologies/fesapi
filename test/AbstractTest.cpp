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
#include "AbstractTest.h"
#include "common/EpcDocument.h"
#include "catch.hpp"

using namespace std;
using namespace commontest;

AbstractTest::AbstractTest(const string & epcDocPath) :
	epcDoc(nullptr),
	epcDocPath(epcDocPath) {
}

AbstractTest::AbstractTest(COMMON_NS::EpcDocument* epcDoc) :
	epcDoc(epcDoc),
	epcDocPath(epcDoc->getStorageDirectory()) {
}

void AbstractTest::serialize() {
	epcDoc = new COMMON_NS::EpcDocument(epcDocPath, COMMON_NS::EpcDocument::OVERWRITE);
	epcDoc->createHdfProxy("75f5b460-3ccb-4102-a06e-e9c1019769b2", "Hdf Proxy Test", epcDoc->getStorageDirectory(), epcDoc->getName() + ".h5");

	initEpcDoc();
	
	epcDoc->serialize();
	epcDoc->close();
	delete epcDoc;
}

void AbstractTest::deserialize() {
	epcDoc = new COMMON_NS::EpcDocument(epcDocPath);

	std::string validationResult = epcDoc->deserialize();
	if (!validationResult.empty()) {
		cout << "Validation error: " << validationResult << endl;
	}
	REQUIRE( validationResult.empty() );
	
	REQUIRE( epcDoc->getHdfProxySet().size() == 1 );

	vector<string> warningSet = epcDoc->getWarnings();
	if (!warningSet.empty()) {
		cout << "EPC document " << epcDoc->getName() << ".epc deserialized with " << warningSet.size() << " warning(s)" << endl;
		for (size_t i=0; i < warningSet.size(); ++i){
			cout << "Warning " << i+1 << ": " << warningSet[i] << endl;
		}
		cout << endl;
	}

	readEpcDoc();
	epcDoc->close();

	delete epcDoc;
}
