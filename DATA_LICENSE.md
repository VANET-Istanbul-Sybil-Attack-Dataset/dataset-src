# DATASET LICENSE AND USAGE RESTRICTIONS

**Effective Date:** February 18, 2026

**Read-Only Access Originally Revoked:** July 15, 2025

**Public Notification of Restriction:** September 11, 2025

## 1. Distinction between Code and Data
This repository contains two distinct types of intellectual property:
* **Source Code:** The C/C++, Python, and Shell scripts, along with configuration files hosted directly in this repository, are licensed under **GPL-2.0**.
* **Dataset Dump Files:** The specific dataset dump files (e.g., `.xz` archives) referenced via external links (e.g., Google Drive) are **NOT** covered by the GPL-2.0 license.

## 2. Status and History of the Dataset Files
The **Dataset Dump Files** are **PROPRIETARY and ALL RIGHTS RESERVED**.

* **Limited Purpose of Prior Access:** Any previous availability of these files via Google Drive links was provided as **read-only access**, granted **solely for the purpose of scientific verification**.
    * **Accessibility vs. Rights:** The ability to access these files for review purposes did **not** constitute a public release or a transfer of copyright.
    * **No Redistribution:** This limited access did **not** grant permission for "Open Access" publication, mirroring on third-party servers, or any form of redistribution.

* **Unauthorized Claims of Creation:** Two separate academic articles have falsely claimed the ownership of this dataset.
    * **Lack of Consent:** The inclusion of this dataset in these publications was performed without the owner's consent.
    * **Violation of Authorship:** In one instance, the owner's name was removed entirely; in another, the work was published without the owner's signature or consent.
    * **No Creative Commons:** These files are **NOT** covered by the Creative Commons (CC-BY) license associated with any unauthorized article.

* **No License Granted:** The copyright holder (Taner Guven) has **not** granted any right to host, mirror, redistribute, or commercially use these specific data files.

## 3. Unauthorized Distribution Warning
Any hosting, mirroring, or distribution of these `.xz` files on **any external servers** (including but not limited to public repositories, cloud storage, or academic archives) is a direct violation of copyright.

The presence of this data in any "Data Availability Statement" in published literature was made without the owner's signature or consent.

**Current Legal Status:** UNAUTHORIZED DISTRIBUTION STRICTLY PROHIBITED / UNDER LEGAL DISPUTE.

## 4. Statement of Original Ownership & Resource Investment
The dataset files, hosted on the owner's personal Google Drive, are the result of significant personal investment and independent labor by the owner (Taner Guven).

* **Infrastructure:** The simulation was executed exclusively on custom personal hardware owned and paid for by Taner Guven, requiring over 20 days of continuous, uninterrupted runtime following failed attempts on commercial cloud infrastructure (Hetzner) due to extreme I/O demands. No university or institutional computing resources were utilized.
* **Computational Cost:** The dataset generation required approximately **20-25 days of continuous runtime** to produce just 25 minutes of simulated traffic.
* **Technical Labor:** The custom simulation software built on Veins/OMNeT++ is single-threaded and highly unstable. The simulation execution process required constant manual intervention, monitoring, and error recovery, all performed solely by the owner. This operation produced over 820GB of raw data, causing severe filesystem corruption due to extreme I/O intensity on the owner's private hardware. Consequently, the final dataset is not a raw machine output but a **'Salvaged Compilation,'** requiring the owner’s constant manual intervention. First, the owner used filesystem recovery tools to reconstruct the files from the corrupted storage. Second, the owner manually identified and removed invalid data segments produced by the simulator, subsequently cleaning and merging only the remaining valid data streams to ensure a functional and accurate final dataset.
* **Academic Status & Support:** While the owner produced this work during a period of receiving a personal doctoral scholarship, this support was provided as a monthly stipend solely for the purpose of supporting the student. As per official scholarship regulations, recipients are strictly prohibited from holding university staff or faculty positions during the scholarship period. Therefore, this scholarship does **not** constitute an employment contract or "work for hire" agreement. All computational infrastructure costs (Hetzner and private hardware) were **funded personally by the owner and were not covered by this or any other institutional grant.**

**Legal Note:** As the sole funder of the **computational infrastructure** and executor of this work, the owner retains full **Sui Generis Database Rights** under applicable copyright laws. The dataset is NOT "work for hire" and is NOT public domain.
