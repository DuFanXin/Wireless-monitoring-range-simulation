/* Process model C form file: bursty_source_measure.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char bursty_source_measure_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A modeler 7 5855F4C9 5855F4C9 1 FlyingDutchman yzc 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1d79 5                                                                                                                                                                                                                                                                                                                                                                                                       ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

/* Include files. */
#include "oms_dist_support.h"
#include <math.h>
#include <../include/pk_config.h>
#include <../include/mac_function.h>

/* Define constants used in the process model.			*/
#define	OFF_TO_ON			10
#define	ON_TO_ON			20
#define	ON_TO_OFF			30

/* Define a small value (= 1 psec), which will be used	*/
/* to recover from double arithmetic precision losts	*/
/* while doing time related	precision sensitive			*/
/* computations.										*/
#define	PRECISION_RECOVERY	0.000000000001

/* State transition macro definitions.					*/
#define	INACTIVE_TO_ACTIVE	(intrpt_type == OPC_INTRPT_SELF && intrpt_code == OFF_TO_ON)
#define	REMAIN_ACTIVE		(intrpt_type == OPC_INTRPT_SELF && intrpt_code == ON_TO_ON)
#define	ACTIVE_TO_INACTIVE	(intrpt_type == OPC_INTRPT_SELF && intrpt_code == ON_TO_OFF)

/* Function Declarations.	*/
static void			bursty_source_sv_init ();
int once=1;
FILE *fp=NULL;
char nodename[56];
char Apname[10]="AP_Node";
double lat_ptr,long_ptr,alt_ptr,x_ptr,y_ptr,z_ptr;

/*
OmsT_Dist_Handle           on_state_dist_handle;
OmsT_Dist_Handle           off_state_dist_handle;
OmsT_Dist_Handle           intarrvl_time_dist_handle;
OmsT_Dist_Handle           packet_size_dist_handle;  */

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
	char	                   		pid_string [64]                                 ;	/* Process ID display string */
	Boolean	                		debug_mode                                      ;	/* Determines whether the simulation is in debug mode */
	double	                 		stop_time                                       ;	/* Stop time for traffic generation */
	double	                 		off_state_start_time                            ;	/* Time at which the process will enter the OFF state */
	Stathandle	             		pksize_stathandle                               ;	/* Statistic handle to the "Packet Generation Status" statistic */
	OmsT_Dist_Handle	       		on_state_dist_handle                            ;	/* On state distribution handle */
	OmsT_Dist_Handle	       		off_state_dist_handle                           ;	/* Off state distribution handle */
	OmsT_Dist_Handle	       		intarrvl_time_dist_handle                       ;	/* Interarrival time distribution handle */
	OmsT_Dist_Handle	       		packet_size_dist_handle                         ;	/* Packet size distribution handle */
	double	                 		start_time                                      ;	/* Start time for traffic generation */
	Stathandle	             		bits_sent_stathandle                            ;
	Stathandle	             		bitssec_sent_stathandle                         ;
	Stathandle	             		pkts_sent_stathandle                            ;
	Stathandle	             		pktssec_sent_stathandle                         ;
	Stathandle	             		bits_sent_gstathandle                           ;
	Stathandle	             		bitssec_sent_gstathandle                        ;
	Stathandle	             		pkts_sent_gstathandle                           ;
	Stathandle	             		pktssec_sent_gstathandle                        ;
	int	                    		segmentation_size                               ;	/* Size using which segmentation will occur. */
	Sbhandle	               		segmentation_buf_handle                         ;
	int	                    		mac_node_index                                  ;
	int	                    		mac_id                                          ;
	Boolean	                		ap_enable                                       ;
	} bursty_source_measure_state;

#define pid_string              		op_sv_ptr->pid_string
#define debug_mode              		op_sv_ptr->debug_mode
#define stop_time               		op_sv_ptr->stop_time
#define off_state_start_time    		op_sv_ptr->off_state_start_time
#define pksize_stathandle       		op_sv_ptr->pksize_stathandle
#define on_state_dist_handle    		op_sv_ptr->on_state_dist_handle
#define off_state_dist_handle   		op_sv_ptr->off_state_dist_handle
#define intarrvl_time_dist_handle		op_sv_ptr->intarrvl_time_dist_handle
#define packet_size_dist_handle 		op_sv_ptr->packet_size_dist_handle
#define start_time              		op_sv_ptr->start_time
#define bits_sent_stathandle    		op_sv_ptr->bits_sent_stathandle
#define bitssec_sent_stathandle 		op_sv_ptr->bitssec_sent_stathandle
#define pkts_sent_stathandle    		op_sv_ptr->pkts_sent_stathandle
#define pktssec_sent_stathandle 		op_sv_ptr->pktssec_sent_stathandle
#define bits_sent_gstathandle   		op_sv_ptr->bits_sent_gstathandle
#define bitssec_sent_gstathandle		op_sv_ptr->bitssec_sent_gstathandle
#define pkts_sent_gstathandle   		op_sv_ptr->pkts_sent_gstathandle
#define pktssec_sent_gstathandle		op_sv_ptr->pktssec_sent_gstathandle
#define segmentation_size       		op_sv_ptr->segmentation_size
#define segmentation_buf_handle 		op_sv_ptr->segmentation_buf_handle
#define mac_node_index          		op_sv_ptr->mac_node_index
#define mac_id                  		op_sv_ptr->mac_id
#define ap_enable               		op_sv_ptr->ap_enable

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	bursty_source_measure_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((bursty_source_measure_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif

static void
bursty_source_sv_init ()
	{
	Prohandle			my_prohandle;
	int					my_pro_id;
	Objid				my_id;
	Objid				traf_gen_comp_attr_objid, traf_conf_objid;
	Objid				pkt_gen_comp_attr_objid, pkt_gen_args_objid;
	char				on_state_string [128], off_state_string [128];
	char				intarrvl_rate_string [128], packet_size_string [128];
	char				start_time_string [128];
	OmsT_Dist_Handle	start_time_dist_handle;

	/**	Initializes state variables associated with		**/
	/**	this process model.								**/
	FIN (bursty_source_sv_init ());

	/*	Determine the prohandle of this process as well as	*/
	/*	the object IDs of the containing module and node.*/
	my_prohandle = op_pro_self ();
	my_pro_id = op_pro_id (my_prohandle);
	my_id = op_id_self ();

	/*	Determine the process ID display string.	*/
	sprintf (pid_string, "bursty_source PID (%d)", my_pro_id);

	/*	Determine whether or not the simulation is in debug	*/
	/*	mode.  Trace statement are only enabled when the	*/
	/*	simulation is in debug mode.						*/
	debug_mode = op_sim_debug ();

	/* Read the traffic generation parameters.	*/
	op_ima_obj_attr_get (my_id, "Traffic Generation Parameters", &traf_gen_comp_attr_objid);
	traf_conf_objid = op_topo_child (traf_gen_comp_attr_objid, OPC_OBJTYPE_GENERIC, 0);

	/* Determine the start time for traffic generation.	*/
	op_ima_obj_attr_get (traf_conf_objid, "Start Time", start_time_string);
	if (strcmp (start_time_string, "Never") != 0)
		{
		start_time_dist_handle = oms_dist_load_from_string (start_time_string);
		start_time = oms_dist_outcome (start_time_dist_handle);
		}
	else
		{
		start_time = -1.0;
		}

	/* Determine the stop time for traffic generation.	*/
	op_ima_obj_attr_get (traf_conf_objid, "Stop Time", &stop_time);
	if (stop_time == -1.0)
		stop_time = OPC_DBL_INFINITY;

	/* If the start time is set to "Infinity", then there	*/
	/* is no need to schedule an interrupt as this node has	*/
	/* been set will not generate any traffic.				*/
	if ((start_time >= 0.0) && (stop_time > start_time))
		{
		/* Load the distribution used to determine the time	*/
		/* for which the process stays in the "ON" state.	*/
		op_ima_obj_attr_get (traf_conf_objid, "ON State Time", on_state_string);
		on_state_dist_handle = oms_dist_load_from_string (on_state_string); 

		/* Load the distribution used to determine the time	*/
		/* for which the process stays in the "OFF" state.	*/
		op_ima_obj_attr_get (traf_conf_objid, "OFF State Time", off_state_string);
		off_state_dist_handle = oms_dist_load_from_string (off_state_string);

		/* Load the distribution used to determine the packet	*/
		/* interarrivals.										*/
		op_ima_obj_attr_get (traf_conf_objid, "Packet Generation Arguments", &pkt_gen_comp_attr_objid);
		pkt_gen_args_objid = op_topo_child (pkt_gen_comp_attr_objid, OPC_OBJTYPE_GENERIC, 0);

		op_ima_obj_attr_get (pkt_gen_args_objid, "Interarrival Time", intarrvl_rate_string);
		intarrvl_time_dist_handle = oms_dist_load_from_string (intarrvl_rate_string);

		/* Load the distribution used to determine the size of	*/
		/* the packets being generated.							*/
		op_ima_obj_attr_get (pkt_gen_args_objid, "Packet Size", packet_size_string);
		packet_size_dist_handle = oms_dist_load_from_string (packet_size_string);

		/*	Initilaize the packet generation status statistic	*/
		/*	to indicate that currently there are no packets 	*/
		/*	being generated.									*/
		pksize_stathandle = op_stat_reg ("Traffic Source.Packet Generation Status", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
		op_stat_write (pksize_stathandle, (double) OPC_FALSE);
		
		/* Initilaize the statistic handles to keep	*/
		/* track of traffic Sourceed by this process.	*/
		bits_sent_stathandle 		= op_stat_reg ("Traffic Source.Traffic Sent (bits)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
		bitssec_sent_stathandle 	= op_stat_reg ("Traffic Source.Traffic Sent (bits/sec)",	OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
		pkts_sent_stathandle 		= op_stat_reg ("Traffic Source.Traffic Sent (packets)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
		pktssec_sent_stathandle 	= op_stat_reg ("Traffic Source.Traffic Sent (packets/sec)",	OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);

		bits_sent_gstathandle 		= op_stat_reg ("Traffic Source.Traffic Sent (bits)",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
		bitssec_sent_gstathandle 	= op_stat_reg ("Traffic Source.Traffic Sent (bits/sec)",	OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
		pkts_sent_gstathandle 		= op_stat_reg ("Traffic Source.Traffic Sent (packets)",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
		pktssec_sent_gstathandle 	= op_stat_reg ("Traffic Source.Traffic Sent (packets/sec)",	OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
		
		/* Check if packet segmentation is modeled.	*/
		op_ima_obj_attr_get (pkt_gen_args_objid, "Segmentation Size", &segmentation_size);
		if (segmentation_size != -1)
			{
			segmentation_size *= 8;
			segmentation_buf_handle = op_sar_buf_create (OPC_SAR_BUF_TYPE_SEGMENT, OPC_SAR_BUF_OPT_PK_BNDRY);
			}
		}

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
	void bursty_source_measure (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_bursty_source_measure_init (int * init_block_ptr);
	void _op_bursty_source_measure_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_bursty_source_measure_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_bursty_source_measure_alloc (VosT_Obtype, int);
	void _op_bursty_source_measure_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
bursty_source_measure (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (bursty_source_measure ());

		{
		/* Temporary Variables */
		int					intrpt_type;
		int					intrpt_code;
		
		
		Packet*				pkptr;
		double				pksize;
		double				on_period;
		double				off_period;
		double				next_packet_arrival_time;
		/* End of Temporary Variables */


		FSM_ENTER ("bursty_source_measure")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_UNFORCED_NOLABEL (0, "init", "bursty_source_measure [init enter execs]")
				FSM_PROFILE_SECTION_IN ("bursty_source_measure [init enter execs]", state0_enter_exec)
				{
				/* Initialize the traffic generation parameters.	*/
				bursty_source_sv_init ();
				
				
				/*获取MAC的ID号，进行后续处理*/
				mac_node_index = nodeMacListIndexGet(op_topo_parent(op_id_self()));
				mac_id = nodeMacAddressGet(mac_node_index);
				
				/*对于基站AP，可以发送探测报文，其他节点则只收到数据，进行移动，进而探测最大的传输距离*/
				op_ima_obj_attr_get(op_topo_parent(op_id_self()),"Cluster Header Enable",&ap_enable);
				
				/* Schedule the first OFF-period scheduling by setting	*/
				/* a self-interrupt for the start time. If the start	*/
				/* time is set to "Infinity", then there is no need to	*/
				/* to schedule an interrupt as this node has been set	*/
				/* will not generate any traffic.	*/
				op_ima_obj_attr_get(op_topo_parent(op_id_self()),"name",&nodename);//
				if(!strcmp(nodename,Apname)){//
					if(once == 1){//
					if((fp=fopen("C:/Users/yzc/Desktop/AllResult.txt","w+"))==NULL)
					{
					 printf("file cannot be opened\n");
					}
					else{
					op_ima_obj_pos_get (op_topo_parent(op_id_self()), &lat_ptr, &long_ptr, &alt_ptr, &x_ptr, &y_ptr, &z_ptr);
					fprintf(fp,"%s:   %lf  %lf\n",nodename,lat_ptr,long_ptr);
					fclose(fp);
					}
					once = 0;
					}
				  }
				/*只有基站有发送报文的权限,其他节点只负责接收，移动*/
				if (start_time >= 0.0 && ap_enable == OPC_TRUE)
					{
					op_intrpt_schedule_self (op_sim_time () + start_time, 0);
					}
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (1,"bursty_source_measure")


			/** state (init) exit executives **/
			FSM_STATE_EXIT_UNFORCED (0, "init", "bursty_source_measure [init exit execs]")


			/** state (init) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "init", "off", "tr_0", "bursty_source_measure [init -> off : default / ]")
				/*---------------------------------------------------------*/



			/** state (off) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "off", state1_enter_exec, "bursty_source_measure [off enter execs]")
				FSM_PROFILE_SECTION_IN ("bursty_source_measure [off enter execs]", state1_enter_exec)
				{
				/*	Determine the time for which the process remains in	*/
				/*	the "OFF" (inactive) state.  This is determined by	*/
				/*	the distribution loaded to characterize the "OFF"	*/
				/*	state duration (i.e., time for which the source		*/
				/*	does not generate any packets.						*/
				
				off_period = oms_dist_outcome (off_state_dist_handle);
				
				/*	Schedule a self-interrupt to transit to the "ON"	*/
				/*	when the "OFF" state duration expires.				*/
				if (op_sim_time () + off_period < stop_time)
					op_intrpt_schedule_self (op_sim_time () + off_period, OFF_TO_ON);
				
				}
				FSM_PROFILE_SECTION_OUT (state1_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"bursty_source_measure")


			/** state (off) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "off", "bursty_source_measure [off exit execs]")
				FSM_PROFILE_SECTION_IN ("bursty_source_measure [off exit execs]", state1_exit_exec)
				{
				/* Determine the type of interrupt.		*/
				intrpt_type = op_intrpt_type ();
				intrpt_code = op_intrpt_code ();
				
				}
				FSM_PROFILE_SECTION_OUT (state1_exit_exec)


			/** state (off) transition processing **/
			FSM_TRANSIT_ONLY ((INACTIVE_TO_ACTIVE), 2, state2_enter_exec, ;, off, "INACTIVE_TO_ACTIVE", "", "off", "on", "tr_1", "bursty_source_measure [off -> on : INACTIVE_TO_ACTIVE / ]")
				/*---------------------------------------------------------*/



			/** state (on) enter executives **/
			FSM_STATE_ENTER_UNFORCED (2, "on", state2_enter_exec, "bursty_source_measure [on enter execs]")
				FSM_PROFILE_SECTION_IN ("bursty_source_measure [on enter execs]", state2_enter_exec)
				{
				if (op_intrpt_code () == OFF_TO_ON)
					{
					/*	Determine the time at which this process will	*/
					/*	enter the next	"OFF" state.					*/
					on_period = oms_dist_positive_outcome_with_error_msg (on_state_dist_handle, 
						"This occurs for ON period distribution in bursty_source process model.");
					
					off_state_start_time = op_sim_time () + on_period;
				
					/*	Schedule a self-interrupt to transit to "OFF"	*/
					/*	state when the "ON" state duration expires.		*/
					op_intrpt_schedule_self (off_state_start_time, ON_TO_OFF);
					}
				
				/*	Generate the packets based on the loaded parameters	*/
				/*	for traffic generation.								*/
				next_packet_arrival_time = op_sim_time () + oms_dist_positive_outcome_with_error_msg (intarrvl_time_dist_handle,
									"This occurs for packet inter-arrival time distribution in bursty_source process model.");
				
				/* Whenever the ON state is entered, we should always	*/
				/* send atleast one packet.								*/
				
				/*	Create a packet using the outcome of the loaded	*/
				/*	distribution.									*/
				pksize = floor ((double) oms_dist_positive_outcome_with_error_msg (packet_size_dist_handle, 
									"This occurs for packet size distribution in bursty_source process model."));
				pksize *= 8;
				pkptr  = op_pk_create (pksize);
				
				
				
				
				/*****************************Attention********************************/
				/*user defined function*/
				//pkptr = createPacket("deana_packet", pksize);
				//dest_address = randomGenerateMacAddressOthers(mac_node_index );
				//randomGenerateMacAddress(mac_node_index);
				//op_pk_nfd_set(pkptr,"src_field",mac_id);
				//op_pk_nfd_set(pkptr,"dest_field",dest_address);
				/*****************************End*************************************/
				
				
				
				/*	Record statistics to indicate that a packet 	*/
				/*	was generated at the current simulation time.	*/
				op_stat_write (pksize_stathandle, (double) OPC_TRUE);
				
				/* Update local statistics.				*/
				op_stat_write (bits_sent_stathandle, 		pksize);
				op_stat_write (pkts_sent_stathandle, 		1.0);
					
				op_stat_write (bitssec_sent_stathandle, 	pksize);
				op_stat_write (bitssec_sent_stathandle, 	0.0);
				op_stat_write (pktssec_sent_stathandle, 	1.0);
				op_stat_write (pktssec_sent_stathandle, 	0.0);
				
				/* Update global statistics.				*/
				op_stat_write (bits_sent_gstathandle, 		pksize);
				op_stat_write (pkts_sent_gstathandle, 		1.0);
				op_stat_write (bitssec_sent_gstathandle, 	pksize);
				op_stat_write (bitssec_sent_gstathandle, 	0.0);
				op_stat_write (pktssec_sent_gstathandle, 	1.0);
				op_stat_write (pktssec_sent_gstathandle, 	0.0);
				
				/* Loop through to remove segments of the original application packet and   */
				/* send them out to the lower layer.                                        */
				if (segmentation_size > 0  && pksize > segmentation_size)
				   {
				   /* Insert the packet into the segmentation buffer and pull out segments */
				   /* of size specified by the attribute segment size.                     */
				
				   op_sar_segbuf_pk_insert (segmentation_buf_handle, pkptr, 0);
					  
				   while (pksize > 0)
					   {
				       /* Remove segments of size equal to the segment size and send them to   */
				       /* transport layer. If the number of available bits in the buffer is    */
				       /* is lesser than the segment size a packet will be created only with   */
				       /* remaining bits.                                                      */
				       pkptr = op_sar_srcbuf_seg_remove (segmentation_buf_handle, segmentation_size);
						  
				       /* Update the pk_size remaining to reflect the reduced size.    */
				       pksize -= segmentation_size;
				          
					   /* Send the packet to the lower layer.	*/
				       op_pk_send (pkptr, 0);
					   }
				    }
				else
					{
					/* Send the packet to the lower layer.	*/
					op_pk_send (pkptr, 0);
					}
				
				/*	Check if the next packet arrival time is within the	*/
				/*	time in which the process remains in "ON" (active)	*/
				/*	state or not.										*/
				/*	Schedule the next packet arrival.					*/
				if ((next_packet_arrival_time + PRECISION_RECOVERY < off_state_start_time) &&
					(next_packet_arrival_time + PRECISION_RECOVERY < stop_time))
					{
					op_intrpt_schedule_self (next_packet_arrival_time, ON_TO_ON);
					}
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (5,"bursty_source_measure")


			/** state (on) exit executives **/
			FSM_STATE_EXIT_UNFORCED (2, "on", "bursty_source_measure [on exit execs]")
				FSM_PROFILE_SECTION_IN ("bursty_source_measure [on exit execs]", state2_exit_exec)
				{
				/* Determine the type of interrupt.		*/
				intrpt_type = op_intrpt_type ();
				intrpt_code = op_intrpt_code ();
				
				}
				FSM_PROFILE_SECTION_OUT (state2_exit_exec)


			/** state (on) transition processing **/
			FSM_PROFILE_SECTION_IN ("bursty_source_measure [on trans conditions]", state2_trans_conds)
			FSM_INIT_COND (ACTIVE_TO_INACTIVE)
			FSM_TEST_COND (REMAIN_ACTIVE)
			FSM_TEST_LOGIC ("on")
			FSM_PROFILE_SECTION_OUT (state2_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, ;, "ACTIVE_TO_INACTIVE", "", "on", "off", "tr_3", "bursty_source_measure [on -> off : ACTIVE_TO_INACTIVE / ]")
				FSM_CASE_TRANSIT (1, 2, state2_enter_exec, ;, "REMAIN_ACTIVE", "", "on", "on", "tr_4", "bursty_source_measure [on -> on : REMAIN_ACTIVE / ]")
				}
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"bursty_source_measure")
		}
	}




void
_op_bursty_source_measure_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_bursty_source_measure_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_bursty_source_measure_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_bursty_source_measure_svar function. */
#undef pid_string
#undef debug_mode
#undef stop_time
#undef off_state_start_time
#undef pksize_stathandle
#undef on_state_dist_handle
#undef off_state_dist_handle
#undef intarrvl_time_dist_handle
#undef packet_size_dist_handle
#undef start_time
#undef bits_sent_stathandle
#undef bitssec_sent_stathandle
#undef pkts_sent_stathandle
#undef pktssec_sent_stathandle
#undef bits_sent_gstathandle
#undef bitssec_sent_gstathandle
#undef pkts_sent_gstathandle
#undef pktssec_sent_gstathandle
#undef segmentation_size
#undef segmentation_buf_handle
#undef mac_node_index
#undef mac_id
#undef ap_enable

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_bursty_source_measure_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_bursty_source_measure_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (bursty_source_measure)",
		sizeof (bursty_source_measure_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_bursty_source_measure_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	bursty_source_measure_state * ptr;
	FIN_MT (_op_bursty_source_measure_alloc (obtype))

	ptr = (bursty_source_measure_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "bursty_source_measure [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_bursty_source_measure_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	bursty_source_measure_state		*prs_ptr;

	FIN_MT (_op_bursty_source_measure_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (bursty_source_measure_state *)gen_ptr;

	if (strcmp ("pid_string" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->pid_string);
		FOUT
		}
	if (strcmp ("debug_mode" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->debug_mode);
		FOUT
		}
	if (strcmp ("stop_time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->stop_time);
		FOUT
		}
	if (strcmp ("off_state_start_time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->off_state_start_time);
		FOUT
		}
	if (strcmp ("pksize_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pksize_stathandle);
		FOUT
		}
	if (strcmp ("on_state_dist_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->on_state_dist_handle);
		FOUT
		}
	if (strcmp ("off_state_dist_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->off_state_dist_handle);
		FOUT
		}
	if (strcmp ("intarrvl_time_dist_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->intarrvl_time_dist_handle);
		FOUT
		}
	if (strcmp ("packet_size_dist_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->packet_size_dist_handle);
		FOUT
		}
	if (strcmp ("start_time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->start_time);
		FOUT
		}
	if (strcmp ("bits_sent_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->bits_sent_stathandle);
		FOUT
		}
	if (strcmp ("bitssec_sent_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->bitssec_sent_stathandle);
		FOUT
		}
	if (strcmp ("pkts_sent_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pkts_sent_stathandle);
		FOUT
		}
	if (strcmp ("pktssec_sent_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pktssec_sent_stathandle);
		FOUT
		}
	if (strcmp ("bits_sent_gstathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->bits_sent_gstathandle);
		FOUT
		}
	if (strcmp ("bitssec_sent_gstathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->bitssec_sent_gstathandle);
		FOUT
		}
	if (strcmp ("pkts_sent_gstathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pkts_sent_gstathandle);
		FOUT
		}
	if (strcmp ("pktssec_sent_gstathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pktssec_sent_gstathandle);
		FOUT
		}
	if (strcmp ("segmentation_size" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->segmentation_size);
		FOUT
		}
	if (strcmp ("segmentation_buf_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->segmentation_buf_handle);
		FOUT
		}
	if (strcmp ("mac_node_index" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->mac_node_index);
		FOUT
		}
	if (strcmp ("mac_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->mac_id);
		FOUT
		}
	if (strcmp ("ap_enable" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ap_enable);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

