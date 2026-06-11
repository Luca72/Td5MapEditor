# Td5MapEditor
Editor for Td5 ECU map files

Thiss editor is made specifically for Td5 NNN ECUs. Opening a Nanocom .map file will show the most common tables and scalars and they can be edited.
It is possible to load TunerPro's XDF for a complete identification of the tables.

The sowftware is developed under Linux 64 bit (using Code::Blocks or VSCode or make and wxWidgets 3.2) and can be compiled on Windows 64 bit too (using Code::Blocks and wxWidgets 3.2 with static linking). Compiling under Windows 64 bit requires statically compiling wxWidgets 3.2 and installing it it the system under folder C:/wxWidgets-3.2. Static libraries are espected under folder C:/wxWidgets-3.2/lib/gcc1420UCRT_x64. If you want to use another folder, change C::B Build settings accordingly.

Td5MapEditor is still under development and considered UNSTABLE. Downloading map files modified by Td5MapEditor into the ECU potentially can brick the ECU itself. Do it only if you really know what you are doing.
