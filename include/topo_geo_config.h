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
#include "opnet.h"

/*Define the Node type in the subnet*/
//#define NODE_TYPE OPC_OBJTYPE_NDFIX
//#define NODE_TYPE OPC_OBJTYPE_NDMOB
#define NAME_SIZE 128
//#define INIT_STEP 1000
//double x_center;
//double y_center;
//double z_center;
//double x_span;
//double y_span;
double ap_latitude,ap_longitude;

//double measure_step;

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
	double antenna_height;
	double measure_step;
	double latitude,altitude;
	double init_step;
	double long_degrees,longitude,lat_degrees;
	
	double x_temp,y_temp,z_temp;
	double terrain_height;
	Boolean center_ap;
	int angle_index = 0;
	char node_name[56];
	FIN(static void networkTopoInfoInit(void));
	node_total_number = op_topo_object_count (NODE_TYPE);
	angle_base = 360.0/(node_total_number - 1); 
	for(node_index = 0; node_index < node_total_number; node_index++)
	{
		node_objid = op_topo_object (NODE_TYPE, node_index);
		op_ima_obj_attr_get(node_objid,"Cluster Header Enable",&center_ap);
		if(center_ap == OPC_TRUE)
		{
			op_ima_obj_pos_get (node_objid, &ap_latitude, &ap_longitude, &altitude, &x_temp, &y_temp ,&z_temp);
			//op_ima_obj_attr_get_dbl(node_objid, "x position", &x_center);
			//op_ima_obj_attr_get_dbl(node_objid, "y position", &y_center);
			//op_ima_obj_attr_get_dbl(op_topo_parent(node_objid), "x span", &x_span);
			//op_ima_obj_attr_get_dbl(op_topo_parent(node_objid), "y span", &y_span);
			
			terrain_height = tmm_elev_point_get(ap_latitude,ap_longitude);
			printf("\nAP Node-BaseStation Terrain Height:%f",terrain_height);
			
			//op_ima_obj_attr_get_dbl(node_objid, "Antenna Height", &antenna_height);
			//antenna_height = terrain_height + antenna_height;
			//op_ima_obj_attr_set_dbl(node_objid, "altitude", antenna_height);
			op_ima_obj_attr_get(node_objid,"Measure Radius",&measure_step);
			op_ima_obj_attr_get(node_objid,"Init Distance",&init_step);
			 
			//op_ima_obj_attr_get_dbl(node_objid, "Measure Step", &measure_step);
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
			op_ima_obj_attr_set(node_objid,"Init Distance",init_step);
			//op_ima_obj_pos_get (node_objid, &latitude, &longitude, &altitude, &x_temp, &y_temp ,&z_temp);
			
			/*计算节点相对于AP基站的位置，为X Y距离*/
			ypos = init_step * sin(node_angle * 3.1415 / 180.0);
			//ypos = y_center + ypos;
			
			xpos = init_step * cos(node_angle * 3.1415 / 180.0);
			//xpos = x_center + xpos;
			
			/*将距离换算成经纬度坐标的变换*/
			prg_geo_distances_to_degrees_convert (ap_longitude, ap_latitude, xpos, ypos,  PrgC_Unit_Meters_Type, &long_degrees, &lat_degrees);
			
			//if(xpos >= 0)
				//{
					longitude = ap_longitude + long_degrees;
					//}
			//else 
			//	{
				//	longitude = ap_longitude - long_degrees;
				//	}
				
			//if(ypos >= 0)
				//{
					//latitude = ap_latitude - lat_degrees;
					//}
			//else 
				//{
					latitude = ap_latitude + lat_degrees;
				//	}
				
			terrain_height = tmm_elev_point_get(latitude,longitude);
			op_ima_obj_attr_get(node_objid,"name",&node_name);
			printf("\n%s New Terrain Height:%f",node_name,terrain_height);

			//op_ima_obj_attr_set_dbl(node_objid, "x position", xpos);
			//ypos = y_center + ypos/8905.55;
			
			/*更新节点的经纬度的值*/
			//op_ima_obj_attr_set_dbl(node_objid, "longitude", (longitude + long_degrees));
			//op_ima_obj_attr_set_dbl(node_objid, "latitude", (latitude + lat_degrees));
			
			//op_ima_obj_pos_set_geocentric (node_objid, xpos, ypos, 5);
			op_ima_obj_attr_get_dbl(node_objid, "Antenna Height", &antenna_height);
			op_ima_obj_pos_set_geodetic (node_objid, latitude, longitude,antenna_height);

			
			//op_ima_obj_attr_set_dbl(node_objid, "geo_y", ypos);
			//op_ima_obj_attr_get_dbl(node_objid, "geo_z", &zpos);
			//op_ima_obj_attr_get_dbl(node_objid, "altitude", &zpos);
			//op_ima_obj_attr_get_dbl(node_objid, "Antenna Height", &antenna_height);
			//antenna_height = antenna_height +  zpos;
			//op_ima_obj_attr_set_dbl(node_objid, "altitude",antenna_height);
			
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
