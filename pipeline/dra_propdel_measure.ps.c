/* dra_propdel.ps.c */                                                       
/* Default propagation delay model for radio link Transceiver Pipeline */

/****************************************/
/*		  Copyright (c) 1993-2008		*/
/*		by OPNET Technologies, Inc.		*/
/*		(A Delaware Corporation)		*/
/*	7255 Woodmont Av., Suite 250  		*/
/*     Bethesda, MD 20814, U.S.A.       */
/*			All Rights Reserved.		*/
/****************************************/

#include "opnet.h"


/***** constants *****/

/* propagation velocity of radio signal (m/s) */
#define	PROP_VELOCITY	3.0E+08			


/***** pipeline procedure *****/

#if defined (__cplusplus)
extern "C"
#endif
void
dra_propdel_measure_mt (OP_SIM_CONTEXT_ARG_OPT_COMMA Packet * pkptr)
	{
	double		start_prop_delay, end_prop_delay;
	double		start_prop_distance, end_prop_distance;
	Objid       node_objid;
	char        node_name[56];
	
	/** Compute the propagation delay separating the	**/
	/** radio transmitter from the radio receiver.		**/
	FIN_MT (dra_propdel_measure (pkptr));

	/* Get the start distance between transmitter and receiver. */
	start_prop_distance = op_td_get_dbl (pkptr, OPC_TDA_RA_START_DIST);

	/* Get the end distance between transmitter and receiver. */
	end_prop_distance = op_td_get_dbl (pkptr, OPC_TDA_RA_END_DIST);
	
	node_objid = op_topo_parent(op_td_get_int(pkptr, OPC_TDA_RA_RX_OBJID));
	op_ima_obj_attr_get(node_objid,"name",node_name);
	printf("\n %s Received Packet, Distance: %f.",node_name,end_prop_distance);
	/* Compute propagation delay to start of reception. */
	//start_prop_delay = start_prop_distance / PROP_VELOCITY;
	
	start_prop_delay = 0.0;

	/* Compute propagation delay to end of reception. */
	//end_prop_delay = end_prop_distance / PROP_VELOCITY;
	
	end_prop_delay = 0.0;

	/* Place both propagation delays in packet transmission data attributes. */
	op_td_set_dbl (pkptr, OPC_TDA_RA_START_PROPDEL, start_prop_delay);
	op_td_set_dbl (pkptr, OPC_TDA_RA_END_PROPDEL, end_prop_delay);

	FOUT
	}                
