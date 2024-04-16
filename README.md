<!-- PROJECT LOGO -->
<br />
<div align="center">
  <h3 align="center">Mosaic5G Documentation</h3>
  <p align="center">
    @DEI.FCTUC
  </p>
  <br>
</div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#Introduction">Introduction</a>
    </li>
    <li>
      <a href="#useful-links">Useful Links</a>
    </li>
    <li><a href="#Mosaic5G">Mosaic5G</a></li>
      <ul>
        <li><a href="##Documentation">Documentation</a></li>
        <ul>
          <li><a href="#1-flexran-rtc">flexran-rtc</a></li>
          <li><a href="#2-jox">jox</a></li>
          <li><a href="#3-kube5g">kube5G</a></li>
          <li><a href="#4-ll-mec">ll-mec</a></li>
          <li><a href="#5-oai-cn">oai-cn</a></li>
          <li><a href="#6-oai-ran--oai-ue">oai-ran & oai-ue</a></li>
          <li><a href="#7-ovs-gtp">ovs-gtp</a></li>
          <li><a href="#8-store">store</a></li>
        </ul>
      </ul>
    <li><a href="#OAI">OAI</a></li>
        <ul>
        <li><a href="##Documentation">Documentation</a></li>
        </ul>
    <li><a href="#Implementation">Implementation</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
  <br>
</details>
<br>

# Introduction
The following `readme` tries to make sense of the enumerous projects and subprojects that the current *Mosaic5G* project has of today. We will approach the Mosaic5G repository available on Github and Gitlab, and the OAI repository available only on Gitlab.

It (more or less) tries to follow, for all the mechanisms and components of the 5G Network (and sometimes 4G LTE), this basic template: 
  1. A small summary about what is the component
  2. The last known update for the project repository
  3. The key elements: not necessarily the subprojects, but the main features, or main utilities or mechanisms inside within the component
  4. Required packages for the instantiation of the component
  5. Supported OS and Kernel (with some recommended hardware settings, if mencioned)
  6. Additional Comments

# Useful Links

These descriptions where retrieved from the following links and articles provided inside of these: 

```md
Project Link: https://deiucpt.sharepoint.com/sites/MOSAIC5G
Mosaic5G Home Website: https://mosaic5g.io/
```

These are the repositories that are useful: 
  - **github** project folder for [mosaic5g](https://github.com/simula/mosaic5g-mosaic5g/tree/master)
  - **gitlab** project folder for [mosaic5g](https://gitlab.eurecom.fr/mosaic5g)
  - **gitlab** project folder for [oai](https://gitlab.eurecom.fr/oai/cn5g)


# Mosaic5G

##  **Documentation**

## 1. flexran-rtc

```md
Platform to apply the *Software Defined Networking* principle at the RAN domain that enables software defined RAN (SD-RAN).
```

> Authored 3 years ago

It is the real-time access network controller. It is composed by 2 main key elements (*software speaking*): 

  - **RTC or Real time controller**: enables coordinated control over multiple RANs, reveals high/low-level primitives and provision SDKs for control application.
  - **RAN Runtime**: acts as a local agent controlled by RTC, virtualizes the underlying RAN radio resources, pipelines the RAN service function chain, and provides SDKs enabling distributed control applications. Further, the RAN runtime can support various slice requirements (e.g., isolation) and also improve multiplexing benefits (e.g., sharing) in terms of radio resource abstractions and modularized/customized RAN compositions for RAN slicing purpose.

Essentially it aims to provide a programmable architecture for SD-RANs. It aims to separate the control and data plane of the RAN.

### Required Packages:

```bash
Cmake >= 3.5
libprotobuf v3.3.0
boost >= 1.54 (libboost-system-dev, libboost-program-options-dev)
log4cxx >= 0.10.0 (liblog4cxx-dev liblog4cxx10v5)
curl-dev (libcurl4-openssl-dev)
Compiler with support for C++11
Pistache library (e.g. later than Nov 22, 2017) for RESTful northbound API support
Optionally: nodejs >= 4.2.6
```

These packages are only required if you are not running the script `install_dependencies` in the tools directory. *Otherwise, we must install these dependencies individually to run the build.*

### Supported Versions: 
```bash
Ubuntu 16.04 and 18.04 (no specified kernel version)
```


## 2. jox


```md
This is an event-driven Juju-based service orchestrator core with several plugins to interact with different network domains, e.g., RAN and CN
```

> Authored 4 years ago

The JOX architecture includes two main components: 
  - **JOX core:** comprises both JSlice (representing each slice as a set of models with a policy) and JCloud (host and control the underlying cloud resources) controllers to control slice and cloud resources respectively.
  - **JOX plugin framework**: enables different plugins for RAN, CN, MEC, and VIM to enable fast reactions like event handling and monitoring.

Essentially, using JOX we can define network slices that can be independently optimized with specific configurations on its resources, network functions, and service chains. 

### Required Packages:

```bash
Juju version 2.5.1 (Juju version 2+ recommended)
```

These packages are only required if you are not running the script `install_dependencies` located in the tools directory.

### Supported Versions: 
```bash
Ubuntu 14.04, 16.04 and 18.04 (no specified kernel version)
Testbed w/ Ubuntu VM, 5G RAM, 12GB Memory.
```

## 3. kube5G

```md
Cloud-Native Agile 5G Service Platforms. 
```

> Authored 4 years ago

This project does not represent a specific element of a 5G network like the last ones. It simply aims to facilitate the deployment and management of 4G/5G network and services in cloud like environments - kubernetes and OpenShitft. 

Within the project we get:
  - **Dockers**: contains tools and scripts for building Docker images of OpenAirInterface (OAI) and deploying 4G/5G networks using Docker and Docker Compose.
  - **Kubernetes**: contains examples and configurations for deploying 4G/5G networks using Kubernetes.
  - **OpenShift**: includes the m5g-operator, which is described as an orchestrator tool for managing 5G services in Kubernetes deployments.

With Kube5G we can deploy services on a VM , or any private/public cloud for R&D of cellular mobile networks and 5G purposes.

Every component of these 3 has diferent requirements:

#### Required Packages for docker/kubernets implementation: 

```bash
- One PC with Ubuntu 16.04/18.04 with (preferable) 8GB RAM
- Kubernetes deployment microk8s
- Snap enabled
- An USRP (as frontend) attached to the PC
- Commecial phone equipped with SIM card to be connected to the network

- GigaByte Box with ubuntu 16.04 and 16 BG RAM
- microk8s version: v1.14.10
- kubectl version: 1.17.3
- USRP: B210 mini
- Phone: Google pixel 2
```


#### Required Packages for Openshift (m5g-operator): 

```bash
Operator SDK v0.15.2
Golang 1.12+
Docker 17.03+
kubectl v1.11.3+
a Kubernetes environment (microk8s or minikube, etc)
Optional: [dep][https://golang.github.io/dep/docs/installation.html] version v0.5.0+.
Optional: delve version 1.2.0+ (for up local --enable-delve).

An above average machine (the testbed used was a i7-8700k, 16gb ram)
Ubuntu 18.04
```


## 4. ll-mec

```md
Low latency mobile/multi-RAT edge computing and core network controller platform. Core Network (CN) & Edge Controller is an ETSI-aligned MEC platform that can act as a software-defined core network controller.
```

> Authored 4 years ago

Within the LL-MEC platform, two main services are provided: 
  - **Edge packet service (EPS)** - (equivalent to traffic rule control) that manages the static and dynamic traffic rules and handles multiple OpenFlow libraries and OVS.
  - **Radio network information service (RNIS)** - extracts real-time RAN information (e.g., user and radio bearer statistics) and delegates the control decision over the user plane.

> *Note: the OVS or Open vSwitch is mencioned in another section, because it is included in a different subproject*. 

With LL-MEC we can develop RAN & CN network applications by leveraging LL-MEC and FlexRAN SDKs.

### Required Packages:

```bash
libssl-dev
autoconf
libtool
CMake version 2.8 or newer
GCC version 4.8.1 or newer (for c++11 support)
```

### Supported Versions: 
```bash
Ubuntu 16.04 and 18.04 | Kernel 4.9.0
```

## 5. oai-cn

```md
Implementation of OpenAirInterface Core Network.
```

> Authored 5 years ago

This is an implementation of a 3GPP compliant specificationEvolved Packet Core Network, with the following network elements: MME, HSS, S-GW, P-GW. The OAI-CN is in support of ll-mec (the controller). 

> *Note: for the documentation of this subsection we had to follow the github project, because the gitlab meta project is only available under subscription under eurocom accounts.* 

### Required Packages:

```bash
No required packages as they are all retrieved from the docker-compose script.
```

### Supported Versions: 
```bash
The CN without LL-MEC on snap has no special kernel dependencies. Some issues with kernel 4.15 have been observed and fixed.
```

## 6. oai-ran & oai-ue

```md
3GPP compliant implementations of 5G and LTE/LTE-A RAN.
```

> Extremely Recent

The documentation for this subproject is extense. This is a small summary:
  - The [**OAI EPC**](https://github.com/OPENAIRINTERFACE/openair-epc-fed/blob/master/docs/DEPLOY_HOME_MAGMA_MME.md) and [**OAI 5G Core**](https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-fed/-/blob/master/docs/DEPLOY_HOME.md) is developed in disctinct projects with their own documentation.
  - OAI softmodem sources, that aim to implement 3GPP compliant UEs, eNodeB and gNodeB can be downloaded from this [repository](https://gitlab.eurecom.fr/oai/openairinterface5g/-/blob/develop/doc/GET_SOURCES.md)
  - All of the sources are installed automatically with the build command. 
  - A list of [features](https://gitlab.eurecom.fr/oai/openairinterface5g/-/blob/develop/doc/FEATURE_SET.md) is also provided.
  - There are also some build tutorials for all the [subsubprojects](https://gitlab.eurecom.fr/oai/openairinterface5g/-/blob/develop/doc/BUILD.md) available (all the specific elements of the RAN & UE)


### Required Packages:

```bash
No required packages.
```

### Supported Versions: 
```bash
Ubuntu 18, 20, 22
Redhat 8, 9
Fedora 3.7
```

## 7. ovs-gtp


```md
OVS Switching with GTP Patch
```
This is an open-source implementation of a distributed virtual multilayer switch. The main purpose of OpenvSwitch is to provide a switching stack for hardware virtualization environments, while supporting multiple protocols and standards used in computer networks.

> Authored 4 years ago

This subproject is implemented as a necessary part of the ll-mec - in order to run ll-mec we need OpenvSwitching with GTP ability. 

### Required Packages:

```bash
- CMake version 2.8 or newer
- GCC version 4.8.1 or newer (for c++11 support)
```

### Supported Versions: 
```bash
Ubuntu 18.04, 16.04
```

## 8. store


```md
The network store - a repository of network functions, controll applications, SDK, charms, templates, images, tools and datasets.
```
> Authored 6 years ago

From the article mencioned previously (available in the project folder mencioned in the first section) we get:
*The Store is in form of a distribution repository that contains a constellation of platform packages, SDKs, control applications, datasets and models. It aims to develop and bundle plug-and-play (P&P) network applications tailored to a particular use case, and also to compose and customize a network service delivery platform across reusable applications. Each control application has its control purpose, and it relies on different granularities of net-work status information from the platform SDK and may further provide APIs to other control applications.*

There are different projects inside this subproject. The main one is flexRAN SDK, a software development kit, that has been developed to facilitate the use of the API that is exposed by flexRAN-rtc. Several applications have been made with this sdk:
  - RRM
  - monitoring
  - spectrum sharing apps
  - docker deployments of full topologies
  - kubernetes cluster deployment


### Required Packages:

```bash
Nothing was specified
```

### Supported Versions: 
```bash
Nothing was specified
```

# OAI

##  **Documentation**


#  **Implementation**


