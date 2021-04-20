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
#include "StringTableLookup.h"

#include <limits>
#include <stdexcept>

using namespace std;
using namespace RESQML2_2_NS;
using namespace gsoap_eml2_3;

const char* StringTableLookup::XML_NS = "resqml22";

StringTableLookup::StringTableLookup(COMMON_NS::DataObjectRepository* repo, const string & guid, const string & title)
{
	if (repo == nullptr) {
		throw invalid_argument("The repo cannot be null.");
	}

	gsoapProxy2_3 = soap_new_resqml22__StringTableLookup(repo->getGsoapContext());

	initMandatoryMetadata();
	setMetadata(guid, title, std::string(), -1, std::string(), std::string(), -1, std::string());

	repo->addDataObject(this);
}

unsigned int StringTableLookup::getItemCount() const
{
	return static_cast<_resqml22__StringTableLookup*>(gsoapProxy2_3)->Value.size();
}

long StringTableLookup::getKeyAtIndex(unsigned int index) const
{
	if (getItemCount() <= index) {
		throw out_of_range("The index is out of range.");
	}

	return (static_cast<_resqml22__StringTableLookup*>(gsoapProxy2_3)->Value)[index]->Key;
}

std::string StringTableLookup::getStringValueAtIndex(unsigned int index) const
{
	if (getItemCount() <= index) {
		throw out_of_range("The index is out of range.");
	}

	return (static_cast<_resqml22__StringTableLookup*>(gsoapProxy2_3)->Value)[index]->Value;
}

bool StringTableLookup::containsKey(long longValue)
{
	_resqml22__StringTableLookup* stringLookup = static_cast<_resqml22__StringTableLookup*>(gsoapProxy2_3);

	for (size_t i = 0; i < stringLookup->Value.size(); ++i) {
		if (stringLookup->Value[i]->Key == longValue) {
			return true;
		}
	}

	return false;
}

std::string StringTableLookup::getStringValue(long longValue)
{
	_resqml22__StringTableLookup* stringLookup = static_cast<_resqml22__StringTableLookup*>(gsoapProxy2_3);

	for (size_t i = 0; i < stringLookup->Value.size(); ++i) {
		if (stringLookup->Value[i]->Key == longValue) {
			return stringLookup->Value[i]->Value;
		}
	}

	return "";
}

void StringTableLookup::addValue(const string & strValue, long longValue)
{
	_resqml22__StringTableLookup* stringLookup = static_cast<_resqml22__StringTableLookup*>(gsoapProxy2_3);

	resqml22__StringLookup* xmlValue = soap_new_resqml22__StringLookup(gsoapProxy2_3->soap);
	xmlValue->Key = longValue;
	xmlValue->Value = strValue;
	stringLookup->Value.push_back(xmlValue);
}

void StringTableLookup::setValue(const string & strValue, long longValue)
{
	_resqml22__StringTableLookup* stringLookup = static_cast<_resqml22__StringTableLookup*>(gsoapProxy2_3);

	for (size_t i = 0; i < stringLookup->Value.size(); ++i) {
		if (stringLookup->Value[i]->Key == longValue) {
			stringLookup->Value[i]->Value = strValue;
			return;
		}
	}
}

int64_t StringTableLookup::getMinimumValue()
{
	_resqml22__StringTableLookup* stringLookup = static_cast<_resqml22__StringTableLookup*>(gsoapProxy2_3);

	int64_t min = (std::numeric_limits<int64_t>::max)();
	for (size_t i = 0; i < stringLookup->Value.size(); ++i) {
		if (min > stringLookup->Value[i]->Key) {
			min = stringLookup->Value[i]->Key;
		}
	}

	return min;
}

int64_t StringTableLookup::getMaximumValue()
{
	_resqml22__StringTableLookup* stringLookup = static_cast<_resqml22__StringTableLookup*>(gsoapProxy2_3);

	int64_t max = (std::numeric_limits<int64_t>::min)();
	for (size_t i = 0; i < stringLookup->Value.size(); ++i) {
		if (max < stringLookup->Value[i]->Key) {
			max = stringLookup->Value[i]->Key;
		}
	}

	return max;
}

unordered_map<long, string> StringTableLookup::getMap() const
{
	unordered_map<long, string> result;

	_resqml22__StringTableLookup* stringLookup = static_cast<_resqml22__StringTableLookup*>(gsoapProxy2_3);

	for (size_t i = 0; i < stringLookup->Value.size(); ++i) {
		result[stringLookup->Value[i]->Key] = stringLookup->Value[i]->Value;
	}

	return result;
}
