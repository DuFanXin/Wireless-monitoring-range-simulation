/* Process model C form file: measure_inter_adv.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char measure_inter_adv_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A modeler 7 56A18D40 56A18D40 1 lenovo-PC lenovo 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1d79 5                                                                                                                                                                                                                                                                                                                                                                                                         ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

#include <../include/node_strm_stat_init.h>
#include <../include/mac_function.h>
#include <../include/pk_config.h>
#include <../include/animation_header.h>

int RCVD_NODE_NUM = 5;

#define SELF_INTRPT_SEND (op_intrpt_type() == OPC_INTRPT_SELF  && \
						op_intrpt_code() == 0)

#define SELF_INTRPT_Animation (op_intrpt_type() == OPC_INTRPT_SELF  && \
						op_intrpt_code() == 1)

#define SIM_END (op_intrpt_type() == OPC_INTRPT_SELF  && \
						op_intrpt_code() == 3)

/*动画绘制的各种函数，线条*/
void small_line(void);

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
	int	                    		mac_id                                          ;
	Objid	                  		own_id                                          ;
	Objid	                  		node_id                                         ;
	Boolean	                		cluster_enable                                  ;
	int	                    		total_node_num                                  ;
	Boolean	                		Animation_Enable                                ;
	char	                   		net_topo[256]                                   ;
	char	                   		subnet_name[256]                                ;
	char	                   		node_name[256]                                  ;
	Andid**	                		line_ptr                                        ;
	int	                    		new_variable                                    ;
	} measure_inter_adv_state;

#define mac_id                  		op_sv_ptr->mac_id
#define own_id                  		op_sv_ptr->own_id
#define node_id                 		op_sv_ptr->node_id
#define cluster_enable          		op_sv_ptr->cluster_enable
#define total_node_num          		op_sv_ptr->total_node_num
#define Animation_Enable        		op_sv_ptr->Animation_Enable
#define net_topo                		op_sv_ptr->net_topo
#define subnet_name             		op_sv_ptr->subnet_name
#define node_name               		op_sv_ptr->node_name
#define line_ptr                		op_sv_ptr->line_ptr
#define new_variable            		op_sv_ptr->new_variable

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	measure_inter_adv_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((measure_inter_adv_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif

static void radioTransimissionAreaDraw(void)
	{
		int node_total_number;
		Objid node_objid,node_objid_next;
		
		Objid subnet_objid;
		int node_index;
		char mobile_name[128];
		char mobile_name_next[128];
		double tgt_lat,tgt_lon,tgt_alt;
		double tgt_x,tgt_y,tgt_z;
		
		double tgt_lat_1,tgt_lon_1,tgt_alt_1;
		double tgt_x_1,tgt_y_1,tgt_z_1;
		int  tgt_vx,tgt_vy;
		int  tgt_vx_1,tgt_vy_1;
		FIN(static void radioTransimissionAreaDraw(void));
		subnet_objid = op_topo_parent(node_id);
		node_total_number = op_topo_object_count (NODE_TYPE);
		big_line();
		//line_ptr = (Andid **)op_prg_mem_alloc(sizeof(Andid) * (node_total_number-1));
		//for(node_index = 0; node_index < node_total_number-1;node_index++)
			//{
				//node_objid = op_topo_object (NODE_TYPE, node_index);
				//sprintf(mobile_name,"Station_%d",node_index);
				//node_objid = op_id_from_name(subnet_objid,NODE_TYPE,mobile_name);
			
				//op_ima_obj_pos_get (node_objid, &tgt_lat, &tgt_lon, &tgt_alt,&tgt_x, &tgt_y, &tgt_z);
				
				//op_anim_ime_nobj_update (vid, OPC_ANIM_OBJTYPE_NODE, mobile_name, 	OPC_ANIM_OBJ_ATTR_ICON, "ace_qv_exclude" ,OPC_ANIM_OBJ_ATTR_XPOS,tgt_lon ,OPC_ANIM_OBJ_ATTR_YPOS,tgt_lat,OPC_EOL);
				//}
		
		
		for(node_index = 0; node_index < node_total_number-1;node_index++)
			{
				//node_objid = op_topo_object (NODE_TYPE, node_index);
				sprintf(mobile_name,"Station_%d",node_index);
				node_objid = op_id_from_name(subnet_objid,NODE_TYPE,mobile_name);
			
				
				if(node_index == (node_total_number-2))
					{
						sprintf(mobile_name_next,"Station_%d",0);
						node_objid_next = op_id_from_name(subnet_objid,NODE_TYPE,mobile_name_next);
					}
				else 
					{
						sprintf(mobile_name_next,"Station_%d",(node_index+1));
						node_objid_next = op_id_from_name(subnet_objid,NODE_TYPE,mobile_name_next);
					}
				op_ima_obj_pos_get (node_objid, &tgt_lat, &tgt_lon, &tgt_alt,&tgt_x, &tgt_y, &tgt_z);

				op_ima_obj_pos_get (node_objid_next, &tgt_lat_1, &tgt_lon_1, &tgt_alt_1,&tgt_x_1, &tgt_y_1, &tgt_z_1);
				/* Convert target position from model to viewer coordinates. */
				op_anim_ime_gen_pos (vid, tgt_lon, tgt_lat, &tgt_vx, &tgt_vy);
				op_anim_ime_gen_pos (vid, tgt_lon_1, tgt_lat_1, &tgt_vx_1, &tgt_vy_1);
				
				//printf("\n %s longtitude:%f latitude:%f convert_x:%d convert_y:%d.",mobile_name,tgt_lon,tgt_lat,tgt_vx,tgt_vy);
				//op_id_from_name(subnet_objid,NODE_TYPE,mobile_name);
				//*(*(line_ptr+node_index)) = op_anim_igp_macro_draw (vid, OPC_ANIM_RETAIN, mid, 	OPC_ANIM_REG_A_STR, mobile_name, OPC_ANIM_REG_D_STR, mobile_name_next, OPC_EOL);
				//op_anim_ime_nobj_update (vid, OPC_ANIM_OBJTYPE_NODE, mobile_name, 	OPC_ANIM_OBJ_ATTR_ICON, "ace_qv_exclude" ,OPC_ANIM_OBJ_ATTR_XPOS,tgt_lon ,OPC_ANIM_OBJ_ATTR_YPOS,tgt_lat,OPC_EOL);
				//op_anim_ime_nobj_update (vid, OPC_ANIM_OBJTYPE_NODE, mobile_name_next, 	OPC_ANIM_OBJ_ATTR_ICON, "ace_qv_exclude" ,OPC_ANIM_OBJ_ATTR_XPOS,tgt_lon_1 ,OPC_ANIM_OBJ_ATTR_YPOS,tgt_lat_1,OPC_EOL);
				//op_anim_ime_nobj_update (vid, OPC_ANIM_OBJTYPE_NODE, mobile_name_next, 	OPC_ANIM_OBJ_ATTR_ICON, "ace_qv_exclude" , OPC_EOL);
				
				//op_anim_ime_nobj_update (vid, OPC_ANIM_OBJTYPE_NODE, mobile_name_next, 	OPC_ANIM_OBJ_ATTR_ICON, "blank" , OPC_EOL);
				//op_anim_ime_nobj_update (vid, OPC_ANIM_OBJTYPE_NODE, mobile_name_next, 	OPC_ANIM_OBJ_ATTR_ICON, "blank" , OPC_EOL);
				//op_anim_ime_nobj_update (vid, OPC_ANIM_OBJTYPE_NODE, mobile_name,OPC_ANIM_OBJ_ATTR_XPOS, tgt_vx, OPC_EOL); 
				//op_anim_ime_nobj_update (vid, OPC_ANIM_OBJTYPE_NODE, mobile_name,OPC_ANIM_OBJ_ATTR_XPOS, tgt_vy, OPC_EOL);
				
				op_anim_igp_macro_draw (vid, OPC_ANIM_RETAIN, mid, 	OPC_ANIM_REG_A_STR, mobile_name, OPC_ANIM_REG_D_STR, mobile_name_next, OPC_EOL);
				//op_anim_igp_macro_redraw (vid, mid, OPC_ANIM_ERASE_MODE_XOR, OPC_ANIM_REG_A_STR, mobile_name, OPC_ANIM_REG_D_STR, mobile_name_next,OPC_EOL);

				//op_anim_ime_nobj_update (vid, OPC_ANIM_OBJTYPE_NODE, mobile_name, 	OPC_ANIM_OBJ_ATTR_ICON, "ace_qv_exclude" , OPC_EOL);
				//op_anim_igp_macro_draw (vid, OPC_ANIM_RETAIN, mid, 	OPC_ANIM_REG_A_STR, "Station_0", OPC_ANIM_REG_D_STR, "Station_1", OPC_EOL);
				//op_anim_igp_line_draw (vid, (OPC_ANIM_COLOR_BLACK | OPC_ANIM_RETAIN), tgt_vx, tgt_vy , tgt_vx_1, tgt_vy_1);
				}
		
		
		
		op_anim_force ();
		op_intrpt_schedule_self(op_sim_time() + 10.0, 3);
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
	void measure_inter_adv (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_measure_inter_adv_init (int * init_block_ptr);
	void _op_measure_inter_adv_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_measure_inter_adv_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_measure_inter_adv_alloc (VosT_Obtype, int);
	void _op_measure_inter_adv_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
measure_inter_adv (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (measure_inter_adv ());

		{
		/* Temporary Variables */
		Packet* pkptr;
		/* End of Temporary Variables */


		FSM_ENTER ("measure_inter_adv")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (IDLE) enter executives **/
			FSM_STATE_ENTER_UNFORCED (0, "IDLE", state0_enter_exec, "measure_inter_adv [IDLE enter execs]")
				FSM_PROFILE_SECTION_IN ("measure_inter_adv [IDLE enter execs]", state0_enter_exec)
				{
				
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (1,"measure_inter_adv")


			/** state (IDLE) exit executives **/
			FSM_STATE_EXIT_UNFORCED (0, "IDLE", "measure_inter_adv [IDLE exit execs]")
				FSM_PROFILE_SECTION_IN ("measure_inter_adv [IDLE exit execs]", state0_exit_exec)
				{
				/*探测数据包到达，接收并转发至上层处理，移动节点*/
				if(LOWER_STRM)
					{
						pkptr = op_pk_get(op_intrpt_strm());
						op_pk_send(pkptr,UPPER_OUT_STRM_INDEX);
					}
				}
				FSM_PROFILE_SECTION_OUT (state0_exit_exec)


			/** state (IDLE) transition processing **/
			FSM_PROFILE_SECTION_IN ("measure_inter_adv [IDLE trans conditions]", state0_trans_conds)
			FSM_INIT_COND (UPPER_STRM)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("IDLE")
			FSM_PROFILE_SECTION_OUT (state0_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, ;, "UPPER_STRM", "", "IDLE", "TRANSMIT", "tr_39", "measure_inter_adv [IDLE -> TRANSMIT : UPPER_STRM / ]")
				FSM_CASE_TRANSIT (1, 0, state0_enter_exec, ;, "default", "", "IDLE", "IDLE", "tr_10", "measure_inter_adv [IDLE -> IDLE : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (TRANSMIT) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "TRANSMIT", state1_enter_exec, "measure_inter_adv [TRANSMIT enter execs]")
				FSM_PROFILE_SECTION_IN ("measure_inter_adv [TRANSMIT enter execs]", state1_enter_exec)
				{
				/*首先检查上次的数据包是否收发完毕,全部丢失否*/
				pkptr = op_pk_get(op_intrpt_strm());
				
				/*没有节点收到数据，动画统计画线*/
				if(RCVD_NODE_NUM == 0)
					{
						op_sim_message("Measurment End, Draw Transmission Span",OPC_NIL);
						op_intrpt_schedule_self(op_sim_time(),1);
					}
				else 
					{
						op_pk_send(pkptr,LOWER_OUT_STRM_INDEX);
						op_intrpt_schedule_self(op_sim_time(),0);
						RCVD_NODE_NUM = 0;
					}
				}
				FSM_PROFILE_SECTION_OUT (state1_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"measure_inter_adv")


			/** state (TRANSMIT) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "TRANSMIT", "measure_inter_adv [TRANSMIT exit execs]")


			/** state (TRANSMIT) transition processing **/
			FSM_PROFILE_SECTION_IN ("measure_inter_adv [TRANSMIT trans conditions]", state1_trans_conds)
			FSM_INIT_COND (SELF_INTRPT_Animation)
			FSM_TEST_COND (SELF_INTRPT_SEND)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("TRANSMIT")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 3, state3_enter_exec, radioTransimissionAreaDraw();, "SELF_INTRPT_Animation", "radioTransimissionAreaDraw()", "TRANSMIT", "Animation", "tr_41", "measure_inter_adv [TRANSMIT -> Animation : SELF_INTRPT_Animation / radioTransimissionAreaDraw()]")
				FSM_CASE_TRANSIT (1, 0, state0_enter_exec, ;, "SELF_INTRPT_SEND", "", "TRANSMIT", "IDLE", "tr_42", "measure_inter_adv [TRANSMIT -> IDLE : SELF_INTRPT_SEND / ]")
				FSM_CASE_TRANSIT (2, 1, state1_enter_exec, ;, "default", "", "TRANSMIT", "TRANSMIT", "tr_12", "measure_inter_adv [TRANSMIT -> TRANSMIT : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (Init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (2, "Init", "measure_inter_adv [Init enter execs]")
				FSM_PROFILE_SECTION_IN ("measure_inter_adv [Init enter execs]", state2_enter_exec)
				{
				/*获取节点和进程的id号*/
				own_id = op_id_self();
				
				node_id = op_topo_parent(own_id);
				
				/*获取进程的属性，获得设置参数*/
				op_ima_obj_attr_get(node_id,"Cluster Header Enable",&cluster_enable);
				
				if(cluster_enable == OPC_TRUE)
					{
						total_node_num = op_topo_object_count (OPC_OBJTYPE_NDMOB);
					}
					
				mac_id = nodeMacAddressGet(nodeMacListIndexGet(node_id));
				
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (Init) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "Init", "measure_inter_adv [Init exit execs]")
				FSM_PROFILE_SECTION_IN ("measure_inter_adv [Init exit execs]", state2_exit_exec)
				{
				if(cluster_enable == OPC_TRUE)
					{
						//printf("\nEnter the Animation Function");
				/*获取本进程和node节点的句柄*/
				//own_id = op_id_self();
				//node_objid = op_topo_parent(my_own_id);
				
				op_ima_obj_attr_get(node_id,"Animation Enable",&Animation_Enable);
				
				if(Animation_Enable)
					{
				/*得到网络场景名称，net_topo是场景名称*/
				op_sim_info_get(OPC_STRING,OPC_SIM_INFO_NETWORK_NAME ,net_topo);
				
				/*得到自定义动画标签*/
				vid = op_anim_lprobe_anvid ("wireless");
				
				/*测试自定义动画标签是否打开成功*/
				if (op_prg_odb_ltrace_active("anim")==OPC_TRUE)
					{
						printf("vid %d\n",vid);
					}
				
				/*得到场景的名称*/
				op_ima_obj_attr_get(op_topo_parent(node_id),"name",node_name);
				if(strcmp("top",node_name) == 0)
					{
						sprintf(subnet_name,"top","");
					}
				else 
					sprintf(subnet_name,"top.%s",node_name);
				
				if (op_prg_odb_ltrace_active("anim")==OPC_TRUE)
					{
					printf("subnet %s\n",subnet_name);
					}
				
				/*将场景内容显示在动画浏览器中*/
				op_anim_ime_nmod_draw (vid, OPC_ANIM_MODTYPE_NETWORK,net_topo,subnet_name,OPC_ANIM_MOD_OPTION_NONE,OPC_ANIM_DEFPROPS);
				
				/*设置无线效果动画宏*/
				
				//small_line();
				
				//big_line();
				//thick_line();
				
				
				if(OPC_FALSE)
				{
					/*能量动画宏*/
				energy_mid = op_anim_macro_create("battery_capacity");
								
				/* Determine position of NE corner of the station icon in the viewer window */
				op_anim_mme_nobj_pos (energy_mid, OPC_ANIM_OBJTYPE_NODE, 
					OPC_ANIM_REG_A_STR, OPC_ANIM_REG_X_INT, OPC_ANIM_REG_Y_INT,
					OPC_ANIM_VERTEX_ICON_NE);
								
				/* Calculate postion of queue meter */
				op_anim_mgp_reg_set (energy_mid, OPC_ANIM_REG_C_INT, 5);
				op_anim_mgp_arop (energy_mid, OPC_ANIM_REG_X_INT, 
					OPC_ANIM_AROP_ADD, OPC_ANIM_REG_C_INT,
					OPC_ANIM_REG_X_INT);
								
				op_anim_mgp_reg_set (energy_mid, OPC_ANIM_REG_C_INT, 2);
				op_anim_mgp_arop (energy_mid, OPC_ANIM_REG_X_INT,
					OPC_ANIM_AROP_ADD, OPC_ANIM_REG_C_INT,
					OPC_ANIM_REG_A_INT);
								
				op_anim_mgp_arop (energy_mid, OPC_ANIM_REG_Y_INT,
					OPC_ANIM_AROP_ADD, OPC_ANIM_REG_C_INT,
					OPC_ANIM_REG_B_INT);
								    
				/* Compute text position offset */
				op_anim_mgp_reg_set (energy_mid, OPC_ANIM_REG_C_INT, 2);                            
				op_anim_mgp_arop (energy_mid, OPC_ANIM_REG_Y_INT,
					OPC_ANIM_AROP_SUBTRACT, OPC_ANIM_REG_C_INT, 
					OPC_ANIM_REG_D_INT);
								
				op_anim_mgp_setup_end (energy_mid);
								
				op_anim_mgp_text_draw (energy_mid, OPC_ANIM_COLOR_WHITE | OPC_ANIM_ALIGNV_BOTTOM |
					OPC_ANIM_FONT_OPEN_10, OPC_ANIM_REG_X_INT, OPC_ANIM_REG_D_INT,
					OPC_ANIM_REG_E_STR);
				
				op_anim_macro_close (energy_mid); /* End of macro */
				}
				}
				
					}
				
				}
				FSM_PROFILE_SECTION_OUT (state2_exit_exec)


			/** state (Init) transition processing **/
			FSM_TRANSIT_FORCE (0, state0_enter_exec, ;, "default", "", "Init", "IDLE", "tr_38", "measure_inter_adv [Init -> IDLE : default / ]")
				/*---------------------------------------------------------*/



			/** state (Animation) enter executives **/
			FSM_STATE_ENTER_UNFORCED (3, "Animation", state3_enter_exec, "measure_inter_adv [Animation enter execs]")
				FSM_PROFILE_SECTION_IN ("measure_inter_adv [Animation enter execs]", state3_enter_exec)
				{
				
				/*绘画出AP Node节点的覆盖范围*/
				//radioTransimissionAreaDraw();
				
				
				
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (7,"measure_inter_adv")


			/** state (Animation) exit executives **/
			FSM_STATE_EXIT_UNFORCED (3, "Animation", "measure_inter_adv [Animation exit execs]")
				FSM_PROFILE_SECTION_IN ("measure_inter_adv [Animation exit execs]", state3_exit_exec)
				{
				if(SIM_END)
					{
						op_sim_end("Animation End, Terminate Process",OPC_NIL,OPC_NIL,OPC_NIL);
					}
				}
				FSM_PROFILE_SECTION_OUT (state3_exit_exec)


			/** state (Animation) transition processing **/
			FSM_TRANSIT_FORCE (3, state3_enter_exec, ;, "default", "", "Animation", "Animation", "tr_43", "measure_inter_adv [Animation -> Animation : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (2,"measure_inter_adv")
		}
	}




void
_op_measure_inter_adv_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_measure_inter_adv_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_measure_inter_adv_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_measure_inter_adv_svar function. */
#undef mac_id
#undef own_id
#undef node_id
#undef cluster_enable
#undef total_node_num
#undef Animation_Enable
#undef net_topo
#undef subnet_name
#undef node_name
#undef line_ptr
#undef new_variable

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_measure_inter_adv_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_measure_inter_adv_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (measure_inter_adv)",
		sizeof (measure_inter_adv_state));
	*init_block_ptr = 4;

	FRET (obtype)
	}

VosT_Address
_op_measure_inter_adv_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	measure_inter_adv_state * ptr;
	FIN_MT (_op_measure_inter_adv_alloc (obtype))

	ptr = (measure_inter_adv_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "measure_inter_adv [Init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_measure_inter_adv_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	measure_inter_adv_state		*prs_ptr;

	FIN_MT (_op_measure_inter_adv_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (measure_inter_adv_state *)gen_ptr;

	if (strcmp ("mac_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->mac_id);
		FOUT
		}
	if (strcmp ("own_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->own_id);
		FOUT
		}
	if (strcmp ("node_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->node_id);
		FOUT
		}
	if (strcmp ("cluster_enable" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->cluster_enable);
		FOUT
		}
	if (strcmp ("total_node_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->total_node_num);
		FOUT
		}
	if (strcmp ("Animation_Enable" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Animation_Enable);
		FOUT
		}
	if (strcmp ("net_topo" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->net_topo);
		FOUT
		}
	if (strcmp ("subnet_name" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->subnet_name);
		FOUT
		}
	if (strcmp ("node_name" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->node_name);
		FOUT
		}
	if (strcmp ("line_ptr" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->line_ptr);
		FOUT
		}
	if (strcmp ("new_variable" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->new_variable);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

