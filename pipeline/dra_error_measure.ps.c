/* dra_error.ps.c */
/* Default error allocation model for radio link Transceiver Pipeline. The	*/
/* model terminates the computation of the total number of packet's bit		*/
/* errors when it realizes that the number of already discovered errors		*/
/* exceeds the error correction threshold of the receiver, indicating that	*/
/* the receiver will reject the packet anyway even this model doesn't find	*/
/* any more bit errors. Hence, this model provides a faster execution of	*/
/* the error pipeline stage without affecting the behavior of the			*/
/* simulation model When used, the statistics "number of bit errors" and	*/
/* "actual bit-error-rate" collected for each packet will not be accurate.	*/
/* So, for studies where these statistics are important, the "*_all_stats"	*/
/* version of the error pipeline stage model should be used.				*/                                                        

/****************************************/
/*		 Copyright (c) 1993-2008		*/
/*		by OPNET Technologies, Inc.		*/
/*		 (A Delaware Corporation)		*/
/*	   7255 Woodmont Av., Suite 250  	*/
/*      Bethesda, MD 20814, U.S.A.      */
/*		   All Rights Reserved.			*/
/****************************************/

#include "opnet.h"
#include <math.h>

/* Define a convenient macro for computing factorials using the gamma		*/
/* function Pick the mt-safe version for parallel execution, which is		*/
/* available only for Solaris.												*/	
#if defined (OPD_PARALLEL) && !defined (HOST_PC_INTEL_WIN32)
#define log_factorial(n)		lgamma_r ((double) n + 1.0, &signgam)
extern double lgamma_r (double, int *);
#else
#define log_factorial(n)		lgamma ((double) n + 1.0)
extern double lgamma (double);
#endif

#define round(x) (floor (x + 0.5))

#if defined (__cplusplus)
extern "C"
#endif
void
dra_error_measure_mt (OP_SIM_CONTEXT_ARG_OPT_COMMA Packet* pkptr)
	{
	double			pe, r, p_accum, p_exact;
	double			data_rate, elap_time;
	double			log_p1, log_p2, log_arrange;
	double			ecc_thresh;
	double			pklen;
	OpT_Packet_Size	seg_size;
	int				num_errs, prev_num_errs;
	Objid           node_objid;
	char            node_name[56];
#if defined (OPD_PARALLEL) && !defined (HOST_PC_INTEL_WIN32)
	int				signgam;
#endif

	/** Compute the number of errors assigned to a segment of bits within	**/
	/** a packet based on its length and the bit error probability.			**/
	FIN_MT (dra_error_measure (pkptr));

	/* Get the total of bir errors that are already found in the previous	*/
	/* segments of the packet.												*/
	prev_num_errs = op_td_get_int (pkptr, OPC_TDA_RA_NUM_ERRORS);
	
	/* If the packet already contains bit errors and their number exceeds	*/
	/* the receiver's ecc threshold, then there is no need to check whether	*/
	/* there are additional bit errors, since the packet is going to be		*/
	/* rejected due to already found bit errors. Get the packet size and	*/
	/* error correction threshold to make this check only if there are		*/
	/* already found bit errors.											*/
	if (prev_num_errs > 0)
		{
		ecc_thresh = op_td_get_dbl (pkptr, OPC_TDA_RA_ECC_THRESH);
		pklen      = (double) op_pk_total_size_get (pkptr);
		
		/* Check whether the errors have already exceeded the threshold.	*/
		if ((double) prev_num_errs / pklen > ecc_thresh)
			{
			FOUT;
			}
		}
	else
		{
		/* Set the packet length to an invalid value indicating that it is	*/
		/* not obtained, yet.												*/
		pklen = -1.0;
		}
	
	/* Obtain the expected Bit-Error-Rate 'pe'.								*/
	pe = op_td_get_dbl (pkptr, OPC_TDA_RA_BER);
	
	/* Calculate time elapsed since last BER change.						*/
	elap_time = op_sim_time () - op_td_get_dbl (pkptr, OPC_TDA_RA_SNR_CALC_TIME);

	/* Use datarate to determine how many bits in the segment. 				*/
	data_rate = op_td_get_dbl (pkptr, OPC_TDA_RA_RX_DRATE);
	seg_size = (OpT_uInt64) round (elap_time * data_rate);
    
	/* Case 1: if the bit error rate is zero, so is the number of errors. 	*/
    if (pe == 0.0 || seg_size == 0)
		num_errs = 0;

	/* Case 2: if the bit error rate is 1.0, then all the bits are in error.	*/
	/* (note however, that bit error rates should not normally exceed 0.5).		*/
	else if (pe >= 1.0)
		num_errs = seg_size;

	/* Case 3: The bit error rate is not zero or one. */
	else
		{
		/* The error count can be obtained by mapping a uniform random number 	*/
		/* in [0, 1[ via the inverse of the cumulative mass function (CMF) 		*/
		/* for the bit error count distribution. 								*/
	
		/* Obtain a uniform random number in [0, 1[ to represent 		*/
		/* the value of the CDF at the outcome that will be produced. 	*/
		//r = op_dist_uniform (1.0);
			r = 0.1;
		/* Integrate probability mass over possible outcomes until r is exceeded. 	*/
		/* The loop iteratively corresponds to "inverting" the CMF since it finds	*/
		/* the bit error count at which the CMF first meets or exceeds the value r.	*/
		for (p_accum = 0.0, num_errs = 0; num_errs <= seg_size; num_errs++)
			{
			/* Compute the probability of exactly 'num_errs' bit errors occurring.	*/
	
			/* The probability that the first 'num_errs' bits will be in error 		*/
			/* is given by pow (pe, num_errs). Here it is obtained in logarithmic	*/
			/* form to avoid underflow for small 'pe' or large 'num_errs'.			*/
			log_p1 = (double) num_errs * log (pe);
	
			/* Similarly, obtain the probability that the remaining bits will not 	*/
			/* be in error. The combination of these two events represents one 		*/
			/* possible configuration of bits yielding a total of 'num_errs' errors.*/
			log_p2 = (double) (seg_size - num_errs) * log (1.0 - pe);
	
			/* Compute the number of arrangements that are possible with the same 	*/
			/* number of bits in error as the particular case above. Again obtain 	*/
			/* this number in logarithmic form (to avoid overflow in this case).	*/
			/* This result is expressed as the logarithmic form of the formula for	*/
			/* the number N of combinations of k items from n:  N = n!/(n-k)!k!		*/
			log_arrange = 	log_factorial (seg_size) -
							log_factorial (num_errs) - 
							log_factorial (seg_size - num_errs);
	
			/* Compure the probability that exactly 'num_errs' are present in the	*/
			/* segment of bits, in any arrangement.									*/
			p_exact = exp (log_arrange + log_p1 + log_p2);
		
			/* Add this to the probability mass accumulated so far for previously 	*/
			/* tested outcomes to obtain the value of the CMF at outcome = num_errs.*/
			p_accum += p_exact;
	
			/*'num_errs' is the outcome for this trial if the CMF meets or exceeds 	*/
			/* the uniform random value selected earlier. 							*/
			if (p_accum >= r)
				break;
			
			/* If we reach this point then the packet has at least one bit error,	*/
			/* which may be already sufficient to exceed the ECC threshold. If this	*/
			/* is the case, stop computing the number of bit errors, since we know	*/
			/* that the packet will be rejected anyway.	If this is the very first	*/
			/* bit error then get the total packet size and threshold value, which	*/
			/* we need for the comparison below.									*/
			if (pklen < 0.0)
				{
				ecc_thresh = op_td_get_dbl (pkptr, OPC_TDA_RA_ECC_THRESH);
				pklen      = (double) op_pk_total_size_get (pkptr);
				}
		
			/* Check whether the total errors have already exceeded the threshold.	*/				
			if ((double) (prev_num_errs + num_errs + 1) / pklen > ecc_thresh)
				{
				/* Increment the number of bit errors, which we would have done if	*/
				/* we continued with searching for a higher number of bit errors.	*/ 
				num_errs++;
				
				/* Terminate the for-loop.											*/
				break;
				}
			}
		}
	   
	/* Increase number of bit errors in packet transmission data attribute. */
	op_td_set_int (pkptr, OPC_TDA_RA_NUM_ERRORS, num_errs + prev_num_errs);
	
	node_objid = op_topo_parent(op_td_get_int(pkptr, OPC_TDA_RA_RX_OBJID));
	op_ima_obj_attr_get(node_objid,"name",node_name);
	
		
	/* Assign actual (allocated) bit-error rate over tested segment. */
	if (seg_size != 0)
		op_td_set_dbl (pkptr, OPC_TDA_RA_ACTUAL_BER, (double) num_errs / seg_size);
	else op_td_set_dbl (pkptr, OPC_TDA_RA_ACTUAL_BER, pe);
	
	printf("\n %s Received Packet, Bits Error: %d.",node_name,num_errs + prev_num_errs);
	
	FOUT
	}

