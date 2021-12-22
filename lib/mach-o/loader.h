// loader.h: minimal header for working with mach-o files
//-----------------------------------------------------------------------------
//
// The contents of this file, except for the `#include`(s) following this
// message, are adapted from the following APSL-licensed file:
//
//   https://github.com/apple/darwin-xnu/blob/xnu-7195.121.3/EXTERNAL_HEADERS/mach-o/loader.h
//
// The APSL license header:
//
//-----------------------------------------------------------------------------
// Portions Copyright (c) 1999-2007 Apple Inc.  All Rights Reserved.
//
// This file contains Original Code and/or Modifications of Original Code as
// defined in and that are subject to the Apple Public Source License Version
// 2.0 (the 'License').  You may not use this file except in compliance with
// the License.  Please obtain a copy of the License at
// http://www.opensource.apple.com/apsl/ and read it before using this file.
//
// The Original Code and all software distributed under the License are
// distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
// EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
// INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS FOR
// A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.  Please see the
// License for the specific language governing rights and limitations under the
// License.
//-----------------------------------------------------------------------------
//
// The full text of the APSL, obtained from https://opensource.apple.com/apsl/:
//
//-----------------------------------------------------------------------------
//
// APPLE PUBLIC SOURCE LICENSE
// Version 2.0 -  August 6, 2003
//
// Please read this License carefully before downloading this software.  By
// downloading or using this software, you are agreeing to be bound by the
// terms of this License.  If you do not or cannot agree to the terms of this
// License, please do not download or use the software.
//
// Apple Note:  In January 2007, Apple changed its corporate name from "Apple
// Computer, Inc." to "Apple Inc."  This change has been reflected below and
// copyright years updated, but no other changes have been made to the APSL
// 2.0.
//
// 1.	General; Definitions.  This License applies to any program or other work
// which Apple Inc. ("Apple") makes publicly available and which contains a
// notice placed by Apple identifying such program or work as "Original Code"
// and stating that it is subject to the terms of this Apple Public Source
// License version 2.0 ("License").  As used in this License:
//
// 1.1	 "Applicable Patent Rights" mean:  (a) in the case where Apple is the
// grantor of rights, (i) claims of patents that are now or hereafter acquired,
// owned by or assigned to Apple and (ii) that cover subject matter contained
// in the Original Code, but only to the extent necessary to use, reproduce
// and/or distribute the Original Code without infringement; and (b) in the
// case where You are the grantor of rights, (i) claims of patents that are now
// or hereafter acquired, owned by or assigned to You and (ii) that cover
// subject matter in Your Modifications, taken alone or in combination with
// Original Code.
//
// 1.2	"Contributor" means any person or entity that creates or contributes to
// the creation of Modifications.
//
// 1.3	 "Covered Code" means the Original Code, Modifications, the combination
// of Original Code and any Modifications, and/or any respective portions
// thereof.
//
// 1.4	"Externally Deploy" means: (a) to sublicense, distribute or otherwise
// make Covered Code available, directly or indirectly, to anyone other than
// You; and/or (b) to use Covered Code, alone or as part of a Larger Work, in
// any way to provide a service, including but not limited to delivery of
// content, through electronic communication with a client other than You.
//
// 1.5	"Larger Work" means a work which combines Covered Code or portions
// thereof with code not governed by the terms of this License.
//
// 1.6	"Modifications" mean any addition to, deletion from, and/or change to,
// the substance and/or structure of the Original Code, any previous
// Modifications, the combination of Original Code and any previous
// Modifications, and/or any respective portions thereof.  When code is
// released as a series of files, a Modification is:  (a) any addition to or
// deletion from the contents of a file containing Covered Code; and/or (b) any
// new file or other representation of computer program statements that
// contains any part of Covered Code.
//
// 1.7	"Original Code" means (a) the Source Code of a program or other work as
// originally made available by Apple under this License, including the Source
// Code of any updates or upgrades to such programs or works made available by
// Apple under this License, and that has been expressly identified by Apple as
// such in the header file(s) of such work; and (b) the object code compiled
// from such Source Code and originally made available by Apple under this
// License
//
// 1.8	"Source Code" means the human readable form of a program or other work
// that is suitable for making modifications to it, including all modules it
// contains, plus any associated interface definition files, scripts used to
// control compilation and installation of an executable (object code).
//
// 1.9	"You" or "Your" means an individual or a legal entity exercising rights
// under this License.  For legal entities, "You" or "Your" includes any entity
// which controls, is controlled by, or is under common control with, You,
// where "control" means (a) the power, direct or indirect, to cause the
// direction or management of such entity, whether by contract or otherwise, or
// (b) ownership of fifty percent (50%) or more of the outstanding shares or
// beneficial ownership of such entity.
//
// 2.	Permitted Uses; Conditions & Restrictions.   Subject to the terms and
// conditions of this License, Apple hereby grants You, effective on the date
// You accept this License and download the Original Code, a world-wide,
// royalty-free, non-exclusive license, to the extent of Apple's Applicable
// Patent Rights and copyrights covering the Original Code, to do the
// following:
//
// 2.1	Unmodified Code.  You may use, reproduce, display, perform, internally
// distribute within Your organization, and Externally Deploy verbatim,
// unmodified copies of the Original Code, for commercial or non-commercial
// purposes, provided that in each instance:
//
// (a)	You must retain and reproduce in all copies of Original Code the
// copyright and other proprietary notices and disclaimers of Apple as they
// appear in the Original Code, and keep intact all notices in the Original
// Code that refer to this License; and
//
// (b) 	You must include a copy of this License with every copy of Source Code
// of Covered Code and documentation You distribute or Externally Deploy, and
// You may not offer or impose any terms on such Source Code that alter or
// restrict this License or the recipients' rights hereunder, except as
// permitted under Section 6.
//
// 2.2	Modified Code.  You may modify Covered Code and use, reproduce,
// display, perform, internally distribute within Your organization, and
// Externally Deploy Your Modifications and Covered Code, for commercial or
// non-commercial purposes, provided that in each instance You also meet all of
// these conditions:
//
// (a)	You must satisfy all the conditions of Section 2.1 with respect to the
// Source Code of the Covered Code;
//
// (b)	You must duplicate, to the extent it does not already exist, the notice
// in Exhibit A in each file of the Source Code of all Your Modifications, and
// cause the modified files to carry prominent notices stating that You changed
// the files and the date of any change; and
//
// (c)	If You Externally Deploy Your Modifications, You must make Source Code
// of all Your Externally Deployed Modifications either available to those to
// whom You have Externally Deployed Your Modifications, or publicly available.
// Source Code of Your Externally Deployed Modifications must be released under
// the terms set forth in this License, including the license grants set forth
// in Section 3 below, for as long as you Externally Deploy the Covered Code or
// twelve (12) months from the date of initial External Deployment, whichever
// is longer. You should preferably distribute the Source Code of Your
// Externally Deployed Modifications electronically (e.g. download from a web
// site).
//
// 2.3	Distribution of Executable Versions.  In addition, if You Externally
// Deploy Covered Code (Original Code and/or Modifications) in object code,
// executable form only, You must include a prominent notice, in the code
// itself as well as in related documentation, stating that Source Code of the
// Covered Code is available under the terms of this License with information
// on how and where to obtain such Source Code.
//
// 2.4	Third Party Rights.  You expressly acknowledge and agree that although
// Apple and each Contributor grants the licenses to their respective portions
// of the Covered Code set forth herein, no assurances are provided by Apple or
// any Contributor that the Covered Code does not infringe the patent or other
// intellectual property rights of any other entity. Apple and each Contributor
// disclaim any liability to You for claims brought by any other entity based
// on infringement of intellectual property rights or otherwise. As a condition
// to exercising the rights and licenses granted hereunder, You hereby assume
// sole responsibility to secure any other intellectual property rights needed,
// if any. For example, if a third party patent license is required to allow
// You to distribute the Covered Code, it is Your responsibility to acquire
// that license before distributing the Covered Code.
//
// 3.	Your Grants.  In consideration of, and as a condition to, the licenses
// granted to You under this License, You hereby grant to any person or entity
// receiving or distributing Covered Code under this License a non-exclusive,
// royalty-free, perpetual, irrevocable license, under Your Applicable Patent
// Rights and other intellectual property rights (other than patent) owned or
// controlled by You, to use, reproduce, display, perform, modify, sublicense,
// distribute and Externally Deploy Your Modifications of the same scope and
// extent as Apple's licenses under Sections 2.1 and 2.2 above.
//
// 4.	Larger Works.  You may create a Larger Work by combining Covered Code
// with other code not governed by the terms of this License and distribute the
// Larger Work as a single product.  In each such instance, You must make sure
// the requirements of this License are fulfilled for the Covered Code or any
// portion thereof.
//
// 5.	Limitations on Patent License.   Except as expressly stated in Section 2,
// no other patent rights, express or implied, are granted by Apple herein.
// Modifications and/or Larger Works may require additional patent licenses
// from Apple which Apple may grant in its sole discretion.
//
// 6.	Additional Terms.  You may choose to offer, and to charge a fee for,
// warranty, support, indemnity or liability obligations and/or other rights
// consistent with the scope of the license granted herein ("Additional Terms")
// to one or more recipients of Covered Code. However, You may do so only on
// Your own behalf and as Your sole responsibility, and not on behalf of Apple
// or any Contributor. You must obtain the recipient's agreement that any such
// Additional Terms are offered by You alone, and You hereby agree to
// indemnify, defend and hold Apple and every Contributor harmless for any
// liability incurred by or claims asserted against Apple or such Contributor
// by reason of any such Additional Terms.
//
// 7.	Versions of the License.  Apple may publish revised and/or new versions
// of this License from time to time.  Each version will be given a
// distinguishing version number.  Once Original Code has been published under
// a particular version of this License, You may continue to use it under the
// terms of that version. You may also choose to use such Original Code under
// the terms of any subsequent version of this License published by Apple.  No
// one other than Apple has the right to modify the terms applicable to Covered
// Code created under this License.
//
// 8.	NO WARRANTY OR SUPPORT.  The Covered Code may contain in whole or in part
// pre-release, untested, or not fully tested works.  The Covered Code may
// contain errors that could cause failures or loss of data, and may be
// incomplete or contain inaccuracies.  You expressly acknowledge and agree
// that use of the Covered Code, or any portion thereof, is at Your sole and
// entire risk.  THE COVERED CODE IS PROVIDED "AS IS" AND WITHOUT WARRANTY,
// UPGRADES OR SUPPORT OF ANY KIND AND APPLE AND APPLE'S LICENSOR(S)
// (COLLECTIVELY REFERRED TO AS "APPLE" FOR THE PURPOSES OF SECTIONS 8 AND 9)
// AND ALL CONTRIBUTORS EXPRESSLY DISCLAIM ALL WARRANTIES AND/OR CONDITIONS,
// EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// AND/OR CONDITIONS OF MERCHANTABILITY, OF SATISFACTORY QUALITY, OF FITNESS
// FOR A PARTICULAR PURPOSE, OF ACCURACY, OF QUIET ENJOYMENT, AND
// NONINFRINGEMENT OF THIRD PARTY RIGHTS.  APPLE AND EACH CONTRIBUTOR DOES NOT
// WARRANT AGAINST INTERFERENCE WITH YOUR ENJOYMENT OF THE COVERED CODE, THAT
// THE FUNCTIONS CONTAINED IN THE COVERED CODE WILL MEET YOUR REQUIREMENTS,
// THAT THE OPERATION OF THE COVERED CODE WILL BE UNINTERRUPTED OR ERROR-FREE,
// OR THAT DEFECTS IN THE COVERED CODE WILL BE CORRECTED.  NO ORAL OR WRITTEN
// INFORMATION OR ADVICE GIVEN BY APPLE, AN APPLE AUTHORIZED REPRESENTATIVE OR
// ANY CONTRIBUTOR SHALL CREATE A WARRANTY.  You acknowledge that the Covered
// Code is not intended for use in the operation of nuclear facilities,
// aircraft navigation, communication systems, or air traffic control machines
// in which case the failure of the Covered Code could lead to death, personal
// injury, or severe physical or environmental damage.
//
// 9.	LIMITATION OF LIABILITY. TO THE EXTENT NOT PROHIBITED BY LAW, IN NO EVENT
// SHALL APPLE OR ANY CONTRIBUTOR BE LIABLE FOR ANY INCIDENTAL, SPECIAL,
// INDIRECT OR CONSEQUENTIAL DAMAGES ARISING OUT OF OR RELATING TO THIS LICENSE
// OR YOUR USE OR INABILITY TO USE THE COVERED CODE, OR ANY PORTION THEREOF,
// WHETHER UNDER A THEORY OF CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE),
// PRODUCTS LIABILITY OR OTHERWISE, EVEN IF APPLE OR SUCH CONTRIBUTOR HAS BEEN
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGES AND NOTWITHSTANDING THE FAILURE
// OF ESSENTIAL PURPOSE OF ANY REMEDY. SOME JURISDICTIONS DO NOT ALLOW THE
// LIMITATION OF LIABILITY OF INCIDENTAL OR CONSEQUENTIAL DAMAGES, SO THIS
// LIMITATION MAY NOT APPLY TO YOU. In no event shall Apple's total liability
// to You for all damages (other than as may be required by applicable law)
// under this License exceed the amount of fifty dollars ($50.00).
//
// 10.	Trademarks.  This License does not grant any rights to use the
// trademarks or trade names  "Apple", "Mac", "Mac OS", "QuickTime", "QuickTime
// Streaming Server" or any other trademarks, service marks, logos or trade
// names belonging to Apple (collectively "Apple Marks") or to any trademark,
// service mark, logo or trade name belonging to any Contributor.  You agree
// not to use any Apple Marks in or as part of the name of products derived
// from the Original Code or to endorse or promote products derived from the
// Original Code other than as expressly permitted by and in strict compliance
// at all times with Apple's third party trademark usage guidelines which are
// posted at http://www.apple.com/legal/guidelinesfor3rdparties.html.
//
// 11.	Ownership. Subject to the licenses granted under this License, each
// Contributor retains all rights, title and interest in and to any
// Modifications made by such Contributor.  Apple retains all rights, title and
// interest in and to the Original Code and any Modifications made by or on
// behalf of Apple ("Apple Modifications"), and such Apple Modifications will
// not be automatically subject to this License.  Apple may, at its sole
// discretion, choose to license such Apple Modifications under this License,
// or on different terms from those contained in this License or may choose not
// to license them at all.
//
// 12.	Termination.
//
// 12.1	Termination.  This License and the rights granted hereunder will
// terminate:
//
// (a)	automatically without notice from Apple if You fail to comply with any
// term(s) of this License and fail to cure such breach within 30 days of
// becoming aware of such breach;
// (b)	immediately in the event of the circumstances described in Section
// 13.5(b); or
// (c)	automatically without notice from Apple if You, at any time during the
// term of this License, commence an action for patent infringement against
// Apple; provided that Apple did not first commence an action for patent
// infringement against You in that instance.
//
// 12.2	Effect of Termination.  Upon termination, You agree to immediately stop
// any further use, reproduction, modification, sublicensing and distribution
// of the Covered Code.  All sublicenses to the Covered Code which have been
// properly granted prior to termination shall survive any termination of this
// License.  Provisions which, by their nature, should remain in effect beyond
// the termination of this License shall survive, including but not limited to
// Sections 3, 5, 8, 9, 10, 11, 12.2 and 13.  No party will be liable to any
// other for compensation, indemnity or damages of any sort solely as a result
// of terminating this License in accordance with its terms, and termination of
// this License will be without prejudice to any other right or remedy of any
// party.
//
// 13. 	Miscellaneous.
//
// 13.1	Government End Users.   The Covered Code is a "commercial item" as
// defined in FAR 2.101.  Government software and technical data rights in the
// Covered Code include only those rights customarily provided to the public as
// defined in this License. This customary commercial license in technical data
// and software is provided in accordance with FAR 12.211 (Technical Data) and
// 12.212 (Computer Software) and, for Department of Defense purchases, DFAR
// 252.227-7015 (Technical Data -- Commercial Items) and 227.7202-3 (Rights in
// Commercial Computer Software or Computer Software Documentation).
// Accordingly, all U.S. Government End Users acquire Covered Code with only
// those rights set forth herein.
//
// 13.2	Relationship of Parties.  This License will not be construed as
// creating an agency, partnership, joint venture or any other form of legal
// association between or among You, Apple or any Contributor, and You will not
// represent to the contrary, whether expressly, by implication, appearance or
// otherwise.
//
// 13.3	Independent Development.   Nothing in this License will impair Apple's
// right to acquire, license, develop, have others develop for it, market
// and/or distribute technology or products that perform the same or similar
// functions as, or otherwise compete with, Modifications, Larger Works,
// technology or products that You may develop, produce, market or distribute.
//
// 13.4	Waiver; Construction.  Failure by Apple or any Contributor to enforce
// any provision of this License will not be deemed a waiver of future
// enforcement of that or any other provision.  Any law or regulation which
// provides that the language of a contract shall be construed against the
// drafter will not apply to this License.
//
// 13.5	Severability.  (a) If for any reason a court of competent jurisdiction
// finds any provision of this License, or portion thereof, to be
// unenforceable, that provision of the License will be enforced to the maximum
// extent permissible so as to effect the economic benefits and intent of the
// parties, and the remainder of this License will continue in full force and
// effect.  (b) Notwithstanding the foregoing, if applicable law prohibits or
// restricts You from fully and/or specifically complying with Sections 2
// and/or 3 or prevents the enforceability of either of those Sections, this
// License will immediately terminate and You must immediately discontinue any
// use of the Covered Code and destroy all copies of it that are in your
// possession or control.
//
// 13.6	Dispute Resolution.  Any litigation or other dispute resolution between
// You and Apple relating to this License shall take place in the Northern
// District of California, and You and Apple hereby consent to the personal
// jurisdiction of, and venue in, the state and federal courts within that
// District with respect to this License. The application of the United Nations
// Convention on Contracts for the International Sale of Goods is expressly
// excluded.
//
// 13.7	Entire Agreement; Governing Law.  This License constitutes the entire
// agreement between the parties with respect to the subject matter hereof.
// This License shall be governed by the laws of the United States and the
// State of California, except that body of California law concerning conflicts
// of law.
//
// Where You are located in the province of Quebec, Canada, the following
// clause applies:  The parties hereby confirm that they have requested that
// this License and all related documents be drafted in English.  Les parties
// ont exigÃ© que le prÃ©sent contrat et tous les documents connexes soient
// rÃ©digÃ©s en anglais.
//
// EXHIBIT A.
//
// "Portions Copyright (c) 1999-2007 Apple Inc.  All Rights Reserved.
//
// This file contains Original Code and/or Modifications of Original Code as
// defined in and that are subject to the Apple Public Source License Version
// 2.0 (the 'License').  You may not use this file except in compliance with
// the License.  Please obtain a copy of the License at
// http://www.opensource.apple.com/apsl/ and read it before using this file.
//
// The Original Code and all software distributed under the License are
// distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
// EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
// INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS FOR
// A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.  Please see the
// License for the specific language governing rights and limitations under the
// License."
//-----------------------------------------------------------------------------

#include <stdint.h>

//--

#define cpu_type_t    int32_t
#define cpu_subtype_t int32_t
typedef int vm_prot_t;

#define MH_MAGIC_64 0xfeedfacf // 64-bit mach magic number

#define CPU_ARCH_ABI64  0x01000000 // 64 bit ABI
#define CPU_TYPE_X86    ((cpu_type_t)7)
#define CPU_TYPE_X86_64 (CPU_TYPE_X86 | CPU_ARCH_ABI64)

#define CPU_SUBTYPE_X86_64_ALL ((cpu_subtype_t)3)
#define MH_EXECUTE             0x2 // demand paged executable file

#define MH_NOUNDEFS 0x1 // the object file has no undefined references

#define LC_SEGMENT_64 0x19 // 64-bit segment of this file to be mapped

#define VM_PROT_READ    ((vm_prot_t)0x01) // read permission
#define VM_PROT_WRITE   ((vm_prot_t)0x02) // write permission
#define VM_PROT_EXECUTE ((vm_prot_t)0x04) // execute permission

typedef struct mach_header_64 {
  uint32_t      magic;      // mach magic number identifier
  cpu_type_t    cputype;    // cpu specifier
  cpu_subtype_t cpusubtype; // machine specifier
  uint32_t      filetype;   // type of file
  uint32_t      ncmds;      // number of load commands
  uint32_t      sizeofcmds; // the size of all the load commands
  uint32_t      flags;
  uint32_t      reserved;
} mach_header_64;

typedef struct segment_command_64 {
  uint32_t  cmd;         // LC_SEGMENT_64
  uint32_t  cmdsize;     // includes sizeof section_64 structs
  char      segname[16]; // segment name
  uint64_t  vmaddr;      // memory address of this segment
  uint64_t  vmsize;      // memory size of this segment
  uint64_t  fileoff;     // file offset of this segment
  uint64_t  filesize;    // amount to map from the file
  vm_prot_t maxprot;     // maximum VM protection
  vm_prot_t initprot;    // initial VM protection
  uint32_t  nsects;      // number of sections in segment
  uint32_t  flags;
} segment_command_64;

typedef struct section_64 { /* for 64-bit architectures */
  char     sectname[16];    // name of this section
  char     segname[16];     // segment this section goes in
  uint64_t addr;            // memory address of this section
  uint64_t size;            // size in bytes of this section
  uint32_t offset;          // file offset of this section
  uint32_t align;           // section alignment (power of 2)
  uint32_t reloff;          // file offset of relocation entries
  uint32_t nreloc;          // number of relocation entries
  uint32_t flags;           // flags (section type and attributes
  uint32_t reserved1;       // reserved (for offset or index)
  uint32_t reserved2;       // reserved (for count or sizeof)
  uint32_t reserved3;
} section_64;

// the pagezero segment which has no protections and
// catches NULL references for MH_EXECUTE files
#define SEG_PAGEZERO "__PAGEZERO"

// the tradition UNIX text segment
#define SEG_TEXT "__TEXT"

// the real text part of the text section no headers,
// and no padding
#define SECT_TEXT "__text"

// section contains only true machine instructions
#define S_ATTR_PURE_INSTRUCTIONS 0x80000000

// section contains some machine instructions
#define S_ATTR_SOME_INSTRUCTIONS 0x00000400

// the segment containing all structs created and maintained
// by the link editor.  Created with -seglinkedit option to
// ld(1) for MH_EXECUTE and FVMLIB file types only
#define SEG_LINKEDIT "__LINKEDIT"

// link-edit stab symbol table info
#define LC_SYMTAB 0x2

// the uuid
#define LC_UUID 0x1b

// source version used to build binary
#define LC_SOURCE_VERSION 0x2A

// unix thread (includes a stack)
#define LC_UNIXTHREAD 0x5

#define x86_THREAD_STATE64 4

#define _STRUCT_X86_THREAD_STATE64 struct x86_thread_state64
_STRUCT_X86_THREAD_STATE64 {
  uint64_t rax;
  uint64_t rbx;
  uint64_t rcx;
  uint64_t rdx;
  uint64_t rdi;
  uint64_t rsi;
  uint64_t rbp;
  uint64_t rsp;
  uint64_t r8;
  uint64_t r9;
  uint64_t r10;
  uint64_t r11;
  uint64_t r12;
  uint64_t r13;
  uint64_t r14;
  uint64_t r15;
  uint64_t rip;
  uint64_t rflags;
  uint64_t cs;
  uint64_t fs;
  uint64_t gs;
};

typedef unsigned int       __darwin_natural_t;
typedef __darwin_natural_t natural_t;
typedef natural_t          mach_msg_type_number_t;

typedef _STRUCT_X86_THREAD_STATE64 x86_thread_state64_t;
#define x86_THREAD_STATE64_COUNT                                               \
  ((mach_msg_type_number_t)(sizeof(x86_thread_state64_t) / sizeof(int)))

typedef struct symtab_command {
  uint32_t cmd;     // LC_SYMTAB
  uint32_t cmdsize; // sizeof(struct symtab_command)
  uint32_t symoff;  // symbol table offset
  uint32_t nsyms;   // number of symbol table entries
  uint32_t stroff;  // string table offset
  uint32_t strsize; // string table size in bytes
} symtab_command;

// The uuid load command contains a single 128-bit unique
// random number that identifies an object produced by the
// static link editor.
typedef struct uuid_command {
  uint32_t cmd;      // LC_UUID
  uint32_t cmdsize;  // sizeof(struct uuid_command)
  uint8_t  uuid[16]; // the 128-bit uuid
} uuid_command;

typedef struct source_version_command {
  uint32_t cmd;     // LC_SOURCE_VERSION
  uint32_t cmdsize; // 16
  uint64_t version; // A.B.C.D.E packed as a24.b10.c10.d10.e10
} source_version_command;

typedef struct thread_command {
  uint32_t cmd;     // LC_THREAD or  LC_UNIXTHREAD
  uint32_t cmdsize; // total size of this command
  uint32_t flavor;  // flavor of thread state
  uint32_t count;   // count of uint32_t's in thread state
  // struct cpu_thread_state state;
} thread_command;

typedef struct nlist_64 {
  union {
    uint32_t n_strx; /* index into the string table */
  } n_un;
  uint8_t  n_type;  /* type flag, see below */
  uint8_t  n_sect;  /* section number or NO_SECT */
  uint16_t n_desc;  /* see <mach-o/stab.h> */
  uint64_t n_value; /* value of this symbol (or stab offset) */
} nlist_64;
