/*Created by Ziang Niu at 2015-12-16, used for educational purpose;
  The c Header used for initializing process stream index and state wire index,
  & Define the corresponding interrupt;
  
  Pay Attention: The process connectivity important for using the Header!!!!!
  Using Method: create header file called: include , add following to the process Header
  #include <../include/node_strm_stat_init.h>
*/
#ifndef _NODE_STRM_STAT_INIT_H_
#define _NODE_STRM_STAT_INIT_H_

#define UPPER_IN_STRM_INDEX 0
#define UPPER_OUT_STRM_INDEX 0

#define LOWER_IN_STRM_INDEX 1
#define LOWER_OUT_STRM_INDEX 1

#define LOWER_IN_STRM_INDEX_1 2
#define LOWER_OUT_STRM_INDEX_1 2

#define UPPER_STRM ((op_intrpt_type() == OPC_INTRPT_STRM) &&\
		(op_intrpt_strm() == UPPER_IN_STRM_INDEX))
		
#define LOWER_STRM ((op_intrpt_type() == OPC_INTRPT_STRM) &&\
		(op_intrpt_strm() == LOWER_IN_STRM_INDEX))

#define LOWER_STRM_1 ((op_intrpt_type() == OPC_INTRPT_STRM) &&\
		(op_intrpt_strm() == LOWER_IN_STRM_INDEX_1))		
#endif