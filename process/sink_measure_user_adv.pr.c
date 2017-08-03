/* Process model C form file: sink_measure_user_adv.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char sink_measure_user_adv_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A modeler 7 56A18D0D 56A18D0D 1 lenovo-PC lenovo 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1d79 5                                                                                                                                                                                                                                                                                                                                                                                                         ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

#include <../include/pk_config.h>
#include <math.h>
//#define DEBUG
#include <prg_geo.h>
#include "opnet.h"
#define NAME_SIZE 128

//extern double x_center;
//extern double y_center;
//extern double z_center;
//extern double measure_step;
extern int RCVD_NODE_NUM;

extern double ap_latitude,ap_longitude;
void handle_error_msg(const char* error);
void moveNodePositionFuther(Objid n);

extern vid; 

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
	Stathandle	             		bits_rcvd_stathandle                            ;
	Stathandle	             		bitssec_rcvd_stathandle                         ;
	Stathandle	             		pkts_rcvd_stathandle                            ;
	Stathandle	             		pktssec_rcvd_stathandle                         ;
	Stathandle	             		ete_delay_stathandle                            ;
	Stathandle	             		bits_rcvd_gstathandle                           ;
	Stathandle	             		bitssec_rcvd_gstathandle                        ;
	Stathandle	             		pkts_rcvd_gstathandle                           ;
	Stathandle	             		pktssec_rcvd_gstathandle                        ;
	Stathandle	             		ete_delay_gstathandle                           ;
	Stathandle	             		hops_average_gstathandle                        ;
	double	                 		init_step                                       ;
	Andid*	                 		node_did                                        ;
	Boolean	                		first_draw                                      ;
	} sink_measure_user_adv_state;

#define bits_rcvd_stathandle    		op_sv_ptr->bits_rcvd_stathandle
#define bitssec_rcvd_stathandle 		op_sv_ptr->bitssec_rcvd_stathandle
#define pkts_rcvd_stathandle    		op_sv_ptr->pkts_rcvd_stathandle
#define pktssec_rcvd_stathandle 		op_sv_ptr->pktssec_rcvd_stathandle
#define ete_delay_stathandle    		op_sv_ptr->ete_delay_stathandle
#define bits_rcvd_gstathandle   		op_sv_ptr->bits_rcvd_gstathandle
#define bitssec_rcvd_gstathandle		op_sv_ptr->bitssec_rcvd_gstathandle
#define pkts_rcvd_gstathandle   		op_sv_ptr->pkts_rcvd_gstathandle
#define pktssec_rcvd_gstathandle		op_sv_ptr->pktssec_rcvd_gstathandle
#define ete_delay_gstathandle   		op_sv_ptr->ete_delay_gstathandle
#define hops_average_gstathandle		op_sv_ptr->hops_average_gstathandle
#define init_step               		op_sv_ptr->init_step
#define node_did                		op_sv_ptr->node_did
#define first_draw              		op_sv_ptr->first_draw

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	sink_measure_user_adv_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((sink_measure_user_adv_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif

void handle_error_msg(const char* error)
{
	char process_name[NAME_SIZE];
	
	FIN(handle_error(error));
	
	op_ima_obj_attr_get_str(op_id_self(), "process model", NAME_SIZE, 
		process_name);
	sprintf(process_name, "Error occurred in process <%s>:", process_name);
	op_sim_end(process_name, error, OPC_NIL, OPC_NIL);

	FOUT;
}

/* Change mobile node position by setting the three components of 
the position of the communications node by id. */
/* "n" is the id of the node to move; "xpos", "ypos" and "altitude" are the 
three components of the position */
void moveNodePositionFuther(Objid n)
{
//	double xpos,ypos;
	double node_angle;
	double radius;
	double terrain_height;
	double long_degrees,lat_degrees;
	double longitude,latitude;
	double antenna_height;
	double x_dist,y_dist;
	Objid node_objid;
	char node_name[56];
	//double init_step;
	FIN(void moveNodePosition(Objid n));
	node_objid = n;
	//node_objid = op_topo_parent(op_id_self());
	op_ima_obj_attr_get(node_objid,"Measure Angle",&node_angle);
	op_ima_obj_attr_get(node_objid,"Measure Radius",&radius);
	//radius = radius + measure_step;
	//op_ima_obj_attr_set(node_objid,"Measure Radius",radius);
	//op_ima_obj_attr_get_dbl(node_objid, "x position", &xpos);
	//op_ima_obj_attr_get_dbl(node_objid, "y position", &ypos);
	
	init_step = init_step + radius;
	
	y_dist = init_step * sin(node_angle * 3.1415 / 180.0);
	x_dist = init_step * cos(node_angle * 3.1415 / 180.0);
	//ypos = ypos + radius * sin(node_angle * 3.1415 / 180.0);
	//xpos = xpos + radius * cos(node_angle * 3.1415 / 180.0);
	
	/*以测量节点为中心，换算经纬度的数值*/
	prg_geo_distances_to_degrees_convert (ap_longitude, ap_latitude, x_dist, y_dist,  PrgC_Unit_Meters_Type, &long_degrees, &lat_degrees);
	
	//if(xpos >= 0)
		//{
			longitude = ap_longitude + long_degrees;
			//}
	//else 
		//{
			//longitude = ap_longitude - long_degrees;
			//}
				
	//if(ypos >= 0)
		//{
			//latitude = ap_latitude - lat_degrees;
			//}
	//else 
		//{
			latitude = ap_latitude + lat_degrees;
			//}
				
	terrain_height = tmm_elev_point_get(latitude,longitude);
	op_ima_obj_attr_get(node_objid,"name",&node_name);
	printf("\n%s New Terrain Height:%f",node_name,terrain_height);
		
	op_ima_obj_attr_get_dbl(node_objid, "Antenna Height", &antenna_height);
	op_ima_obj_pos_set_geodetic (node_objid, latitude, longitude,antenna_height);
			
	//if(op_ima_obj_attr_set_dbl(n, "x position", xpos)== OPC_COMPCODE_FAILURE)
		//handle_error_msg("Unable to set \"x position\" attribute.");
	//if(op_ima_obj_attr_set_dbl(n, "y position", ypos)== OPC_COMPCODE_FAILURE)
		//handle_error_msg("Unable to set \"y position\" attribute.");
	/* Check existence of "altitude" attribute.
	Fixed communications node object has no "altitude" attribute */
	//if(op_ima_obj_attr_exists(n, "altitude") == OPC_TRUE)
	//{
		//if(op_ima_obj_attr_set_dbl(n, "altitude", z_center)== OPC_COMPCODE_FAILURE)
			//handle_error_msg("Unable to set \"altitude\" attribute.");
	//}
	FOUT;
}

static void nodeAnimationPositionDrawUpdate(void)
	{
		
		char mobile_name[128];
		double tgt_lat,tgt_lon,tgt_alt;
		double tgt_x,tgt_y,tgt_z;
		double sn_x,sn_y;
		int v_x,v_y;
		Objid node_objid;
		FIN(static void nodeAnimationPositionDrawUpdate(void));
		node_objid = op_topo_parent(op_id_self());
		op_ima_obj_attr_get(node_objid,"name",mobile_name);
		op_ima_obj_pos_get (node_objid, &tgt_lat, &tgt_lon, &tgt_alt,&tgt_x, &tgt_y, &tgt_z);

		op_anim_ime_nobj_update (vid, OPC_ANIM_OBJTYPE_NODE, mobile_name, 	OPC_ANIM_OBJ_ATTR_XPOS,tgt_lon ,OPC_ANIM_OBJ_ATTR_YPOS,tgt_lat,OPC_EOL);
		/** Mobile node position update interrupt: animate node's new position. **/ 


		/* Obtain new position of node in subnet coordinates. */
		op_ima_obj_attr_get (node_objid, "x position", &sn_x);
		op_ima_obj_attr_get (node_objid, "y position", &sn_y);

		/* Convert node position from model coordinates to animation viewer coordinates. */
		op_anim_ime_gen_pos (vid, sn_x, sn_y, &v_x, &v_y);

		if(first_draw == OPC_FALSE)
			{
				/* draw node's icon at new position. */
				*node_did = op_anim_igp_icon_draw (vid, OPC_ANIM_RETAIN | OPC_ANIM_PIXOP_XOR, "field_svc", v_x, v_y); 
				first_draw = OPC_TRUE;
				}
		else 
			{
				/* Erase node's icon at previous position. */
				op_anim_igp_drawing_erase (vid, *node_did, OPC_ANIM_ERASE_MODE_XOR);

				/* Redraw node's icon at new position. */
				*node_did = op_anim_igp_icon_draw (vid, OPC_ANIM_RETAIN | OPC_ANIM_PIXOP_XOR, "field_svc", v_x, v_y); 
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
	void sink_measure_user_adv (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_sink_measure_user_adv_init (int * init_block_ptr);
	void _op_sink_measure_user_adv_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_sink_measure_user_adv_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_sink_measure_user_adv_alloc (VosT_Obtype, int);
	void _op_sink_measure_user_adv_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
sink_measure_user_adv (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (sink_measure_user_adv ());

		{
		/* Temporary Variables */
		Packet*		pkptr;
		double		pk_size;
		double		ete_delay;
		//int         hops;
		/* End of Temporary Variables */


		FSM_ENTER_NO_VARS ("sink_measure_user_adv")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (Move) enter executives **/
			FSM_STATE_ENTER_UNFORCED (0, "Move", state0_enter_exec, "sink_measure_user_adv [Move enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (1,"sink_measure_user_adv")


			/** state (Move) exit executives **/
			FSM_STATE_EXIT_UNFORCED (0, "Move", "sink_measure_user_adv [Move exit execs]")
				FSM_PROFILE_SECTION_IN ("sink_measure_user_adv [Move exit execs]", state0_exit_exec)
				{
				/*当节点收到数据负责移动报文中的距离，收不到信息，停止在原地*/
				/* Obtain the incoming packet.	*/
				pkptr = op_pk_get (op_intrpt_strm ());
				
				/*统计成功接收数据的节点数目*/
				RCVD_NODE_NUM++;
				
				/*收到探测数据包，移动节点*/
				moveNodePositionFuther(op_topo_parent(op_id_self()));
				
				nodeAnimationPositionDrawUpdate();
				/* Caclulate metrics to be updated.		*/
				pk_size = (double) op_pk_total_size_get (pkptr);
				ete_delay = op_sim_time () - op_pk_creation_time_get (pkptr);
				//op_pk_nfd_get(pkptr,"hops",&hops);
				
				//printf("\nThe Delay %f.",ete_delay);
#ifdef DEBUG
				packetCreateTimePrint( pkptr);
#endif
					
				/* Update local statistics.				*/
				op_stat_write (bits_rcvd_stathandle, 		pk_size);
				op_stat_write (pkts_rcvd_stathandle, 		1.0);
				op_stat_write (ete_delay_stathandle, 		ete_delay);
				
				op_stat_write (bitssec_rcvd_stathandle, 	pk_size);
				op_stat_write (bitssec_rcvd_stathandle, 	0.0);
				op_stat_write (pktssec_rcvd_stathandle, 	1.0);
				op_stat_write (pktssec_rcvd_stathandle, 	0.0);
				
				/* Update global statistics.	*/
				op_stat_write (bits_rcvd_gstathandle, 		pk_size);
				op_stat_write (pkts_rcvd_gstathandle, 		1.0);
				op_stat_write (ete_delay_gstathandle, 		ete_delay);
				
				op_stat_write (bitssec_rcvd_gstathandle, 	pk_size);
				op_stat_write (bitssec_rcvd_gstathandle, 	0.0);
				op_stat_write (pktssec_rcvd_gstathandle, 	1.0);
				op_stat_write (pktssec_rcvd_gstathandle, 	0.0);
				
				//op_stat_write (hops_average_gstathandle, 	hops);
				/* Destroy the received packet.	*/
				op_pk_destroy (pkptr);
				}
				FSM_PROFILE_SECTION_OUT (state0_exit_exec)


			/** state (Move) transition processing **/
			FSM_TRANSIT_FORCE (0, state0_enter_exec, ;, "default", "", "Move", "Move", " ", "sink_measure_user_adv [Move -> Move : default / ]")
				/*---------------------------------------------------------*/



			/** state (INIT) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (1, "INIT", "sink_measure_user_adv [INIT enter execs]")
				FSM_PROFILE_SECTION_IN ("sink_measure_user_adv [INIT enter execs]", state1_enter_exec)
				{
				/* Initilaize the statistic handles to keep	*/
				/* track of traffic sinked by this process.	*/
				bits_rcvd_stathandle 		= op_stat_reg ("Traffic Sink.Traffic Received (bits)",			OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				bitssec_rcvd_stathandle 	= op_stat_reg ("Traffic Sink.Traffic Received (bits/sec)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				pkts_rcvd_stathandle 		= op_stat_reg ("Traffic Sink.Traffic Received (packets)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				pktssec_rcvd_stathandle 	= op_stat_reg ("Traffic Sink.Traffic Received (packets/sec)",	OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				ete_delay_stathandle		= op_stat_reg ("Traffic Sink.End-to-End Delay (seconds)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				
				bits_rcvd_gstathandle 		= op_stat_reg ("Traffic Sink.Traffic Received (bits)",			OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				bitssec_rcvd_gstathandle 	= op_stat_reg ("Traffic Sink.Traffic Received (bits/sec)",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				pkts_rcvd_gstathandle 		= op_stat_reg ("Traffic Sink.Traffic Received (packets)",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				pktssec_rcvd_gstathandle 	= op_stat_reg ("Traffic Sink.Traffic Received (packets/sec)",	OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				ete_delay_gstathandle		= op_stat_reg ("Traffic Sink.End-to-End Delay (seconds)",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				hops_average_gstathandle = op_stat_reg ("Traffic Sink.Traffic Received Hops",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				}
				FSM_PROFILE_SECTION_OUT (state1_enter_exec)

			/** state (INIT) exit executives **/
			FSM_STATE_EXIT_FORCED (1, "INIT", "sink_measure_user_adv [INIT exit execs]")
				FSM_PROFILE_SECTION_IN ("sink_measure_user_adv [INIT exit execs]", state1_exit_exec)
				{
				op_ima_obj_attr_get(op_topo_parent(op_id_self()),"Init Distance",&init_step);
				first_draw = OPC_FALSE;
				}
				FSM_PROFILE_SECTION_OUT (state1_exit_exec)


			/** state (INIT) transition processing **/
			FSM_TRANSIT_FORCE (0, state0_enter_exec, ;, "default", "", "INIT", "Move", "tr_12", "sink_measure_user_adv [INIT -> Move : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (1,"sink_measure_user_adv")
		}
	}




void
_op_sink_measure_user_adv_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_sink_measure_user_adv_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_sink_measure_user_adv_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_sink_measure_user_adv_svar function. */
#undef bits_rcvd_stathandle
#undef bitssec_rcvd_stathandle
#undef pkts_rcvd_stathandle
#undef pktssec_rcvd_stathandle
#undef ete_delay_stathandle
#undef bits_rcvd_gstathandle
#undef bitssec_rcvd_gstathandle
#undef pkts_rcvd_gstathandle
#undef pktssec_rcvd_gstathandle
#undef ete_delay_gstathandle
#undef hops_average_gstathandle
#undef init_step
#undef node_did
#undef first_draw

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_sink_measure_user_adv_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_sink_measure_user_adv_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (sink_measure_user_adv)",
		sizeof (sink_measure_user_adv_state));
	*init_block_ptr = 2;

	FRET (obtype)
	}

VosT_Address
_op_sink_measure_user_adv_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	sink_measure_user_adv_state * ptr;
	FIN_MT (_op_sink_measure_user_adv_alloc (obtype))

	ptr = (sink_measure_user_adv_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "sink_measure_user_adv [INIT enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_sink_measure_user_adv_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	sink_measure_user_adv_state		*prs_ptr;

	FIN_MT (_op_sink_measure_user_adv_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (sink_measure_user_adv_state *)gen_ptr;

	if (strcmp ("bits_rcvd_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->bits_rcvd_stathandle);
		FOUT
		}
	if (strcmp ("bitssec_rcvd_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->bitssec_rcvd_stathandle);
		FOUT
		}
	if (strcmp ("pkts_rcvd_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pkts_rcvd_stathandle);
		FOUT
		}
	if (strcmp ("pktssec_rcvd_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pktssec_rcvd_stathandle);
		FOUT
		}
	if (strcmp ("ete_delay_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ete_delay_stathandle);
		FOUT
		}
	if (strcmp ("bits_rcvd_gstathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->bits_rcvd_gstathandle);
		FOUT
		}
	if (strcmp ("bitssec_rcvd_gstathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->bitssec_rcvd_gstathandle);
		FOUT
		}
	if (strcmp ("pkts_rcvd_gstathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pkts_rcvd_gstathandle);
		FOUT
		}
	if (strcmp ("pktssec_rcvd_gstathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pktssec_rcvd_gstathandle);
		FOUT
		}
	if (strcmp ("ete_delay_gstathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ete_delay_gstathandle);
		FOUT
		}
	if (strcmp ("hops_average_gstathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->hops_average_gstathandle);
		FOUT
		}
	if (strcmp ("init_step" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->init_step);
		FOUT
		}
	if (strcmp ("node_did" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->node_did);
		FOUT
		}
	if (strcmp ("first_draw" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->first_draw);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

