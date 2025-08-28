

# ADR Support partition table in cominit

## Table of Contents

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [Problem](#problem)
- [GPT vs MBR](#gpt-vs-mbr)
  - [MBR (Master Boot Record)](#mbr-master-boot-record)
  - [GPT (GUID Partition Table)](#gpt-guid-partition-table)
- [Assumptions](#assumptions)
- [Considered alternatives](#considered-alternatives)
  - [1) MBR](#1-mbr)
  - [2) GPT](#2-gpt)
    - [bootloader offset vs GPT](#bootloader-offset-vs-gpt)
  - [3) MBR + GPT](#3-mbr--gpt)
- [Decision](#decision)
  - [Review Team](#review-team)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Problem

While MBR is considered legacy, it remains common in embedded Linux deployments.
Cominit has only been validated with MBR partition tables.
However, using UUIDs as defined by the Discoverable Partitions Specification (DPS) requires GPT,
since DPS relies on GPT Partition Type GUIDs. This raises the question of whether we should continue supporting MBR,
or adopt GPT exclusively to enable DPS-based auto-mount functionality.

see [DPS](https://uapi-group.org/specifications/specs/discoverable_partitions_specification/)

## GPT vs MBR

### MBR (Master Boot Record)

  * very old standard (1980s).
  * Uses 32-bit LBAs → max disk size ~2 TiB (with 512-byte sectors).
  * Partition table lives in the first sector (LBA 0).
  * Up to 4 primary partitions. To have more, one of them must be an extended partition containing “logical” partitions.

### GPT (GUID Partition Table)

  * Defined in the UEFI specification (2000s).
  * Uses 64-bit LBAs → essentially unlimited size support.
  * Large number of partitions (commonly 128 by default, but configurable).
  * Has redundant headers: one at the beginning (primary), one at the end (backup).
  * Uses CRCs to detect corruption.
  * Every partition has a type GUID and a unique GUID.

## Assumptions

At current state the only partition table layout tested in cominit is MBR.
MBR is superseded by GPT and de-facto standard in x86/ARM-architectures.
GPT also supports UUID that can be used for automounting and autoconfigure features in cominit.
One reason for still using MBR is that on the NXP S32 platform the ROM reads bootloader from an offset
(0x1000) that overlaps with GPT standard size.

## Considered alternatives

### 1) MBR

MBR is simple, small and universally supported. There is a proprietary approach in cominit
to config its partitions. That works well with MBR and without UUIDs.

### 2) GPT

GPT is de-facto standard. As cominit has no need to parse partition tables,
only device node enumerating changes (no extended partition, just logical partitions).
As they can be passed through kernel command line, there is no reason to not support GPT.

#### bootloader offset vs GPT

ROM reads bootloader from an offset that overlaps with GPT:

GPT layout (without backup):
LBA 0 – Protective MBR (for compatibility)
LBA 1 – Primary GPT Header (metadata)
LBA 2–33 – Primary Partition Entry Array (128 entries)

LBA = Logical Block Address (default: 512 Bytes)

bootloader offset is 0x1000 -> LBA 8 on default block size which is
within the Primary Partition Entry Array.

But: The Primary Partition Entry Array can be moved or resized.
Resizing and moving is within the UEFI specification and can i.e. be done
with gdisk (available in the OpenEmbedded Layer).

So the S32 platform specific problem on sd card images can be avoided.
Other NXP platforms such as imx9 are reading the bootloader from a different offset.

So GPT can be used to implement a automount or autoconfig feature based on UUIDs on cominit.
In that specific case of S32 platform there is just a post-process needed that moves/resize the
built image. That can be done with standard tools.

### 3) MBR + GPT

There is no reason to deny MBR or GPT:
* Kernel and bootloader must be configured to support UEFI/GPT.
* In case of S32 platform and SD card boot image there has to be a post-process invoked (with standard tools and conform to UEFI specification)
* Only Device node enumerating is different for cominit. But in case of MBR there are forwarded on the kernel
  command line (as implemented right now) and for GPT they can be deviated from UUIDs.

## Decision

Option 3.

### Review Team

Following members and teams have reviewed and checked the decision:

| *Team* | *Status* | *Date*   |
| -------| -------- | -------- |
| emlix  | OK       | 29.07.25 |


*Status*

- OK
- not OK
- not yet checked
- don't care
