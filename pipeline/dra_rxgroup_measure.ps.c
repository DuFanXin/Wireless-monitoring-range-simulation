/* dra_rxgroup.ps.c */                                                       
/* Default receiver group model for radio link Transceiver	*/
/* Pipeline. This model populates the state information of	*/
/* the receiver channels to be used in power and ecc		*/
/* pipeline stage models. If you don't want the receiver	*/
/* channel state information to be set and used by radio	*/
/* pipeline stage models, then use "*_no_rxstate" version	*/
/* of rxgroup, power and ecc stage models in your node		*/
/* models.													*/					

/****************************************/
/*		 Copyright (c) 1993-2008		*/
/*		by OPNET Technologies, Inc.		*/
/*		 (A Delaware Corporation)		*/
/*	   7255 Woodmont Av., Suite 250  	*/
/*      Bethesda, MD 20814, U.S.A.      */
/*		   All Rights Reserved.			*/
/****************************************/

#include "opnet.h"
#include "dra.h"

#if defined (__cplusplus)
extern "C"
#endif
	
int
dra_rxgroup_measure_mt (OP_SIM_CONTEXT_ARG_OPT_COMMA Objid tx_obid, Objid rx_obid)
	{
	DraT_Rxch_State_Info*	rxch_state_ptr;
	Objid tx_node_objid;
	Objid rx_node_objid;
	Boolean rx_node_enable = OPC_FALSE;
	//Boolean node_enable;
	
	/** Determine the potential for communication between	**/
	/** given transmitter and receiver channel objects.		**/
	/** Also create and initialize the receiver channel's	**/
	/** state information to be used by other pipeline		**/
	/** stages during the simulation.						**/
	FIN_MT (dra_rxgroup_measure(tx_obid, rx_obid));

	/* Unless it is already done, initialize the receiver	*/
	/* channel's state information.							*/
	if (op_ima_obj_state_get (rx_obid) == OPC_NIL)
		{
#if defined (OPD_PARALLEL)
		/* Channel state information doesn't exist. Lock	*/
		/* the global mutex before continuing.				*/
		op_prg_mt_global_lock ();
		
		/* Check again since another thread may have		*/
		/* already set up the state information.			*/
		if (op_ima_obj_state_get (rx_obid) == OPC_NIL)
			{
#endif /* OPD_PARALLEL */
			/* Create and set the initial state information	*/
			/* for the receiver channel. State information	*/
			/* is used by other pipeline stages to			*/
			/* access/update channel specific data			*/
			/* efficiently.									*/
			rxch_state_ptr = (DraT_Rxch_State_Info *) 
				op_prg_mem_alloc (sizeof (DraT_Rxch_State_Info));
			rxch_state_ptr->signal_lock = OPC_FALSE;
			op_ima_obj_state_set (rx_obid, rxch_state_ptr);
#if defined (OPD_PARALLEL)
			}
		
		/* Unlock the global mutex.							*/
		op_prg_mt_global_unlock ();
#endif /* OPD_PARALLEL */
		}
		
	/* By default, all receivers are considered as			*/
	/* potential destinations.								*/
	tx_node_objid = op_topo_parent(op_topo_parent(op_topo_parent(rx_obid)));
	rx_node_objid = op_topo_parent(op_topo_parent(op_topo_parent(tx_obid)));
	
	/*此阶段的处理在仿真初始，在节点获得属性之前，需要重新计算*/
	//op_ima_obj_attr_get(rx_node_objid,"Enable",&rx_node_enable);
	if(tx_node_objid == rx_node_objid)
		{
			FRET(OPC_FALSE);
			}
//	else if(rx_node_enable == OPC_FALSE)
	//	{
		//	FRET(OPC_FALSE);
		//}
	else 
		{
			FRET (OPC_TRUE);
			}
	}                
