#ifndef __BUS_ADDRESSING__
#define __BUS_ADDRESSING__
/*======================================================================*/
/* Project:	Bsp2 ESE Bus Addresses					*/
/* MCU          Atmel AVR						*/
/**@file	bus_addressing.h
   @author 	Nick Mayerhofer
   @version 	1.0
   @date 	2009/11/19						*/
/*======================================================================*/

//ND<x> Steht fuer Node x	{Byte Order: Byte0, Byte1, ... ,Byte15}
#define ADR_ND0_TIME	96	//{z, ss, mm, hh}	z==tenth
#define ADR_ND3_TEMP	128	//{eezOC}       sendbrightness[1]=(hell>>8)
			//			sendbrightness[0]=(hell & 0xFF);	Measured temepature
#define ADR_ND3_BRIGHTN	      144	//{%}
#define ADR_ND1_BRIGHTN_SCALE    239         //scalierung des Helligskeitssenor

#define ADR_ND1_TEMP	192	//{eezOC}
#define ADR_ND1_BRIGHTN	224	//{%}

#define ADR_ND3_ERR_OVERFLOW 	230	//{foo}
#define ADR_ND2_ERR_PRIM 	238	//{primL, primH}

#define ADR_MEMEVAL_REQ_OFFSET 	   240 	//{1}
#define ADR_MEMEVAL_REPLY_OFFSET   250	//{HB, LB, HE, LE, HS, LS} Begin, End, Size
#define ADR_MEMEVAL_REQ_N1     241  //{2}
#define ADR_MEMEVAL_REPLY_N1   251	//{HB, LB, HE, LE, HS, LS}
#define ADR_MEMEVAL_REQ_N2     242	//{2}
#define ADR_MEMEVAL_REPLY_N2   252	//{HB, LB, HE, LE, HS, LS}
#define ADR_MEMEVAL_REQ_N3     243	//{3}
#define ADR_MEMEVAL_REPLY_N3   253	//{HB, LB, HE, LE, HS, LS}

#endif
