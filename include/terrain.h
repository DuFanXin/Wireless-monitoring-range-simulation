/*Created by Ziang Niu at 2015-12-22, used for educational purpose;
  The C Header used for initializing the network topology , measure the distance
  &also provide method for setting the node mobility
  
  Using Method: create header file called: include , add following to the process Header;
  &Add "Mac Address" attribute to the node-level;
  #include <../include/topo_geo_config.h>
*/
#ifndef _TOPO_GEO_CONFIG_H_
#define _TOPO_GEO_CONFIG_H_

#include <prg_geo.h>
#include <prg_list_funcs.h>
#include <stdlib.h>
#include <math.h>

/*Define the Node type in the subnet*/
//#define NODE_TYPE OPC_OBJTYPE_NDFIX
//#define NODE_TYPE OPC_OBJTYPE_NDMOB
#define NAME_SIZE 128

double x_center;
double y_center;
double z_center;
double measure_step;

static void networkTopoInfoInit(void);

/*Print the error info*/
void handle_error(const char* error);


/*@param Void
Function: initializing the network topology structure and assign every node (x,y,z) position*/
static void networkTopoInfoInit(void)
{
	int node_total_number;
	Objid node_objid;
	double angle_base;
	int node_index;
	double xpos,ypos;
	double node_angle;
	Boolean center_ap;
	int angle_index = 0;
	FIN(static void networkTopoInfoInit(void));
	node_total_number = op_topo_object_count (NODE_TYPE);
	angle_base = 360.0/(node_total_number - 1); 
	for(node_index = 0; node_index < node_total_number; node_index++)
	{
		node_objid = op_topo_object (NODE_TYPE, node_index);
		op_ima_obj_attr_get(node_objid,"Cluster Header Enable",&center_ap);
		if(center_ap == OPC_TRUE)
		{
			op_ima_obj_attr_get_dbl(node_objid, "x position", &x_center);
			op_ima_obj_attr_get_dbl(node_objid, "y position", &y_center);
			op_ima_obj_attr_get_dbl(node_objid, "altitude", &z_center);
			op_ima_obj_attr_get_dbl(node_objid, "Measure Step", &measure_step);
			break;
			}
		else 
			continue;
			}
	if(node_index == node_total_number)
		{
			op_sim_message("Not Set the AP Center.",OPC_NIL);
		}
	else 
	{
	for(node_index = 0; node_index < node_total_number; node_index++)
	{
		node_objid = op_topo_object (NODE_TYPE, node_index);
		op_ima_obj_attr_get(node_objid,"Cluster Header Enable",&center_ap);
		if(center_ap == OPC_FALSE)
		{
			node_angle = angle_index * angle_base;
			op_ima_obj_attr_set(node_objid,"Measure Angle",node_angle);
			op_ima_obj_attr_set(node_objid,"Measure Radius",measure_step);
			ypos = measure_step * sin(node_angle * 3.1415 / 180.0);
			ypos = ypos + y_center;
			xpos = measure_step * cos(node_angle * 3.1415 / 180.0);
			xpos = xpos + x_center;
			op_ima_obj_attr_set_dbl(node_objid, "x position", xpos);
			op_ima_obj_attr_set_dbl(node_objid, "y position", ypos);
			op_ima_obj_attr_set_dbl(node_objid, "altitude", z_center);
			angle_index++;
			}
		else 
			continue;
			}
			}
	FOUT;
}

void handle_error(const char* error)
{
	char process_name[NAME_SIZE];
	FIN(handle_error(error));
	op_ima_obj_attr_get_str(op_id_self(), "process model", NAME_SIZE, 
		process_name);
	sprintf(process_name, "Error occurred in process <%s>:", process_name);
	op_sim_end(process_name, error, OPC_NIL, OPC_NIL);
	FOUT;
}

#endif
