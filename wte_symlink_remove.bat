@echo off
REM Remove wtengine symlink
REM Either run this as admin
REM Or edit your local security policy to allow symlink creation

rmdir "%~dp0\src\wte_demo\include\wte"
