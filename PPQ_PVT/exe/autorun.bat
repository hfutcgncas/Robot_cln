@echo off
start .\Camera\CameraSimulater.exe



Rem 用于延时----------
ping -n 3 127.0.0.1>nul 
Rem 用于延时----------

start .\ppq_sym.exe

Rem start .\Arm\TabletennisCln.exe