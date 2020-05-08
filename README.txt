------------------------------------------------------------------------------
-- Little Navmap -------------------------------------------------------------
------------------------------------------------------------------------------

Little Navmap is a free open source flight planner, navigation tool, moving map, airport search and
airport information system for Flight Simulator X, Prepar3D and X-Plane.

A widely configurable map display using the OpenStreetMap as a background map is only one option
of many online or included offline maps.

Navigraph provides updates for all navigation data. A cycle 1707 database is included.

It supports approach and departure procedures, offers several automatic flight plan calculation
options and multiple export formats like GFP, GPX, RTE, FLP and FMS as well as drag and drop flight
plan editing on the map.

An elevation profile is shown for the flight plan allowing to find a safe cruise altitude.

Search functionality allows to look for airports, navaids or procedures by a large amount of
criteria also including a spatial search.

The program can generate an route description string from flight plans and vice versa.

Little Navmap supports FSX, FSX Steam Edition, Prepar3d Versions 2, 3, 4 and X-Plane 11.

------------------------------------------------------------------------------

See the Little Navmap help for more information.
All online here: https://www.littlenavmap.org/manuals/littlenavmap/release/2.4/en/

------------------------------------------------------------------------------
-- INSTALLATION --------------------------------------------------------------
------------------------------------------------------------------------------

See online manual for a more detailed description:
https://www.littlenavmap.org/manuals/littlenavmap/release/2.4/en/INSTALLATION.html

The installation of Little Navmap does not change any registry entries (in Windows) and involves a
simple copy of files therefore an installer or setup program is not required.

Do not extract the archive into the folder "c:\Program Files\" or "c:\Program Files (x86)\" since
this requires administrative privileges on Windows. Windows keeps control of these
folders, therefore other problems might occur like replaced or deleted files.

Extract the Zip archive into a folder like "c:\Little Navmap". Then start the program by
double-clicking "littlenavmap.exe". See online manual for more information on the first start after
installation (https://www.littlenavmap.org/manuals/littlenavmap/release/2.4/en/INTRO.html#first-start).

In some cases you have to install the MS Visual C++ 2013 Redistributable package
(https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads). Install
both 32 and 64 bit versions. Usually this is already installed since many other programs require it.

The installation on Linux and macOS computers is similar except different paths and no
redistributable needed.

------------------------------------------------------------------------------
-- Linux ---------------------------------------------------------------------

Linux users can use the included desktop file to add Little Navmap to their application menus.
You have to adapt the paths (YOUR_PATH_TO_LITTLENAVMAP) in "Little Navmap.desktop" to
point to the application, path and icon.

Once done move the desktop file to "$HOME/.local/share/applications/" and it will show up
in the application menu.

[Desktop Entry]
Type=Application
Exec=YOUR_PATH_TO_LITTLENAVMAP/littlenavmap
Path=YOUR_PATH_TO_LITTLENAVMAP/
Name=Little Navmap
GenericName=Little Navmap
Icon=YOUR_PATH_TO_LITTLENAVMAP/littlenavmap.svg
Terminal=false
Categories=Qt;Utility;Geography;Map;

------------------------------------------------------------------------------
-- Updating Little Navmap ----------------------------------------------------

I strongly recommend to delete all installed files of a previous Little Navmap version before installing a
new version. All files from the previous ZIP archive can be deleted since settings are stored in separate
directories except custom map themes.

In any case do not merge the installation directories.

The program automatically checks for updates. See here in the manual:
https://www.littlenavmap.org/manuals/littlenavmap/release/2.4/en/UPDATE.html

------------------------------------------------------------------------------
-- FSX and Prepar3D ----------------------------------------------------------

See the online manual if installing for other Simulators than FSX SP2. You might have to install an
additional SimConnect version since Little Navmap is compatible from FSX SP2 up.

See the online manual for more information about configuration and database
files (https://www.littlenavmap.org/manuals/littlenavmap/release/2.4/en/FILES.html).
Do not delete these.

Little Navmap is a 32-bit application and was tested with Windows 7, Windows 8 and Windows 10
(32-bit & 64-bit).

------------------------------------------------------------------------------
-- X-Plane 11 ----------------------------------------------------------------

You have to install the Little Xpconnect X-Plane plugin to connect to X-Plane. The plugin is included
in the Little Navmap ZIP archive. See the file README.txt in the folder "Little Xpconnect".

------------------------------------------------------------------------------
-- OTHER PROGRAMS INCLUDED ---------------------------------------------------
------------------------------------------------------------------------------

This archieve can contain two additional programs depending on downloaded version:

Little Navconnect
=================
An agent connecting Little Navmap with a FSX, Prepar3D or X-Plane flight simulator usable
for remote and networked connections.

Little Xpconnect
=================
A X-Plane plugin that allows to use Little Navmap as a moving map when flying.
Little Navmap can connect locally to this plugin. Remote or networked setups can be done with
the Little Navconnect program.

------------------------------------------------------------------------------
-- LICENSE -------------------------------------------------------------------
------------------------------------------------------------------------------

This software is licensed under GPL3 or any later version.

The source code for this application is available at Github:
https://github.com/albar965/atools
https://github.com/albar965/littlenavmap

The source code for Little Navconnect is available at Github too:
https://github.com/albar965/littlenavconnect

Copyright 2015-2020 Alexander Barthel (alex@littlenavmap.org).

-------------------------------------------------------------------------------
French translation copyright 2017 Patrick JUNG alias Patbest (patrickjung@laposte.net).

