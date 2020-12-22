# batch_thread_model_cplus
batch thread model written in c++

This file has two modes low mem mode (macro definition LOW_MEM) and high mem mode (no macro definition)

If you are running this on linux and have less than about 64 GB ram and 16 GB swap, run in low mem mode.
On mac you can run in either mode, though high mem mode may run really slowly due to memory compression and swapping to disk but it will finish.
On Windows 10 due to type changes when compiling, the amount of ram needed is reduced by half.

Both modes can finish in about the same time, so choose whichever one will run on your machine. 

Additionally this code base includes some python that will run at the end of the c++ application. If you want to turn this off comment out the system call in batch_thread_model.cpp and change the return statement to return 0.
