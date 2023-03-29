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
%{
#include "../src/witsml2/Trajectory.h"
#include "../src/witsml2/Well.h"
#include "../src/witsml2/Wellbore.h"
#include "../src/witsml2/WellboreObject.h"
%}

#define GETTER_AND_SETTER_GENERIC_ATTRIBUTE(attributeDatatype, attributeName)\
	void set##attributeName(const attributeDatatype & value);\
	attributeDatatype get##attributeName() const;
#define GETTER_PRESENCE_ATTRIBUTE(attributeName) bool has##attributeName() const;
#define GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(attributeDatatype, attributeName)\
	GETTER_AND_SETTER_GENERIC_ATTRIBUTE(attributeDatatype, attributeName)\
	GETTER_PRESENCE_ATTRIBUTE(attributeName)

#define GETTER_AND_SETTER_MEASURE_ATTRIBUTE(attributeName, uomDatatype)\
	void set##attributeName(double value, uomDatatype uom);\
	double get##attributeName##Value() const;\
	uomDatatype get##attributeName##Uom() const;
#define GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE(attributeName, uomDatatype)\
	GETTER_AND_SETTER_MEASURE_ATTRIBUTE(attributeName, uomDatatype)\
	GETTER_PRESENCE_ATTRIBUTE(attributeName)

#define GETTER_AND_SETTER_DEPTH_MEASURE_ATTRIBUTE(attributeName, uomDatatype)\
	void set##attributeName(double value, uomDatatype uom, const std::string & datum);\
	double get##attributeName##Value() const;\
	uomDatatype get##attributeName##Uom() const;\
	std::string get##attributeName##Datum() const;
#define GETTER_AND_SETTER_DEPTH_MEASURE_OPTIONAL_ATTRIBUTE(attributeName, uomDatatype)\
	GETTER_AND_SETTER_DEPTH_MEASURE_ATTRIBUTE(attributeName, uomDatatype)\
	GETTER_PRESENCE_ATTRIBUTE(attributeName)

#define GETTER_AND_SETTER_GENERIC_ATTRIBUTE_IN_VECTOR(attributeDatatype, vectorName, attributeName)\
	void set##vectorName##attributeName(unsigned int index, const attributeDatatype & value);\
	attributeDatatype get##vectorName##attributeName(unsigned int index) const;	
	
#define GETTER_AND_SETTER_MEASURE_ATTRIBUTE_IN_VECTOR(vectorName, attributeName, uomDatatype)\
	void set##vectorName##attributeName(unsigned int index, double value, uomDatatype uom);\
	double get##vectorName##attributeName##Value(unsigned int index) const;\
	uomDatatype get##vectorName##attributeName##Uom(unsigned int index) const;

#define GETTER_PRESENCE_ATTRIBUTE_IN_VECTOR(vectorName, attributeName) bool has##vectorName##attributeName(unsigned int index) const;	
#define GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(attributeDatatype, vectorName, attributeName)\
	GETTER_AND_SETTER_GENERIC_ATTRIBUTE_IN_VECTOR(attributeDatatype, vectorName, attributeName)\
	GETTER_PRESENCE_ATTRIBUTE_IN_VECTOR(vectorName, attributeName)	
	
#define GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(vectorName, attributeName, uomDatatype)\
	GETTER_AND_SETTER_MEASURE_ATTRIBUTE_IN_VECTOR(vectorName, attributeName, uomDatatype)\
	GETTER_PRESENCE_ATTRIBUTE_IN_VECTOR(vectorName, attributeName)
	
#define GETTER_AND_SETTER_DEPTH_MEASURE_ATTRIBUTE_IN_VECTOR(vectorName, attributeName, uomDatatype)\
	void set##vectorName##attributeName(unsigned int index, double value, uomDatatype uom, const std::string & datum);\
	double get##vectorName##attributeName##Value(unsigned int index) const;\
	uomDatatype get##vectorName##attributeName##Uom(unsigned int index) const;\
	std::string get##vectorName##attributeName##Datum(unsigned int index) const;
#define GETTER_AND_SETTER_DEPTH_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(vectorName, attributeName, uomDatatype)\
	GETTER_AND_SETTER_DEPTH_MEASURE_ATTRIBUTE_IN_VECTOR(vectorName, attributeName, uomDatatype)\
	GETTER_PRESENCE_ATTRIBUTE_IN_VECTOR(vectorName, attributeName)

namespace gsoap_eml2_1
{
	enum class eml21__WellStatus {
		abandoned = 0,
		active = 1,
		active_x0020_x002d__x0020injecting = 2,
		active_x0020_x002d__x0020producing = 3,
		completed = 4,
		drilling = 5,
		partially_x0020plugged = 6,
		permitted = 7,
		plugged_x0020and_x0020abandoned = 8,
		proposed = 9,
		sold = 10,
		suspended = 11,
		temporarily_x0020abandoned = 12,
		testing = 13,
		tight = 14,
		working_x0020over = 15,
		unknown = 16
	};
	enum class witsml20__WellPurpose {
		appraisal = 0,
		appraisal_x0020_x002d__x0020confirmation_x0020appraisal = 1,
		appraisal_x0020_x002d__x0020exploratory_x0020appraisal = 2,
		exploration = 3,
		exploration_x0020_x002d__x0020deeper_pool_x0020wildcat = 4,
		exploration_x0020_x002d__x0020new_field_x0020wildcat = 5,
		exploration_x0020_x002d__x0020new_pool_x0020wildcat = 6,
		exploration_x0020_x002d__x0020outpost_x0020wildcat = 7,
		exploration_x0020_x002d__x0020shallower_pool_x0020wildcat = 8,
		development = 9,
		development_x0020_x002d__x0020infill_x0020development = 10,
		development_x0020_x002d__x0020injector = 11,
		development_x0020_x002d__x0020producer = 12,
		fluid_x0020storage = 13,
		fluid_x0020storage_x0020_x002d__x0020gas_x0020storage = 14,
		general_x0020srvc = 15,
		general_x0020srvc_x0020_x002d__x0020borehole_x0020re_acquisition = 16,
		general_x0020srvc_x0020_x002d__x0020observation = 17,
		general_x0020srvc_x0020_x002d__x0020relief = 18,
		general_x0020srvc_x0020_x002d__x0020research = 19,
		general_x0020srvc_x0020_x002d__x0020research_x0020_x002d__x0020drill_x0020test = 20,
		general_x0020srvc_x0020_x002d__x0020research_x0020_x002d__x0020strat_x0020test = 21,
		general_x0020srvc_x0020_x002d__x0020waste_x0020disposal = 22,
		mineral = 23
	};
	enum class witsml20__WellFluid {
		air = 0,
		condensate = 1,
		dry = 2,
		gas = 3,
		gas_water = 4,
		non_x0020HC_x0020gas = 5,
		non_x0020HC_x0020gas_x0020_x002d__x0020CO2 = 6,
		oil = 7,
		oil_gas = 8,
		oil_water = 9,
		steam = 10,
		water = 11,
		water_x0020_x002d__x0020brine = 12,
		water_x0020_x002d__x0020fresh_x0020water = 13
	};
	enum class witsml20__WellDirection {
		huff_n_puff = 0,
		injector = 1,
		producer = 2,
		uncertain = 3
	};
	enum class eml21__WellboreDatumReference {
		ground_x0020level = 0,
		kelly_x0020bushing = 1,
		mean_x0020sea_x0020level = 2,
		derrick_x0020floor = 3,
		casing_x0020flange = 4,
		crown_x0020valve = 5,
		rotary_x0020bushing = 6,
		rotary_x0020table = 7,
		sea_x0020floor = 8,
		lowest_x0020astronomical_x0020tide = 9,
		mean_x0020higher_x0020high_x0020water = 10,
		mean_x0020high_x0020water = 11,
		mean_x0020lower_x0020low_x0020water = 12,
		mean_x0020low_x0020water = 13,
		mean_x0020tide_x0020level = 14,
		kickoff_x0020point = 15
	};
	enum class witsml20__WellboreType {
		bypass = 0,
		initial = 1,
		redrill = 2,
		reentry = 3,
		respud = 4,
		sidetrack = 5
	};
	enum class witsml20__WellboreShape {
		build_x0020and_x0020hold = 0,
		deviated = 1,
		double_x0020kickoff = 2,
		horizontal = 3,
		S_shaped = 4,
		vertical = 5
	};
	enum class witsml20__PhysicalStatus {
		closed = 0,
		open = 1,
		proposed = 2
	};
	enum class witsml20__ElevCodeEnum {
		CF = 0,
		CV = 1,
		DF = 2,
		GL = 3,
		KB = 4,
		RB = 5,
		RT = 6,
		SF = 7,
		LAT = 8,
		SL = 9,
		MHHW = 10,
		MHW = 11,
		MLLW = 12,
		MLW = 13,
		MTL = 14,
		KO = 15,
		unknown = 16
	};
	enum class witsml20__TrajStationType {
		azimuth_x0020on_x0020plane = 0,
		buildrate_x0020to_x0020delta_MD = 1,
		buildrate_x0020to_x0020INCL = 2,
		buildrate_x0020to_x0020MD = 3,
		buildrate_x0020and_x0020turnrate_x0020to_x0020AZI = 4,
		buildrate_x0020and_x0020turnrate_x0020to_x0020delta_MD = 5,
		buildrate_x0020and_x0020turnrate_x0020to_x0020INCL = 6,
		buildrate_x0020and_x0020turnrate_x0020to_x0020INCL_x0020and_x0020AZI = 7,
		buildrate_x0020and_x0020turnrate_x0020to_x0020MD = 8,
		buildrate_x0020and_x0020turnrate_x0020to_x0020TVD = 9,
		buildrate_x0020TVD = 10,
		casing_x0020MD = 11,
		casing_x0020TVD = 12,
		DLS = 13,
		DLS_x0020to_x0020AZI_x0020and_x0020MD = 14,
		DLS_x0020to_x0020AZI_TVD = 15,
		DLS_x0020to_x0020INCL = 16,
		DLS_x0020to_x0020INCL_x0020and_x0020AZI = 17,
		DLS_x0020to_x0020INCL_x0020and_x0020MD = 18,
		DLS_x0020to_x0020INCL_x0020and_x0020TVD = 19,
		DLS_x0020to_x0020NS = 20,
		DLS_x0020and_x0020toolface_x0020to_x0020AZI = 21,
		DLS_x0020and_x0020toolface_x0020to_x0020delta_MD = 22,
		DLS_x0020and_x0020toolface_x0020to_x0020INCL = 23,
		DLS_x0020and_x0020toolface_x0020to_x0020INCL_AZI = 24,
		DLS_x0020and_x0020toolface_x0020to_x0020MD = 25,
		DLS_x0020and_x0020toolface_x0020to_x0020TVD = 26,
		formation_x0020MD = 27,
		formation_x0020TVD = 28,
		hold_x0020to_x0020delta_MD = 29,
		hold_x0020to_x0020MD = 30,
		hold_x0020to_x0020TVD = 31,
		INCL_x0020AZI_x0020and_x0020TVD = 32,
		interpolated = 33,
		marker_x0020MD = 34,
		marker_x0020TVD = 35,
		MD_x0020and_x0020INCL = 36,
		MD_x0020INCL_x0020and_x0020AZI = 37,
		N_x0020E_x0020and_x0020TVD = 38,
		NS_x0020EW_x0020and_x0020TVD = 39,
		target_x0020center = 40,
		target_x0020offset = 41,
		tie_x0020in_x0020point = 42,
		turnrate_x0020to_x0020AZI = 43,
		turnrate_x0020to_x0020delta_MD = 44,
		turnrate_x0020to_x0020MD = 45,
		turnrate_x0020to_x0020TVD = 46,
		unknown = 47
	};
	enum class witsml20__ChannelStatus {
		active = 0,
		closed = 1,
		inactive = 2
	};
	enum class witsml20__AziRef {
		magnetic_x0020north = 0,
		grid_x0020north = 1,
		true_x0020north = 2
	};
	enum class witsml20__TypeSurveyTool {
		gyroscopic_x0020inertial = 0,
		gyroscopic_x0020MWD = 1,
		gyroscopic_x0020north_x0020seeking = 2,
		magnetic_x0020multiple_shot = 3,
		magnetic_x0020MWD = 4,
		magnetic_x0020single_shot = 5
	};
	enum class witsml20__TrajStnCalcAlgorithm {
		average_x0020angle = 0,
		balanced_x0020tangential = 1,
		constant_x0020tool_x0020face = 2,
		custom = 3,
		inertial = 4,
		minimum_x0020curvature = 5,
		radius_x0020of_x0020curvature = 6,
		tangential = 7
	};
	enum class witsml20__TrajStationStatus {
		open = 0,
		rejected = 1,
		position = 2
	};
	enum class witsml20__HoleCasingType {
		blow_x0020out_x0020preventer = 0,
		casing = 1,
		conductor = 2,
		curved_x0020conductor = 3,
		liner = 4,
		open_x0020hole = 5,
		riser = 6,
		tubing = 7
	};
	enum class witsml20__EtpDataType {
		boolean = 0,
		bytes = 1,
		double_ = 2,
		float_ = 3,
		int_ = 4,
		long_ = 5,
#if defined(SWIGJAVA)
		_null = 6,
#else
		null = 6,
#endif
		string = 7,
		vector = 8
	};
	enum class witsml20__ChannelIndexType {
		measured_x0020depth = 0,
		true_x0020vertical_x0020depth = 1,
		pass_x0020indexed_x0020depth = 2,
		date_x0020time = 3,
		elapsed_x0020time = 4,
		temperature = 5,
		pressure = 6
	};
	enum class eml21__UnitOfMeasure {
		A =
#ifdef SWIGPYTHON
(int)
#endif
			'A',
		a =
#ifdef SWIGPYTHON
(int)
#endif
			'a',
		b =
#ifdef SWIGPYTHON
(int)
#endif
			'b',
		B =
#ifdef SWIGPYTHON
(int)
#endif
			'B',
		C =
#ifdef SWIGPYTHON
(int)
#endif
			'C',
		D =
#ifdef SWIGPYTHON
(int)
#endif
			'D',
		d =
#ifdef SWIGPYTHON
(int)
#endif
			'd',
		F =
#ifdef SWIGPYTHON
(int)
#endif
			'F',
		g =
#ifdef SWIGPYTHON
(int)
#endif
			'g',
		H =
#ifdef SWIGPYTHON
(int)
#endif
			'H',
		h =
#ifdef SWIGPYTHON
(int)
#endif
			'h',
		J =
#ifdef SWIGPYTHON
(int)
#endif
			'J',
		K =
#ifdef SWIGPYTHON
(int)
#endif
			'K',
		L =
#ifdef SWIGPYTHON
(int)
#endif
			'L',
		m =
#ifdef SWIGPYTHON
(int)
#endif
			'm',
		N =
#ifdef SWIGPYTHON
(int)
#endif
			'N',
		O =
#ifdef SWIGPYTHON
(int)
#endif
			'O',
		P =
#ifdef SWIGPYTHON
(int)
#endif
			'P',
		S =
#ifdef SWIGPYTHON
(int)
#endif
			'S',
		s =
#ifdef SWIGPYTHON
(int)
#endif
			's',
		t =
#ifdef SWIGPYTHON
(int)
#endif
			't',
		T =
#ifdef SWIGPYTHON
(int)
#endif
			'T',
		V =
#ifdef SWIGPYTHON
(int)
#endif
			'V',
		W =
#ifdef SWIGPYTHON
(int)
#endif
			'W',
		_x0025 =
#ifdef SWIGPYTHON
(int)
#endif
			'u',
		_x0025_x005barea_x005d =
#ifdef SWIGPYTHON
(int)
#endif
			'v',
		_x0025_x005bmass_x005d =
#ifdef SWIGPYTHON
(int)
#endif
			'w',
		_x0025_x005bmolar_x005d =
#ifdef SWIGPYTHON
(int)
#endif
			'x',
		_x0025_x005bvol_x005d =
#ifdef SWIGPYTHON
(int)
#endif
			'y',
		_x0028bbl_x002fd_x0029_x002f_x0028bbl_x002fd_x0029 =
#ifdef SWIGPYTHON
(int)
#endif
			'z',
		_x0028m3_x002fd_x0029_x002f_x0028m3_x002fd_x0029 = 123,
		_x0028m3_x002fs_x0029_x002f_x0028m3_x002fs_x0029 = 124,
		_0_x002e001_x0020bbl_x002fft3 = 125,
		_0_x002e001_x0020bbl_x002fm3 = 126,
		_0_x002e001_x0020d_x002fft3 = 127,
		_0_x002e001_x0020gal_x005bUK_x005d_x002fbbl = 128,
		_0_x002e001_x0020gal_x005bUK_x005d_x002fgal_x005bUK_x005d = 129,
		_0_x002e001_x0020gal_x005bUS_x005d_x002fbbl = 130,
		_0_x002e001_x0020gal_x005bUS_x005d_x002fft3 = 131,
		_0_x002e001_x0020gal_x005bUS_x005d_x002fgal_x005bUS_x005d = 132,
		_0_x002e001_x0020h_x002fft = 133,
		_0_x002e001_x0020kPa2_x002fcP = 134,
		_0_x002e001_x0020lbm_x002fbbl = 135,
		_0_x002e001_x0020lbm_x002fgal_x005bUK_x005d = 136,
		_0_x002e001_x0020lbm_x002fgal_x005bUS_x005d = 137,
		_0_x002e001_x0020psi_x002fft = 138,
		_0_x002e001_x0020pt_x005bUK_x005d_x002fbbl = 139,
		_0_x002e001_x0020seca = 140,
		_0_x002e01_x0020bbl_x002fbbl = 141,
		_0_x002e01_x0020dega_x002fft = 142,
		_0_x002e01_x0020degF_x002fft = 143,
		_0_x002e01_x0020dm3_x002fkm = 144,
		_0_x002e01_x0020ft_x002fft = 145,
		_0_x002e01_x0020grain_x002fft3 = 146,
		_0_x002e01_x0020L_x002fkg = 147,
		_0_x002e01_x0020L_x002fkm = 148,
		_0_x002e01_x0020lbf_x002fft = 149,
		_0_x002e01_x0020lbf_x002fft2 = 150,
		_0_x002e01_x0020lbm_x002fft2 = 151,
		_0_x002e01_x0020psi_x002fft = 152,
		_0_x002e1_x0020ft = 153,
		_0_x002e1_x0020ft_x005bUS_x005d = 154,
		_0_x002e1_x0020gal_x005bUS_x005d_x002fbbl = 155,
		_0_x002e1_x0020in = 156,
		_0_x002e1_x0020L_x002fbbl = 157,
		_0_x002e1_x0020lbm_x002fbbl = 158,
		_0_x002e1_x0020pt_x005bUS_x005d_x002fbbl = 159,
		_0_x002e1_x0020yd = 160,
		_1_x002f_x0028kg_x002es_x0029 = 161,
		_1_x002f16_x0020in = 162,
		_1_x002f2_x0020ft = 163,
		_1_x002f2_x0020ms = 164,
		_1_x002f30_x0020cm3_x002fmin = 165,
		_1_x002f30_x0020dega_x002fft = 166,
		_1_x002f30_x0020dega_x002fm = 167,
		_1_x002f30_x0020lbf_x002fm = 168,
		_1_x002f30_x0020m_x002fm = 169,
		_1_x002f30_x0020N_x002fm = 170,
		_1_x002f32_x0020in = 171,
		_1_x002f64_x0020in = 172,
		_1_x002fa = 173,
		_1_x002fangstrom = 174,
		_1_x002fbar = 175,
		_1_x002fbbl = 176,
		_1_x002fcm = 177,
		_1_x002fd = 178,
		_1_x002fdegC = 179,
		_1_x002fdegF = 180,
		_1_x002fdegR = 181,
		_1_x002fft = 182,
		_1_x002fft2 = 183,
		_1_x002fft3 = 184,
		_1_x002fg = 185,
		_1_x002fgal_x005bUK_x005d = 186,
		_1_x002fgal_x005bUS_x005d = 187,
		_1_x002fh = 188,
		_1_x002fH = 189,
		_1_x002fin = 190,
		_1_x002fK = 191,
		_1_x002fkg = 192,
		_1_x002fkm2 = 193,
		_1_x002fkPa = 194,
		_1_x002fL = 195,
		_1_x002flbf = 196,
		_1_x002flbm = 197,
		_1_x002fm = 198,
		_1_x002fm2 = 199,
		_1_x002fm3 = 200,
		_1_x002fmi = 201,
		_1_x002fmi2 = 202,
		_1_x002fmin = 203,
		_1_x002fmm = 204,
		_1_x002fms = 205,
		_1_x002fN = 206,
		_1_x002fnm = 207,
		_1_x002fPa = 208,
		_1_x002fpPa = 209,
		_1_x002fpsi = 210,
		_1_x002fs = 211,
		_1_x002fupsi = 212,
		_1_x002fus = 213,
		_1_x002fuV = 214,
		_1_x002fV = 215,
		_1_x002fwk = 216,
		_1_x002fyd = 217,
		_10_x0020ft = 218,
		_10_x0020in = 219,
		_10_x0020km = 220,
		_10_x0020kN = 221,
		_10_x0020Mg_x002fm3 = 222,
		_100_x0020ft = 223,
		_100_x0020ka_x005bt_x005d = 224,
		_100_x0020km = 225,
		_1000_x0020bbl = 226,
		_1000_x0020bbl_x002eft_x002fd = 227,
		_1000_x0020bbl_x002fd = 228,
		_1000_x0020ft = 229,
		_1000_x0020ft_x002fh = 230,
		_1000_x0020ft_x002fs = 231,
		_1000_x0020ft3 = 232,
		_1000_x0020ft3_x002f_x0028d_x002eft_x0029 = 233,
		_1000_x0020ft3_x002f_x0028psi_x002ed_x0029 = 234,
		_1000_x0020ft3_x002fbbl = 235,
		_1000_x0020ft3_x002fd = 236,
		_1000_x0020gal_x005bUK_x005d = 237,
		_1000_x0020gal_x005bUS_x005d = 238,
		_1000_x0020lbf_x002eft = 239,
		_1000_x0020m3 = 240,
		_1000_x0020m3_x002f_x0028d_x002em_x0029 = 241,
		_1000_x0020m3_x002f_x0028h_x002em_x0029 = 242,
		_1000_x0020m3_x002fd = 243,
		_1000_x0020m3_x002fh = 244,
		_1000_x0020m3_x002fm3 = 245,
		_1000_x0020m4_x002fd = 246,
		_1E12_x0020ft3 = 247,
		_1E6_x0020_x0028ft3_x002fd_x0029_x002f_x0028bbl_x002fd_x0029 = 248,
		_1E_6_x0020acre_x002eft_x002fbbl = 249,
		_1E6_x0020bbl = 250,
		_1E6_x0020bbl_x002f_x0028acre_x002eft_x0029 = 251,
		_1E6_x0020bbl_x002facre = 252,
		_1E6_x0020bbl_x002fd = 253,
		_1E_6_x0020bbl_x002fft3 = 254,
		_1E_6_x0020bbl_x002fm3 = 255,
		_1E6_x0020Btu_x005bIT_x005d = 256,
		_1E6_x0020Btu_x005bIT_x005d_x002fh = 257,
		_1E6_x0020ft3 = 258,
		_1E6_x0020ft3_x002f_x0028acre_x002eft_x0029 = 259,
		_1E6_x0020ft3_x002fbbl = 260,
		_1E6_x0020ft3_x002fd = 261,
		_1E_6_x0020gal_x005bUS_x005d = 262,
		_1E6_x0020lbm_x002fa = 263,
		_1E6_x0020m3 = 264,
		_1E_6_x0020m3_x002f_x0028m3_x002edegC_x0029 = 265,
		_1E_6_x0020m3_x002f_x0028m3_x002edegF_x0029 = 266,
		_1E6_x0020m3_x002fd = 267,
		_1E_9_x00201_x002fft = 268,
		_1E9_x0020bbl = 269,
		_1E9_x0020ft3 = 270,
		_30_x0020ft = 271,
		_30_x0020m = 272,
		A_x002eh = 273,
		A_x002em2 = 274,
		A_x002es = 275,
		A_x002es_x002fkg = 276,
		A_x002es_x002fm3 = 277,
		A_x002fcm2 = 278,
		A_x002fft2 = 279,
		A_x002fm = 280,
		A_x002fm2 = 281,
		A_x002fmm = 282,
		A_x002fmm2 = 283,
		a_x005bt_x005d = 284,
		acre = 285,
		acre_x002eft = 286,
		ag = 287,
		aJ = 288,
		angstrom = 289,
		at = 290,
		atm = 291,
		atm_x002fft = 292,
		atm_x002fh = 293,
		atm_x002fhm = 294,
		atm_x002fm = 295,
		B_x002eW = 296,
		b_x002fcm3 = 297,
		B_x002fm = 298,
		B_x002fO = 299,
		bar = 300,
		bar_x002fh = 301,
		bar_x002fkm = 302,
		bar_x002fm = 303,
		bar2 = 304,
		bar2_x002fcP = 305,
		bbl = 306,
		bbl_x002f_x0028acre_x002eft_x0029 = 307,
		bbl_x002f_x0028d_x002eacre_x002eft_x0029 = 308,
		bbl_x002f_x0028d_x002eft_x0029 = 309,
		bbl_x002f_x0028ft_x002epsi_x002ed_x0029 = 310,
		bbl_x002f_x0028kPa_x002ed_x0029 = 311,
		bbl_x002f_x0028psi_x002ed_x0029 = 312,
		bbl_x002facre = 313,
		bbl_x002fbbl = 314,
		bbl_x002fd = 315,
		bbl_x002fd2 = 316,
		bbl_x002fft = 317,
		bbl_x002fft3 = 318,
		bbl_x002fh = 319,
		bbl_x002fh2 = 320,
		bbl_x002fin = 321,
		bbl_x002fm3 = 322,
		bbl_x002fmi = 323,
		bbl_x002fmin = 324,
		bbl_x002fpsi = 325,
		bbl_x002fton_x005bUK_x005d = 326,
		bbl_x002fton_x005bUS_x005d = 327,
		Bd = 328,
		bit = 329,
		bit_x002fs = 330,
		Bq = 331,
		Bq_x002fkg = 332,
		Btu_x005bIT_x005d = 333,
		Btu_x005bIT_x005d_x002ein_x002f_x0028h_x002eft2_x002edegF_x0029 = 334,
		Btu_x005bIT_x005d_x002f_x0028h_x002eft_x002edegF_x0029 = 335,
		Btu_x005bIT_x005d_x002f_x0028h_x002eft2_x0029 = 336,
		Btu_x005bIT_x005d_x002f_x0028h_x002eft2_x002edegF_x0029 = 337,
		Btu_x005bIT_x005d_x002f_x0028h_x002eft2_x002edegR_x0029 = 338,
		Btu_x005bIT_x005d_x002f_x0028h_x002eft3_x0029 = 339,
		Btu_x005bIT_x005d_x002f_x0028h_x002eft3_x002edegF_x0029 = 340,
		Btu_x005bIT_x005d_x002f_x0028h_x002em2_x002edegC_x0029 = 341,
		Btu_x005bIT_x005d_x002f_x0028hp_x002eh_x0029 = 342,
		Btu_x005bIT_x005d_x002f_x0028lbm_x002edegF_x0029 = 343,
		Btu_x005bIT_x005d_x002f_x0028lbm_x002edegR_x0029 = 344,
		Btu_x005bIT_x005d_x002f_x0028lbmol_x002edegF_x0029 = 345,
		Btu_x005bIT_x005d_x002f_x0028s_x002eft2_x0029 = 346,
		Btu_x005bIT_x005d_x002f_x0028s_x002eft2_x002edegF_x0029 = 347,
		Btu_x005bIT_x005d_x002f_x0028s_x002eft3_x0029 = 348,
		Btu_x005bIT_x005d_x002f_x0028s_x002eft3_x002edegF_x0029 = 349,
		Btu_x005bIT_x005d_x002fbbl = 350,
		Btu_x005bIT_x005d_x002fft3 = 351,
		Btu_x005bIT_x005d_x002fgal_x005bUK_x005d = 352,
		Btu_x005bIT_x005d_x002fgal_x005bUS_x005d = 353,
		Btu_x005bIT_x005d_x002fh = 354,
		Btu_x005bIT_x005d_x002flbm = 355,
		Btu_x005bIT_x005d_x002flbmol = 356,
		Btu_x005bIT_x005d_x002fmin = 357,
		Btu_x005bIT_x005d_x002fs = 358,
		Btu_x005bth_x005d = 359,
		Btu_x005bUK_x005d = 360,
		byte = 361,
		byte_x002fs = 362,
		C_x002em = 363,
		C_x002fcm2 = 364,
		C_x002fcm3 = 365,
		C_x002fg = 366,
		C_x002fkg = 367,
		C_x002fm2 = 368,
		C_x002fm3 = 369,
		C_x002fmm2 = 370,
		C_x002fmm3 = 371,
		ca = 372,
		cA = 373,
		cal_x005bIT_x005d = 374,
		cal_x005bth_x005d = 375,
		cal_x005bth_x005d_x002f_x0028g_x002eK_x0029 = 376,
		cal_x005bth_x005d_x002f_x0028h_x002ecm_x002edegC_x0029 = 377,
		cal_x005bth_x005d_x002f_x0028h_x002ecm2_x0029 = 378,
		cal_x005bth_x005d_x002f_x0028h_x002ecm2_x002edegC_x0029 = 379,
		cal_x005bth_x005d_x002f_x0028h_x002ecm3_x0029 = 380,
		cal_x005bth_x005d_x002f_x0028mol_x002edegC_x0029 = 381,
		cal_x005bth_x005d_x002f_x0028s_x002ecm_x002edegC_x0029 = 382,
		cal_x005bth_x005d_x002f_x0028s_x002ecm2_x002edegC_x0029 = 383,
		cal_x005bth_x005d_x002f_x0028s_x002ecm3_x0029 = 384,
		cal_x005bth_x005d_x002fcm3 = 385,
		cal_x005bth_x005d_x002fg = 386,
		cal_x005bth_x005d_x002fh = 387,
		cal_x005bth_x005d_x002fkg = 388,
		cal_x005bth_x005d_x002flbm = 389,
		cal_x005bth_x005d_x002fmL = 390,
		cal_x005bth_x005d_x002fmm3 = 391,
		cC = 392,
		ccal_x005bth_x005d = 393,
		ccgr = 394,
		cd = 395,
		cd_x002fm2 = 396,
		cEuc = 397,
		ceV = 398,
		cF = 399,
		cg = 400,
		cgauss = 401,
		cgr = 402,
		cGy = 403,
		cH = 404,
		chain = 405,
		chain_x005bBnA_x005d = 406,
		chain_x005bBnB_x005d = 407,
		chain_x005bCla_x005d = 408,
		chain_x005bInd37_x005d = 409,
		chain_x005bSe_x005d = 410,
		chain_x005bSeT_x005d = 411,
		chain_x005bUS_x005d = 412,
		cHz = 413,
		Ci = 414,
		cJ = 415,
		cm = 416,
		cm_x002fa = 417,
		cm_x002fs = 418,
		cm_x002fs2 = 419,
		cm2 = 420,
		cm2_x002fg = 421,
		cm2_x002fs = 422,
		cm3 = 423,
		cm3_x002fcm3 = 424,
		cm3_x002fg = 425,
		cm3_x002fh = 426,
		cm3_x002fL = 427,
		cm3_x002fm3 = 428,
		cm3_x002fmin = 429,
		cm3_x002fs = 430,
		cm4 = 431,
		cmH2O_x005b4degC_x005d = 432,
		cN = 433,
		cohm = 434,
		cP = 435,
		cPa = 436,
		crd = 437,
		cS = 438,
		cs = 439,
		cSt = 440,
		ct = 441,
		cT = 442,
		cu = 443,
		cV = 444,
		cW = 445,
		cWb = 446,
		cwt_x005bUK_x005d = 447,
		cwt_x005bUS_x005d = 448,
		D_x002eft = 449,
		D_x002em = 450,
		D_x002f_x0028Pa_x002es_x0029 = 451,
		d_x002fbbl = 452,
		D_x002fcP = 453,
		d_x002fft3 = 454,
		d_x002fm3 = 455,
		D_x005bAPI_x005d = 456,
		dA = 457,
		dam = 458,
		daN = 459,
		daN_x002em = 460,
		dAPI = 461,
		dB = 462,
		dB_x002emW = 463,
		dB_x002eMW = 464,
		dB_x002eW = 465,
		dB_x002fft = 466,
		dB_x002fkm = 467,
		dB_x002fm = 468,
		dB_x002fO = 469,
		dC = 470,
		dcal_x005bth_x005d = 471,
		dega = 472,
		dega_x002fft = 473,
		dega_x002fh = 474,
		dega_x002fm = 475,
		dega_x002fmin = 476,
		dega_x002fs = 477,
		degC = 478,
		degC_x002em2_x002eh_x002fkcal_x005bth_x005d = 479,
		degC_x002fft = 480,
		degC_x002fh = 481,
		degC_x002fhm = 482,
		degC_x002fkm = 483,
		degC_x002fkPa = 484,
		degC_x002fm = 485,
		degC_x002fmin = 486,
		degC_x002fs = 487,
		degF = 488,
		degF_x002eft2_x002eh_x002fBtu_x005bIT_x005d = 489,
		degF_x002fft = 490,
		degF_x002fh = 491,
		degF_x002fm = 492,
		degF_x002fmin = 493,
		degF_x002fpsi = 494,
		degF_x002fs = 495,
		degR = 496,
		dEuc = 497,
		deV = 498,
		dF = 499,
		dgauss = 500,
		dGy = 501,
		dH = 502,
		dHz = 503,
		dJ = 504,
		dm = 505,
		dm_x002fs = 506,
		dm3 = 507,
		dm3_x002f_x0028kW_x002eh_x0029 = 508,
		dm3_x002fkg = 509,
		dm3_x002fkmol = 510,
		dm3_x002fm = 511,
		dm3_x002fm3 = 512,
		dm3_x002fMJ = 513,
		dm3_x002fs = 514,
		dm3_x002fs2 = 515,
		dm3_x002ft = 516,
		dN = 517,
		dN_x002em = 518,
		dohm = 519,
		dP = 520,
		dPa = 521,
		drd = 522,
		ds = 523,
		dS = 524,
		dT = 525,
		dV = 526,
		dW = 527,
		dWb = 528,
		dyne = 529,
		dyne_x002ecm2 = 530,
		dyne_x002es_x002fcm2 = 531,
		dyne_x002fcm = 532,
		dyne_x002fcm2 = 533,
		EA = 534,
		Ea_x005bt_x005d = 535,
		EC = 536,
		Ecal_x005bth_x005d = 537,
		EEuc = 538,
		EeV = 539,
		EF = 540,
		Eg = 541,
		Egauss = 542,
		EGy = 543,
		EH = 544,
		EHz = 545,
		EJ = 546,
		EJ_x002fa = 547,
		Em = 548,
		EN = 549,
		Eohm = 550,
		EP = 551,
		EPa = 552,
		Erd = 553,
		erg = 554,
		erg_x002fa = 555,
		erg_x002fcm2 = 556,
		erg_x002fcm3 = 557,
		erg_x002fg = 558,
		erg_x002fkg = 559,
		erg_x002fm3 = 560,
		ES = 561,
		ET = 562,
		Euc = 563,
		eV = 564,
		EW = 565,
		EWb = 566,
		F_x002fm = 567,
		fa = 568,
		fA = 569,
		fathom = 570,
		fC = 571,
		fcal_x005bth_x005d = 572,
		fEuc = 573,
		feV = 574,
		fF = 575,
		fg = 576,
		fgauss = 577,
		fGy = 578,
		fH = 579,
		fHz = 580,
		fJ = 581,
		floz_x005bUK_x005d = 582,
		floz_x005bUS_x005d = 583,
		fm = 584,
		fN = 585,
		fohm = 586,
		footcandle = 587,
		footcandle_x002es = 588,
		fP = 589,
		fPa = 590,
		frd = 591,
		fS = 592,
		ft = 593,
		fT = 594,
		ft_x002fbbl = 595,
		ft_x002fd = 596,
		ft_x002fdegF = 597,
		ft_x002fft = 598,
		ft_x002fft3 = 599,
		ft_x002fgal_x005bUS_x005d = 600,
		ft_x002fh = 601,
		ft_x002fin = 602,
		ft_x002flbm = 603,
		ft_x002fm = 604,
		ft_x002fmi = 605,
		ft_x002fmin = 606,
		ft_x002fms = 607,
		ft_x002fpsi = 608,
		ft_x002fs = 609,
		ft_x002fs2 = 610,
		ft_x002fus = 611,
		ft_x005bBnA_x005d = 612,
		ft_x005bBnB_x005d = 613,
		ft_x005bBr36_x005d = 614,
		ft_x005bBr65_x005d = 615,
		ft_x005bCla_x005d = 616,
		ft_x005bGC_x005d = 617,
		ft_x005bInd_x005d = 618,
		ft_x005bInd37_x005d = 619,
		ft_x005bInd62_x005d = 620,
		ft_x005bInd75_x005d = 621,
		ft_x005bSe_x005d = 622,
		ft_x005bSeT_x005d = 623,
		ft_x005bUS_x005d = 624,
		ft2 = 625,
		ft2_x002fh = 626,
		ft2_x002fin3 = 627,
		ft2_x002flbm = 628,
		ft2_x002fs = 629,
		ft3 = 630,
		ft3_x002f_x0028d_x002eft_x0029 = 631,
		ft3_x002f_x0028ft_x002epsi_x002ed_x0029 = 632,
		ft3_x002f_x0028min_x002eft2_x0029 = 633,
		ft3_x002f_x0028s_x002eft2_x0029 = 634,
		ft3_x002fbbl = 635,
		ft3_x002fd = 636,
		ft3_x002fd2 = 637,
		ft3_x002fft = 638,
		ft3_x002fft2 = 639,
		ft3_x002fft3 = 640,
		ft3_x002fh = 641,
		ft3_x002fh2 = 642,
		ft3_x002fkg = 643,
		ft3_x002flbm = 644,
		ft3_x002flbmol = 645,
		ft3_x002fmin = 646,
		ft3_x002fmin2 = 647,
		ft3_x002frad = 648,
		ft3_x002fs = 649,
		ft3_x002fs2 = 650,
		ft3_x002fsack_x005b94lbm_x005d = 651,
		fur_x005bUS_x005d = 652,
		fV = 653,
		fW = 654,
		fWb = 655,
		g_x002eft_x002f_x0028cm3_x002es_x0029 = 656,
		g_x002em_x002f_x0028cm3_x002es_x0029 = 657,
		g_x002fcm3 = 658,
		g_x002fcm4 = 659,
		g_x002fdm3 = 660,
		g_x002fgal_x005bUK_x005d = 661,
		g_x002fgal_x005bUS_x005d = 662,
		g_x002fkg = 663,
		g_x002fL = 664,
		g_x002fm3 = 665,
		g_x002fmol = 666,
		g_x002fs = 667,
		g_x002ft = 668,
		GA = 669,
		Ga_x005bt_x005d = 670,
		Gal = 671,
		gal_x005bUK_x005d = 672,
		gal_x005bUK_x005d_x002f_x0028h_x002eft_x0029 = 673,
		gal_x005bUK_x005d_x002f_x0028h_x002eft2_x0029 = 674,
		gal_x005bUK_x005d_x002f_x0028h_x002ein_x0029 = 675,
		gal_x005bUK_x005d_x002f_x0028h_x002ein2_x0029 = 676,
		gal_x005bUK_x005d_x002f_x0028min_x002eft_x0029 = 677,
		gal_x005bUK_x005d_x002f_x0028min_x002eft2_x0029 = 678,
		gal_x005bUK_x005d_x002fd = 679,
		gal_x005bUK_x005d_x002fft3 = 680,
		gal_x005bUK_x005d_x002fh = 681,
		gal_x005bUK_x005d_x002fh2 = 682,
		gal_x005bUK_x005d_x002flbm = 683,
		gal_x005bUK_x005d_x002fmi = 684,
		gal_x005bUK_x005d_x002fmin = 685,
		gal_x005bUK_x005d_x002fmin2 = 686,
		gal_x005bUS_x005d = 687,
		gal_x005bUS_x005d_x002f_x0028h_x002eft_x0029 = 688,
		gal_x005bUS_x005d_x002f_x0028h_x002eft2_x0029 = 689,
		gal_x005bUS_x005d_x002f_x0028h_x002ein_x0029 = 690,
		gal_x005bUS_x005d_x002f_x0028h_x002ein2_x0029 = 691,
		gal_x005bUS_x005d_x002f_x0028min_x002eft_x0029 = 692,
		gal_x005bUS_x005d_x002f_x0028min_x002eft2_x0029 = 693,
		gal_x005bUS_x005d_x002fbbl = 694,
		gal_x005bUS_x005d_x002fd = 695,
		gal_x005bUS_x005d_x002fft = 696,
		gal_x005bUS_x005d_x002fft3 = 697,
		gal_x005bUS_x005d_x002fh = 698,
		gal_x005bUS_x005d_x002fh2 = 699,
		gal_x005bUS_x005d_x002flbm = 700,
		gal_x005bUS_x005d_x002fmi = 701,
		gal_x005bUS_x005d_x002fmin = 702,
		gal_x005bUS_x005d_x002fmin2 = 703,
		gal_x005bUS_x005d_x002fsack_x005b94lbm_x005d = 704,
		gal_x005bUS_x005d_x002fton_x005bUK_x005d = 705,
		gal_x005bUS_x005d_x002fton_x005bUS_x005d = 706,
		gAPI = 707,
		gauss = 708,
		gauss_x002fcm = 709,
		GBq = 710,
		GC = 711,
		Gcal_x005bth_x005d = 712,
		GEuc = 713,
		GeV = 714,
		gf = 715,
		GF = 716,
		Gg = 717,
		Ggauss = 718,
		GGy = 719,
		GH = 720,
		GHz = 721,
		GJ = 722,
		Gm = 723,
		GN = 724,
		gn = 725,
		Gohm = 726,
		gon = 727,
		GP = 728,
		GPa = 729,
		GPa_x002fcm = 730,
		GPa2 = 731,
		grain = 732,
		grain_x002fft3 = 733,
		grain_x002fgal_x005bUS_x005d = 734,
		Grd = 735,
		GS = 736,
		GT = 737,
		GV = 738,
		GW = 739,
		GW_x002eh = 740,
		GWb = 741,
		Gy = 742,
		h_x002fft3 = 743,
		h_x002fkm = 744,
		H_x002fm = 745,
		h_x002fm3 = 746,
		ha = 747,
		ha_x002em = 748,
		hbar = 749,
		hg = 750,
		hL = 751,
		hm = 752,
		hN = 753,
		hp = 754,
		hp_x002eh = 755,
		hp_x002eh_x002fbbl = 756,
		hp_x002eh_x002flbm = 757,
		hp_x002fft3 = 758,
		hp_x002fin2 = 759,
		hp_x005belec_x005d = 760,
		hp_x005bhyd_x005d = 761,
		hp_x005bhyd_x005d_x002fin2 = 762,
		hp_x005bmetric_x005d = 763,
		hp_x005bmetric_x005d_x002eh = 764,
		hs = 765,
		Hz = 766,
		in = 767,
		in_x002f_x0028in_x002edegF_x0029 = 768,
		in_x002fa = 769,
		in_x002fmin = 770,
		in_x002fs = 771,
		in_x002fs2 = 772,
		in_x005bUS_x005d = 773,
		in2 = 774,
		in2_x002fft2 = 775,
		in2_x002fin2 = 776,
		in2_x002fs = 777,
		in3 = 778,
		in3_x002fft = 779,
		in4 = 780,
		inH2O_x005b39degF_x005d = 781,
		inH2O_x005b60degF_x005d = 782,
		inHg_x005b32degF_x005d = 783,
		inHg_x005b60degF_x005d = 784,
		J_x002em_x002f_x0028s_x002em2_x002eK_x0029 = 785,
		J_x002em_x002fm2 = 786,
		J_x002f_x0028g_x002eK_x0029 = 787,
		J_x002f_x0028kg_x002eK_x0029 = 788,
		J_x002f_x0028mol_x002eK_x0029 = 789,
		J_x002f_x0028s_x002em2_x002edegC_x0029 = 790,
		J_x002fcm2 = 791,
		J_x002fdm3 = 792,
		J_x002fg = 793,
		J_x002fK = 794,
		J_x002fkg = 795,
		J_x002fm = 796,
		J_x002fm2 = 797,
		J_x002fm3 = 798,
		J_x002fmol = 799,
		J_x002fs = 800,
		K_x002em2_x002fkW = 801,
		K_x002em2_x002fW = 802,
		K_x002fkm = 803,
		K_x002fm = 804,
		K_x002fPa = 805,
		K_x002fs = 806,
		K_x002fW = 807,
		kA = 808,
		ka_x005bt_x005d = 809,
		kC = 810,
		kcal_x005bth_x005d = 811,
		kcal_x005bth_x005d_x002em_x002fcm2 = 812,
		kcal_x005bth_x005d_x002f_x0028h_x002em_x002edegC_x0029 = 813,
		kcal_x005bth_x005d_x002f_x0028h_x002em2_x002edegC_x0029 = 814,
		kcal_x005bth_x005d_x002f_x0028kg_x002edegC_x0029 = 815,
		kcal_x005bth_x005d_x002fcm3 = 816,
		kcal_x005bth_x005d_x002fg = 817,
		kcal_x005bth_x005d_x002fh = 818,
		kcal_x005bth_x005d_x002fkg = 819,
		kcal_x005bth_x005d_x002fm3 = 820,
		kcal_x005bth_x005d_x002fmol = 821,
		kcd = 822,
		kdyne = 823,
		kEuc = 824,
		keV = 825,
		kF = 826,
		kg = 827,
		kg_x002em = 828,
		kg_x002em_x002fcm2 = 829,
		kg_x002em_x002fs = 830,
		kg_x002em2 = 831,
		kg_x002f_x0028kW_x002eh_x0029 = 832,
		kg_x002f_x0028m_x002es_x0029 = 833,
		kg_x002f_x0028m2_x002es_x0029 = 834,
		kg_x002fd = 835,
		kg_x002fdm3 = 836,
		kg_x002fdm4 = 837,
		kg_x002fh = 838,
		kg_x002fJ = 839,
		kg_x002fkg = 840,
		kg_x002fL = 841,
		kg_x002fm = 842,
		kg_x002fm2 = 843,
		kg_x002fm3 = 844,
		kg_x002fm4 = 845,
		kg_x002fmin = 846,
		kg_x002fMJ = 847,
		kg_x002fmol = 848,
		kg_x002fs = 849,
		kg_x002fsack_x005b94lbm_x005d = 850,
		kg_x002ft = 851,
		kgauss = 852,
		kgf = 853,
		kgf_x002em = 854,
		kgf_x002em_x002fcm2 = 855,
		kgf_x002em_x002fm = 856,
		kgf_x002em2 = 857,
		kgf_x002es_x002fm2 = 858,
		kgf_x002fcm = 859,
		kgf_x002fcm2 = 860,
		kgf_x002fkgf = 861,
		kgf_x002fm2 = 862,
		kgf_x002fmm2 = 863,
		kGy = 864,
		kH = 865,
		kHz = 866,
		Kibyte = 867,
		kJ = 868,
		kJ_x002em_x002f_x0028h_x002em2_x002eK_x0029 = 869,
		kJ_x002f_x0028h_x002em2_x002eK_x0029 = 870,
		kJ_x002f_x0028kg_x002eK_x0029 = 871,
		kJ_x002f_x0028kmol_x002eK_x0029 = 872,
		kJ_x002fdm3 = 873,
		kJ_x002fkg = 874,
		kJ_x002fkmol = 875,
		kJ_x002fm3 = 876,
		klbf = 877,
		klbm = 878,
		klbm_x002fin = 879,
		klx = 880,
		km = 881,
		km_x002fcm = 882,
		km_x002fdm3 = 883,
		km_x002fh = 884,
		km_x002fL = 885,
		km_x002fs = 886,
		km2 = 887,
		km3 = 888,
		kmol = 889,
		kmol_x002fh = 890,
		kmol_x002fm3 = 891,
		kmol_x002fs = 892,
		kN = 893,
		kN_x002em = 894,
		kN_x002em2 = 895,
		kN_x002fm = 896,
		kN_x002fm2 = 897,
		knot = 898,
		kohm = 899,
		kohm_x002em = 900,
		kP = 901,
		kPa = 902,
		kPa_x002es_x002fm = 903,
		kPa_x002fh = 904,
		kPa_x002fhm = 905,
		kPa_x002fm = 906,
		kPa_x002fmin = 907,
		kPa2 = 908,
		kPa2_x002fcP = 909,
		kpsi = 910,
		kpsi2 = 911,
		krad = 912,
		krd = 913,
		kS = 914,
		kS_x002fm = 915,
		kT = 916,
		kV = 917,
		kW = 918,
		kW_x002eh = 919,
		kW_x002eh_x002f_x0028kg_x002edegC_x0029 = 920,
		kW_x002eh_x002fdm3 = 921,
		kW_x002eh_x002fkg = 922,
		kW_x002eh_x002fm3 = 923,
		kW_x002f_x0028m2_x002eK_x0029 = 924,
		kW_x002f_x0028m3_x002eK_x0029 = 925,
		kW_x002fcm2 = 926,
		kW_x002fm2 = 927,
		kW_x002fm3 = 928,
		kWb = 929,
		L_x002f_x0028bar_x002emin_x0029 = 930,
		L_x002fh = 931,
		L_x002fkg = 932,
		L_x002fkmol = 933,
		L_x002fm = 934,
		L_x002fm3 = 935,
		L_x002fmin = 936,
		L_x002fmol = 937,
		L_x002fs = 938,
		L_x002fs2 = 939,
		L_x002ft = 940,
		L_x002fton_x005bUK_x005d = 941,
		lbf = 942,
		lbf_x002eft = 943,
		lbf_x002eft_x002fbbl = 944,
		lbf_x002eft_x002fgal_x005bUS_x005d = 945,
		lbf_x002eft_x002fin = 946,
		lbf_x002eft_x002fin2 = 947,
		lbf_x002eft_x002flbm = 948,
		lbf_x002eft_x002fmin = 949,
		lbf_x002eft_x002fs = 950,
		lbf_x002ein = 951,
		lbf_x002ein_x002fin = 952,
		lbf_x002ein2 = 953,
		lbf_x002es_x002fft2 = 954,
		lbf_x002es_x002fin2 = 955,
		lbf_x002fft = 956,
		lbf_x002fft2 = 957,
		lbf_x002fft3 = 958,
		lbf_x002fgal_x005bUS_x005d = 959,
		lbf_x002fin = 960,
		lbf_x002flbf = 961,
		lbm = 962,
		lbm_x002eft = 963,
		lbm_x002eft_x002fs = 964,
		lbm_x002eft2 = 965,
		lbm_x002eft2_x002fs2 = 966,
		lbm_x002f_x0028ft_x002eh_x0029 = 967,
		lbm_x002f_x0028ft_x002es_x0029 = 968,
		lbm_x002f_x0028ft2_x002eh_x0029 = 969,
		lbm_x002f_x0028ft2_x002es_x0029 = 970,
		lbm_x002f_x0028gal_x005bUK_x005d_x002eft_x0029 = 971,
		lbm_x002f_x0028gal_x005bUS_x005d_x002eft_x0029 = 972,
		lbm_x002f_x0028hp_x002eh_x0029 = 973,
		lbm_x002fbbl = 974,
		lbm_x002fd = 975,
		lbm_x002fft = 976,
		lbm_x002fft2 = 977,
		lbm_x002fft3 = 978,
		lbm_x002fft4 = 979,
		lbm_x002fgal_x005bUK_x005d = 980,
		lbm_x002fgal_x005bUS_x005d = 981,
		lbm_x002fh = 982,
		lbm_x002fin3 = 983,
		lbm_x002flbmol = 984,
		lbm_x002fmin = 985,
		lbm_x002fs = 986,
		lbmol = 987,
		lbmol_x002f_x0028h_x002eft2_x0029 = 988,
		lbmol_x002f_x0028s_x002eft2_x0029 = 989,
		lbmol_x002fft3 = 990,
		lbmol_x002fgal_x005bUK_x005d = 991,
		lbmol_x002fgal_x005bUS_x005d = 992,
		lbmol_x002fh = 993,
		lbmol_x002fs = 994,
		link = 995,
		link_x005bBnA_x005d = 996,
		link_x005bBnB_x005d = 997,
		link_x005bCla_x005d = 998,
		link_x005bSe_x005d = 999,
		link_x005bSeT_x005d = 1000,
		link_x005bUS_x005d = 1001,
		lm = 1002,
		lm_x002es = 1003,
		lm_x002fm2 = 1004,
		lm_x002fW = 1005,
		lx = 1006,
		lx_x002es = 1007,
		m_x002f_x0028m_x002eK_x0029 = 1008,
		m_x002fcm = 1009,
		m_x002fd = 1010,
		m_x002fh = 1011,
		m_x002fK = 1012,
		m_x002fkg = 1013,
		m_x002fkm = 1014,
		m_x002fkPa = 1015,
		m_x002fm = 1016,
		m_x002fm3 = 1017,
		m_x002fmin = 1018,
		m_x002fms = 1019,
		m_x002fPa = 1020,
		m_x002fs = 1021,
		m_x002fs2 = 1022,
		m_x005bGer_x005d = 1023,
		m2 = 1024,
		m2_x002f_x0028kPa_x002ed_x0029 = 1025,
		m2_x002f_x0028Pa_x002es_x0029 = 1026,
		m2_x002fcm3 = 1027,
		m2_x002fd = 1028,
		m2_x002fg = 1029,
		m2_x002fh = 1030,
		m2_x002fkg = 1031,
		m2_x002fm2 = 1032,
		m2_x002fm3 = 1033,
		m2_x002fmol = 1034,
		m2_x002fs = 1035,
		m3 = 1036,
		m3_x002f_x0028bar_x002ed_x0029 = 1037,
		m3_x002f_x0028bar_x002eh_x0029 = 1038,
		m3_x002f_x0028bar_x002emin_x0029 = 1039,
		m3_x002f_x0028d_x002em_x0029 = 1040,
		m3_x002f_x0028h_x002em_x0029 = 1041,
		m3_x002f_x0028ha_x002em_x0029 = 1042,
		m3_x002f_x0028kPa_x002ed_x0029 = 1043,
		m3_x002f_x0028kPa_x002eh_x0029 = 1044,
		m3_x002f_x0028kW_x002eh_x0029 = 1045,
		m3_x002f_x0028m3_x002eK_x0029 = 1046,
		m3_x002f_x0028Pa_x002es_x0029 = 1047,
		m3_x002f_x0028psi_x002ed_x0029 = 1048,
		m3_x002f_x0028s_x002eft_x0029 = 1049,
		m3_x002f_x0028s_x002em_x0029 = 1050,
		m3_x002f_x0028s_x002em2_x0029 = 1051,
		m3_x002f_x0028s_x002em3_x0029 = 1052,
		m3_x002fbbl = 1053,
		m3_x002fd = 1054,
		m3_x002fd2 = 1055,
		m3_x002fg = 1056,
		m3_x002fh = 1057,
		m3_x002fJ = 1058,
		m3_x002fkg = 1059,
		m3_x002fkm = 1060,
		m3_x002fkmol = 1061,
		m3_x002fkPa = 1062,
		m3_x002fm = 1063,
		m3_x002fm2 = 1064,
		m3_x002fm3 = 1065,
		m3_x002fmin = 1066,
		m3_x002fmol = 1067,
		m3_x002fPa = 1068,
		m3_x002frad = 1069,
		m3_x002frev = 1070,
		m3_x002fs = 1071,
		m3_x002fs2 = 1072,
		m3_x002ft = 1073,
		m3_x002fton_x005bUK_x005d = 1074,
		m3_x002fton_x005bUS_x005d = 1075,
		m4 = 1076,
		m4_x002fs = 1077,
		mA = 1078,
		MA = 1079,
		mA_x002fcm2 = 1080,
		mA_x002fft2 = 1081,
		Ma_x005bt_x005d = 1082,
		mbar = 1083,
		MBq = 1084,
		mC = 1085,
		MC = 1086,
		mC_x002fm2 = 1087,
		mcal_x005bth_x005d = 1088,
		Mcal_x005bth_x005d = 1089,
		mCi = 1090,
		mD = 1091,
		mD_x002eft = 1092,
		mD_x002eft2_x002f_x0028lbf_x002es_x0029 = 1093,
		mD_x002ein2_x002f_x0028lbf_x002es_x0029 = 1094,
		mD_x002em = 1095,
		mD_x002f_x0028Pa_x002es_x0029 = 1096,
		mD_x002fcP = 1097,
		MEuc = 1098,
		mEuc = 1099,
		meV = 1100,
		MeV = 1101,
		MF = 1102,
		mF = 1103,
		mg = 1104,
		Mg = 1105,
		Mg_x002fa = 1106,
		Mg_x002fd = 1107,
		mg_x002fdm3 = 1108,
		mg_x002fg = 1109,
		mg_x002fgal_x005bUS_x005d = 1110,
		Mg_x002fh = 1111,
		Mg_x002fin = 1112,
		mg_x002fJ = 1113,
		mg_x002fkg = 1114,
		mg_x002fL = 1115,
		Mg_x002fm2 = 1116,
		mg_x002fm3 = 1117,
		Mg_x002fm3 = 1118,
		Mg_x002fmin = 1119,
		mGal = 1120,
		mgauss = 1121,
		Mgauss = 1122,
		Mgf = 1123,
		mgn = 1124,
		MGy = 1125,
		mGy = 1126,
		mH = 1127,
		MH = 1128,
		mHz = 1129,
		MHz = 1130,
		mi = 1131,
		mi_x002fgal_x005bUK_x005d = 1132,
		mi_x002fgal_x005bUS_x005d = 1133,
		mi_x002fh = 1134,
		mi_x002fin = 1135,
		mi_x005bnaut_x005d = 1136,
		mi_x005bnautUK_x005d = 1137,
		mi_x005bUS_x005d = 1138,
		mi_x005bUS_x005d2 = 1139,
		mi2 = 1140,
		mi3 = 1141,
		Mibyte = 1142,
		mil = 1143,
		mil_x002fa = 1144,
		mila = 1145,
		min_ = 1146,
		min_x002fft = 1147,
		min_x002fm = 1148,
		mina = 1149,
		mJ = 1150,
		MJ = 1151,
		MJ_x002fa = 1152,
		mJ_x002fcm2 = 1153,
		MJ_x002fkg = 1154,
		MJ_x002fkmol = 1155,
		MJ_x002fm = 1156,
		mJ_x002fm2 = 1157,
		MJ_x002fm3 = 1158,
		mL = 1159,
		mL_x002fgal_x005bUK_x005d = 1160,
		mL_x002fgal_x005bUS_x005d = 1161,
		mL_x002fmL = 1162,
		mm = 1163,
		Mm = 1164,
		mm_x002f_x0028mm_x002eK_x0029 = 1165,
		mm_x002fa = 1166,
		mm_x002fs = 1167,
		mm2 = 1168,
		mm2_x002fmm2 = 1169,
		mm2_x002fs = 1170,
		mm3 = 1171,
		mm3_x002fJ = 1172,
		mmHg_x005b0degC_x005d = 1173,
		mmol = 1174,
		MN = 1175,
		mN = 1176,
		mN_x002em2 = 1177,
		mN_x002fkm = 1178,
		mN_x002fm = 1179,
		Mohm = 1180,
		mohm = 1181,
		mol = 1182,
		mol_x002em2_x002f_x0028mol_x002es_x0029 = 1183,
		mol_x002f_x0028s_x002em2_x0029 = 1184,
		mol_x002fm2 = 1185,
		mol_x002fm3 = 1186,
		mol_x002fmol = 1187,
		mol_x002fs = 1188,
		MP = 1189,
		mP = 1190,
		mPa = 1191,
		MPa = 1192,
		mPa_x002es = 1193,
		MPa_x002es_x002fm = 1194,
		MPa_x002fh = 1195,
		MPa_x002fm = 1196,
		Mpsi = 1197,
		Mrad = 1198,
		mrad = 1199,
		mrd = 1200,
		Mrd = 1201,
		mrem = 1202,
		mrem_x002fh = 1203,
		ms = 1204,
		MS = 1205,
		mS = 1206,
		mS_x002fcm = 1207,
		ms_x002fcm = 1208,
		ms_x002fft = 1209,
		ms_x002fin = 1210,
		mS_x002fm = 1211,
		ms_x002fm = 1212,
		ms_x002fs = 1213,
		mSv = 1214,
		mSv_x002fh = 1215,
		mT = 1216,
		mT_x002fdm = 1217,
		MV = 1218,
		mV = 1219,
		mV_x002fft = 1220,
		mV_x002fm = 1221,
		mW = 1222,
		MW = 1223,
		MW_x002eh = 1224,
		MW_x002eh_x002fkg = 1225,
		MW_x002eh_x002fm3 = 1226,
		mW_x002fm2 = 1227,
		MWb = 1228,
		mWb = 1229,
		N_x002em = 1230,
		N_x002em_x002fm = 1231,
		N_x002em2 = 1232,
		N_x002es_x002fm2 = 1233,
		N_x002fm = 1234,
		N_x002fm2 = 1235,
		N_x002fm3 = 1236,
		N_x002fmm2 = 1237,
		N_x002fN = 1238,
		na = 1239,
		nA = 1240,
		nAPI = 1241,
		nC = 1242,
		ncal_x005bth_x005d = 1243,
		nCi = 1244,
		nEuc = 1245,
		neV = 1246,
		nF = 1247,
		ng = 1248,
		ng_x002fg = 1249,
		ng_x002fmg = 1250,
		ngauss = 1251,
		nGy = 1252,
		nH = 1253,
		nHz = 1254,
		nJ = 1255,
		nm = 1256,
		nm_x002fs = 1257,
		nN = 1258,
		nohm = 1259,
		nohm_x002emil2_x002fft = 1260,
		nohm_x002emm2_x002fm = 1261,
		nP = 1262,
		nPa = 1263,
		nrd = 1264,
		ns = 1265,
		nS = 1266,
		ns_x002fft = 1267,
		ns_x002fm = 1268,
		nT = 1269,
		nV = 1270,
		nW = 1271,
		nWb = 1272,
		Oe = 1273,
		ohm = 1274,
		ohm_x002ecm = 1275,
		ohm_x002em = 1276,
		ohm_x002em2_x002fm = 1277,
		ohm_x002fm = 1278,
		ozf = 1279,
		ozm = 1280,
		ozm_x005btroy_x005d = 1281,
		Pa = 1282,
		pA = 1283,
		Pa_x002es = 1284,
		Pa_x002es_x002em3_x002fkg = 1285,
		Pa_x002es_x002fm3 = 1286,
		Pa_x002es2_x002fm3 = 1287,
		Pa_x002fh = 1288,
		Pa_x002fm = 1289,
		Pa_x002fm3 = 1290,
		Pa_x002fs = 1291,
		Pa2 = 1292,
		Pa2_x002f_x0028Pa_x002es_x0029 = 1293,
		pC = 1294,
		pcal_x005bth_x005d = 1295,
		pCi = 1296,
		pCi_x002fg = 1297,
		pdl = 1298,
		pdl_x002ecm2 = 1299,
		pdl_x002eft = 1300,
		pdl_x002fcm = 1301,
		pEuc = 1302,
		peV = 1303,
		pF = 1304,
		pg = 1305,
		pgauss = 1306,
		pGy = 1307,
		pHz = 1308,
		pJ = 1309,
		pm = 1310,
		pN = 1311,
		pohm = 1312,
		pP = 1313,
		pPa = 1314,
		ppk = 1315,
		ppm = 1316,
		ppm_x005bmass_x005d = 1317,
		ppm_x005bvol_x005d = 1318,
		ppm_x005bvol_x005d_x002fdegC = 1319,
		ppm_x005bvol_x005d_x002fdegF = 1320,
		prd = 1321,
		pS = 1322,
		ps = 1323,
		psi = 1324,
		psi_x002ed_x002fbbl = 1325,
		psi_x002es = 1326,
		psi_x002fft = 1327,
		psi_x002fh = 1328,
		psi_x002fm = 1329,
		psi_x002fmin = 1330,
		psi2 = 1331,
		psi2_x002ed_x002f_x0028cP_x002eft3_x0029 = 1332,
		psi2_x002fcP = 1333,
		pT = 1334,
		pt_x005bUK_x005d = 1335,
		pt_x005bUK_x005d_x002f_x0028hp_x002eh_x0029 = 1336,
		pt_x005bUS_x005d = 1337,
		pV = 1338,
		pW = 1339,
		pWb = 1340,
		qt_x005bUK_x005d = 1341,
		qt_x005bUS_x005d = 1342,
		quad = 1343,
		quad_x002fa = 1344,
		rad = 1345,
		rad_x002fft = 1346,
		rad_x002fft3 = 1347,
		rad_x002fm = 1348,
		rad_x002fm3 = 1349,
		rad_x002fs = 1350,
		rad_x002fs2 = 1351,
		rd = 1352,
		rem = 1353,
		rem_x002fh = 1354,
		rev = 1355,
		rev_x002fft = 1356,
		rev_x002fm = 1357,
		rev_x002fs = 1358,
		rod_x005bUS_x005d = 1359,
		rpm = 1360,
		rpm_x002fs = 1361,
		s_x002fcm = 1362,
		s_x002fft = 1363,
		s_x002fft3 = 1364,
		s_x002fin = 1365,
		s_x002fkg = 1366,
		s_x002fL = 1367,
		s_x002fm = 1368,
		S_x002fm = 1369,
		s_x002fm3 = 1370,
		s_x002fqt_x005bUK_x005d = 1371,
		s_x002fqt_x005bUS_x005d = 1372,
		s_x002fs = 1373,
		sack_x005b94lbm_x005d = 1374,
		seca = 1375,
		section = 1376,
		sr = 1377,
		St = 1378,
		Sv = 1379,
		Sv_x002fh = 1380,
		Sv_x002fs = 1381,
		t_x002fa = 1382,
		t_x002fd = 1383,
		t_x002fh = 1384,
		T_x002fm = 1385,
		t_x002fm3 = 1386,
		t_x002fmin = 1387,
		TA = 1388,
		Ta_x005bt_x005d = 1389,
		TBq = 1390,
		TC = 1391,
		Tcal_x005bth_x005d = 1392,
		TD_x005bAPI_x005d = 1393,
		TD_x005bAPI_x005d_x002em = 1394,
		TD_x005bAPI_x005d_x002f_x0028Pa_x002es_x0029 = 1395,
		TEuc = 1396,
		TeV = 1397,
		TF = 1398,
		Tg = 1399,
		Tgauss = 1400,
		TGy = 1401,
		TH = 1402,
		therm_x005bEC_x005d = 1403,
		therm_x005bUK_x005d = 1404,
		therm_x005bUS_x005d = 1405,
		THz = 1406,
		TJ = 1407,
		TJ_x002fa = 1408,
		Tm = 1409,
		TN = 1410,
		Tohm = 1411,
		ton_x005bUK_x005d = 1412,
		ton_x005bUK_x005d_x002fa = 1413,
		ton_x005bUK_x005d_x002fd = 1414,
		ton_x005bUK_x005d_x002fh = 1415,
		ton_x005bUK_x005d_x002fmin = 1416,
		ton_x005bUS_x005d = 1417,
		ton_x005bUS_x005d_x002fa = 1418,
		ton_x005bUS_x005d_x002fd = 1419,
		ton_x005bUS_x005d_x002fft2 = 1420,
		ton_x005bUS_x005d_x002fh = 1421,
		ton_x005bUS_x005d_x002fmin = 1422,
		tonf_x005bUK_x005d = 1423,
		tonf_x005bUK_x005d_x002eft2 = 1424,
		tonf_x005bUK_x005d_x002fft = 1425,
		tonf_x005bUK_x005d_x002fft2 = 1426,
		tonf_x005bUS_x005d = 1427,
		tonf_x005bUS_x005d_x002eft = 1428,
		tonf_x005bUS_x005d_x002eft2 = 1429,
		tonf_x005bUS_x005d_x002emi = 1430,
		tonf_x005bUS_x005d_x002emi_x002fbbl = 1431,
		tonf_x005bUS_x005d_x002emi_x002fft = 1432,
		tonf_x005bUS_x005d_x002fft = 1433,
		tonf_x005bUS_x005d_x002fft2 = 1434,
		tonf_x005bUS_x005d_x002fin2 = 1435,
		tonRefrig = 1436,
		torr = 1437,
		TP = 1438,
		TPa = 1439,
		Trd = 1440,
		TS = 1441,
		TT = 1442,
		TV = 1443,
		TW = 1444,
		TW_x002eh = 1445,
		TWb = 1446,
		uA = 1447,
		uA_x002fcm2 = 1448,
		uA_x002fin2 = 1449,
		ubar = 1450,
		uC = 1451,
		ucal_x005bth_x005d = 1452,
		ucal_x005bth_x005d_x002f_x0028s_x002ecm2_x0029 = 1453,
		ucal_x005bth_x005d_x002fs = 1454,
		uCi = 1455,
		uEuc = 1456,
		ueV = 1457,
		uF = 1458,
		uF_x002fm = 1459,
		ug = 1460,
		ug_x002fcm3 = 1461,
		ug_x002fg = 1462,
		ug_x002fmg = 1463,
		ugauss = 1464,
		uGy = 1465,
		uH = 1466,
		uH_x002fm = 1467,
		uHz = 1468,
		uJ = 1469,
		um = 1470,
		um_x002fs = 1471,
		um2 = 1472,
		um2_x002em = 1473,
		umHg_x005b0degC_x005d = 1474,
		umol = 1475,
		uN = 1476,
		uohm = 1477,
		uohm_x002fft = 1478,
		uohm_x002fm = 1479,
		uP = 1480,
		uPa = 1481,
		upsi = 1482,
		urad = 1483,
		urd = 1484,
		us = 1485,
		uS = 1486,
		us_x002fft = 1487,
		us_x002fin = 1488,
		us_x002fm = 1489,
		uT = 1490,
		uV = 1491,
		uV_x002fft = 1492,
		uV_x002fm = 1493,
		uW = 1494,
		uW_x002fm3 = 1495,
		uWb = 1496,
		V_x002fB = 1497,
		V_x002fdB = 1498,
		V_x002fm = 1499,
		W_x002em2_x002eK_x002f_x0028J_x002eK_x0029 = 1500,
		W_x002f_x0028m_x002eK_x0029 = 1501,
		W_x002f_x0028m2_x002eK_x0029 = 1502,
		W_x002f_x0028m2_x002esr_x0029 = 1503,
		W_x002f_x0028m3_x002eK_x0029 = 1504,
		W_x002fcm2 = 1505,
		W_x002fK = 1506,
		W_x002fkW = 1507,
		W_x002fm2 = 1508,
		W_x002fm3 = 1509,
		W_x002fmm2 = 1510,
		W_x002fsr = 1511,
		W_x002fW = 1512,
		Wb = 1513,
		Wb_x002em = 1514,
		Wb_x002fm = 1515,
		Wb_x002fmm = 1516,
		wk = 1517,
		yd = 1518,
		yd_x005bBnA_x005d = 1519,
		yd_x005bBnB_x005d = 1520,
		yd_x005bCla_x005d = 1521,
		yd_x005bInd_x005d = 1522,
		yd_x005bInd37_x005d = 1523,
		yd_x005bInd62_x005d = 1524,
		yd_x005bInd75_x005d = 1525,
		yd_x005bSe_x005d = 1526,
		yd_x005bSeT_x005d = 1527,
		yd_x005bUS_x005d = 1528,
		yd2 = 1529,
		yd3 = 1530
	};
	enum class eml21__AnglePerLengthUom {
		_0_x002e01_x0020dega_x002fft = 0,
		_1_x002f30_x0020dega_x002fft = 1,
		_1_x002f30_x0020dega_x002fm = 2,
		dega_x002fft = 3,
		dega_x002fm = 4,
		rad_x002fft = 5,
		rad_x002fm = 6,
		rev_x002fft = 7,
		rev_x002fm = 8
	};
	enum class eml21__LengthUom {
		m =
#ifdef SWIGPYTHON
(int)
#endif
			'm',
		_0_x002e1_x0020ft =
#ifdef SWIGPYTHON
(int)
#endif
			'n',
		_0_x002e1_x0020ft_x005bUS_x005d =
#ifdef SWIGPYTHON
(int)
#endif
			'o',
		_0_x002e1_x0020in =
#ifdef SWIGPYTHON
(int)
#endif
			'p',
		_0_x002e1_x0020yd =
#ifdef SWIGPYTHON
(int)
#endif
			'q',
		_1_x002f16_x0020in =
#ifdef SWIGPYTHON
(int)
#endif
			'r',
		_1_x002f2_x0020ft =
#ifdef SWIGPYTHON
(int)
#endif
			's',
		_1_x002f32_x0020in =
#ifdef SWIGPYTHON
(int)
#endif
			't',
		_1_x002f64_x0020in =
#ifdef SWIGPYTHON
(int)
#endif
			'u',
		_10_x0020ft =
#ifdef SWIGPYTHON
(int)
#endif
			'v',
		_10_x0020in =
#ifdef SWIGPYTHON
(int)
#endif
			'w',
		_10_x0020km =
#ifdef SWIGPYTHON
(int)
#endif
			'x',
		_100_x0020ft =
#ifdef SWIGPYTHON
(int)
#endif
			'y',
		_100_x0020km =
#ifdef SWIGPYTHON
(int)
#endif
			'z',
		_1000_x0020ft = 123,
		_30_x0020ft = 124,
		_30_x0020m = 125,
		angstrom = 126,
		chain = 127,
		chain_x005bBnA_x005d = 128,
		chain_x005bBnB_x005d = 129,
		chain_x005bCla_x005d = 130,
		chain_x005bInd37_x005d = 131,
		chain_x005bSe_x005d = 132,
		chain_x005bSeT_x005d = 133,
		chain_x005bUS_x005d = 134,
		cm = 135,
		dam = 136,
		dm = 137,
		Em = 138,
		fathom = 139,
		fm = 140,
		ft = 141,
		ft_x005bBnA_x005d = 142,
		ft_x005bBnB_x005d = 143,
		ft_x005bBr36_x005d = 144,
		ft_x005bBr65_x005d = 145,
		ft_x005bCla_x005d = 146,
		ft_x005bGC_x005d = 147,
		ft_x005bInd_x005d = 148,
		ft_x005bInd37_x005d = 149,
		ft_x005bInd62_x005d = 150,
		ft_x005bInd75_x005d = 151,
		ft_x005bSe_x005d = 152,
		ft_x005bSeT_x005d = 153,
		ft_x005bUS_x005d = 154,
		fur_x005bUS_x005d = 155,
		Gm = 156,
		hm = 157,
		in = 158,
		in_x005bUS_x005d = 159,
		km = 160,
		link = 161,
		link_x005bBnA_x005d = 162,
		link_x005bBnB_x005d = 163,
		link_x005bCla_x005d = 164,
		link_x005bSe_x005d = 165,
		link_x005bSeT_x005d = 166,
		link_x005bUS_x005d = 167,
		m_x005bGer_x005d = 168,
		mi = 169,
		mi_x005bnaut_x005d = 170,
		mi_x005bnautUK_x005d = 171,
		mi_x005bUS_x005d = 172,
		mil = 173,
		mm = 174,
		Mm = 175,
		nm = 176,
		pm = 177,
		rod_x005bUS_x005d = 178,
		Tm = 179,
		um = 180,
		yd = 181,
		yd_x005bBnA_x005d = 182,
		yd_x005bBnB_x005d = 183,
		yd_x005bCla_x005d = 184,
		yd_x005bInd_x005d = 185,
		yd_x005bInd37_x005d = 186,
		yd_x005bInd62_x005d = 187,
		yd_x005bInd75_x005d = 188,
		yd_x005bSe_x005d = 189,
		yd_x005bSeT_x005d = 190,
		yd_x005bUS_x005d = 191
	};
	enum class eml21__LinearAccelerationUom {
		cm_x002fs2 = 0,
		ft_x002fs2 = 1,
		Gal = 2,
		gn = 3,
		in_x002fs2 = 4,
		m_x002fs2 = 5,
		mGal = 6,
		mgn = 7
	};
	enum class eml21__MagneticFluxDensityUom {
		T =
#ifdef SWIGPYTHON
(int)
#endif
			'T',
		cgauss =
#ifdef SWIGPYTHON
(int)
#endif
			'U',
		cT =
#ifdef SWIGPYTHON
(int)
#endif
			'V',
		dgauss =
#ifdef SWIGPYTHON
(int)
#endif
			'W',
		dT =
#ifdef SWIGPYTHON
(int)
#endif
			'X',
		Egauss =
#ifdef SWIGPYTHON
(int)
#endif
			'Y',
		ET =
#ifdef SWIGPYTHON
(int)
#endif
			'Z',
		fgauss = 91,
		fT = 92,
		gauss = 93,
		Ggauss = 94,
		GT = 95,
		kgauss = 96,
		kT =
#ifdef SWIGPYTHON
(int)
#endif
			'a',
		mgauss =
#ifdef SWIGPYTHON
(int)
#endif
			'b',
		Mgauss =
#ifdef SWIGPYTHON
(int)
#endif
			'c',
		mT =
#ifdef SWIGPYTHON
(int)
#endif
			'd',
		ngauss =
#ifdef SWIGPYTHON
(int)
#endif
			'e',
		nT =
#ifdef SWIGPYTHON
(int)
#endif
			'f',
		pgauss =
#ifdef SWIGPYTHON
(int)
#endif
			'g',
		pT =
#ifdef SWIGPYTHON
(int)
#endif
			'h',
		Tgauss =
#ifdef SWIGPYTHON
(int)
#endif
			'i',
		TT =
#ifdef SWIGPYTHON
(int)
#endif
			'j',
		ugauss =
#ifdef SWIGPYTHON
(int)
#endif
			'k',
		uT =
#ifdef SWIGPYTHON
(int)
#endif
			'l'
	};
	enum class eml21__MassPerLengthUom {
		kg_x002em_x002fcm2 = 0,
		kg_x002fm = 1,
		klbm_x002fin = 2,
		lbm_x002fft = 3,
		Mg_x002fin = 4
	};
	enum class eml21__PlaneAngleUom {
		_0_x002e001_x0020seca = 0,
		ccgr = 1,
		cgr = 2,
		dega = 3,
		gon = 4,
		krad = 5,
		mila = 6,
		mina = 7,
		Mrad = 8,
		mrad = 9,
		rad = 10,
		rev = 11,
		seca = 12,
		urad = 13
	};
	enum class eml21__VerticalCoordinateUom {
		m =
#ifdef SWIGPYTHON
(int)
#endif
			'm',
		ft =
#ifdef SWIGPYTHON
(int)
#endif
			'n',
		ftUS =
#ifdef SWIGPYTHON
(int)
#endif
			'o',
		ftBr_x002865_x0029 =
#ifdef SWIGPYTHON
(int)
#endif
			'p'
	};
}

#if defined(SWIGJAVA) || defined(SWIGCSHARP)
	%nspace WITSML2_NS::Trajectory;	
	%nspace WITSML2_NS::Well;
	%nspace WITSML2_NS::Wellbore;
	%nspace WITSML2_NS::WellboreObject;
#endif

namespace WITSML2_NS
{
	%nodefaultctor; // Disable creation of default constructors
	
	/** 
	 * @brief	Contains all information that is the same for all wellbores (sidetracks). 
	 */
#if defined(SWIGPYTHON)
	%rename(Witsml2_Well) Well;
#endif
	/** 
	 * @brief	Contains all information that is the same for all wellbores (sidetracks). 
	 */
	class Well : public COMMON_NS::AbstractObject
	{
	public:
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(std::string, NameLegal)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(std::string, NumLicense)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(std::string, NumGovt)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(std::string, Field)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(std::string, Country)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(std::string, State)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(std::string, County)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(std::string, Region)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(std::string, District)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(std::string, Block)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(std::string, Operator)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(std::string, OperatorDiv)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(std::string, OriginalOperator)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(std::string, NumAPI)

		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(gsoap_eml2_1::eml21__WellStatus, StatusWell)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(gsoap_eml2_1::witsml20__WellPurpose, PurposeWell)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(gsoap_eml2_1::witsml20__WellFluid, FluidWell)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(gsoap_eml2_1::witsml20__WellDirection, DirectionWell)

		GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE(WaterDepth, gsoap_eml2_1::eml21__LengthUom)
		GETTER_PRESENCE_ATTRIBUTE(GroundElevation)

		GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE(PcInterest, gsoap_eml2_1::eml21__DimensionlessUom)

		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(time_t, DTimLicense)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(time_t, DTimSpud)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(time_t, DTimPa)
		
		void setGroundElevation(double value, gsoap_eml2_1::eml21__LengthUom uom, const std::string& datum);
		double getGroundElevationValue() const;
		gsoap_eml2_1::eml21__LengthUom getGroundElevationUom() const;
		std::string getGroundElevationDatum() const;
		
		void setTimeZone(bool direction, unsigned short hours, unsigned short minutes);
		GETTER_PRESENCE_ATTRIBUTE(TimeZone)
		bool getTimeZoneDirection() const;
		unsigned short getTimeZoneHours() const;
		unsigned short getTimeZoneMinutes() const;
		
		double getLocationProjectedX(unsigned int locationIndex);
		double getLocationProjectedY(unsigned int locationIndex);
		void pushBackLocation(
			const std::string & guid,
			double projectedX,
			double projectedY,
			unsigned int projectedCrsEpsgCode);
		unsigned int geLocationCount() const;
		
		void pushBackDatum(
			const std::string & guid, 
			const std::string & title,
			gsoap_eml2_1::eml21__WellboreDatumReference code,
			const std::string & datum,
			gsoap_eml2_1::eml21__LengthUom elevationUnit,
			double elevation,
			unsigned int verticalCrsEpsgCode);
		
		unsigned int getDatumCount() const;
		
		SWIG_GETTER_DATAOBJECTS(WITSML2_NS::Wellbore, Wellbore)
		SWIG_GETTER_DATAOBJECTS(WITSML2_0_NS::WellCompletion, Wellcompletion)
	};

#if defined(SWIGPYTHON)
	%rename(Witsml2_Wellbore) Wellbore;
#endif
	/** 
	 * @brief	A wellbore represents the path from surface to a unique bottomhole location.
	 */
	class Wellbore : public COMMON_NS::AbstractObject
	{
	public:
		Well* getWell() const;
		void setWell(Well* witsmlWell);

		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(std::string, Number);
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(std::string, SuffixAPI);
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(std::string, NumGovt);
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(gsoap_eml2_1::eml21__WellStatus, StatusWellbore);
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(bool, IsActive);
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(gsoap_eml2_1::witsml20__WellPurpose, PurposeWellbore);
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(gsoap_eml2_1::witsml20__WellboreType, TypeWellbore);
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(gsoap_eml2_1::witsml20__WellboreShape, Shape);
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(gsoap_eml2_1::witsml20__WellFluid, FluidWellbore)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(bool, AchievedTD);
		GETTER_AND_SETTER_DEPTH_MEASURE_OPTIONAL_ATTRIBUTE(Md, gsoap_eml2_1::eml21__LengthUom);
		GETTER_AND_SETTER_DEPTH_MEASURE_OPTIONAL_ATTRIBUTE(MdBit, gsoap_eml2_1::eml21__LengthUom);
		GETTER_AND_SETTER_DEPTH_MEASURE_OPTIONAL_ATTRIBUTE(MdKickoff, gsoap_eml2_1::eml21__LengthUom);
		GETTER_AND_SETTER_DEPTH_MEASURE_OPTIONAL_ATTRIBUTE(MdPlanned, gsoap_eml2_1::eml21__LengthUom);
		GETTER_AND_SETTER_DEPTH_MEASURE_OPTIONAL_ATTRIBUTE(MdSubSeaPlanned, gsoap_eml2_1::eml21__LengthUom);
		
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::WellboreFeature, ResqmlWellboreFeature)
		
		SWIG_GETTER_DATAOBJECTS(WITSML2_NS::Trajectory, Trajectory)
		SWIG_GETTER_DATAOBJECTS(WITSML2_0_NS::WellboreCompletion, WellboreCompletion)
		SWIG_GETTER_DATAOBJECTS(WITSML2_0_NS::WellboreGeometry, WellboreGeometry)
		SWIG_GETTER_DATAOBJECTS(WITSML2_0_NS::Log, Log)
	};
	
#if defined(SWIGPYTHON)
	%rename(Witsml2_WellboreObject) WellboreObject;
#endif
	/**
	 * The class is the super class for all wellbore objects (all top level objects pointing to
	 * Wellbore)
	 */
	class WellboreObject : public COMMON_NS::AbstractObject
	{
	public:
		Wellbore* getWellbore() const;
		void setWellbore(Wellbore* witsmlWellbore) = 0;
	};
	
#if defined(SWIGPYTHON)
	%rename(Witsml2_Trajectory) Trajectory;
#endif
	/** 
	 * @brief	It contains many trajectory stations to capture the information about individual survey points.
	 */
	class Trajectory : public WellboreObject
	{
	public:
		GETTER_AND_SETTER_GENERIC_ATTRIBUTE(gsoap_eml2_1::witsml20__ChannelStatus, GrowingStatus)

		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(time_t, DTimTrajStart)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(time_t, DTimTrajEnd)

		GETTER_AND_SETTER_DEPTH_MEASURE_OPTIONAL_ATTRIBUTE(MdMn, gsoap_eml2_1::eml21__LengthUom)
		GETTER_AND_SETTER_DEPTH_MEASURE_OPTIONAL_ATTRIBUTE(MdMx, gsoap_eml2_1::eml21__LengthUom)

		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(std::string, ServiceCompany)

		GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE(MagDeclUsed, gsoap_eml2_1::eml21__PlaneAngleUom)
		GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE(GridConUsed, gsoap_eml2_1::eml21__PlaneAngleUom)
		GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE(AziVertSect, gsoap_eml2_1::eml21__PlaneAngleUom)

		GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE(DispNsVertSectOrig, gsoap_eml2_1::eml21__LengthUom)
		GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE(DispEwVertSectOrig, gsoap_eml2_1::eml21__LengthUom)

		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(bool, Definitive)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(bool, Memory)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(bool, FinalTraj)

		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(gsoap_eml2_1::witsml20__AziRef, AziRef)

		//***************************************/
		// ******* TRAJECTORY STATIONS **********/
		//***************************************/

		// Mandatory
		GETTER_AND_SETTER_GENERIC_ATTRIBUTE_IN_VECTOR(std::string, TrajectoryStation, uid)
		GETTER_AND_SETTER_GENERIC_ATTRIBUTE_IN_VECTOR(gsoap_eml2_1::witsml20__TrajStationType, TrajectoryStation, TypeTrajStation)
		GETTER_AND_SETTER_DEPTH_MEASURE_ATTRIBUTE_IN_VECTOR(TrajectoryStation, Md, gsoap_eml2_1::eml21__LengthUom)

		// Optional bool
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(bool, TrajectoryStation, ManuallyEntered)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(bool, TrajectoryStation, GravAccelCorUsed)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(bool, TrajectoryStation, MagXAxialCorUsed)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(bool, TrajectoryStation, SagCorUsed)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(bool, TrajectoryStation, MagDrlstrCorUsed)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(bool, TrajectoryStation, InfieldRefCorUsed)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(bool, TrajectoryStation, InterpolatedInfieldRefCorUsed)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(bool, TrajectoryStation, InHoleRefCorUsed)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(bool, TrajectoryStation, AxialMagInterferenceCorUsed)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(bool, TrajectoryStation, CosagCorUsed)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(bool, TrajectoryStation, MSACorUsed)

		// Optional string
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(std::string, TrajectoryStation, Target)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(std::string, TrajectoryStation, MagModelUsed)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(std::string, TrajectoryStation, MagModelValid)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(std::string, TrajectoryStation, GeoModelUsed)

		// Optional timestamp
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(time_t, TrajectoryStation, DTimStn)

		// Optional enum
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(gsoap_eml2_1::witsml20__TypeSurveyTool, TrajectoryStation, TypeSurveyTool)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(gsoap_eml2_1::witsml20__TrajStnCalcAlgorithm, TrajectoryStation, CalcAlgorithm)
		GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE_IN_VECTOR(gsoap_eml2_1::witsml20__TrajStationStatus, TrajectoryStation, StatusTrajStation)

		// Optional Length Measure
		GETTER_AND_SETTER_DEPTH_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, Tvd, gsoap_eml2_1::eml21__LengthUom)
		GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, DispNs, gsoap_eml2_1::eml21__LengthUom)
		GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, DispEw, gsoap_eml2_1::eml21__LengthUom)
		GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, VertSect, gsoap_eml2_1::eml21__LengthUom)
		GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, MdDelta, gsoap_eml2_1::eml21__LengthUom)
		GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, TvdDelta, gsoap_eml2_1::eml21__LengthUom)

		// Optional Plane Angle Measure
		GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, Incl, gsoap_eml2_1::eml21__PlaneAngleUom)
		GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, Azi, gsoap_eml2_1::eml21__PlaneAngleUom)
		GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, Mtf, gsoap_eml2_1::eml21__PlaneAngleUom)
		GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, Gtf, gsoap_eml2_1::eml21__PlaneAngleUom)
		GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, DipAngleUncert, gsoap_eml2_1::eml21__PlaneAngleUom)
		GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, MagDipAngleReference, gsoap_eml2_1::eml21__PlaneAngleUom)

		// Optional Angle per Length Measure
		GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, Dls, gsoap_eml2_1::eml21__AnglePerLengthUom)
		GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, RateTurn, gsoap_eml2_1::eml21__AnglePerLengthUom)
		GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, RateBuild, gsoap_eml2_1::eml21__AnglePerLengthUom)

		// Optional Linear Acceleration Measure
		GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, GravTotalUncert, gsoap_eml2_1::eml21__LinearAccelerationUom)
		GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, GravTotalFieldReference, gsoap_eml2_1::eml21__LinearAccelerationUom)

		// Optional Magnetic Flux Density Measure
		GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, MagTotalUncert, gsoap_eml2_1::eml21__MagneticFluxDensityUom)
		GETTER_AND_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(TrajectoryStation, MagTotalFieldReference, gsoap_eml2_1::eml21__MagneticFluxDensityUom)

		void pushBackTrajectoryStation(gsoap_eml2_1::witsml20__TrajStationType kind, double mdValue, gsoap_eml2_1::eml21__LengthUom uom, const std::string & datum, const std::string & uid = "");
		unsigned int getTrajectoryStationCount() const;
	};
}

%include "swigWitsml2_0Include.i"
