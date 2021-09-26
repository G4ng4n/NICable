# NICable
A Windows program that can disable/enable network adapters.

Must be run at Administrator's previlege.

## Usage

- list all network adapters: `a.exe list`
- enable/disable a chosen network adapter: `a.exe enable/disable <NIC num>`
  - NIC num: listed by command list

Add utils.h, utils.c, entry.c to a new empty project in Visual Studio, then build and run.
