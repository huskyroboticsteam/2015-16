#ifndef XRFparameterIndex_h
#define XRFparameterIndex_h

//		Indices for parameters in conditions array
// Added X-ray tube current     Nov. 30, 2011 (also added parentheses)
// Tabbed 'break;' over to align in a column					 NY 9-9-2013
// Added test_optic_param to allow specification of optic types, NY 9-10-2013

#define XRF_PARAMETER_FIRST			-1
#define ANODE_Z_INDEX				(XRF_PARAMETER_FIRST+1)
#define KV_INDEX					(XRF_PARAMETER_FIRST+2)
#define TUBE_INC_ANGLE_INDEX		(XRF_PARAMETER_FIRST+3)
#define TUBE_TAKEOFF_ANGLE_INDEX 	(XRF_PARAMETER_FIRST+4)
#define TUBE_BE_WINDOW_INDEX		(XRF_PARAMETER_FIRST+5)
#define TUBE_CURRENT_INDEX			(XRF_PARAMETER_FIRST+6)
#define FILTER_Z_INDEX				(XRF_PARAMETER_FIRST+7)
#define FILTER_THICK_INDEX			(XRF_PARAMETER_FIRST+8)
#define EXCIT_ANGLE_INDEX			(XRF_PARAMETER_FIRST+9)
#define EMERG_ANGLE_INDEX			(XRF_PARAMETER_FIRST+10)
#define SOLID_ANGLE_INDEX			(XRF_PARAMETER_FIRST+11)
#define PATH_TYPE_INDEX				(XRF_PARAMETER_FIRST+12)
#define INC_PATH_LENGTH_INDEX		(XRF_PARAMETER_FIRST+13)
#define EMERG_PATH_LENGTH_INDEX		(XRF_PARAMETER_FIRST+14)
#define WINDOW_TYPE_INDEX			(XRF_PARAMETER_FIRST+15)
#define WINDOW_THICK_INDEX			(XRF_PARAMETER_FIRST+16)
#define DETECTOR_TYPE_INDEX			(XRF_PARAMETER_FIRST+17)
#define TEST_OPTIC_TYPE_INDEX		(XRF_PARAMETER_FIRST+18)		// NY Edit 9-10-2013
#define MINIMUM_ENERGY_INDEX		(XRF_PARAMETER_FIRST+19)
#define XRF_PARAMETER_LAST			(MINIMUM_ENERGY_INDEX+1)

#endif

//PATH_TYPE
//		case 1:	pathAtm = Vacuum;		break;
//		case 2:	pathAtm = Helium;		break;
//		case 3:	pathAtm = Mars;			break;
//		case 4:	pathAtm = HeMars;		break;
//		case 5:	pathAtm = Air;			break;
//		default: pathAtm = Vacuum;		break;

//WINDOW_TYPE
//		case 1:	winMaterial = B4C;		break;
//		case 2:	winMaterial = Plastic;	break;
//		case 4:	winMaterial = Zr;		break;	//	zirconium
//		case 5:	winMaterial = Al;		break;	//	Aluminum
//		default: winMaterial = Plastic;	break;

//DETECTOR_TYPE
//		case 1:	detectorType = PIN;		break;
//		case 2:	detectorType = SDD;		break;
//		case 3:	detectorType = CdTe;	break;
//		case 4:	detectorType = HPGe;	break;
//		default: detectorType = PIN;	break;


//OPTIC_TYPE														// Added Optic type, NY 9-10-2013
//		case 1: Default (no optic, 100% transmission) for now
//		case 2: Boxcar optic, acts like a bandpass 
//		case 3: Polycapillary described in file "Energy vs Efficiency smoothed.txt"  //     Added Sep. 2013 by NY
//		default: 
