/*Created by Ziang Niu at 2015-12-16, used for educational purpose;
  The C Header used for creating packet format, initializing the pk_size, pk_nfd field attribute ,
  & print the packet information;
  
  Using Method: create header file called: include , add following to the process Header
  #include <../include/pk_config.h>
  
  OPC_FIELD_TYPE_INTEGER  integer field  
  OPC_FIELD_TYPE_DOUBLE  double field  
  OPC_FIELD_TYPE_FLOATING_POINT  floating point field  
*/
#ifndef _PK_CONFIG_H_
#define _PK_CONFIG_H_
static Packet* createPacket(char* pk_format_str, double pk_size);
static void packetLengthPrint(Packet* pkptr); 
static void packetCreateTimePrint(Packet* pkptr);
static void packetInfoPrint(Packet* pkptr);

/*Not Used, Not Efficient*/
static void packetSetAttribute(Packet* pkptr, char* fild_name1, void* value1, int type1, char* fild_name2, void* value2, int type2, char* fild_name3, void* value3, int type3);
//packetSetAttribute(pkptr, "src_field" ,&(mac_id), OPC_FIELD_TYPE_INTEGER , "dest_field", &(dest_address) , OPC_FIELD_TYPE_INTEGER , OPC_NIL , OPC_NIL, OPC_FIELD_TYPE_INTEGER);
static void packetSetSegmentAttribute(Packet* pkptr, char* fild_name, void* value, int type);

/*used for create the Packet, set the packet length*/
static Packet* createPacket(char* pk_format_str, double pk_size)
{
	Packet* pkptr;
	char msg[128];
	FIN(Packet* createPacket(char* pk_format_str, double pk_size));
	if(pk_format_str == OPC_NIL)
		pkptr = op_pk_create(pk_size);
	else
		{
			pkptr = op_pk_create_fmt(pk_format_str);
			if(pkptr == OPC_NIL)
			{
				sprintf(msg, "Create Packet Format %s, Failed.", pk_format_str);
				op_sim_message("Simulation Error:", msg);
				}
			else 
				op_pk_total_size_set(pkptr, pk_size);
			}
	FRET(pkptr);
}
//op_pk_nfd_set (f_pkptr, "ds_field", ds_ptr, op_prg_mem_copy_create, op_prg_mem_free, sizeof (Custom_DS));

/*@param packet pointer, fild_name, value, value_type;
  used for: setting the packet multiple fields attribute*/
static void packetSetAttribute(Packet* pkptr, char* fild_name1, void* value1, int type1, char* fild_name2, void* value2, int type2, char* fild_name3, void* value3, int type3)
{
	FIN(packetSetAttribute(Packet* pkptr, char* fild_name1, void* value1, int type1, char* fild_name2, void* value2, int type2, char* fild_name3, void* value3, int type3));
	if(pkptr == OPC_NIL)
		{
			op_sim_message("Packet Pointer is NULL", OPC_NIL);
				}
	else
		{
			if(fild_name1 == OPC_NIL)
				{
					FOUT;
					}
			else
			{
				packetSetSegmentAttribute(pkptr, fild_name1, value1, type1);
				}
				
			if(fild_name2== OPC_NIL)
				{
					FOUT;
					}
			else
			{
				packetSetSegmentAttribute(pkptr, fild_name2, value2, type2);
				}
				
			if(fild_name3 == OPC_NIL)
				{
					FOUT;
					}
			else
			{
				packetSetSegmentAttribute(pkptr, fild_name3, value3, type3);
				}
			}
	FOUT;
}

/*@param packet pointer, fild_name, value, value_type;
  used for: setting the packet just-one field attribute*/
static void packetSetSegmentAttribute(Packet* pkptr, char* fild_name, void* value, int type)
{
	FIN(packetSetSegmentAttribute(Packet* pkptr, char* fild_name, void* value, int type));
	switch(type)
	{
		case OPC_FIELD_TYPE_INTEGER : 
		if(op_pk_nfd_set_int32(pkptr, fild_name,*((int*)value)) == OPC_COMPCODE_FAILURE)
			{
				op_sim_message("Packet Field Attribute Setting Error", OPC_NIL);
				break;
				}
		case OPC_FIELD_TYPE_DOUBLE: 
		if(op_pk_nfd_set_dbl(pkptr, fild_name,*((double*)value)) == OPC_COMPCODE_FAILURE)
			{
				op_sim_message("Packet Field Attribute Setting Error", OPC_NIL);
				break;
				}
		case OPC_FIELD_TYPE_INT64:
		if(op_pk_nfd_set_int64(pkptr, fild_name,*((OpT_Int64*)value)) == OPC_COMPCODE_FAILURE)
			{
				op_sim_message("Packet Field Attribute Setting Error", OPC_NIL);
				break;
				}
		default: op_sim_message("Check the Packet Field Type Setting", OPC_NIL); break;
		}
	FOUT;
}

/*@param packet pointer
   used for printing the packet length*/
static void packetLengthPrint(Packet* pkptr)
{
	double pk_size;
	char msg[128];
	FIN(static void packetLengthPrint(Packet* pkptr))
	pk_size = op_pk_total_size_get(pkptr);
	sprintf(msg, "Payload packet size: %f", pk_size);
	op_sim_message(msg ,OPC_NIL);
	FOUT;
}

/*@param packet pointer
   used for printing the packet creating time and stamp time*/
static void packetCreateTimePrint(Packet* pkptr)
{
	double create_time;
	double stamp_time;
	char msg[128];
	FIN(static void packetCreateTimePrint(Packet* pkptr))
	create_time = op_pk_creation_time_get(pkptr);
	stamp_time = op_pk_stamp_time_get(pkptr);
	sprintf(msg, "Packet Creation Time : %f , Stamp Time: %f.", create_time, stamp_time );
	op_sim_message(msg ,OPC_NIL);
	FOUT;
}

/*@param packet pointer
   used for printing the packet all field info */
static void packetInfoPrint(Packet* pkptr)
{
	FIN(static void packetInfoPrint(Packet* pkptr))
	op_pk_print(pkptr);
	FOUT;
}


#endif