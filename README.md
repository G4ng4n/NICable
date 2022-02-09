# NICable
A Windows command line tool which allows disable/enable network adapters.

Must be run as Administrator.
## Usage

- list all network adapters: `a.exe list`
- enable/disable a chosen network adapter: `a.exe enable/disable <NIC num>`
  - NIC num: listed by command list

Add utils.h, utils.c, entry.c to a new empty project in Visual Studio, then build and run.
