call prepare.bat
call testsuite.bat
test\konnekt.exe
@echo "Sprawd� czy dzia�a!"
@pause
call debug.bat
call update.bat

@pause