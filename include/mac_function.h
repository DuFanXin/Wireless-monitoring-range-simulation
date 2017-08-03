/*Created by Ziang Niu at 2015-12-16, used for educational purpose;
  The C Header used for initializing the network mac address and provided the print info,
  &also provide method for randomly getting Mac address differ self_mac_id 
  
  Using Method: create header file called: include , add following to the process Header;
  &Add "Mac Address" attribute to the node-level;
  #include <../include/mac_function.h>
*/
#ifndef _MAC_FUNCTION_H_
#define _MAC_FUNCTION_H_

/*Define the Node type in the subnet*/
#define NODE_TYPE OPC_OBJTYPE_NDMOB

/*Store the subnet nodes mac information, type list*/
extern PrgT_List *Network_Mac_Info_List;
/*store to store the node info, type struct*/
typedef struct
{
	Objid node_objid;
	int mac_address;
	}Node_Mac_Info;

static int nodeMacListIndexGet(Objid node_objid);
static void nodeMacListPrint(void);
static int nodeMacAddressGet(int index);
static int randomGenerateMacAddressOthers(int index);
static int randomGenerateMacAddressAll(void);
static void nodeNameGetByMacAddress(int mac_address,char* node_name);

/*@param : node mac address
Function: return the node name by the input mac adress*/
static void nodeNameGetByMacAddress(int mac_address,char* node_name)
{
	int list_size;
	Node_Mac_Info* node_mac_info;
	int node_index;
	FIN(static void nodeNameGetByMacAddress(int mac_address,char* node_name));
	//node_total_number = op_topo_object_count (OPC_OBJTYPE_NODE_MOB);
	list_size = prg_list_size (Network_Mac_Info_List);
	for(node_index = 0; node_index < list_size; node_index++)
	{
		node_mac_info = prg_list_access(Network_Mac_Info_List, node_index);
		if(node_mac_info-> mac_address == mac_address)
			{
				op_ima_obj_attr_get(node_mac_info-> node_objid,"name",node_name);
				FOUT;
				}
		else 
			{
				continue;
				}
	}
	/*Print the Error information*/
	{
	char msg[128];
	sprintf(msg,"Not find MAC Address%d corresponding Node.",mac_address);
	op_sim_message(msg,OPC_NIL);
	}
	FOUT;
}


/*@param : node objid, return the node index which stored in the global list
Function: get the index indicated the position stored in the list*/
static int nodeMacListIndexGet(Objid node_objid)
{
	int list_size;
	Node_Mac_Info* node_mac_info;
	int node_index;
	FIN(static int nodeMacListIndexGet(Objid node_objid));
	//node_total_number = op_topo_object_count (OPC_OBJTYPE_NODE_MOB);
	list_size = prg_list_size (Network_Mac_Info_List);
	for(node_index = 0; node_index < list_size; node_index++)
	{
		node_mac_info = prg_list_access(Network_Mac_Info_List, node_index);
		if(node_mac_info-> node_objid == node_objid)
			{
				FRET(node_index);
				}
		else 
			{
				continue;
				}
	}
	/*Print the Error information*/
	{
	char node_name[128];
	char msg[128];
	op_ima_obj_attr_get(node_objid,"name",node_name);
	sprintf(msg,"Node: %s assign the negative mac address.",node_name);
	op_sim_message(msg,OPC_NIL);
	}
	FRET(-1);
}

/*@param : Index that Node Mac info stored in list 
Function: Integer, return node assigned Mac Address */
static int nodeMacAddressGet(int index)
{
	Node_Mac_Info* node_mac_info;
	FIN(static int nodeMacListIndexGet(int index));
	node_mac_info = prg_list_access(Network_Mac_Info_List, index);
	FRET(node_mac_info-> mac_address);
}


/*@param: void, used the global mac info list
Function: print all the stored information in the global mac list*/
static void nodeMacListPrint(void)
{
	int list_size;
	Node_Mac_Info* node_mac_info;
	int node_index;
	char node_name[128];
	char msg[128];
	FIN(static int nodeMacListPrint(void));
	//node_total_number = op_topo_object_count (OPC_OBJTYPE_NODE_MOB);
	list_size = prg_list_size (Network_Mac_Info_List);
	if(list_size <= 0)
	{
		op_sim_message("None node assign the Mac Address, Check the attribute Level.",OPC_NIL);
	}
	else 
	{
		for(node_index = 0; node_index < list_size; node_index++)
		{
			node_mac_info = prg_list_access(Network_Mac_Info_List, node_index);
			op_ima_obj_attr_get(node_mac_info->node_objid,"name",node_name);
			sprintf(msg,"Node: %s Mac Address: %d .",node_name, node_mac_info->mac_address);
			op_sim_message(msg,OPC_NIL);
			}
	}
	FOUT;
}

/*@param: The Node mac info stored index in the global mac info list
Function: randomly generate only-one mac address that differ from self-mac address*/ 
static int randomGenerateMacAddressOthers(int index)
{
	Distribution* random_mac_address_dist;
	int list_size;
	int mac_address_index;
	Node_Mac_Info* node_mac_info;
	
	FIN(static int randomGenerateMacAddressOthers(int index));
	list_size = prg_list_size (Network_Mac_Info_List);
	random_mac_address_dist = op_dist_load("uniform_int", 0, (list_size - 1));
	while(OPC_TRUE)
		{
			mac_address_index = op_dist_outcome(random_mac_address_dist);
			if(mac_address_index != index)
				break;
		}
	node_mac_info = prg_list_access(Network_Mac_Info_List, mac_address_index);
	op_dist_unload(random_mac_address_dist);
	FRET(node_mac_info-> mac_address);
}

/*@param: void
Function: randomly generate one mac address from all the Nodes mac address*/ 
static int randomGenerateMacAddressAll(void)
{
	Distribution* random_mac_address_dist;
	int list_size;
	int mac_address_index;
	Node_Mac_Info* node_mac_info;
	
	FIN(static int randomGenerateMacAddressAll(void));
	list_size = prg_list_size (Network_Mac_Info_List);
	random_mac_address_dist = op_dist_load("uniform_int", 0, (list_size - 1));
	mac_address_index = op_dist_outcome(random_mac_address_dist);
	node_mac_info = prg_list_access(Network_Mac_Info_List, mac_address_index);
	op_dist_unload(random_mac_address_dist);
	FRET(node_mac_info-> mac_address);
}


#endif