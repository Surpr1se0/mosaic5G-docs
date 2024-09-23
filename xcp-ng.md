<br />
<div align="center">
  <h1 align="center">XCP-ng</h1>
    <h4> Instalation</h4>
  <p align="center">
  </p>
  <br>
</div>

## Introduction: 

XCP-ng is a type 2 open-source hypervisor with capabilities of creating and managing Virtual Machines, from a remote host. XO (XenOrchestra) is the web-based management platform, the virtual appliance or the "dashboard" of XCP-ng in which we can perform several operations on the VM's, the storage units, network, create templates, etc. 

## Implementation: 

### 1. Tutorial seguido:** 

[Install XCP-ng | XCP-ng Documentation](https://docs.xcp-ng.org/installation/install-xcp-ng/)

### 2. Ativação do SSH para comunicações remotas:** 

[Enable SSH on XCP-ng](https://support.citrix.com/s/article/CTX238295-how-to-enabledisable-ssh-on-xenserver-host?language=en_US)

### 3. Instalação do Web UI (XOA):**

*Nota: a ligação à VPN do DEI já tem de estar efetuada - foi utilizado o OpenVPN*

[Xen Orchestra Web UI | XCP-ng Documentation](https://docs.xcp-ng.org/management/manage-at-scale/xo-web-ui/)

Uma virtual appliance que deve aparecer na forma de uma nova VM no dashboard. Feito através deste link: https://vates.tech/deploy/ (como root).

Apartir deste momento, podemos entrar no dashboard através do link: https://[ip_da_máquina].

### 4. Instalação do XCP-ng Center (opcional)**

https://github.com/xcp-ng/xenadmin

> *This software is depreceated*

### 5. Download do ISO ubuntu 22.04 LTS:**

[Ubuntu 22.04.5 LTS (Jammy Jellyfish)](https://releases.ubuntu.com/jammy/)

### 6. Inserir um novo ficheiro .iso remotamente (sem accesso a servidores/serviços NFS, iSCSI, …):**

**First Step:** Create a new storage unit

<div style="text-align: center;">
  <img src="img/xcp-ng img/Screenshot_3.png" alt="descrição" width="300"/>
</div>

**Second Step:** Configure the storage

<div style="text-align: center;">
  <img src="img/xcp-ng img/Screenshot_2.png" alt="descrição" width="500"/>
</div>

**Third Step:** Import a new disc (this will be our .iso)

<div style="text-align: center;">
  <img src="img/xcp-ng img/Screenshot_4.png" alt="descrição" width="300"/>
</div>

**Fourth Step:** Drag and drop the .iso file

<div style="text-align: center;">
  <img src="img/xcp-ng img/Screenshot_5.png" alt="descrição" width="500"/>
</div>

##  Passos Adicionais:

### 6. Ativação do SSH na VM (ubuntu):** 

```bash
sudo apt update
sudo apt upgrade
sudo apt install openssh-server
systemctl enable ssh
systemctl start ssh
vi /etc/ssh/sshd_config
	#Change the PermitRootLogin to ON
systemctl restart ssh
	# Always login with User and then Root.
```

## Links Úteis:

[Virtual Machines (VMs) | XCP-ng Documentation](https://docs.xcp-ng.org/vms/)

[xe command reference | XCP-ng Documentation](https://docs.xcp-ng.org/appendix/cli_reference/)

## Problems

### Problem no 1:

- After starting the eNodeB and UE - both containers are exited and the logs from the gNB report this:

```
== Starting gNB soft modem
Additional option(s): --sa -E --rfsim --log_config.global_log_options level,nocolor,time
/opt/oai-gnb/bin/nr-softmodem -O /opt/oai-gnb/etc/gnb.conf --sa -E --rfsim --log_config.global_log_options level,nocolor,time
[INFO  tini (1)] Spawned child process '/opt/oai-gnb/bin/entrypoint.sh' with pid '6'
[INFO  tini (1)] Main child exited with signal (with signal 'Illegal instruction')
```

### Problem no 2:

- /var/ folder got filled with 50gb of files - maybe implement logrotate, and evaluate the necessary space for the docker-compose and the implications of running docker-compose various times.

[View history of commands run in terminal - Ask Ubuntu](https://askubuntu.com/questions/624848/view-history-of-commands-run-in-terminal)

[logrotate(8) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man8/logrotate.8.html)

[Rotate and archive logs with the Linux logrotate command | Opensource.com](https://opensource.com/article/21/10/linux-logrotate)

[How To Manage Logfiles with Logrotate on Ubuntu 20.04 | DigitalOcean](https://www.digitalocean.com/community/tutorials/how-to-manage-logfiles-with-logrotate-on-ubuntu-20-04)


> "Solved" by using one of the snapshots created after setting up the OAI docker-compose and restoring the space.
> 

