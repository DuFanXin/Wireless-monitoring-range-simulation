MIL_3_Tfile_Hdr_ 145A 140A modeler 9 568F9617 56A0F20E 54 lenovo-PC lenovo 0 0 none none 0 0 none C3CB54B3 3038 0 0 0 0 0 0 1d79 5                                                                                                                                                                                                                                                                                                                                                                                              ��g�      @   D   H      �  .  .  .  .   .,  .0  .4  �           	   begsim intrpt         
   ����   
   doc file            	nd_module      endsim intrpt         
   ����   
   failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����             int	\mac_id;       Objid	\own_id;       Objid	\node_id;       Boolean	\cluster_enable;       int	\total_node_num;       Boolean	\Animation_Enable;       char	\net_topo[256];       char	\subnet_name[256];       char	\node_name[256];       Andid**	\line_ptr;       int	\new_variable;          Packet* pkptr;      +#include <../include/node_strm_stat_init.h>   $#include <../include/mac_function.h>   !#include <../include/pk_config.h>   (#include <../include/animation_header.h>       int RCVD_NODE_NUM = 5;       C#define SELF_INTRPT_SEND (op_intrpt_type() == OPC_INTRPT_SELF  && \   						op_intrpt_code() == 0)       H#define SELF_INTRPT_Animation (op_intrpt_type() == OPC_INTRPT_SELF  && \   						op_intrpt_code() == 1)       :#define SIM_END (op_intrpt_type() == OPC_INTRPT_SELF  && \   						op_intrpt_code() == 3)       /*�������Ƶĸ��ֺ���������*/   void small_line(void);   T   ,static void radioTransimissionAreaDraw(void)   	{   		int node_total_number;   #		Objid node_objid,node_objid_next;   		   		Objid subnet_objid;   		int node_index;   		char mobile_name[128];   		char mobile_name_next[128];   !		double tgt_lat,tgt_lon,tgt_alt;   		double tgt_x,tgt_y,tgt_z;   		   '		double tgt_lat_1,tgt_lon_1,tgt_alt_1;   !		double tgt_x_1,tgt_y_1,tgt_z_1;   		int  tgt_vx,tgt_vy;   		int  tgt_vx_1,tgt_vy_1;   4		FIN(static void radioTransimissionAreaDraw(void));   )		subnet_objid = op_topo_parent(node_id);   7		node_total_number = op_topo_object_count (NODE_TYPE);   		big_line();   Q		//line_ptr = (Andid **)op_prg_mem_alloc(sizeof(Andid) * (node_total_number-1));   D		for(node_index = 0; node_index < node_total_number-1;node_index++)   			{   :				//node_objid = op_topo_object (NODE_TYPE, node_index);   1				sprintf(mobile_name,"Station_%d",node_index);   E				node_objid = op_id_from_name(subnet_objid,NODE_TYPE,mobile_name);   			   Y				op_ima_obj_pos_get (node_objid, &tgt_lat, &tgt_lon, &tgt_alt,&tgt_x, &tgt_y, &tgt_z);   				   �				op_anim_ime_nobj_update (vid, OPC_ANIM_OBJTYPE_NODE, mobile_name, 	OPC_ANIM_OBJ_ATTR_ICON, "ace_qv_exclude" ,OPC_ANIM_OBJ_ATTR_XPOS,tgt_lon ,OPC_ANIM_OBJ_ATTR_YPOS,tgt_lat,OPC_EOL);   				}   		   		   D		for(node_index = 0; node_index < node_total_number-1;node_index++)   			{   :				//node_objid = op_topo_object (NODE_TYPE, node_index);   1				sprintf(mobile_name,"Station_%d",node_index);   E				node_objid = op_id_from_name(subnet_objid,NODE_TYPE,mobile_name);   			   				   +				if(node_index == (node_total_number-2))   					{   /						sprintf(mobile_name_next,"Station_%d",0);   Q						node_objid_next = op_id_from_name(subnet_objid,NODE_TYPE,mobile_name_next);   					}   					else    					{   <						sprintf(mobile_name_next,"Station_%d",(node_index+1));   Q						node_objid_next = op_id_from_name(subnet_objid,NODE_TYPE,mobile_name_next);   					}   Y				op_ima_obj_pos_get (node_objid, &tgt_lat, &tgt_lon, &tgt_alt,&tgt_x, &tgt_y, &tgt_z);       j				op_ima_obj_pos_get (node_objid_next, &tgt_lat_1, &tgt_lon_1, &tgt_alt_1,&tgt_x_1, &tgt_y_1, &tgt_z_1);   C				/* Convert target position from model to viewer coordinates. */   B				op_anim_ime_gen_pos (vid, tgt_lon, tgt_lat, &tgt_vx, &tgt_vy);   J				op_anim_ime_gen_pos (vid, tgt_lon_1, tgt_lat_1, &tgt_vx_1, &tgt_vy_1);   				   u				//printf("\n %s longtitude:%f latitude:%f convert_x:%d convert_y:%d.",mobile_name,tgt_lon,tgt_lat,tgt_vx,tgt_vy);   :				//op_id_from_name(subnet_objid,NODE_TYPE,mobile_name);   �				//*(*(line_ptr+node_index)) = op_anim_igp_macro_draw (vid, OPC_ANIM_RETAIN, mid, 	OPC_ANIM_REG_A_STR, mobile_name, OPC_ANIM_REG_D_STR, mobile_name_next, OPC_EOL);   �				//op_anim_ime_nobj_update (vid, OPC_ANIM_OBJTYPE_NODE, mobile_name, 	OPC_ANIM_OBJ_ATTR_ICON, "ace_qv_exclude" ,OPC_ANIM_OBJ_ATTR_XPOS,tgt_lon ,OPC_ANIM_OBJ_ATTR_YPOS,tgt_lat,OPC_EOL);   �				//op_anim_ime_nobj_update (vid, OPC_ANIM_OBJTYPE_NODE, mobile_name_next, 	OPC_ANIM_OBJ_ATTR_ICON, "ace_qv_exclude" ,OPC_ANIM_OBJ_ATTR_XPOS,tgt_lon_1 ,OPC_ANIM_OBJ_ATTR_YPOS,tgt_lat_1,OPC_EOL);   �				//op_anim_ime_nobj_update (vid, OPC_ANIM_OBJTYPE_NODE, mobile_name_next, 	OPC_ANIM_OBJ_ATTR_ICON, "ace_qv_exclude" , OPC_EOL);   				   y				//op_anim_ime_nobj_update (vid, OPC_ANIM_OBJTYPE_NODE, mobile_name_next, 	OPC_ANIM_OBJ_ATTR_ICON, "blank" , OPC_EOL);   y				//op_anim_ime_nobj_update (vid, OPC_ANIM_OBJTYPE_NODE, mobile_name_next, 	OPC_ANIM_OBJ_ATTR_ICON, "blank" , OPC_EOL);   q				//op_anim_ime_nobj_update (vid, OPC_ANIM_OBJTYPE_NODE, mobile_name,OPC_ANIM_OBJ_ATTR_XPOS, tgt_vx, OPC_EOL);    p				//op_anim_ime_nobj_update (vid, OPC_ANIM_OBJTYPE_NODE, mobile_name,OPC_ANIM_OBJ_ATTR_XPOS, tgt_vy, OPC_EOL);   				   �				//op_anim_igp_macro_draw (vid, OPC_ANIM_RETAIN, mid, 	OPC_ANIM_REG_A_STR, mobile_name, OPC_ANIM_REG_D_STR, mobile_name_next, OPC_EOL);   �				//op_anim_igp_macro_redraw (vid, mid, OPC_ANIM_ERASE_MODE_XOR, OPC_ANIM_REG_A_STR, mobile_name, OPC_ANIM_REG_D_STR, mobile_name_next,OPC_EOL);       }				//op_anim_ime_nobj_update (vid, OPC_ANIM_OBJTYPE_NODE, mobile_name, 	OPC_ANIM_OBJ_ATTR_ICON, "ace_qv_exclude" , OPC_EOL);   �				//op_anim_igp_macro_draw (vid, OPC_ANIM_RETAIN, mid, 	OPC_ANIM_REG_A_STR, "Station_0", OPC_ANIM_REG_D_STR, "Station_1", OPC_EOL);   o				op_anim_igp_line_draw (vid, (OPC_ANIM_COLOR_BLACK | OPC_ANIM_RETAIN), tgt_vx, tgt_vy , tgt_vx_1, tgt_vy_1);   				}   		   		   		   		op_anim_force ();   3		op_intrpt_schedule_self(op_sim_time() + 10.0, 3);   		FOUT;   	}   	                                         :            
   IDLE   
       
          
       
      2/*̽�����ݰ�������ղ�ת�����ϲ㴦�����ƶ��ڵ�*/   if(LOWER_STRM)   	{   &		pkptr = op_pk_get(op_intrpt_strm());   )		op_pk_send(pkptr,UPPER_OUT_STRM_INDEX);   	}   
           ����             pr_state        �            
   TRANSMIT   
       
      //*���ȼ���ϴε����ݰ��Ƿ��շ����,ȫ����ʧ��*/   $pkptr = op_pk_get(op_intrpt_strm());       "/*û�нڵ��յ����ݣ�����ͳ�ƻ���*/   if(RCVD_NODE_NUM == 0)   	{   C		op_sim_message("Measurment End, Draw Transmission Span",OPC_NIL);   +		op_intrpt_schedule_self(op_sim_time(),1);   	}   else    	{   )		op_pk_send(pkptr,LOWER_OUT_STRM_INDEX);   +		op_intrpt_schedule_self(op_sim_time(),0);   		RCVD_NODE_NUM = 0;   	}   
       
       
       
    ����   
          pr_state        �            
   Init   
       
      /*��ȡ�ڵ�ͽ��̵�id��*/   own_id = op_id_self();       !node_id = op_topo_parent(own_id);        /*��ȡ���̵����ԣ�������ò���*/   Eop_ima_obj_attr_get(node_id,"Cluster Header Enable",&cluster_enable);       if(cluster_enable == OPC_TRUE)   	{   <		total_node_num = op_topo_object_count (OPC_OBJTYPE_NDMOB);   	}   	   9mac_id = nodeMacAddressGet(nodeMacListIndexGet(node_id));       
       
   [   if(cluster_enable == OPC_TRUE)   	{   -		//printf("\nEnter the Animation Function");   /*��ȡ�����̺�node�ڵ�ľ��*/   //own_id = op_id_self();   )//node_objid = op_topo_parent(my_own_id);       Bop_ima_obj_attr_get(node_id,"Animation Enable",&Animation_Enable);       if(Animation_Enable)   	{   (/*�õ����糡�����ƣ�net_topo�ǳ�������*/   @op_sim_info_get(OPC_STRING,OPC_SIM_INFO_NETWORK_NAME ,net_topo);       /*�õ��Զ��嶯����ǩ*/   (vid = op_anim_lprobe_anvid ("wireless");       "/*�����Զ��嶯����ǩ�Ƿ�򿪳ɹ�*/   /if (op_prg_odb_ltrace_active("anim")==OPC_TRUE)   	{   		printf("vid %d\n",vid);   	}       /*�õ�����������*/   >op_ima_obj_attr_get(op_topo_parent(node_id),"name",node_name);    if(strcmp("top",node_name) == 0)   	{    		sprintf(subnet_name,"top","");   	}   else    )	sprintf(subnet_name,"top.%s",node_name);       /if (op_prg_odb_ltrace_active("anim")==OPC_TRUE)   	{   #	printf("subnet %s\n",subnet_name);   	}        /*������������ʾ�ڶ����������*/   vop_anim_ime_nmod_draw (vid, OPC_ANIM_MODTYPE_NETWORK,net_topo,subnet_name,OPC_ANIM_MOD_OPTION_NONE,OPC_ANIM_DEFPROPS);       /*��������Ч��������*/       //small_line();       //big_line();   //thick_line();           if(OPC_FALSE)   {   	/*����������*/   6energy_mid = op_anim_macro_create("battery_capacity");   				   N/* Determine position of NE corner of the station icon in the viewer window */   9op_anim_mme_nobj_pos (energy_mid, OPC_ANIM_OBJTYPE_NODE,    <	OPC_ANIM_REG_A_STR, OPC_ANIM_REG_X_INT, OPC_ANIM_REG_Y_INT,   	OPC_ANIM_VERTEX_ICON_NE);   				   &/* Calculate postion of queue meter */   8op_anim_mgp_reg_set (energy_mid, OPC_ANIM_REG_C_INT, 5);   2op_anim_mgp_arop (energy_mid, OPC_ANIM_REG_X_INT,    '	OPC_ANIM_AROP_ADD, OPC_ANIM_REG_C_INT,   	OPC_ANIM_REG_X_INT);   				   8op_anim_mgp_reg_set (energy_mid, OPC_ANIM_REG_C_INT, 2);   1op_anim_mgp_arop (energy_mid, OPC_ANIM_REG_X_INT,   '	OPC_ANIM_AROP_ADD, OPC_ANIM_REG_C_INT,   	OPC_ANIM_REG_A_INT);   				   1op_anim_mgp_arop (energy_mid, OPC_ANIM_REG_Y_INT,   '	OPC_ANIM_AROP_ADD, OPC_ANIM_REG_C_INT,   	OPC_ANIM_REG_B_INT);   				       "/* Compute text position offset */   Top_anim_mgp_reg_set (energy_mid, OPC_ANIM_REG_C_INT, 2);                               1op_anim_mgp_arop (energy_mid, OPC_ANIM_REG_Y_INT,   -	OPC_ANIM_AROP_SUBTRACT, OPC_ANIM_REG_C_INT,    	OPC_ANIM_REG_D_INT);   				   #op_anim_mgp_setup_end (energy_mid);   				   Rop_anim_mgp_text_draw (energy_mid, OPC_ANIM_COLOR_WHITE | OPC_ANIM_ALIGNV_BOTTOM |   ?	OPC_ANIM_FONT_OPEN_10, OPC_ANIM_REG_X_INT, OPC_ANIM_REG_D_INT,   	OPC_ANIM_REG_E_STR);       4op_anim_macro_close (energy_mid); /* End of macro */   }   }       	}       
       
   ����   
          pr_state        �  �          
   	Animation   
       
          /*�滭��AP Node�ڵ�ĸ��Ƿ�Χ*/   //radioTransimissionAreaDraw();               
       
      if(SIM_END)   	{   I		op_sim_end("Animation End, Terminate Process",OPC_NIL,OPC_NIL,OPC_NIL);   	}   
           ����             pr_state            
          >  L     ,      <  `  <  C            
   tr_10   
       
   default   
       ����          
    ����   
          ����                       pr_transition              �   �     �   �  �   �     �  �             
   tr_12   
       
   default   
       ����          
    ����   
          ����                       pr_transition      &         �       �    :            
   tr_38   
       ����          ����          
    ����   
          ����                       pr_transition      '         �       ;    �            
   tr_39   
       
   
UPPER_STRM   
       ����          
    ����   
          ����                       pr_transition      )        �  W     �    �  �          
   tr_41   
       
   SELF_INTRPT_Animation   
       
   radioTransimissionAreaDraw()   
       
    ����   
          ����                       pr_transition      *         �       �    F            
   tr_42   
       
   SELF_INTRPT_SEND   
       ����          
    ����   
          ����                       pr_transition      +        �  �     �  �  �  �    �  �  �          
   tr_43   
       
   default   
       ����          
    ����   
          ����                       pr_transition         -                                    