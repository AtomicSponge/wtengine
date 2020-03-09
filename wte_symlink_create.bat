@echo off
REM Create symlink to wtengine files
REM Either run this as admin
REM Or edit your local security policy to allow symlink creation

mklink /D "%~dp0\src\wte_demo\include\wte" "%~dp0\src\wte"
