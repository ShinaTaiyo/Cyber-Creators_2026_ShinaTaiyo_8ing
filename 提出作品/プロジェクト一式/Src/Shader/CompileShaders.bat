@echo off
setlocal

set BASE_DIR=%~dp0
set FX_DIR=%BASE_DIR%fx
set FXO_DIR=%BASE_DIR%fxo
set FXC_PATH="C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Utilities\bin\x86\fxc.exe"

echo  FX����: %FX_DIR%
echo  FXO�o��: %FXO_DIR%
echo  fxc.exe: %FXC_PATH%
echo -------------------------------

if not exist %FXC_PATH% (
    echo  fxc.exe ��������܂���I
    pause
    exit /b
)

if not exist "%FX_DIR%\*.fx" (
    echo  .fx �t�@�C����������܂���I
    pause
    exit /b
)

if not exist "%FXO_DIR%" (
    mkdir "%FXO_DIR%"
)

for %%f in ("%FX_DIR%\*.fx") do (
    echo  �R���p�C����: %%~nxf ...
    %FXC_PATH% /T fx_2_0 /Fo "%FXO_DIR%\%%~nf.fxo" "%%f" >nul 2> "%FXO_DIR%\%%~nf_log.txt"
    if exist "%FXO_DIR%\%%~nf.fxo" (
        echo  �o�͐���: %%~nf.fxo
    ) else (
        echo  �o�͎��s: %%~nf.fxo �� �G���[���O�m�F�F%%~nf_log.txt
    )
)

echo.
echo  ���ׂẴt�@�C�����������܂����B
pause
