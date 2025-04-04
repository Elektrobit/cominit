

# ADR TPM library for measured boot

## Table of Contents

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [Problem](#problem)
- [Influencing factors](#influencing-factors)
  - [1 - primary factors](#1---primary-factors)
  - [2 - secondary factors](#2---secondary-factors)
  - [3 - tertiary factory](#3---tertiary-factory)
- [Assumptions](#assumptions)
- [Considered alternatives](#considered-alternatives)
  - [1) tpm2-tss](#1-tpm2-tss)
  - [2) wolfTPM](#2-wolftpm)
  - [3) TSS.MSR](#3-tssmsr)
  - [4) IBM TPM2 TSS](#4-ibm-tpm2-tss)
  - [5) go-tpm](#5-go-tpm)
- [6) TrouSerS](#6-trousers)
- [Decision](#decision)
  - [Review Team](#review-team)
- [Open Point](#open-point)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Problem

Measured boot can add another layer of protection against security threats:
Through measured boot it can be ensured that the system is in a known, trusted state before unsealing sensitive keys or data.
For implementing a measured boot with a TPM device, a suitable library should be chosen.

## Influencing factors

For decision which library to choose this factors need to be evaluated:


### 1 - primary factors

These factors need to be considered. They might cause a package to be sorted out if
a mismatch to the requirements is detected.

* Basic Functionality needed:
     * seal/unseal
     * TPM 2.0 support
* Documentation - Is it well documented? Understandable? Complete?
* Security - Is the packages know to be secure/insecure?
     * CVEs - are CVEs open? Are they fixed? How many were opened/fixed in last time?
* Compatibility - compatible to other FOSS components
* Integration:
     * shell and interpreter free binary: libraries should compile into     standalone binary
     * the package shall have yocto-recipes
* License
     * restricted licenses are: GPLv3


### 2 - secondary factors

These factors should to be considered. They might cause a package to be sorted out if
multiple mismatches to the requirements are detected.

* Testability - Does it have a test suite?
* Flexibility - allows to easily extend functionality via configuration
* Complexity - the more flexibility the packages provides, the more complex it becomes
* Effort - Remaining Development effort to integrate and add features
* Established package:
     * Sustainability - Is the package sustainable or might it be orphaned soon?
     * Proven-in-use - Is it already in use in other projects? Successful?
     * Maturity - How mature is the code? Is it just a PoC?


### 3 - tertiary factory

These factors can to be considered. They might ease the decision.

* Governance - Who is "running" the project?
* (if applicable) Safety - support safety mechanisms of Linux for Safe Applications, as needed
* Support - Is support available? Commercial?
* Open to patches - Is the community open to receive patches?

## Assumptions

Only libraries with own implementation of the TPM software stack are considered.
If any primary factor is found with a ruling-out criterion, the option is excluded from further consideration, and the remaining factors are not evaluated.

## Considered alternatives


### 1) tpm2-tss

*pros*
- Primary Factor – Functionality: TPM 2.0 support , seal/unseal functionality  verified
- Primary Factor – Documentation: project is well documented
- Primary Factor – Security: CVE are addressed, currently no open CVE
- Primary Factor – Compatibility: Many tools and libraries are built around TPM2-TSS, including:
    tpm2-tool (command line tool)
    Keylime (remote attestation and key management)
    tpm2-tss-engine (cryptographic operations on TPM)
    LUKS (disk encryption)
- Primary Factor – Integration: yocto support, shell and interpreter free, compilable
- Primary Factor – License: BSD 2-Clause license
- Secondary Factor – Testability: unit and integration tests are given
- Secondary Factor – Flexibility: offers different level of APIs to use
- Secondary Factor – Effort: actively maintained (120 contributors, last commit < 1 month)
- Secondary Factor – Established package: standard library for TPM software stack

*cons
- Secondary Factor – Complexity: invokes dependencies
- Primary Factor – Functionality: no TPM 1.2 support

Repository can be found at [tpm2-tss](https://github.com/tpm2-software/tpm2-tss)

### 2) wolfTPM

*pros*
- Primary Factor – Functionality: TPM 2.0 support , seal/unseal functionality  seems to be given
- Primary Factor – Security: no open CVE 
- Primary Factor – Integration: yocto support, shell and interpreter free, compilable
- Secondary Factor – Testability: unit tests are given
- Secondary Factor – Effort: actively maintained (25 contributors, last commit < 1 month)
- Secondary Factor – Complexity: lightweight, not external dependencies other than wolfSSL needed

*cons*
- Primary Factor – Security: no CVE ever reported
- Primary Factor – Documentation: project is not well documented
- Primary Factor – License: GPLv2 license
- Secondary Factor – Flexibility: lightweight
- Secondary Factor – Established package: not common

Repository can be found at [tpm2-tss](https://github.com/wolfSSL/wolfTPM)

### 3) TSS.MSR

*pros*
- Primary Factor – Functionality: TPM 2.0 support , seal/unseal functionality  seems to be given
- Primary Factor – Integration: shell and interpreter free, compilable
- Primary Factor – Security: no open CVE
- Primary Factor – License: MIT License
- Secondary Factor – Testability: testsuite is given

*cons*
- Primary Factor – Functionality: C implementation is based on fork of tpm2-tss (strong contra, ruling out this option)
- Primary Factor – Security: no CVE ever reported
- Primary Factor – Integration: no yocto support
- Primary Factor – Documentation: project is not well documented
- Secondary Factor – Effort: not actively maintained (21 contributors, last commit > 1 year)
- Secondary Factor – Established package: not common

Repository can be found at [TSS.MSR](https://github.com/microsoft/TSS.MSR)

### 4) IBM TPM2 TSS

- Primary Factor – Functionality: TPM 2.0 support , seal/unseal functionality  seems to be given, TPM 1.2 support
- Primary Factor – Security: no open CVE
- Primary Factor – Integration: yocto support, shell and interpreter free, compilable

*cons*
- Primary Factor – Security: no CVE ever reported
- Primary Factor – Documentation: project is not well documented
- Primary Factor – License: individual license  (strong contra, ruling out this option)
- Secondary Factor – Effort: not actively maintained (9 contributors, last commit > 1 month)
- Secondary Factor – Testability: no tests, only a demo
- Secondary Factor – Established package: not common

Repository can be found at [IBM TPM2 TSS](https://github.com/kgoldman/ibmtss)

### 5) go-tpm

*pros*
- Primary Factor – Security: CVE are addressed, currently no open CVE
- Primary Factor – Functionality: seal/unseal functionality seems to be given, TPM 1.2 support
- Primary Factor – Integration: yocto support, shell and interpreter free, compilable
- Primary Factor – License: Apache 2.0 license
- Secondary Factor – Testability: seems to be well tested

*cons*
- Primary Factor – Functionality: no full TPM 2.0 support (strong contra, ruling out this option)
- Secondary Factor – Effort: not actively maintained (49 contributors, last commit > 1 month)
- Secondary Factor – Established package: not common

Repository can be found at [go-tpm](https://github.com/google/go-tpm)

## 6) TrouSerS

*pros*
- Primary Factor – Functionality: TPM 1.2 support
- Primary Factor – Security: no open CVE
- Primary Factor – Integration: yocto support, shell and interpreter free, compilable
- Primary Factor – License: Common Public License

*cons*
- Primary Factor – Functionality: no TPM 2.0 support (strong contra, ruling out this option)
- Primary Factor – Security: no CVE ever reported
- Secondary Factor – Testability: no tests
- Secondary Factor – Effort: not maintained (9 contributors, last commit > 1 year)
- Secondary Factor – Established package: only common for TPM 1.2

Repository can be found at [TrouSerS](https://github.com/srajiv/trousers)

## Decision
After checking for completeness of alternatives and taking all pros and cons to account following decision was taken:

Option 1

### Review Team

Following members and teams have reviewed and checked the decision:

| *Team* | *Status* | *Date*   |
| -------| -------- | -------- |
| emlix  | OK       | 08.04.24 |


*Status*

- OK
- not OK
- not yet checked
- don't care

## Open Point


