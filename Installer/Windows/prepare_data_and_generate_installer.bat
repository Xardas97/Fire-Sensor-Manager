:: Batch script for generating a windows installer for FireSensorManager

:: USER SETUP - Change these variables to those appropriate for your envionment
SET qt_env=C:\Programs\Qt\6.2.4\mingw_64\bin\qtenv2.bat
SET installer_creator=C:\Programs\Qt\QtIFW-4.4.1\bin\binarycreator.exe

SET release_build=..\..\build-FireSensorManager-Desktop_Qt_6_2_4_MinGW_64_bit-Release

:: Initialize derived and constant variables
SET source=..\..

SET manager_build=%release_build%\FireSensorManager\release
SET sim_build=%release_build%\SensorSim\release

SET manager_package=.\installer_data\packages\com.pat.firesensormanager
SET sim_package=.\installer_data\packages\com.pat.sensorsim

:: Enter QT environment and then go back to current drive
SET work_drive=%CD:~0,2%
call %qt_env%
%work_drive%

:: Delete previous installer
DEL ".\FireSensorManager.exe"

:: Delete previous deployment files
RMDIR /q /s "%manager_package%\data"
RMDIR /q /s "%sim_package%\data"

:: Copy the built executables into the package data folder
XCOPY "%manager_build%\FireSensorManager.exe" "%manager_package%\data\"
XCOPY "%sim_build%\SensorSim.exe" "%sim_package%\data\"

:: Run windeployqt to generate dependencies
windeployqt %manager_package%\data\FireSensorManager.exe --qmldir %source%\FireSensorManager\
windeployqt %sim_package%\data\SensorSim.exe --qmldir %source%\SensorSim\

:: Generate installer
call %installer_creator% -c .\installer_data\config\config.xml -p .\installer_data\packages FireSensorManager.exe
