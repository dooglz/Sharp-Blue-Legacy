REM INCLUDE FILES
xcopy /s /y "..\Sharp-blue\src\*.h" include\sharp-blue\
REM
xcopy /s /y "..\Sharp-blue\bin\Win32_debugLib\*.lib" lib\win32\x86\
xcopy /s /y "..\Sharp-blue\res\*" res\engine
pause>nul