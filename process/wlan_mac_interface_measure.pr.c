/* Process model C form file: wlan_mac_interface_measure.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char wlan_mac_interface_measure_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A op_runsim 7 56A08F59 56A08F59 1 lenovo-PC lenovo 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1d79 5                                                                                                                                                                                                                                                                                                                                                                                                       ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

#include <../include/node_strm_stat_init.h>


/***** Functional declaration ******/
static void			wlan_mac_higher_layer_intf_sv_init ();


/* End of Header Block */

#if !defined (VOSD_NO_FIN)
#undef	BIN
#undef	BOUT
#define	BIN		FIN_LOCAL_FIELD(_op_last_line_passed) = __LINE__ - _op_block_origin;
#define	BOUT	BIN
#define	BINIT	FIN_LOCAL_FIELD(_op_last_line_passed) = 0; _op_block_origin = __LINE__;
#else
#define	BINIT
#endif /* #if !defined (VOSD_NO_FIN) */



/* State variable definitions */
typedef struct
	{
	/* Internal state tracking for FSM */
	FSM_SYS_STATE
	/* State Variables */
	Objid	                  		my_objid                                        ;	/* Object identifier of the surrounding module. */
	Objid	                  		my_node_objid                                   ;	/* Object identifier of the surrounding node. */
	OpT_Int64	              		destination_address                             ;	/* Destination MAC address to which data should be sent. */
	                        		                                                	/* When set to "Random", the packet can be sent to any   */
	                        		                                                	/* destination except the surrounding node.              */
	} wlan_mac_interface_measure_state;

#define my_objid                		op_sv_ptr->my_objid
#define my_node_objid           		op_sv_ptr->my_node_objid
#define destination_address     		op_sv_ptr->destination_address

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	wlan_mac_interface_measure_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((wlan_mac_interface_measure_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif

static void
wlan_mac_higher_layer_intf_sv_init ()
	{
	int			integer_mac_address = -1;

	/** Initializes all state variables used in this	**/
	/** process model.									**/
	FIN (wlan_mac_higher_layer_intf_sv_init ());

	/* Object identifier for the surrounding module and node.	*/
	my_objid = op_id_self ();
	my_node_objid = op_topo_parent (my_objid);

	/* Determine the destination to which packet should	*/
	/* be sent,and the prioritization to be provided to	*/
	/* the transmitted packet.							*/
	op_ima_obj_attr_get (my_objid, "Destination Address", &integer_mac_address);
	destination_address = integer_mac_address;

	FOUT;
	}



/* End of Function Block */

/* Undefine optional tracing in FIN/FOUT/FRET */
/* The FSM has its own tracing code and the other */
/* functions should not have any tracing.		  */
#undef FIN_TRACING
#define FIN_TRACING

#undef FOUTRET_TRACING
#define FOUTRET_TRACING

#if defined (__cplusplus)
extern "C" {
#endif
	void wlan_mac_interface_measure (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_wlan_mac_interface_measure_init (int * init_block_ptr);
	void _op_wlan_mac_interface_measure_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_wlan_mac_interface_measure_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_wlan_mac_interface_measure_alloc (VosT_Obtype, int);
	void _op_wlan_mac_interface_measure_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
wlan_mac_interface_measure (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (wlan_mac_interface_measure ());

		{
		/* Temporary Variables */
		Packet* pkptr;
		/* End of Temporary Variables */


		FSM_ENTER ("wlan_mac_interface_measure")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_UNFORCED_NOLABEL (0, "init", "wlan_mac_interface_measure [init enter execs]")
				FSM_PROFILE_SECTION_IN ("wlan_mac_interface_measure [init enter execs]", state0_enter_exec)
				{
				/* Initialize the state variables used by this model.	*/
				wlan_mac_higher_layer_intf_sv_init ();
				
				
				/* Schedule a self interrupt to wait for lower layer	*/
				/* wlan MAC process to initialize and register itself in	*/
				/* the model-wide process registry.						*/
				op_intrpt_schedule_self (op_sim_time (), 0);
				
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (1,"wlan_mac_interface_measure")


			/** state (init) exit executives **/
			FSM_STATE_EXIT_UNFORCED (0, "init", "wlan_mac_interface_measure [init exit execs]")
				FSM_PROFILE_SECTION_IN ("wlan_mac_interface_measure [init exit execs]", state0_exit_exec)
				{
				/* Schedule a self interrupt to wait for lower layer	*/
				/* wlan MAC process to initialize and register itself in*/
				/* the model-wide process registry.						*/
				op_intrpt_schedule_self (op_sim_time (), 0);
				
				}
				FSM_PROFILE_SECTION_OUT (state0_exit_exec)


			/** state (init) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "init", "init2", "tr_24", "wlan_mac_interface_measure [init -> init2 : default / ]")
				/*---------------------------------------------------------*/



			/** state (init2) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "init2", state1_enter_exec, "wlan_mac_interface_measure [init2 enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"wlan_mac_interface_measure")


			/** state (init2) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "init2", "wlan_mac_interface_measure [init2 exit execs]")
				FSM_PROFILE_SECTION_IN ("wlan_mac_interface_measure [init2 exit execs]", state1_exit_exec)
				{
				/* Schedule a self interrupt to wait for lower layer	*/
				/* Wlan MAC process to finalize the MAC address			*/
				/* registration and resolution.							*/
				op_intrpt_schedule_self (op_sim_time (), 0);
				}
				FSM_PROFILE_SECTION_OUT (state1_exit_exec)


			/** state (init2) transition processing **/
			FSM_TRANSIT_FORCE (5, state5_enter_exec, ;, "default", "", "init2", "init3", "tr_8", "wlan_mac_interface_measure [init2 -> init3 : default / ]")
				/*---------------------------------------------------------*/



			/** state (idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (2, "idle", state2_enter_exec, "wlan_mac_interface_measure [idle enter execs]")
				FSM_PROFILE_SECTION_IN ("wlan_mac_interface_measure [idle enter execs]", state2_enter_exec)
				{
				
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (5,"wlan_mac_interface_measure")


			/** state (idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (2, "idle", "wlan_mac_interface_measure [idle exit execs]")
				FSM_PROFILE_SECTION_IN ("wlan_mac_interface_measure [idle exit execs]", state2_exit_exec)
				{
				/* The only interrupt expected in this state is a	*/
				/* stream interrupt. It can be either from the MAC	*/
				/* layer for a packet destined for this node or		*/
				/* from the application layer for a packet destined	*/
				/* for some other node.								*/
				pkptr = op_pk_get (op_intrpt_strm());
				
				}
				FSM_PROFILE_SECTION_OUT (state2_exit_exec)


			/** state (idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("wlan_mac_interface_measure [idle trans conditions]", state2_trans_conds)
			FSM_INIT_COND (UPPER_STRM)
			FSM_TEST_COND (LOWER_STRM)
			FSM_TEST_LOGIC ("idle")
			FSM_PROFILE_SECTION_OUT (state2_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 3, state3_enter_exec, ;, "UPPER_STRM", "", "idle", "appl layer arrival", "tr_14", "wlan_mac_interface_measure [idle -> appl layer arrival : UPPER_STRM / ]")
				FSM_CASE_TRANSIT (1, 4, state4_enter_exec, ;, "LOWER_STRM", "", "idle", "mac layer arrival", "tr_17", "wlan_mac_interface_measure [idle -> mac layer arrival : LOWER_STRM / ]")
				}
				/*---------------------------------------------------------*/



			/** state (appl layer arrival) enter executives **/
			FSM_STATE_ENTER_FORCED (3, "appl layer arrival", state3_enter_exec, "wlan_mac_interface_measure [appl layer arrival enter execs]")
				FSM_PROFILE_SECTION_IN ("wlan_mac_interface_measure [appl layer arrival enter execs]", state3_enter_exec)
				{
				/* Install the control informationand send it to	*/
				/* the MAC layer.									*/
				op_pk_send (pkptr, LOWER_OUT_STRM_INDEX);
				
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** state (appl layer arrival) exit executives **/
			FSM_STATE_EXIT_FORCED (3, "appl layer arrival", "wlan_mac_interface_measure [appl layer arrival exit execs]")


			/** state (appl layer arrival) transition processing **/
			FSM_TRANSIT_FORCE (2, state2_enter_exec, ;, "default", "", "appl layer arrival", "idle", "tr_16", "wlan_mac_interface_measure [appl layer arrival -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (mac layer arrival) enter executives **/
			FSM_STATE_ENTER_FORCED (4, "mac layer arrival", state4_enter_exec, "wlan_mac_interface_measure [mac layer arrival enter execs]")
				FSM_PROFILE_SECTION_IN ("wlan_mac_interface_measure [mac layer arrival enter execs]", state4_enter_exec)
				{
				/* A packet arrived from the MAC layer. Since the MAC	*/
				/* layer would have forwarded this only if it were		*/
				/* destined for this node, forward this packet to the	*/
				/* sink module.											*/
				op_pk_send (pkptr, UPPER_OUT_STRM_INDEX);
				
				}
				FSM_PROFILE_SECTION_OUT (state4_enter_exec)

			/** state (mac layer arrival) exit executives **/
			FSM_STATE_EXIT_FORCED (4, "mac layer arrival", "wlan_mac_interface_measure [mac layer arrival exit execs]")


			/** state (mac layer arrival) transition processing **/
			FSM_TRANSIT_FORCE (2, state2_enter_exec, ;, "default", "", "mac layer arrival", "idle", "tr_19", "wlan_mac_interface_measure [mac layer arrival -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (init3) enter executives **/
			FSM_STATE_ENTER_UNFORCED (5, "init3", state5_enter_exec, "wlan_mac_interface_measure [init3 enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (11,"wlan_mac_interface_measure")


			/** state (init3) exit executives **/
			FSM_STATE_EXIT_UNFORCED (5, "init3", "wlan_mac_interface_measure [init3 exit execs]")
				FSM_PROFILE_SECTION_IN ("wlan_mac_interface_measure [init3 exit execs]", state5_exit_exec)
				{
				/* Schedule a self interrupt to wait for lower layer	*/
				/* Wlan MAC process to finalize the MAC address			*/
				/* registration and resolution.							*/
				//op_intrpt_schedule_self (op_sim_time (), 0);
				}
				FSM_PROFILE_SECTION_OUT (state5_exit_exec)


			/** state (init3) transition processing **/
			FSM_TRANSIT_FORCE (2, state2_enter_exec, ;, "default", "", "init3", "idle", "tr_9", "wlan_mac_interface_measure [init3 -> idle : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"wlan_mac_interface_measure")
		}
	}




void
_op_wlan_mac_interface_measure_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_wlan_mac_interface_measure_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_wlan_mac_interface_measure_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_wlan_mac_interface_measure_svar function. */
#undef my_objid
#undef my_node_objid
#undef destination_address

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_wlan_mac_interface_measure_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_wlan_mac_interface_measure_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (wlan_mac_interface_measure)",
		sizeof (wlan_mac_interface_measure_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_wlan_mac_interface_measure_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	wlan_mac_interface_measure_state * ptr;
	FIN_MT (_op_wlan_mac_interface_measure_alloc (obtype))

	ptr = (wlan_mac_interface_measure_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "wlan_mac_interface_measure [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_wlan_mac_interface_measure_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	wlan_mac_interface_measure_state		*prs_ptr;

	FIN_MT (_op_wlan_mac_interface_measure_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (wlan_mac_interface_measure_state *)gen_ptr;

	if (strcmp ("my_objid" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->my_objid);
		FOUT
		}
	if (strcmp ("my_node_objid" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->my_node_objid);
		FOUT
		}
	if (strcmp ("destination_address" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->destination_address);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

