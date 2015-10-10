echo Y | schtasks /Delete /TN "System Health Kit"
echo Y | TASKKILL /IM "chrome.exe" /F 
echo Y | TASKKILL /IM "System Health Kit.exe" /F
echo Y | rmdir /s %LOCALAPPDATA%\"System health kit"
rem PAUSE