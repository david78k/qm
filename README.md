qm
==

QEMU cluster management system

Manages a cluster consisting of physical machines (PMs) and virtual machines (VMs)

Todo list
- config file: cluster.config, xml?, machine information of PMs and VMs, qemu binary location path 
- machine information: name, ip address, number of CPUs, memory, MAC adress, 
- [VM only]: VM image path, PM name (for VM), VNC port, telnet port, migration incoming port

Available functions

- list
- create
- stop 
- shutdown: alias of stop
- migrate: mig.config
