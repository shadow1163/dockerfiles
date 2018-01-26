/*
 * This file contains the system call numbers.
 */
#define SYSCALL_restart_syscall          0
#define SYSCALL_exit                     1
#define SYSCALL_fork                     2
#define SYSCALL_read                     3
#define SYSCALL_write                    4
#define SYSCALL_open                     5
#define SYSCALL_close                    6
#define SYSCALL_waitpid                  7
#define SYSCALL_creat                    8
#define SYSCALL_link                     9
#define SYSCALL_unlink                  10
#define SYSCALL_execve                  11
#define SYSCALL_chdir                   12
#define SYSCALL_time                    13
#define SYSCALL_mknod                   14
#define SYSCALL_chmod                   15
#define SYSCALL_lchown32                16
#define SYSCALL_break                   17
#define SYSCALL_oldstat                 18
#define SYSCALL_lseek                   19
#define SYSCALL_getpid                  20
#define SYSCALL_mount                   21
#define SYSCALL_umount                  22
#define SYSCALL_setuid32                23
#define SYSCALL_getuid32                24
#define SYSCALL_stime                   25
#define SYSCALL_ptrace                  26
#define SYSCALL_alarm                   27
#define SYSCALL_oldfstat                28
#define SYSCALL_pause                   29
#define SYSCALL_utime                   30
#define SYSCALL_stty                    31
#define SYSCALL_gtty                    32
#define SYSCALL_access                  33
#define SYSCALL_nice                    34
#define SYSCALL_ftime                   35
#define SYSCALL_sync                    36
#define SYSCALL_kill                    37
#define SYSCALL_rename                  38
#define SYSCALL_mkdir                   39
#define SYSCALL_rmdir                   40
#define SYSCALL_dup                     41
#define SYSCALL_pipe                    42
#define SYSCALL_times                   43
#define SYSCALL_prof                    44
#define SYSCALL_brk                     45
#define SYSCALL_setgid32                46
#define SYSCALL_getgid32                47
#define SYSCALL_signal                  48
#define SYSCALL_geteuid32               49
#define SYSCALL_getegid32               50
#define SYSCALL_acct                    51
#define SYSCALL_umount2                 52
#define SYSCALL_lock                    53
#define SYSCALL_ioctl                   54
#define SYSCALL_fcntl                   55
#define SYSCALL_mpx                     56
#define SYSCALL_setpgid                 57
#define SYSCALL_ulimit                  58
#define SYSCALL_oldolduname             59
#define SYSCALL_umask                   60
#define SYSCALL_chroot                  61
#define SYSCALL_ustat                   62
#define SYSCALL_dup2                    63
#define SYSCALL_getppid                 64
#define SYSCALL_getpgrp                 65
#define SYSCALL_setsid                  66
#define SYSCALL_sigaction               67
#define SYSCALL_sgetmask                68
#define SYSCALL_ssetmask                69
#define SYSCALL_setreuid32              70
#define SYSCALL_setregid32              71
#define SYSCALL_sigsuspend              72
#define SYSCALL_sigpending              73
#define SYSCALL_sethostname             74
#define SYSCALL_setrlimit               75
#define SYSCALL_getrlimit               76
#define SYSCALL_getrusage               77
#define SYSCALL_gettimeofday            78
#define SYSCALL_settimeofday            79
#define SYSCALL_getgroups32             80
#define SYSCALL_setgroups32             81
#define SYSCALL_select                  82
#define SYSCALL_symlink                 83
#define SYSCALL_oldlstat                84
#define SYSCALL_readlink                85
#define SYSCALL_uselib                  86
#define SYSCALL_swapon                  87
#define SYSCALL_reboot                  88
#define SYSCALL_readdir                 89
#define SYSCALL_mmap                    90
#define SYSCALL_munmap                  91
#define SYSCALL_truncate                92
#define SYSCALL_ftruncate               93
#define SYSCALL_fchmod                  94
#define SYSCALL_fchown32                95
#define SYSCALL_getpriority             96
#define SYSCALL_setpriority             97
#define SYSCALL_profil                  98
#define SYSCALL_statfs                  99
#define SYSCALL_fstatfs                100
#define SYSCALL_ioperm                 101
#define SYSCALL_socketcall             102
#define SYSCALL_syslog                 103
#define SYSCALL_setitimer              104
#define SYSCALL_getitimer              105
#define SYSCALL_stat                   106
#define SYSCALL_lstat                  107
#define SYSCALL_fstat                  108
#define SYSCALL_olduname               109
#define SYSCALL_iopl                   110
#define SYSCALL_vhangup                111
#define SYSCALL_idle                   112
#define SYSCALL_vm86                   113
#define SYSCALL_wait4                  114
#define SYSCALL_swapoff                115
#define SYSCALL_sysinfo                116
#define SYSCALL_ipc                    117
#define SYSCALL_fsync                  118
#define SYSCALL_sigreturn              119
#define SYSCALL_clone                  120
#define SYSCALL_setdomainname          121
#define SYSCALL_uname                  122
#define SYSCALL_modify_ldt             123
#define SYSCALL_adjtimex               124
#define SYSCALL_mprotect               125
#define SYSCALL_sigprocmask            126
#define SYSCALL_create_module          127
#define SYSCALL_init_module            128
#define SYSCALL_delete_module          129
#define SYSCALL_get_kernel_syms        130
#define SYSCALL_quotactl               131
#define SYSCALL_getpgid                132
#define SYSCALL_fchdir                 133
#define SYSCALL_bdflush                134
#define SYSCALL_sysfs                  135
#define SYSCALL_personality            136
#define SYSCALL_afs_syscall            137 /* Syscall for Andrew File System */
#define SYSCALL_setfsuid32             138
#define SYSCALL_setfsgid32             139
#define SYSCALL__llseek                140
#define SYSCALL_getdents               141
#define SYSCALL__newselect             142
#define SYSCALL_flock                  143
#define SYSCALL_msync                  144
#define SYSCALL_readv                  145
#define SYSCALL_writev                 146
#define SYSCALL_getsid                 147
#define SYSCALL_fdatasync              148
#define SYSCALL__sysctl                149
#define SYSCALL_mlock                  150
#define SYSCALL_munlock                151
#define SYSCALL_mlockall               152
#define SYSCALL_munlockall             153
#define SYSCALL_sched_setparam         154
#define SYSCALL_sched_getparam         155
#define SYSCALL_sched_setscheduler     156
#define SYSCALL_sched_getscheduler     157
#define SYSCALL_sched_yield            158
#define SYSCALL_sched_get_priority_max 159
#define SYSCALL_sched_get_priority_min 160
#define SYSCALL_sched_rr_get_interval  161
#define SYSCALL_nanosleep              162
#define SYSCALL_mremap                 163
#define SYSCALL_setresuid32            164
#define SYSCALL_getresuid32            165
#define SYSCALL_query_module           166
#define SYSCALL_poll                   167
#define SYSCALL_nfsservctl             168
#define SYSCALL_setresgid32            169
#define SYSCALL_getresgid32            170
#define SYSCALL_prctl                  171
#define SYSCALL_rt_sigreturn           172
#define SYSCALL_rt_sigaction           173
#define SYSCALL_rt_sigprocmask         174
#define SYSCALL_rt_sigpending          175
#define SYSCALL_rt_sigtimedwait        176
#define SYSCALL_rt_sigqueueinfo        177
#define SYSCALL_rt_sigsuspend          178
#define SYSCALL_pread64                179
#define SYSCALL_pwrite64               180
#define SYSCALL_chown32                181
#define SYSCALL_getcwd                 182
#define SYSCALL_capget                 183
#define SYSCALL_capset                 184
#define SYSCALL_sigaltstack            185
#define SYSCALL_sendfile               186
#define SYSCALL_getpmsg                187     /* some people actually want streams */
#define SYSCALL_putpmsg                188     /* some people actually want streams */
#define SYSCALL_vfork                  189
#define SYSCALL_ugetrlimit             190     /* SuS compliant getrlimit */
#define SYSCALL_readahead              191
#define SYSCALL_mmap2                  192
#define SYSCALL_truncate64             193
#define SYSCALL_ftruncate64            194
#define SYSCALL_stat64                 195
#define SYSCALL_lstat64                196
#define SYSCALL_fstat64                197
#define SYSCALL_pciconfig_read         198
#define SYSCALL_pciconfig_write        199
#define SYSCALL_pciconfig_iobase       200
#define SYSCALL_multiplexer            201
#define SYSCALL_getdents64             202
#define SYSCALL_pivot_root             203
#define SYSCALL_fcntl64                204
#define SYSCALL_madvise                205
#define SYSCALL_mincore                206
#define SYSCALL_gettid                 207
#define SYSCALL_tkill                  208
#define SYSCALL_setxattr               209
#define SYSCALL_lsetxattr              210
#define SYSCALL_fsetxattr              211
#define SYSCALL_getxattr               212
#define SYSCALL_lgetxattr              213
#define SYSCALL_fgetxattr              214
#define SYSCALL_listxattr              215
#define SYSCALL_llistxattr             216
#define SYSCALL_flistxattr             217
#define SYSCALL_removexattr            218
#define SYSCALL_lremovexattr           219
#define SYSCALL_fremovexattr           220
#define SYSCALL_futex                  221
#define SYSCALL_sched_setaffinity      222
#define SYSCALL_sched_getaffinity      223
/* 224 currently unused */
#define SYSCALL_tuxcall                225
#define SYSCALL_sendfile64             226
#define SYSCALL_io_setup               227
#define SYSCALL_io_destroy             228
#define SYSCALL_io_getevents           229
#define SYSCALL_io_submit              230
#define SYSCALL_io_cancel              231
#define SYSCALL_set_tid_address        232
#define SYSCALL_fadvise64              233
#define SYSCALL_exit_group             234
#define SYSCALL_lookup_dcookie         235
#define SYSCALL_epoll_create           236
#define SYSCALL_epoll_ctl              237
#define SYSCALL_epoll_wait             238
#define SYSCALL_remap_file_pages       239
#define SYSCALL_timer_create           240
#define SYSCALL_timer_settime          241
#define SYSCALL_timer_gettime          242
#define SYSCALL_timer_getoverrun       243
#define SYSCALL_timer_delete           244
#define SYSCALL_clock_settime          245
#define SYSCALL_clock_gettime          246
#define SYSCALL_clock_getres           247
#define SYSCALL_clock_nanosleep        248
#define SYSCALL_swapcontext            249
#define SYSCALL_tgkill                 250
#define SYSCALL_utimes                 251
#define SYSCALL_statfs64               252
#define SYSCALL_fstatfs64              253
#define SYSCALL_fadvise64_64           254
#define SYSCALL_rtas		255
#define SYSCALL_sys_debug_setcontext 256
/* Number 257 is reserved for vserver */
#define SYSCALL_migrate_pages	258
#define SYSCALL_mbind		259
#define SYSCALL_get_mempolicy	260
#define SYSCALL_set_mempolicy	261
#define SYSCALL_mq_open		262
#define SYSCALL_mq_unlink		263
#define SYSCALL_mq_timedsend	264
#define SYSCALL_mq_timedreceive	265
#define SYSCALL_mq_notify		266
#define SYSCALL_mq_getsetattr	267
#define SYSCALL_kexec_load		268
#define SYSCALL_add_key		269
#define SYSCALL_request_key	270
#define SYSCALL_keyctl		271
#define SYSCALL_waitid		272
#define SYSCALL_ioprio_set		273
#define SYSCALL_ioprio_get		274
#define SYSCALL_inotify_init	275
#define SYSCALL_inotify_add_watch	276
#define SYSCALL_inotify_rm_watch	277
#define SYSCALL_spu_run		278
#define SYSCALL_spu_create		279
#define SYSCALL_pselect6		280
#define SYSCALL_ppoll		281
#define SYSCALL_unshare		282
#define SYSCALL_splice		283
#define SYSCALL_tee		284
#define SYSCALL_vmsplice		285
#define SYSCALL_openat		286
#define SYSCALL_mkdirat		287
#define SYSCALL_mknodat		288
#define SYSCALL_fchownat		289
#define SYSCALL_futimesat		290
#define SYSCALL_newfstatat		291
#define SYSCALL_unlinkat		292
#define SYSCALL_renameat		293
#define SYSCALL_linkat		294
#define SYSCALL_symlinkat		295
#define SYSCALL_readlinkat		296
#define SYSCALL_fchmodat		297
#define SYSCALL_faccessat		298
#define SYSCALL_get_robust_list	299
#define SYSCALL_set_robust_list	300
#define SYSCALL_move_pages		301
#define SYSCALL_getcpu		302
#define SYSCALL_epoll_pwait	303
#define SYSCALL_utimensat		304
#define SYSCALL_signalfd		305
#define SYSCALL_timerfd		306
#define SYSCALL_eventfd		307
#define SYSCALL_sync_file_range2	308
#define SYSCALL_fallocate		309
#define SYSCALL_subpage_prot		310
#define SYSCALL_timerfd_settime	311
#define SYSCALL_timerfd_gettime	312
#define SYSCALL_signalfd4		313
#define SYSCALL_eventfd2		314
#define SYSCALL_epoll_create1	315
#define SYSCALL_dup3			316
#define SYSCALL_pipe2		317
#define SYSCALL_inotify_init1	318
