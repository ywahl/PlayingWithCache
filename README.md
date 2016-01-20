# PlayingWithCache
sample applications to see effect of CPU cache

To compile the applications:
bash compile.sh

Example how to run datalocality:
================================

Not exploiting locality

$ time ./datalocality

real    0m1.866s
user    0m1.850s
sys     0m0.019s

Exploiting locality

$ time ./datalocality 1

real    0m0.088s
user    0m0.066s
sys     0m0.022s



$ time ./cachecoherency 1000000000 0 100 0 8
Level 1 Data cache line size=64
rd_offset=0 wr_offset=100 num_iter=1000000000
writeThread started on cpu=8 ret_affinity=0
readThread started on cpu=0 ret_affinity=0
finished wr_data=1000000000 rd_data=1000000000

real    0m3.512s
user    0m6.930s
sys     0m0.038s


Example how to run cachecoherency:
==================================
Examples executed on a CPU with the following charateristics

2 CPU sockets (2 numa nodes) with each 8 cpu cores each.
Each core has 2 hyperthreads



Each thread runs on different NUMA node + data offset between read & write > L1 cache size
==========================================================================================

$ time ./cachecoherency 1000000000 0 100 0 8
Level 1 Data cache line size=64
rd_offset=0 wr_offset=100 num_iter=1000000000
writeThread started on cpu=8 ret_affinity=0
readThread started on cpu=0 ret_affinity=0
finished wr_data=1000000000 rd_data=1000000000

real    0m3.512s
user    0m6.930s
sys     0m0.038s


Each thread runs on different NUMA node + data offset between read & write < L1 cache size
==========================================================================================
$ time ./cachecoherency 1000000000 0 1 0 8
Level 1 Data cache line size=64
rd_offset=0 wr_offset=1 num_iter=1000000000
readThread started on cpu=0 ret_affinity=0
writeThread started on cpu=8 ret_affinity=0
finished wr_data=1000000000 rd_data=1000000000

real    0m9.686s
user    0m17.690s
sys     0m0.048s


Each thread runs on same NUMA node + data offset between read & write < L1 cache size
==========================================================================================
$ time ./cachecoherency 1000000000 0 1 0 5
Level 1 Data cache line size=64
rd_offset=0 wr_offset=1 num_iter=1000000000
readThread started on cpu=0 ret_affinity=0
writeThread started on cpu=5 ret_affinity=0
finished wr_data=1000000000 rd_data=1000000000

real    0m8.952s
user    0m15.920s
sys     0m0.056s


Each thread runs on same hyperthread + data offset between read & write < L1 cache size
==========================================================================================
$ time ./cachecoherency 1000000000 0 1 0 0
Level 1 Data cache line size=64
rd_offset=0 wr_offset=1 num_iter=1000000000
readThread started on cpu=0 ret_affinity=0
writeThread started on cpu=0 ret_affinity=0
finished wr_data=1000000000 rd_data=1000000000

real    0m6.821s
user    0m6.824s
sys     0m0.003s


Each thread runs on different hyperthread of same cpu core+ data offset between read & write < L1 cache size
==========================================================================================
$ time ./cachecoherency 1000000000 0 1 0 16
Level 1 Data cache line size=64
rd_offset=0 wr_offset=1 num_iter=1000000000
writeThread started on cpu=16 ret_affinity=0
readThread started on cpu=0 ret_affinity=0
finished wr_data=1000000000 rd_data=1000000000

real    0m7.055s
user    0m11.320s
sys     0m0.032s

