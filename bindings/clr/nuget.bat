rmdir packages /S /Q
rmdir C:\Users\%username%\AppData\Local\NuGet\Cache /S /Q
.\tools\nuget install .\src\nsf\packages.config -o packages -source https://nuget.org/api/v2/"