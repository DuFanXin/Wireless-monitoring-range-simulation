/* dra_chanmatch.ps.c */                                                       
/* Default channel match model for radio link Transceiver Pipeline */

/****************************************/
/*		  Copyright (c) 1993-2008		*/
/*		by OPNET Technologies, Inc.		*/
/*		(A Delaware Corporation)		*/
/*	7255 Woodmont Av., Suite 250  		*/
/*     Bethesda, MD 20814, U.S.A.       */
/*			All Rights Reserved.		*/
/****************************************/

#include "opnet.h"
#include "math.h"

#if defined (__cplusplus)
extern "C"
#endif
void
dra_chanmatch_measure_mt (OP_SIM_CONTEXT_ARG_OPT_COMMA Packet * pkptr)
	{
	double		tx_freq, tx_bw, tx_drate, tx_code;
	double		rx_freq, rx_bw, rx_drate, rx_code;
	double      tx_x,tx_y,tx_z;
	double      rx_x,rx_y,rx_z;
	double      distance;
	double      dif_x,dif_y,dif_z;
	Vartype		tx_mod;
	Vartype		rx_mod;
	
	/*         用户自定义的变量区               */
	/*Objid       	rx_objid;
	Objid				tx_objid;
	Boolean     	receiver_state;
	Boolean    		debug;
	Boolean			node_enable;
	int         	tx_node_mac_id;
	int         	rx_node_mac_id;*/
	//char				line0_buf [512];
	/*         用户自定义的变量区               */
	
	
	/** Determine the compatibility between transmitter and receiver channels. **/
	FIN_MT (dra_chanmatch_measure(pkptr));

	
	/*         用户自定义的函数区               */
	//tx_objid = op_td_get_int(pkptr, OPC_TDA_RA_TX_OBJID);
	//rx_objid = op_td_get_int(pkptr, OPC_TDA_RA_RX_OBJID);
	
	/*obtain the previous ruined packet's sender and the receiver*/
	//op_ima_obj_attr_get(op_topo_parent(tx_objid),"user id",&tx_node_mac_id);
	//op_ima_obj_attr_get(op_topo_parent(rx_objid),"user id",&rx_node_mac_id);
	//op_ima_obj_attr_get(op_topo_parent(rx_objid),"Enable",&node_enable);
				
	//if(op_ima_obj_attr_exists(op_topo_parent(tx_objid),"Debug2"))
		//{
			//op_ima_obj_attr_get(op_topo_parent(tx_objid),"Debug2",&debug);
			//}
	//op_ima_obj_attr_get(rx_objid,"State",&receiver_state);
	/*         用户自定义的函数区               */
	
	
	/* Obtain transmitting channel attributes. */
	tx_freq		= op_td_get_dbl (pkptr, OPC_TDA_RA_TX_FREQ);
	tx_bw		= op_td_get_dbl (pkptr, OPC_TDA_RA_TX_BW);
	tx_drate	= op_td_get_dbl (pkptr, OPC_TDA_RA_TX_DRATE);
	tx_code		= op_td_get_dbl (pkptr, OPC_TDA_RA_TX_CODE);
	tx_mod		= op_td_get_ptr (pkptr, OPC_TDA_RA_TX_MOD);	

	/* Obtain receiving channel attributes. */
	rx_freq		= op_td_get_dbl (pkptr, OPC_TDA_RA_RX_FREQ);
	rx_bw		= op_td_get_dbl (pkptr, OPC_TDA_RA_RX_BW);
	rx_drate	= op_td_get_dbl (pkptr, OPC_TDA_RA_RX_DRATE);
	rx_code		= op_td_get_dbl (pkptr, OPC_TDA_RA_RX_CODE);
	rx_mod		= op_td_get_ptr (pkptr, OPC_TDA_RA_RX_MOD);	

	/* For non-overlapping bands, the packet has no	*/
	/* effect; such packets are ignored entirely.	*/
	if ((tx_freq > rx_freq + rx_bw) || (tx_freq + tx_bw < rx_freq))
		{
		op_td_set_int (pkptr, OPC_TDA_RA_MATCH_STATUS, OPC_TDA_RA_MATCH_IGNORE);
		FOUT
		}

	/* Otherwise check for channel attribute mismatches which would	*/
	/* cause the in-band packet to be considered as noise.			*/
	if ((tx_freq != rx_freq) || (tx_bw != rx_bw) || 
		(tx_drate != rx_drate) || (tx_code != rx_code) || (tx_mod != rx_mod))
		{
		op_td_set_int (pkptr, OPC_TDA_RA_MATCH_STATUS, OPC_TDA_RA_MATCH_NOISE);
		FOUT
		}
	
	/*         用户自定义的函数区               */
	/* Obtain the geocentric coordinates of the transmitter. */
	tx_x = op_td_get_dbl (pkptr, OPC_TDA_RA_TX_GEO_X);
	tx_y = op_td_get_dbl (pkptr, OPC_TDA_RA_TX_GEO_Y);
	tx_z = op_td_get_dbl (pkptr, OPC_TDA_RA_TX_GEO_Z);

	/* Obtain the geocentric coordinates of the receiver. */
	rx_x = op_td_get_dbl (pkptr, OPC_TDA_RA_RX_GEO_X);
	rx_y = op_td_get_dbl (pkptr, OPC_TDA_RA_RX_GEO_Y);
	rx_z = op_td_get_dbl (pkptr, OPC_TDA_RA_RX_GEO_Z);
	
	/* Calculate difference vector (transmitter to receiver). */
	dif_x = rx_x - tx_x;
	dif_y = rx_y - tx_y;
	dif_z = rx_z - tx_z;
	
	distance = sqrt (dif_x*dif_x + dif_y*dif_y + dif_z*dif_z);
	
	/*         用户自定义的函数区               */
	
	/* Otherwise the packet is considered a valid transmission which	*/
	/* could eventually be accepted at the error correction stage.		*/
	op_td_set_int (pkptr, OPC_TDA_RA_MATCH_STATUS, OPC_TDA_RA_MATCH_VALID);

	FOUT
	}                
