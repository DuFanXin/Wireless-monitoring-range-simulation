/* dra_bkgnoise.ps.c */                                                       
/* Default background noise model for radio link Transceiver Pipeline */

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

#define BOLTZMANN			1.379E-23
#define BKG_TEMP			290.0
#define AMB_NOISE_LEVEL		1.0E-26


/***** procedure *****/

#if defined (__cplusplus)
extern "C"
#endif
void
dra_bkgnoise_measure_mt (OP_SIM_CONTEXT_ARG_OPT_COMMA Packet * pkptr)
	{
	double		rx_noisefig, rx_temp, rx_bw;
	double		bkg_temp, bkg_noise, amb_noise;
	Objid       node_objid;
	char        node_name[56];
	/** Compute noise sources other than transmission interference. **/
	FIN_MT (dra_bkgnoise_measure (pkptr));

	/* Get receiver noise figure. */
	rx_noisefig = op_td_get_dbl (pkptr, OPC_TDA_RA_RX_NOISEFIG);

	/* Calculate effective receiver temperature. */
	rx_temp = (rx_noisefig - 1.0) * 290.0;

	/* Set the effective background temperature. */
	bkg_temp = BKG_TEMP;

	/* Get receiver channel bandwidth (in Hz). */
	rx_bw = op_td_get_dbl (pkptr, OPC_TDA_RA_RX_BW);

	/* Calculate in-band noise from both background and thermal sources. */
	bkg_noise = (rx_temp + bkg_temp) * rx_bw * BOLTZMANN;

	/* Calculate in-band ambient noise. */
	amb_noise = rx_bw * AMB_NOISE_LEVEL;

	/* Put the sum of both noise sources in the packet transmission data attr.*/
	op_td_set_dbl (pkptr, OPC_TDA_RA_BKGNOISE, (amb_noise + bkg_noise));
	
	node_objid = op_topo_parent(op_td_get_int(pkptr, OPC_TDA_RA_RX_OBJID));
	op_ima_obj_attr_get(node_objid,"name",node_name);
	printf("\n %s Received Packet, Noise: %e.",node_name,(amb_noise + bkg_noise));
	FOUT
	}
