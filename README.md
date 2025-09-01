<!-- PROJECT LOGO -->
<br />
<div align="center">
  <h3 align="center">Mosaic5G/Openairinterface Docs.</h3>
  <p align="center">
    @DEI.FCTUC
  </p>
  <br>
</div>


# Introduction
Welcome to the docs of the **Mosaic5G/Openairinterface**, which **currently** serves as repository for understanding, deploying, and managing the key components of the to be implemented system. 

> *Edit: This doc started out by being developed in the `readme` file of the repository but has since been converted to a Wiki format.* 


# Structure

The following repository contains (currently):
- Wiki
- Cenarios for **OpenAirInterface** implementation:

| ****    | **Component**                 | **Used for thesis**
| -------------- | ------------------------------- |------------------------
| Scenario 3     | CU + DU + FlexRIC + CN          |❌
| Scenario 4     | CUCP + CUUP + DU + FlexRIC + CN |❌
| Monolithic     | " Same nodes on 1 VM            |❌
| Disaggregated Cell-Based | " Same node on 4 Different Machines, with UE cell-based version | ✅
| Disaggregated Cell-Free | " Same node on 4 Different Machines, with UE cell-Free version |✅

- ...

## Wiki

The **wiki** covers the following subjects/sections:
- **Home**: introduction, general purpose notes and project architecture overview;
- **Mosaic5G Components**: description of every Mosaic5G component, including prerequisites and versions
- **OpenAirInterface Components**: description of every OpenAirInterface component, including prerequisites and versions;
- **FlexRAN & FlexRIC**: comparison of FlexRIC and FlexRAN, available features, xApp theory and xApp variable definitions;
- **Pilot Signals in OAI**: in-depth description of pilto signals, their generation, gold sequences, and how they are leveraged for channel estimation;
- **Implemented Architectures**: detailed instructions for every deployment model used, from basic out of the box setups, to fully disaggregated environments with complex nodes; 
- **Additional Notes**: details on configuration parameters used in the previous setups; 
- **FlexRIC Implementation**: guidelines on different types of FlexRIC installation; 
- **Custom Docker Images**: instructions on compiling, building and deploying different developed types of custom docker images;
- **T-tracer Notes**: notes and problem documentation for the T-Tracer tool;
- **Troubleshooting**: issues encountered, and guidelines for their solutions;
- **Portainer Implementation**: implementation notes and guides for the managing tool Portainer;
- **XCP-NG and VM Implementation**: setup guide on implementing a virtualized environment using XCP-NG and troubleshooting common faced errors:
