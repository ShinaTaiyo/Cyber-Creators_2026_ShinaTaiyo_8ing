@echo off
setlocal

set BASE_DIR=%~dp0
set FX_DIR=%BASE_DIR%fx
set FXO_DIR=%BASE_DIR%fxo
set FXC_PATH="C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Utilities\bin\x86\fxc.exe"

echo  FX入力: %FX_DIR%
echo  FXO出力: %FXO_DIR%
echo  fxc.exe: %FXC_PATH%
echo -------------------------------

if not exist %FXC_PATH% (
    echo  fxc.exe が見つかりません！
    pause
    exit /b
)

if not exist "%FX_DIR%\*.fx" (
    echo  .fx ファイルが見つかりません！
    pause
    exit /b
)

if not exist "%FXO_DIR%" (
    mkdir "%FXO_DIR%"
)

for %%f in ("%FX_DIR%\*.fx") do (
    echo  コンパイル中: %%~nxf ...
    %FXC_PATH% /T fx_2_0 /Fo "%FXO_DIR%\%%~nf.fxo" "%%f" >nul 2> "%FXO_DIR%\%%~nf_log.txt"
    if exist "%FXO_DIR%\%%~nf.fxo" (
        echo  出力成功: %%~nf.fxo
    ) else (
        echo  出力失敗: %%~nf.fxo → エラーログ確認：%%~nf_log.txt
    )
)

echo.
echo  すべてのファイルを処理しました。
pause
