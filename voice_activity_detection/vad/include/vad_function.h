/*------------------------------------------------------------------
* Function : short vad(short *buf_frame);
*
* 1. *buf_frame : length 384 
*
* 2. return requestSiri  (0 or 1)
*   requestSiri==1 : trigger requestSiri 
*
*
*
--------------------------------------------------------------------*/
#pragma once

short vad(short *buf_frame , short num_step);

void vad_init();

void vad_del();

