/* dra_closure.ps.c */                                                       
/* TMM enabled closure model. This stage determines if communication is 	*/
/* possible	between sites. When executing during a TMM simulation, this 	*/
/* stage will utilize a TMM propagation model to determine closure and 		*/
/* calculate a path loss value.												*/
/* The path loss value will be used later on in "power" pipeline stage.		*/

/****************************************/
/*  	  Copyright (c) 1993-2008		*/
/*      by OPNET Technologies, Inc.     */
/*		(A Delaware Corporation)		*/
/*	7255 Woodmont Av., Suite 250  		*/
/*     Bethesda, MD 20814, U.S.A.       */
/*			All Rights Reserved.		*/
/****************************************/

#include <math.h>
#include <string.h>
#include "opnet.h"

/* This stage will operate in three basic modes:						*/
/* 		mode #1 The tranmission path will never be occluded. In this 	*/
/* 				mode OPC_TDA_RA_CLOSURE will always be set to OPC_TRUE.	*/
/*				This is the default mode for Wireless LAN model when 	*/
/*				TMM is not enabled.										*/
/*		mode #2 basic free-space propagation closure is Line-of-Sight	*/
/* 				with a spherical earth model.							*/
/* 				for all	transmissions.									*/
/* 		mode #3 Utilize a TMM propagation model.						*/

/****** enum types definition.	******/
typedef enum DraT_Closure_Method
	{
	DraC_Line_Of_Sight_Never_Occluded,
	DraC_Earth_Line_Of_Sight,
	DraC_Terrain_Modeling
	} DraT_Closure_Method;

/***** Global variables			*****/
/* Variables are initialized once and then used throughout the 			*/
/* simulation duration.													*/

/* This variable will contain the closure method used during the whole	*/
/* simulation. Its possible values correspond to those defined under 	*/
/* DraT_Closure_Method type. It is initialized to Earth_Line_Of_Sight, 	*/
/* however its final value will be set by tmm_closure_init().			*/
static DraT_Closure_Method			DraS_Active_Closure_Method = DraC_Earth_Line_Of_Sight;

/* When we are in mode #3, this is the TMM propagation model used for	*/
/* calculating path loss.												*/
static TmmT_Propagation_Model *		DraS_Closure_Prop_Model_Ptr = OPC_NIL;

/* When in mode #3, this variable is the name of the propagation model	*/
/* being used.															*/
static const char *					DraS_Closure_Tmm_Prop_Model_Name;

/* Messages detailing packet transmission are logged when the 			*/
/* tmm_verbose mode is requested. (either by the environment attribute,	*/
/* or via a call to tmm_verbose_set (1)). 								*/
Log_Handle							DraS_TMM_Verbose_Log_H;

/***** Function prototypes.	*****/
static void	tmm_closure_init 				(OP_SIM_CONTEXT_ARG_OPT);
static void dra_non_tmm_closure_method_set 	(OP_SIM_CONTEXT_ARG_OPT);
static void	simple_earth_LOS_closure 		(OP_SIM_CONTEXT_ARG_OPT_COMMA Packet * pkptr);
static void	tmm_model_closure_calc 			(OP_SIM_CONTEXT_ARG_OPT_COMMA Packet * pkptr);

/***** constants *****/
/* Threshold value in dB. Path loss beyond this threshold will be 		*/
/* considered to be so weak as to not have closure with the receiver.	*/
#define LOSS_CUTOFF_THRESHOLD_DB	-140.0

/* Name of the global attribute used by the Wireless LAN model to 		*/
/* select between the two non-TMM closure methods.	This global 		*/
/* attribute is defined at wlan_dispatch.pr.m.							*/							
#define CLOSURE_METHOD_GLOBAL_ATTRIBUTE_NAME	"Closure Method (non-TMM)"


/***** pipeline procedure *****/

#if defined (__cplusplus)
extern "C"
#endif
void
dra_closure_measure_mt (OP_SIM_CONTEXT_ARG_OPT_COMMA Packet * pkptr)
	{
	static int	closure_initialized = 0;

	FIN_MT (dra_closure_measure (pkptr));

	/* When using a parallel kernel, several threads of computation	*/
	/* might reach that point at the same time. All we need is to	*/
	/* ensure that tmm_closure_init is invoked only once.			*/
	/* This simple requirement gives us some flexibility.			*/
	/* closure_initialized might be set to 1 while another thread	*/
	/* is reading the value.  That other thread will get some value	*/
	/* (0, 1, or some garbled bit pattern).							*/
	/* If 1 or the garbled pattern is read, then the test will fail	*/
	/* but since this results from the initialization already done	*/
	/* then it is fine.  											*/
	/* If 0 is read, then we try to create a mutex and lock it.		*/
	/* There might be several calls to op_prg_mt_mutex_create, but	*/
	/* all of them will return the same mutex pointer.  Writing the	*/
	/* same value in a global variable should survive the 			*/
	/* non-atomicity.												*/
	if (!closure_initialized)
		{
		op_prg_mt_global_lock ();
		
		/* Check the variable again.  If another thread also 		*/
		/* "created" the mutex and already did the initialization, 	*/
		/* then the value is going to be 1 and the test will fail.	*/
		if (!closure_initialized)
			{
			/* This function will determine the behavior for 		*/
			/* closure by setting static variables 					*/
			/* (DraS_Active_Closure_Method, 						*/
			/* DraS_Closure_Prop_Model_Ptr).						*/
			tmm_closure_init (OP_SIM_CONTEXT_PTR_OPT);
			
			/* Only once the initialization has been done, we 		*/
			/* reset the flag to make sure another thread is not 	*/
			/* going to rush ahead and assume the initialization is	*/
			/* done. In the worst case, every thread will have 		*/
			/* "created" the same mutex and will be serialized on 	*/
			/* the mutex lock.										*/
			closure_initialized = 1;
			}
		
		op_prg_mt_global_unlock ();
		}

	/* This stage has three modes of operation. (See the comments 	*/
	/* above, near the definition of DraT_Closure_Method type).		*/
	switch (DraS_Active_Closure_Method)
		{
		case DraC_Line_Of_Sight_Never_Occluded:
			/* mode #1 The tranmission path will never be occluded.	*/
			/* Set OPC_TDA_RA_CLOSURE to OPC_TRUE for all 			*/
			/* transmissions.										*/
			op_td_set_int (pkptr, OPC_TDA_RA_CLOSURE, OPC_TRUE);
		break;
		case DraC_Earth_Line_Of_Sight:
			/* mode #2: basic spherical earth closure.				*/
			simple_earth_LOS_closure (OP_SIM_CONTEXT_PTR_OPT_COMMA pkptr);
		break;
		case DraC_Terrain_Modeling:
			/* mode #3: Utilize TMM propagation model.				*/
			tmm_model_closure_calc (OP_SIM_CONTEXT_PTR_OPT_COMMA pkptr);
		break;
		}
	FOUT
	}


/***** Support functions *****/

static void
tmm_closure_init (OP_SIM_CONTEXT_ARG_OPT)
	{
	int				using_tmm;
	Log_Handle		tmm_problem_log_handle;
	int				load_successful;
	char			line0_buf [512];
	char			line1_buf [512];

	/** This function is invoked once at the start of the simulation.	**/
	/** Options that will be used throughout the simulation duration 	**/
	/** are established in this function.								**/
	FIN_MT (tmm_closure_init ());

	/* Define a log handler for all messages related to the				*/
	/* initialization.													*/	
	tmm_problem_log_handle = op_prg_log_handle_create (
		OpC_Log_Category_Configuration, 
		"TMM", "closure stage loading of propagation model", 20);
	
	/* If the nodes are out of valid computation areas such as for 		*/
	/* Longley-Rice, a very large number of log	entries could be 		*/
	/* generated.  Stop recording after 500.							*/
	DraS_TMM_Verbose_Log_H = op_prg_log_handle_create (
		OpC_Log_Category_Lowlevel, 
		"TMM", "path loss calculation", 500);

	/* Determine if we are executing as a simulation that wants to use	*/
	/* TMM.																*/
	if (prg_env_attr_value_get (PrgC_Env_Attr_Boolean, TMMC_ENV_SIMULATE, 
			&using_tmm) == PrgC_Compcode_Failure)
		using_tmm = OPC_FALSE;

	/* Check is TMM is active.											*/
	if (using_tmm == OPC_FALSE)
		{
		/* Simulation is not using TMM for path loss calculations or	*/
		/* closure. Set the non-TMM closure method that will be used.	*/
		dra_non_tmm_closure_method_set (OP_SIM_CONTEXT_PTR_OPT);
		}
	else
		{
		/* Simulation is using TMM for propagation.						*/ 

		/* Initialize a flag to check for a successful load of the TMM	*/
		/* module.														*/
		load_successful = OPC_FALSE;
		
		/* Attempt to load the default TMM propagation model.			*/
		DraS_Closure_Tmm_Prop_Model_Name = tmm_default_propagation_model_get ();
		if (strcmp ("NONE", DraS_Closure_Tmm_Prop_Model_Name) == 0)
			{
			
			/* Propagation model called NONE, is special and indicates	*/
			/* that no default propagation model is set. Use free space.*/
			load_successful = OPC_FALSE;

			/* Simulation is not using TMM for path loss calculations 	*/
			/* or closure. Set the non-TMM closure method that will be 	*/
			/* used.													*/
			dra_non_tmm_closure_method_set (OP_SIM_CONTEXT_PTR_OPT);

			FOUT
			}
		
		/* Read the TMM propagation model.								*/
		DraS_Closure_Prop_Model_Ptr = tmm_propagation_model_get (DraS_Closure_Tmm_Prop_Model_Name);
		
		/* Check if the TMM propagation model was not successfully read.	*/	
		if (DraS_Closure_Prop_Model_Ptr == OPC_NIL)
			{
			/* Failed to load the requested model. Most likely because	*/
			/* is missing model from mod_dirs. 							*/
			load_successful = OPC_FALSE;

			/* Prepare a simulation message.							*/
			sprintf (line0_buf, "TMM: unable to load propagation model (%s). ",
				DraS_Closure_Tmm_Prop_Model_Name);
			strcpy (line1_buf, "Using default closure instead.");

			/* Print simulation message.								*/
			op_sim_message (line0_buf, line1_buf);

			/* Write a log message.										*/
			op_prg_log_entry_write (tmm_problem_log_handle, 
				"Pipeline stage 'closure' during initialization for\n"
				"%s\n"
				"%s\n"
				"\n"
				"Check that your mod_dirs contains all 3 of the needed\n"
				"propagation model files (.prop.d, .prop.p and .prop.[so/dll]).\n",
				line0_buf,
				line1_buf);
			}
		else if (DraS_Closure_Prop_Model_Ptr->initialized_ok_flag == OPC_FALSE)
			{
			/* Propagation model's init function has set the flag		*/
			/* "initialized_ok_flag" to indicate some problem inside	*/
			/* of the propagation model.								*/
			load_successful = OPC_FALSE;

			/* Prepare a simulation message.							*/
			sprintf (line0_buf,
				"TMM propagation model (%s) reported an initialization problem.", 
				DraS_Closure_Tmm_Prop_Model_Name);
			strcpy (line1_buf, "Using default closure instead.");

			/* Print simulation message.								*/
			op_sim_message (line0_buf, line1_buf);

			/* Write a log message.										*/
			op_prg_log_entry_write (tmm_problem_log_handle, 
				"Pipeline stage 'closure' during initialization for TMM:\n"
				"%s\n"
				"%s\n",
				line0_buf,
				line1_buf);
			}
		else
			{
			/* We've successfully obtained the propagation model. We 	*/
			/* are going to use the path loss function from the model 	*/
			/* we have loaded.											*/
			load_successful = OPC_TRUE;			
			
			/* Indicate that the TMM closure method will be used.		*/	
			DraS_Active_Closure_Method = DraC_Terrain_Modeling;
			
			/* If verbose, write a log message.						 	*/
			if (tmm_verbose_get ())
				{
				sprintf (line0_buf, 
					"TMM initialization: successfully loaded the propagation model (%s)",
					DraS_Closure_Tmm_Prop_Model_Name);
				op_prg_log_entry_write (tmm_problem_log_handle,
					line0_buf);
				}
			}

		/* Check if the TMM module was not successfully loaded.			*/
		if (load_successful == OPC_FALSE)
			{
			/* Simulation is not using TMM for path loss calculations 	*/
			/* or closure. Set the non-TMM closure method that will be	*/
			/* used.													*/
			dra_non_tmm_closure_method_set (OP_SIM_CONTEXT_PTR_OPT);
			}
		}

	FOUT
	}

static void 
dra_non_tmm_closure_method_set (OP_SIM_CONTEXT_ARG_OPT)
	{
	int				attr_value;

	/** This function is called to get the closure method that the 		**/
	/** simulation will use if TMM is not active. There are two more 	**/
	/** available closure methods (besides TMM) that may be used:		**/
	/** 	-The transmission path will never be occluded. 				**/
	/**		-Line-of-sight with a spherical earth model					**/	
	/**																	**/	
	/** There are two cases on how the non-TMM closure method will be 	**/
	/** selected:														**/
	/**																	**/	
	/** 	A) For Wireless LAN standard models: The closure method is 	**/
	/** 	selected based on the value set on the global attribute:	**/
	/** 	  CLOSURE_METHOD_GLOBAL_ATTRIBUTE_NAME						**/
	/**																	**/
	/** 	B) Any other custom wireless models: If the global 			**/
	/** 	attribute is not defined in the simulation, then the Earth	**/
	/** 	Line-of-Sight method will always be selected.				**/
	
	FIN_MT(dra_non_tmm_closure_method_set (void));
	
	/* Check if the global attribute exists. If it does, apply the case	*/
	/* for Wireless LAN (A), otherwise this indicates that custom		*/
	/* wireless models are being used (apply case B).					*/	
	if (op_ima_sim_attr_exists (CLOSURE_METHOD_GLOBAL_ATTRIBUTE_NAME))
		{
		if (op_ima_sim_attr_get_int32 (CLOSURE_METHOD_GLOBAL_ATTRIBUTE_NAME, &attr_value) == OPC_COMPCODE_SUCCESS)
			{
			/* Set the closure method based on the global attribute.		*/		
			DraS_Active_Closure_Method = (DraT_Closure_Method) attr_value;
			}
		else
			{
			/* For non-Wireless LAN models always use Earth Line-of-Sight.	*/
			DraS_Active_Closure_Method = DraC_Line_Of_Sight_Never_Occluded;
			}
		}
	else
		{
		/* For non-Wireless LAN models always use Earth Line-of-Sight.	*/
		DraS_Active_Closure_Method = DraC_Earth_Line_Of_Sight;
		}
	FOUT;
	}

static void
tmm_model_closure_calc (OP_SIM_CONTEXT_ARG_OPT_COMMA Packet * pkptr)
	{
	TmmT_Position	tx_position;
	TmmT_Position	rx_position;
	void *			pipeline_invocation_state_ptr;
	double			tx_base_freq;
	double			tx_bandwidth;
	double			tx_center_freq;
	int				verbose_active;
	int				trace_active;
	int				str_index;
	char *			msg_str_ptrs [TMMC_LOSS_MESSAGE_BUF_NUM_STRS];
	char			log_str_buf [16 * TMMC_LOSS_MESSAGE_BUF_STR_SIZE];

	double			tmm_model_path_loss_dB;
	TmmT_Loss_Status	tmm_model_loss_status;
	char 			tmm_model_msg_buffer_v [TMMC_LOSS_MESSAGE_BUF_NUM_STRS] [TMMC_LOSS_MESSAGE_BUF_STR_SIZE];
	char			msg_buf0 [256];
	char			msg_buf1 [256];
	char			msg_buf2 [256];
	char			msg_buf3 [256];
	char			msg_buf4 [256];
	
	//Objid           node_objid;
	Objid           rx_objid;
	Objid           node_objid;
	char            node_name[256];
	//char            msg[256];
	/** Call the propagation model's path loss calculation method.	**/
	/** The function can report:									**/
	/**		a signal loss value										**/
	/**		a error condition (e.g. invalid elevation)				**/
	/**		a lack of link closure - so no communication is 		**/
	/** 	possible.												**/
	FIN_MT (tmm_model_closure_calc (pkptr));


    /* Get transmission frequency in Hz. */
	tx_base_freq = op_td_get_dbl (pkptr, OPC_TDA_RA_TX_FREQ);
	tx_bandwidth = op_td_get_dbl (pkptr, OPC_TDA_RA_TX_BW);
	tx_center_freq = tx_base_freq + (tx_bandwidth / 2.0);

	/* Get transmitter's location. */
	tx_position.latitude = op_td_get_dbl (pkptr, OPC_TDA_RA_TX_LAT);
	tx_position.longitude = op_td_get_dbl (pkptr, OPC_TDA_RA_TX_LONG);
	tx_position.elevation = op_td_get_dbl (pkptr, OPC_TDA_RA_TX_ALT);

	/* Get receiver's location. */
	rx_position.latitude = op_td_get_dbl (pkptr, OPC_TDA_RA_RX_LAT);
	rx_position.longitude = op_td_get_dbl (pkptr, OPC_TDA_RA_RX_LONG);
	rx_position.elevation = op_td_get_dbl (pkptr, OPC_TDA_RA_RX_ALT);

	rx_objid = op_td_get_int(pkptr, OPC_TDA_RA_RX_OBJID);
	op_ima_obj_attr_get(op_topo_parent(rx_objid),"name",node_name);
	//printf("\n%s altitude: %f.",node_name,rx_position.elevation);
	/* Propagation models shipped by OPNET don't use any extra state	*/
	/* so just for clarity set the local state pointer to null.			*/
	/* User created propagation models can pass any value they wish.	*/
	pipeline_invocation_state_ptr = OPC_NIL;

	/* Determine if TMM verbose is requested. */
	/* This can be set using the tmm_verbose environment attribute */
	trace_active = op_prg_odb_trace_active ()
		|| op_prg_odb_pktrace_active (pkptr);
	verbose_active = tmm_verbose_get ();
	/*** Call the propagation model's path_loss_calc_method.			***/

	/* All TMM propagation models have a path loss function prototype	*/
	/* like this (from tmm.h):											*/
	/*
		double TmmT_Path_Loss_Calc_Method (
				TmmT_Propagation_Model * 	model_ptr,
				void * 						tda_state_ptr,
				TmmT_Position * 			tx_position_ptr,
				TmmT_Position * 			rx_position_ptr,
				double 						center_frequency, 
				double 						bandwidth,
				int							verbose,
				TmmT_Loss_Status * 			status_ptr, 
				char  						message_buffer_v [TMMC_LOSS_MESSAGE_BUF_NUM_STRS] [TMMC_LOSS_MESSAGE_BUF_STR_SIZE]);
	*/
	for (str_index = 0; str_index < TMMC_LOSS_MESSAGE_BUF_NUM_STRS; str_index++)
		tmm_model_msg_buffer_v [str_index] [0] = '\0';
	tmm_model_path_loss_dB = 
		DraS_Closure_Prop_Model_Ptr->path_loss_calc_method (
			DraS_Closure_Prop_Model_Ptr,
			pipeline_invocation_state_ptr,
			&tx_position,
			&rx_position,
			tx_center_freq,
			tx_bandwidth,
			verbose_active || trace_active,
			&tmm_model_loss_status,
			tmm_model_msg_buffer_v );

	if (tmm_model_loss_status == TmmC_Loss_No_Closure)
		{
		/* The propagation model has reported that signal	*/
		/* from this packet will not be observable by the	*/
		/* receiver. This packet fails closure.				*/

		if (trace_active || verbose_active)
			{
			sprintf (msg_buf0, "Transmission Closure: Propagation Model %s:",
				DraS_Closure_Tmm_Prop_Model_Name );
			sprintf (msg_buf1, "Path loss between transmitter (ID %d) and receiver (ID %d)",
				op_td_get_int (pkptr, OPC_TDA_RA_TX_OBJID),
				op_td_get_int (pkptr, OPC_TDA_RA_RX_OBJID));
			strcpy (msg_buf2, "fails closure. Packet can not be received by the receiver");
			strcpy (msg_buf3, "Additional messages reported by propagation model follow:");
			for (str_index = 0; str_index < TMMC_LOSS_MESSAGE_BUF_NUM_STRS; str_index++)
				{
				if (tmm_model_msg_buffer_v [str_index] [0] == '\0')
					{
					/* A null pointer to op_prg_odb_print_ will signify end of arguments */
					msg_str_ptrs [str_index] = OPC_NIL;
					break;
					}
				else
					{
					msg_str_ptrs [str_index] = &tmm_model_msg_buffer_v [str_index] [0];
					}
				}
			if (msg_str_ptrs [0] == OPC_NIL)
				{
				strcpy (msg_buf3, "<no messages reported by the propagation model>");
				}
			}
		if (trace_active)
			{
			/* Print message to ODB */
			op_prg_odb_print_major (msg_buf0, msg_buf1, msg_buf2, msg_buf3,
				msg_str_ptrs [0], msg_str_ptrs [1], msg_str_ptrs [2], msg_str_ptrs [3],
				msg_str_ptrs [4], OPC_NIL);
			}
		if (verbose_active)
			{
			/* Log a simulation-log message becasue tmm_verbose is true.	*/

			/* Entries in the log are added via 1 long format string		*/
			log_str_buf [0] = '\0';
			strcat (log_str_buf, msg_buf0);
			strcat (log_str_buf, "\n");

			strcat (log_str_buf, "  ");
			strcat (log_str_buf, msg_buf1);
			strcat (log_str_buf, "\n");

			strcat (log_str_buf, "  ");
			strcat (log_str_buf, msg_buf2);
			strcat (log_str_buf, "\n");

			strcat (log_str_buf, "  ");
			strcat (log_str_buf, msg_buf3);
			strcat (log_str_buf, "\n");

			for (str_index = 0; str_index < TMMC_LOSS_MESSAGE_BUF_NUM_STRS; str_index++)
				{
				if (tmm_model_msg_buffer_v [str_index] [0] != '\0')
					{
					strcat (log_str_buf, "    ");
					strcat (log_str_buf, tmm_model_msg_buffer_v [str_index]);
					strcat (log_str_buf, "\n");
					}
				else
					{
					/* Empty string, so no more string in message buffer from propagation model	*/
					break;
					}
				}

			/* Log the message */
			op_prg_log_entry_write (DraS_TMM_Verbose_Log_H,
				log_str_buf);
			}
		op_td_set_int (pkptr, OPC_TDA_RA_CLOSURE, OPC_FALSE);
		}
	else if (tmm_model_loss_status == TmmC_Loss_Error)
		{
		/* An error condition was reported by the propagation model. 	*/
		/* We will fall back to simple freespace closure. 				*/

		sprintf (msg_buf0, "Transmission Closure: Propagation Model %s:",
			DraS_Closure_Tmm_Prop_Model_Name );
		sprintf (msg_buf1, "Model reported error in computing path between transmitter (ID %d) and receiver (ID %d)",
			op_td_get_int (pkptr, OPC_TDA_RA_TX_OBJID),
			op_td_get_int (pkptr, OPC_TDA_RA_RX_OBJID));
		strcpy (msg_buf2, "Using simple-earth closure model for this transmission");
		strcpy (msg_buf3, "Messages reported by propagation model follow:");
		for (str_index = 0; str_index < TMMC_LOSS_MESSAGE_BUF_NUM_STRS; str_index++)
			{
			if (tmm_model_msg_buffer_v [str_index] [0] == '\0')
				{
				/* A null pointer to op_prg_odb_print_ will signify end of arguments */
				msg_str_ptrs [str_index] = OPC_NIL;
				break;
				}
			else
				{
				msg_str_ptrs [str_index] = &tmm_model_msg_buffer_v [str_index] [0];
				}
			}
		if (msg_str_ptrs [0] == OPC_NIL)
			{
			strcpy (msg_buf3, "<no messages reported by the propagation model>");
			}
		if (trace_active)
			{
			/* Print message to ODB */
			op_prg_odb_print_major (msg_buf0, msg_buf1, msg_buf2, msg_buf3,
				msg_str_ptrs [0], msg_str_ptrs [1], msg_str_ptrs [2], msg_str_ptrs [3],
				msg_str_ptrs [4], OPC_NIL);
			}

		/* Entries in the log are added via 1 long format string		*/
		log_str_buf [0] = '\0';
		strcat (log_str_buf, msg_buf0);
		strcat (log_str_buf, "\n");

		strcat (log_str_buf, "  ");
		strcat (log_str_buf, msg_buf1);
		strcat (log_str_buf, "\n");

		strcat (log_str_buf, "  ");
		strcat (log_str_buf, msg_buf2);
		strcat (log_str_buf, "\n");

		strcat (log_str_buf, "  ");
		strcat (log_str_buf, msg_buf3);
		strcat (log_str_buf, "\n");

		for (str_index = 0; str_index < TMMC_LOSS_MESSAGE_BUF_NUM_STRS; str_index++)
			{
			if (tmm_model_msg_buffer_v [str_index] [0] != '\0')
				{
				strcat (log_str_buf, "    ");
				strcat (log_str_buf, tmm_model_msg_buffer_v [str_index]);
				strcat (log_str_buf, "\n");
				}
			else
				{
				/* Empty string, so no more string in message buffer from propagation model	*/
				break;
				}
			}

		/* Log message indicating that we are reverting to simple freespace model 	*/
		/* for this packet transmission.											*/
		op_prg_log_entry_write (DraS_TMM_Verbose_Log_H,
			log_str_buf);

		/* Fallback to the simple earth line-of-sight closure model 		*/
		simple_earth_LOS_closure (OP_SIM_CONTEXT_PTR_OPT_COMMA pkptr);
		}
	else
		{
		/* The propagation model was able to calculate a signal value.	*/

		if (tmm_model_path_loss_dB > LOSS_CUTOFF_THRESHOLD_DB)
			{
			/* This packet's signal is strong enough. The amount of loss is less	*/
			/* then our cutoff threshold (defined by LOSS_CUTOFF_THRESHOLD_DB). 	*/

			/* The path loss is within our threshold for reception.	*/
			/* We have link closure.								*/
			op_td_set_int (pkptr, OPC_TDA_RA_CLOSURE, OPC_TRUE);

			/* The power pipeline stage will calculate the final received	*/
			/* signal power (included antenna, tx power, etc.).				*/
			/* For now, we store just the propagation loss in the TD for 	*/
			/* received power.												*/

			/* The default power pipeline stage will use a convention of 	*/
			/* interpreting this path loss in raw form (not in dB).			*/
			op_td_set_dbl (pkptr, OPC_TDA_RA_RCVD_POWER,
				pow (10.0, tmm_model_path_loss_dB / 10.0));
			
			node_objid = op_topo_parent(op_td_get_int(pkptr, OPC_TDA_RA_RX_OBJID));
			op_ima_obj_attr_get(node_objid,"name",node_name);
			printf("\n %s Received Packet, Pathloss: %e.",node_name,pow (10.0, tmm_model_path_loss_dB / 10.0));
			
			/* Write an ODB trace message if enabled.						*/
			if (trace_active)
				{
				op_prg_odb_print_major ("Successful path loss computation reported by TMM.", OPC_NIL);
				
				/* Also append the additional messages from the propagation	*/
				/* model if any.											*/
				for (str_index = 0; str_index < TMMC_LOSS_MESSAGE_BUF_NUM_STRS; str_index++)
					{
					if (tmm_model_msg_buffer_v [str_index][0] != '\0')
						{
						op_prg_odb_print_minor (tmm_model_msg_buffer_v [str_index], OPC_NIL);
						}
					else
						{
						/* Empty string indicates end of messages.			*/
						/* Terminate the loop.								*/
						break;
						}
					}
				}
			}
		else
			{
			/* The propagation loss is so great,  that the received signal	*/
			/* is below our threshold. We choose to regard this packet as	*/
			/* failing closure.												*/
			/* See the constant value define for this threshold at the top 	*/
			/* of the file. By using this threshold, we can greatly improve	*/
			/* simulation performance by scheduling fewer pipeline events.	*/

			if (trace_active || verbose_active)
				{
				sprintf (msg_buf0, "Transmission Closure: Propagation Model %s:",
					DraS_Closure_Tmm_Prop_Model_Name );
				sprintf (msg_buf1, "Path loss between transmitter (ID %d) and receiver (ID %d)",
					op_td_get_int (pkptr, OPC_TDA_RA_TX_OBJID),
					op_td_get_int (pkptr, OPC_TDA_RA_RX_OBJID));
				sprintf (msg_buf2, "is beyond threshold of %f dB.",
					LOSS_CUTOFF_THRESHOLD_DB);
				strcpy (msg_buf3, "Transmission is considered to fail closure");
				strcpy (msg_buf4, "Additional messages reported by propagation model follow:");
				for (str_index = 0; str_index < TMMC_LOSS_MESSAGE_BUF_NUM_STRS; str_index++)
					{
					if (tmm_model_msg_buffer_v [str_index] [0] == '\0')
						{
						/* A null pointer to op_prg_odb_print_ will signify end of arguments */
						msg_str_ptrs [str_index] = OPC_NIL;
						break;
						}
					else
						{
						msg_str_ptrs [str_index] = & tmm_model_msg_buffer_v [str_index] [0];
						}
					}
				if (msg_str_ptrs [0] == OPC_NIL)
					{
					strcpy (msg_buf4, "<no messages reported by the propagation model>");
					}
				}

			if (trace_active)
				{
				/* Print message to ODB */
				op_prg_odb_print_major (msg_buf0, msg_buf1, msg_buf2, msg_buf3, msg_buf4,
					msg_str_ptrs [0], msg_str_ptrs [1], msg_str_ptrs [2], msg_str_ptrs [3],
					msg_str_ptrs [4], OPC_NIL);
				}
			if (verbose_active)
				{
				/* Log a simulation-log message because tmm_verbose is true.	*/

				/* Entries in the log are added via 1 long format string		*/
				log_str_buf [0] = '\0';
				strcat (log_str_buf, msg_buf0);
				strcat (log_str_buf, "\n");

				strcat (log_str_buf, "  ");
				strcat (log_str_buf, msg_buf1);
				strcat (log_str_buf, "\n");

				strcat (log_str_buf, "  ");
				strcat (log_str_buf, msg_buf2);
				strcat (log_str_buf, "\n");

				strcat (log_str_buf, "  ");
				strcat (log_str_buf, msg_buf3);
				strcat (log_str_buf, "\n");

				strcat (log_str_buf, "  ");
				strcat (log_str_buf, msg_buf4);
				strcat (log_str_buf, "\n");

				for (str_index = 0; str_index < TMMC_LOSS_MESSAGE_BUF_NUM_STRS; str_index++)
					{
					if (tmm_model_msg_buffer_v [str_index] [0] != '\0')
						{
						strcat (log_str_buf, "    ");
						strcat (log_str_buf, tmm_model_msg_buffer_v [str_index]);
						strcat (log_str_buf, "\n");
						}
					else
						{
						/* Empty string, so no more string in message buffer from propagation model	*/
						break;
						}
					}

				/* Log the message */
				op_prg_log_entry_write (DraS_TMM_Verbose_Log_H,
					log_str_buf);
				}
			op_td_set_int (pkptr, OPC_TDA_RA_CLOSURE, OPC_FALSE);
			}
		}

	FOUT
	}

static void
simple_earth_LOS_closure (OP_SIM_CONTEXT_ARG_OPT_COMMA Packet * pkptr)
	{
	int			occlude;
	double		tx_x, tx_y, tx_z, rx_x, rx_y, rx_z;
	double		dif_x, dif_y, dif_z, dot_rx_dif;
	double		dot_tx_dif, rx_mag, dif_mag, cos_rx_dif, sin_rx_dif;
	double		orth_drop;
	double		tx_alt, rx_alt;

	/** Compute whether or not the packet's transmitter	**/ 
	/** can reach the receiver of interest.				**/

	/** Determinate for closure is direct line of sight	**/
	/** between transmitter	and receiver. The earth is 	**/
	/** modeled as simple sphere.						**/
	FIN_MT (simple_earth_LOS_closure (pkptr));

	/* dra_closure implements a 'ray-tracing' closure model for radio	*/
	/* transmissions, testing the line segment joining tx and rx for	*/
	/* intersection with the earth (modeled as a sphere).  This default	*/
	/* model does not account for any wave bending.						*/

	/* Obtain the cartesian-geocentric coordinates for both nodes. */
	tx_x = op_td_get_dbl (pkptr, OPC_TDA_RA_TX_GEO_X);
	tx_y = op_td_get_dbl (pkptr, OPC_TDA_RA_TX_GEO_Y);
	tx_z = op_td_get_dbl (pkptr, OPC_TDA_RA_TX_GEO_Z);

	rx_x = op_td_get_dbl (pkptr, OPC_TDA_RA_RX_GEO_X);
	rx_y = op_td_get_dbl (pkptr, OPC_TDA_RA_RX_GEO_Y);
	rx_z = op_td_get_dbl (pkptr, OPC_TDA_RA_RX_GEO_Z);

	/* Calculate difference vector (transmitter to receiver). */
	dif_x = rx_x - tx_x;
	dif_y = rx_y - tx_y;
	dif_z = rx_z - tx_z;

	/* Calculate dot product of (rx) and (dif) vectors. */
	dot_rx_dif = rx_x*dif_x + rx_y*dif_y + rx_z*dif_z;

	/* If angle (rx, dif) > 90 deg., there is no occlusion. */
	if (dot_rx_dif <= 0.0) 
		occlude = OPC_FALSE;
	else
		{
		/* Calculate dot product of (tx) and (dif) vectors. */
		dot_tx_dif = tx_x*dif_x + tx_y*dif_y + tx_z*dif_z;

		/* If angle (tx, dif) < 90) there is no occlusion. */
		if (dot_tx_dif >= 0.0) 
			{
			tx_alt = op_td_get_dbl (pkptr, OPC_TDA_RA_TX_ALT);
			rx_alt = op_td_get_dbl (pkptr, OPC_TDA_RA_RX_ALT);
			if ((tx_alt < 0) || (rx_alt < 0))
				{
				occlude = OPC_TRUE;
				}
			else
				{
				occlude = OPC_FALSE;
				}
			}
		else
			{
			/* Calculate magnitude of (rx) and (dif) vectors. */
			rx_mag = sqrt (rx_x*rx_x + rx_y*rx_y + rx_z*rx_z);
			dif_mag = sqrt (dif_x*dif_x + dif_y*dif_y + dif_z*dif_z);

			/* Calculate sin (rx, dif). */
			cos_rx_dif = dot_rx_dif / (rx_mag * dif_mag);
			sin_rx_dif = sqrt (1.0 - (cos_rx_dif * cos_rx_dif));

			/* Calculate length of orthogonal drop	*/
			/* from (dif) to earth center.			*/
			orth_drop = sin_rx_dif * rx_mag;

			/* The satellites are occluded iff this distance is less than	*/
			/* the earth's radius. Depending on the characteristics of your	*/
			/* radio model, you may wish to scale this radius by 4/3 to		*/
			/* account for atmospheric refraction.							*/
			if (orth_drop < VOSC_EARTH_RADIUS_METERS)
				occlude = OPC_TRUE;
			else
				occlude = OPC_FALSE;
			}
		}

	/* Place closure status in packet transmission data block. */
	op_td_set_int (pkptr, OPC_TDA_RA_CLOSURE,
		(occlude == OPC_FALSE) ? OPC_TRUE : OPC_FALSE);

	FOUT
	}                

