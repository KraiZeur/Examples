General Hierarchy and terms:

CPU = Central Processing Unit
FPU = Floating Processing Unit

+ System (The global Computer will all subsequent hardware components) Only 1 System
  + Socket (A slot to install the physical chip) The system can accept multiple Socket
    + Cpu/Core (A Unit Processing that is part of the physical chip)
      + Thread (A Logical processing Unit part of a Core) [@see Hyperthreading]

Example:

2*Intel Core i7 6800K
6 Cores 12 Threads 3.40Ghz

A computer with two socket will have the following configuration:

+ System
  + Socket : 2x Intel Core i7 6800K
    + Cpu/Core : 6 Cores / Chip Shares L2/L3 caches
      + Thread 12Threads (2Threads / Core) Shares L1 cache
  
The System will have 2x6x2 = 24 Cpu's (Physical and logical)

