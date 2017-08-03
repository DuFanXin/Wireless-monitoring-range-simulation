#ifndef ANIMATION_HEADER
#define ANIMATION_HEADER
#include <string.h>
Anvid vid;
Anvid mid;
Anvid energy_mid;
#define SOURCE_REMOTE_INTRPT_CODE 5001
#define SOURCE_REMOTE_INTRPT (op_intrpt_type() == OPC_INTRPT_REMOTE && \
	                                op_intrpt_code() == SOURCE_REMOTE_INTRPT_CODE)

#define SINK_REMOTE_INTRPT_CODE 5002
#define SINK_REMOTE_INTRPT (op_intrpt_type() == OPC_INTRPT_REMOTE && \
	                                op_intrpt_code() == SINK_REMOTE_INTRPT_CODE) 


void small_line(void)
	{
	FIN(void small_line(void));
	
	mid = op_anim_macro_create ("line_draw");

	/* Load the tx position into reg B and reg C */
	op_anim_mme_nobj_pos (mid, OPC_ANIM_OBJTYPE_NODE, OPC_ANIM_REG_A_STR,
		OPC_ANIM_REG_B_INT, OPC_ANIM_REG_C_INT, OPC_ANIM_VERTEX_ICON_CTR);

	/* Load the rx position into reg E and reg F */
	op_anim_mme_nobj_pos (mid, OPC_ANIM_OBJTYPE_NODE, OPC_ANIM_REG_D_STR,
		OPC_ANIM_REG_E_INT, OPC_ANIM_REG_F_INT, OPC_ANIM_VERTEX_ICON_CTR);


	op_anim_mgp_reg_set (mid, OPC_ANIM_REG_I_INT, 3);

	op_anim_mgp_arop (mid, OPC_ANIM_REG_I_INT,
						OPC_ANIM_AROP_ADD, OPC_ANIM_REG_B_INT,
						OPC_ANIM_REG_X_INT);

	op_anim_mgp_arop (mid, OPC_ANIM_REG_I_INT,
						OPC_ANIM_AROP_ADD, OPC_ANIM_REG_C_INT,
						OPC_ANIM_REG_Y_INT);

	op_anim_mgp_arop (mid, OPC_ANIM_REG_I_INT,
						OPC_ANIM_AROP_ADD, OPC_ANIM_REG_E_INT,
						OPC_ANIM_REG_U_INT);

	op_anim_mgp_arop (mid, OPC_ANIM_REG_I_INT,
						OPC_ANIM_AROP_ADD, OPC_ANIM_REG_F_INT,
						OPC_ANIM_REG_V_INT);


	op_anim_mgp_setup_end (mid);

	//op_anim_mgp_line_draw (mid, OPC_ANIM_COLOR_PINK|OPC_ANIM_STYLE_DASHES| OPC_ANIM_PIXOP_XOR,
	op_anim_mgp_line_draw (mid, OPC_ANIM_COLOR_PINK|OPC_ANIM_STYLE_SOLID| OPC_ANIM_PIXOP_XOR,
		OPC_ANIM_REG_B_INT, OPC_ANIM_REG_C_INT,
		OPC_ANIM_REG_E_INT, OPC_ANIM_REG_F_INT);

	op_anim_mgp_line_draw (mid, OPC_ANIM_COLOR_YELLOW|OPC_ANIM_STYLE_SOLID| OPC_ANIM_PIXOP_XOR,
		OPC_ANIM_REG_X_INT, OPC_ANIM_REG_Y_INT,
		OPC_ANIM_REG_U_INT, OPC_ANIM_REG_V_INT);

	op_anim_macro_close (mid);
	FOUT;
	}

void big_line(void)
	{
	FIN(void big_line(void));
	
	mid = op_anim_macro_create ("line_draw");

	/* Load the tx position into reg B and reg C */
	op_anim_mme_nobj_pos (mid, OPC_ANIM_OBJTYPE_NODE, OPC_ANIM_REG_A_STR,
		OPC_ANIM_REG_B_INT, OPC_ANIM_REG_C_INT, OPC_ANIM_VERTEX_ICON_CTR);

	/* Load the rx position into reg E and reg F */
	op_anim_mme_nobj_pos (mid, OPC_ANIM_OBJTYPE_NODE, OPC_ANIM_REG_D_STR,
		OPC_ANIM_REG_E_INT, OPC_ANIM_REG_F_INT, OPC_ANIM_VERTEX_ICON_CTR);


	op_anim_mgp_reg_set (mid, OPC_ANIM_REG_I_INT, 2);

	op_anim_mgp_arop (mid, OPC_ANIM_REG_I_INT,
						OPC_ANIM_AROP_ADD, OPC_ANIM_REG_B_INT,
						OPC_ANIM_REG_X_INT);

	op_anim_mgp_arop (mid, OPC_ANIM_REG_I_INT,
						OPC_ANIM_AROP_ADD, OPC_ANIM_REG_C_INT,
						OPC_ANIM_REG_Y_INT);

	op_anim_mgp_arop (mid, OPC_ANIM_REG_I_INT,
						OPC_ANIM_AROP_ADD, OPC_ANIM_REG_E_INT,
						OPC_ANIM_REG_U_INT);

	op_anim_mgp_arop (mid, OPC_ANIM_REG_I_INT,
						OPC_ANIM_AROP_ADD, OPC_ANIM_REG_F_INT,
						OPC_ANIM_REG_V_INT);

	
	op_anim_mgp_reg_set (mid, OPC_ANIM_REG_J_INT, 4);

	op_anim_mgp_arop (mid, OPC_ANIM_REG_J_INT,
						OPC_ANIM_AROP_ADD, OPC_ANIM_REG_B_INT,
						OPC_ANIM_REG_O_INT);

	op_anim_mgp_arop (mid, OPC_ANIM_REG_J_INT,
						OPC_ANIM_AROP_ADD, OPC_ANIM_REG_C_INT,
						OPC_ANIM_REG_P_INT);

	op_anim_mgp_arop (mid, OPC_ANIM_REG_J_INT,
						OPC_ANIM_AROP_ADD, OPC_ANIM_REG_E_INT,
						OPC_ANIM_REG_Q_INT);

	op_anim_mgp_arop (mid, OPC_ANIM_REG_J_INT,
						OPC_ANIM_AROP_ADD, OPC_ANIM_REG_F_INT,
						OPC_ANIM_REG_R_INT);

	op_anim_mgp_setup_end (mid);

	//op_anim_mgp_line_draw (mid, OPC_ANIM_COLOR_PINK|OPC_ANIM_STYLE_DASHES| OPC_ANIM_PIXOP_XOR,
	op_anim_mgp_line_draw (mid, OPC_ANIM_COLOR_PINK|OPC_ANIM_STYLE_SOLID| OPC_ANIM_PIXOP_XOR,
		OPC_ANIM_REG_B_INT, OPC_ANIM_REG_C_INT,
		OPC_ANIM_REG_E_INT, OPC_ANIM_REG_F_INT);

	op_anim_mgp_line_draw (mid, OPC_ANIM_COLOR_YELLOW|OPC_ANIM_STYLE_SOLID| OPC_ANIM_PIXOP_XOR,
		OPC_ANIM_REG_X_INT, OPC_ANIM_REG_Y_INT,
		OPC_ANIM_REG_U_INT, OPC_ANIM_REG_V_INT);

	op_anim_mgp_line_draw (mid, OPC_ANIM_COLOR_PINK|OPC_ANIM_STYLE_SOLID| OPC_ANIM_PIXOP_XOR,
		OPC_ANIM_REG_O_INT, OPC_ANIM_REG_P_INT,
		OPC_ANIM_REG_Q_INT, OPC_ANIM_REG_R_INT);
		
	op_anim_macro_close (mid);
	FOUT;
	}

	/*ÄÜÁ¿¶¯»­ºê*/

void thick_line(void)
	{
	FIN(void thick_line(void));
mid = op_anim_macro_create ("line_draw");

				
					/* Determine position of Center corner of the station icon in the viewer window */
					op_anim_mme_nobj_pos (mid, OPC_ANIM_OBJTYPE_NODE, 
						OPC_ANIM_REG_A_STR, OPC_ANIM_REG_X_INT, OPC_ANIM_REG_Y_INT,
						OPC_ANIM_VERTEX_ICON_CTR);
				
					op_anim_mgp_setup_end (mid);
				
					/* Draw queue meter and statistics string */
									
					op_anim_mgp_rect_fill (mid, OPC_ANIM_COLOR_PINK,
						" ",OPC_ANIM_REG_X_INT, OPC_ANIM_REG_Y_INT, OPC_ANIM_REG_Q_INT, 3);
					
					
					op_anim_mgp_text_draw (mid, OPC_ANIM_COLOR_WHITE | OPC_ANIM_ALIGNV_BOTTOM |
						OPC_ANIM_FONT_OPEN_10, OPC_ANIM_REG_X_INT, OPC_ANIM_REG_D_INT,
						OPC_ANIM_REG_E_STR);
					
					op_anim_macro_close (mid); /* End of macro */
					
	FOUT;
}

#endif
