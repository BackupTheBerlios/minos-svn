mkdir runtime
cd runtime
mkdir Configuration
mkdir Logs
mkdir Lists

del *.exe
del *.dll
del *.bpl

REM files required for logger

copy ..\MinosLogger\MinosLogger.exe .

copy ..\MinosLogger\Configuration\vhfcontests.xml Configuration
copy ..\MinosLogger\Configuration\vhfcontests09.xml Configuration
copy ..\MinosLogger\Configuration\cty.dat Configuration
copy ..\MinosLogger\Configuration\cty.syn Configuration
copy ..\MinosLogger\Configuration\district.ctl Configuration
copy ..\MinosLogger\Configuration\district.syn Configuration
copy ..\MinosLogger\Configuration\prefix.syn Configuration
copy ..\MinosLogger\Configuration\bandlist.xml Configuration

copy %SystemRoot%\system32\cc3280mt.dll
REM copy "C:\Program Files\CodeGear\RAD Studio\5.0\bin"\IndyCore100.bpl
REM copy "C:\Program Files\CodeGear\RAD Studio\5.0\bin"\IndyProtocols100.bpl
REM copy "C:\Program Files\CodeGear\RAD Studio\5.0\bin"\IndySystem100.bpl
copy "C:\Program Files\CodeGear\RAD Studio\5.0\bin"\borlndmm.dll

copy %SystemRoot%\system32\rtl100.bpl
copy %SystemRoot%\system32\vcl100.bpl
copy %SystemRoot%\system32\vcldb100.bpl
copy %SystemRoot%\system32\vclx100.bpl
copy %SystemRoot%\system32\vcljpg100.bpl
copy %SystemRoot%\system32\vclactnband100.bpl
copy %SystemRoot%\system32\dbrtl100.bpl

copy "C:\Documents and Settings\All Users\Documents\RAD Studio\5.0\BPL"\GJVControls.bpl
copy "C:\Documents and Settings\All Users\Documents\RAD Studio\5.0\BPL"\VTV.bpl

echo Only continue if you want all the networking components as well
pause

REM and files required for all the other networking bits

copy ..\minosbase\minoslines.dll .

copy ..\MinosBandMap\MinosBandMap.exe .
copy ..\MinosChat\MinosChat.exe .
copy ..\MinosControl\MinosControl.exe .
copy ..\MinosKeyer\MinosKeyer.exe .
copy ..\MinosMonitor\MinosMonitor.exe .
copy ..\MinosRigCtl\MinosRigCtl.exe .
copy ..\MinosServer\MinosServer.exe .
copy ..\TestRig\TestRig.exe .

REM samples (for spinner edit)
copy "C:\Program Files\CodeGear\RAD Studio\5.0\bin"\bcbsmp100.bpl


copy "C:\Documents and Settings\All Users\Documents\RAD Studio\5.0\BPL"\GJVAudio.bpl

REM we need extra config files as well

REM MinosConfig.ini
REM keyerConfig.xml
REM portConfig.xml


cd ..
