MIL_3_Tfile_Hdr_ 145A 140A modeler 6 568F95A7 58577D03 25 FlyingDutchman yzc 0 0 none none 0 0 none 857785D1 46D0 0 0 0 0 0 0 1d79 5                                                                                                                                                                                                                                                                                                                                                                                            ЋЭg      8   L     *  	    ц  Ё  "э  "ѕ  A   Cэ      node   WLAN   	stn_wless   	stn_wlessџџџџџџџџ   Wireless LAN station    6   General Node Functions:    -----------------------        The wireless station node model    /represents an IEEE802.11 wireless LAN station.    %The node model consists of following    
processes:       *1. The MAC layer which has a wireless lan    !mac process model with following    attributes:       MAC address -- station address       *Fragmentation Threshold --- based on this    (threshold station decides wether or not    "to send data packets in fragments.       *Rts threshold --- based on this threshold    (station decides wether Rts/Cts exchange    *is needed for every data transmission.           "The wireless LAN MAC layer has an    "interface with higher layer which    &receives packet from higher layer and    "generates random address for them.           2. Wireless LAN interface       #This process model is an interface    )between MAC layer and higher layer.  The    &function of this process is to accept    'packets from higher layer and generate    &random destination address for them.     )This information is then sent to the MAC    layer.            3. Wireless LAN receiver       *This is a wireless receiver which accepts    (any incomming packets from the physical    &layer and pass it to the wireless MAC    process.           4. Wireless LAN transmitter       %This is a wireless transmitter which    #receives packet from MAC layer and    $transmits it to the physical medium.                Mac Address    џџџџџџџ    џџџџ       џџџџ   Auto Assigned          џџџџ          џџџџ         Auto Assigned   џџџџџџџџ       ЅZ             Cluster Header Enable   џџџџџџџ   џџџџ           џџџџ          џџџџ          џџџџ           ЅZ             Measure Angle   џџџџџџџ   џџџџ               џџџџ              џџџџ              џџџџ           ЅZ             Measure Radius   џџџџџџџ   џџџџ               џџџџ              џџџџ              џџџџ           ЅZ             Antenna Height   џџџџџџџ   џџџџ       @      џџџџ              џџџџ              џџџџ           ЅZ             Init Distance   џџџџџџџ   џџџџ       @@     џџџџ              џџџџ              џџџџ           ЅZ             Animation Enable   џџџџџџџ   џџџџ          џџџџ          џџџџ          џџџџ           ЅZ                    Destination Address      !wlan_mac_intf.Destination Addressџџџџ    џџџџ           џџџџ          џџџџ          џџџџ                         Traffic Type of Service      wlan_mac_intf.Type of Serviceџџџџ    џџџџ           џџџџ          џџџџ          џџџџ                         Wireless LAN MAC Address      wireless_lan_mac.Addressџџџџ    џџџџ           џџџџ          џџџџ          џџџџ                         Wireless LAN Parameters      (wireless_lan_mac.Wireless LAN Parametersџџџџ   џџџџ                                             count    џџџџџџџ    џџџџ          џџџџ          џџџџ          џџџџ           ЅZ             list   	џџџџџџџ   	џџџџ                                              ЅZ                       altitude      altitudeџџџџ   џџџџ               џџџџ              џџџџ              џџџџ                        phase      phaseџџџџ   џџџџ               џџџџ              џџџџ              џџџџ                        Destination Address          
       Random   
   
TIM source            none      Traffic Type of Service                 Best Effort (0)      Wireless LAN MAC Address          
    џџџџ   
   Wireless LAN Parameters         
      Default   
   altitude         
        џџџџ   
   altitude modeling            relative to subnet-platform      	condition         
   џџџџ   
   financial cost            0.00      phase                 џџџџ      priority              џџџџ      role         џџџџ      user id              џџџџ            fj   і   l          
   source   
       
   bursty_source_measure   
          	processor                   Traffic Generation Parameters   џџџ   
            count    џџџ   
   џџџџ   
      list   	џџџ   
            
Start Time   џџџ   
   constant (0.5)   
      ON State Time   џџџ   
   constant (10.0)   
      OFF State Time   џџџ   
   constant (0.0)   
      Packet Generation Arguments   џџџ   
            count    џџџ   
   џџџџ   
      list   	џџџ   
            Interarrival Time   џџџ   
   constant (1)   
      Packet Size   џџџ   
   constant (300)   
   
   
   
   
      fl  R   l          
   sink   
       
   sink_measure_user   
          	processor                   fn  $             
   wlan_mac_intf   
       
   wlan_mac_interface_measure   
          	processor                   Destination Address    џџџ   	џџџў   Random   	   	  f|   і  $          
   wlan_port_rx0   
       J            count    џџџ   
   џџџџ   
      list   	џџџ   N            	data rate   џџџ   
A    џџџџ   
      packet formats   џџџ   
   unformatted,all formatted   
      	bandwidth   џџџ   
@Г     џџџџ   
      min frequency   џџџ   N@@     џџџџ   N   N   J       
   dpsk   
       ?№      џџџџ          
?6тыC-џџџџ   
       
   NONE   
       
   dra_power_measure   
       
   dra_bkgnoise_measure   
       
   
dra_inoise   
          dra_snr          
   dra_ber   
       
   dra_error_measure   
       
   dra_ecc_measure   
          ra_rx                       nd_radio_receiver        f  R  $          
   wlan_port_tx0   
       J            count    џџџ   
   џџџџ   
      list   	џџџ   N            	data rate   џџџ   
A    џџџџ   
      packet formats   џџџ   
   unformatted,all formatted   
      	bandwidth   џџџ   
@Г     џџџџ   
      min frequency   џџџ   N@@     џџџџ   N      spreading code   џџџ   
дВI­%У}џџџџ   
      power   џџџ   
?tzсGЎ{џџџџ   
   N   J       
   dpsk   
       
   dra_rxgroup_measure   
       
   	dra_txdel   
       
   dra_closure_measure   
       
   dra_chanmatch_measure   
       
   NONE   
       
   dra_propdel_measure   
          ra_tx                       nd_radio_transmitter        f  $   і          
   wireless_lan_mac   
       
   measure_inter   
          queue                         fj  fn        k     k        
       
   strm_10   
       
   src stream [0]   
       
   dest stream [0]   
          џџџџ                  џџџџ          
@U џџџџџ   
                                        nd_packet_stream            fn  fl     (     (   l  E   l   
       
   strm_11   
       
   src stream [0]   
       
   dest stream [0]   
          џџџџ                  џџџџ          
@џ  џџџџ   
                                        nd_packet_stream            f  fn        э         
       
   strm_12   
       
   src stream [0]   
       
   dest stream [1]   
          џџџџ                  џџџџ             џџџџ                                           nd_packet_stream            f  f     )   њ  R  &   
       
   strm_13   
       
   src stream [1]   
       
   dest stream [0]   
          џџџџ                  џџџџ             џџџџ                                           nd_packet_stream            fn  f     +   Ђ  ,   ю   
       
   strm_14   
       
   src stream [1]   
       
   dest stream [0]   
          џџџџ                  џџџџ          
@џ  џџџџ   
                                        nd_packet_stream            f|  f      ћ         љ   
       
   strm_15   
       
   src stream [0]   
       
   dest stream [1]   
          џџџџ                  џџџџ          
@џ  џџџџ   
                                        nd_packet_stream           f|  f      і     ѕ   љ  "   ј          
   stat_0   
       
   channel [0]   
       
   busy   
       
   
instat [0]   
          џџџџ                  џџџџ          
    џџџџ   
       
    џџџџ   
           џџџџ              џџџџ          дВI­%У}џџџџ          дВI­%У}џџџџ          
@   џџџџ   
                                        nd_statistic_wire           f  f     S  %  P   і  "   і          
   stat_1   
       
   channel [0]   
       
   queue size (packets)   
       
   
instat [1]   
          џџџџ                  џџџџ          
    џџџџ   
       
    џџџџ   
           џџџџ              џџџџ          дВI­%У}џџџџ          дВI­%У}џџџџ          
@   џџџџ   
                                        nd_statistic_wire     f      ,   &wireless_lan_mac.Backoff Slots (slots)   Backoff Slots (slots)           Wireless Lan   bucket/default total/sum   linear   Wireless Lan   0wireless_lan_mac.Control Traffic Rcvd (bits/sec)   Control Traffic Rcvd (bits/sec)           Wireless Lan   bucket/default total/sum_time   linear   Wireless Lan   3wireless_lan_mac.Control Traffic Rcvd (packets/sec)   "Control Traffic Rcvd (packets/sec)           Wireless Lan   bucket/default total/sum_time   linear   Wireless Lan   0wireless_lan_mac.Control Traffic Sent (bits/sec)   Control Traffic Sent (bits/sec)           Wireless Lan   bucket/default total/sum_time   linear   Wireless Lan   3wireless_lan_mac.Control Traffic Sent (packets/sec)   "Control Traffic Sent (packets/sec)           Wireless Lan   bucket/default total/sum_time   linear   Wireless Lan   3wireless_lan_mac.Management Traffic Rcvd (bits/sec)   "Management Traffic Rcvd (bits/sec)           Wireless Lan   bucket/default total/sum_time   linear   Wireless Lan   6wireless_lan_mac.Management Traffic Rcvd (packets/sec)   %Management Traffic Rcvd (packets/sec)           Wireless Lan   bucket/default total/sum_time   linear   Wireless Lan   3wireless_lan_mac.Management Traffic Sent (bits/sec)   "Management Traffic Sent (bits/sec)           Wireless Lan   bucket/default total/sum_time   linear   Wireless Lan   6wireless_lan_mac.Management Traffic Sent (packets/sec)   %Management Traffic Sent (packets/sec)           Wireless Lan   bucket/default total/sum_time   linear   Wireless Lan   -wireless_lan_mac.Data Traffic Rcvd (bits/sec)   Data Traffic Rcvd (bits/sec)           Wireless Lan   bucket/default total/sum_time   linear   Wireless Lan   0wireless_lan_mac.Data Traffic Rcvd (packets/sec)   Data Traffic Rcvd (packets/sec)           Wireless Lan   bucket/default total/sum_time   linear   Wireless Lan   -wireless_lan_mac.Data Traffic Sent (bits/sec)   Data Traffic Sent (bits/sec)           Wireless Lan   bucket/default total/sum_time   linear   Wireless Lan   0wireless_lan_mac.Data Traffic Sent (packets/sec)   Data Traffic Sent (packets/sec)           Wireless Lan   bucket/default total/sum_time   linear   Wireless Lan    wireless_lan_mac.Load (bits/sec)   Load (bits/sec)           Wireless Lan   bucket/default total/sum_time   linear   Wireless Lan   #wireless_lan_mac.Load (packets/sec)   Load (packets/sec)           Wireless Lan   bucket/default total/sum_time   linear   Wireless Lan   &wireless_lan_mac.Throughput (bits/sec)   Throughput (bits/sec)           Wireless Lan   bucket/default total/sum_time   linear   Wireless Lan   2wireless_lan_mac.Retransmission Attempts (packets)   !Retransmission Attempts (packets)           Wireless Lan   bucket/default total/sum   linear   Wireless Lan   )wireless_lan_mac.Media Access Delay (sec)   Media Access Delay (sec)           Wireless Lan    bucket/default total/sample mean   linear   Wireless Lan   wireless_lan_mac.Delay (sec)   Delay (sec)           Wireless Lan    bucket/default total/sample mean   linear   Wireless Lan    wireless_lan_mac.AP Connectivity   AP Connectivity           Wireless Lan   normal   square-wave   Wireless Lan   :wireless_lan_mac.Data Dropped (Buffer Overflow) (bits/sec)   )Data Dropped (Buffer Overflow) (bits/sec)           Wireless Lan   bucket/default total/sum_time   linear   Wireless Lan   =wireless_lan_mac.Data Dropped (Buffer Overflow) (packets/sec)   ,Data Dropped (Buffer Overflow) (packets/sec)           Wireless Lan   bucket/default total/sum_time   linear   Wireless Lan   Cwireless_lan_mac.Data Dropped (Retry Threshold Exceeded) (bits/sec)   2Data Dropped (Retry Threshold Exceeded) (bits/sec)           Wireless Lan   bucket/default total/sum_time   linear   Wireless Lan   %wireless_lan_mac.Queue Size (packets)   Queue Size (packets)           Wireless Lan   !bucket/default total/time average   linear   Wireless Lan   Fwireless_lan_mac.Data Dropped (Retry Threshold Exceeded) (packets/sec)   5Data Dropped (Retry Threshold Exceeded) (packets/sec)           Wireless Lan   bucket/default total/sum_time   linear   Wireless Lan   6wireless_lan_mac.Management Traffic Dropped (bits/sec)   %Management Traffic Dropped (bits/sec)           Wireless Lan   bucket/default total/sum_time   linear   Wireless Lan   9wireless_lan_mac.Management Traffic Dropped (packets/sec)   (Management Traffic Dropped (packets/sec)           Wireless Lan   bucket/default total/sum_time   linear   Wireless Lan    wireless_lan_mac.Load (bits/sec)   Load (bits/sec)           WLAN (Per HCF Access Category)   bucket/default total/sum_time   linear   WLAN (Per HCF Access Category)   #wireless_lan_mac.Load (packets/sec)   Load (packets/sec)           WLAN (Per HCF Access Category)   bucket/default total/sum_time   linear   WLAN (Per HCF Access Category)   (wireless_lan_mac.AC Queue Size (packets)   AC Queue Size (packets)           WLAN (Per HCF Access Category)   !bucket/default total/time average   linear   WLAN (Per HCF Access Category)   :wireless_lan_mac.Data Dropped (Buffer Overflow) (bits/sec)   )Data Dropped (Buffer Overflow) (bits/sec)           WLAN (Per HCF Access Category)   bucket/default total/sum_time   linear   WLAN (Per HCF Access Category)   =wireless_lan_mac.Data Dropped (Buffer Overflow) (packets/sec)   ,Data Dropped (Buffer Overflow) (packets/sec)           WLAN (Per HCF Access Category)   bucket/default total/sum_time   linear   WLAN (Per HCF Access Category)   &wireless_lan_mac.Backoff Slots (slots)   Backoff Slots (slots)           WLAN (Per HCF Access Category)   bucket/default total/sum   linear   WLAN (Per HCF Access Category)   )wireless_lan_mac.Internal Collision Count   Internal Collision Count           WLAN (Per HCF Access Category)   bucket/default total/sum   linear   WLAN (Per HCF Access Category)   )wireless_lan_mac.Media Access Delay (sec)   Media Access Delay (sec)           WLAN (Per HCF Access Category)    bucket/default total/sample mean   linear   WLAN (Per HCF Access Category)   Cwireless_lan_mac.Data Dropped (Retry Threshold Exceeded) (bits/sec)   2Data Dropped (Retry Threshold Exceeded) (bits/sec)           WLAN (Per HCF Access Category)   bucket/default total/sum_time   linear   WLAN (Per HCF Access Category)   Fwireless_lan_mac.Data Dropped (Retry Threshold Exceeded) (packets/sec)   5Data Dropped (Retry Threshold Exceeded) (packets/sec)           WLAN (Per HCF Access Category)   bucket/default total/sum_time   linear   WLAN (Per HCF Access Category)   wireless_lan_mac.Delay (sec)   Delay (sec)           WLAN (Per HCF Access Category)    bucket/default total/sample mean   linear   WLAN (Per HCF Access Category)   &wireless_lan_mac.Throughput (bits/sec)   Throughput (bits/sec)           WLAN (Per HCF Access Category)   bucket/default total/sum_time   linear   WLAN (Per HCF Access Category)   sink.End-to-End Delay (seconds)   End-to-End Delay (seconds)           Traffic Sink    bucket/default total/sample mean   linear   Traffic Sink   sink.Traffic Received (bits)   Traffic Received (bits)           Traffic Sink   bucket/default total/sum   linear   Traffic Sink    sink.Traffic Received (bits/sec)   Traffic Received (bits/sec)           Traffic Sink   bucket/default total/sum_time   linear   Traffic Sink   sink.Traffic Received (packets)   Traffic Received (packets)           Traffic Sink   bucket/default total/sum   linear   Traffic Sink   #sink.Traffic Received (packets/sec)   Traffic Received (packets/sec)           Traffic Sink   bucket/default total/sum_time   linear   Traffic Sink                    
   text_0   
       
      ;sourceНјГЬЃКAPжааФНкЕугаШЈВњЩњЬНВтОрРыБЈЮФЃЌЗЂЫЭИјЦфЫћНкЕу;   GSinkНјГЬЃКЗЧAPНкЕуЪеЕНЪ§ОнБЈЮФЃЌвЦЖЏЕиРэЮЛжУЃЌЭЃжЙНгЪеЪ§ОнЃЌдђОВжЙВЛЖЏ(   !ИУНкЕуИКд№ЭЈжЊAPЃЌвбО­ЭЃжЙвЦЖЏ)ЃЛ   1wlan_mac_intrЃКНгПкВуЃЌИКд№зЊЗЂЩЯЯТВуЕФЪ§ОнБЈЮФЃЛ   #wireless_lan_macЃКИКд№ЗЂЫЭЪ§ОнБЈЮФ;   rx0КЭtx0ЃКИКд№ЮяРэЪеЗЂЪ§Он;   !ЫљгаЕФНкЕуЗЂЫЭЭъБЯЃЌНјааЖЏЛ­ЛцЭМ;   
          џџџџ             џџџџ              џџџџ          
@     џџџџ   
       
@a      џџџџ   
       
@q      џџџџ   
       
@]     џџџџ   
          
annot_text             Annotation Palette          
VЎџџџџ   
       џџџџ          
@РРРџџџџ   
               џџџџ              џџџџ           џџџџ      machine type       station   Model Attributes      14.5.A-January18-2008                interface type       
IEEE802.11   interface class       access           wlan_port_tx<n>   wlan_port_rx<n>           