echo "Creating directory structure for nuget"
New-Item -Path nuget\lib\uap10.0 -ItemType Directory -Force
New-Item -Path nuget\build\native -ItemType Directory -Force

New-Item -Path nuget\runtimes\win10-arm\native -ItemType Directory -Force
New-Item -Path nuget\runtimes\win10-x86\native -ItemType Directory -Force
New-Item -Path nuget\runtimes\win10-x64\native -ItemType Directory -Force

echo "Copying over files..."

Copy-Item -Force Release\x86\*.winmd nuget\lib\uap10.0
Copy-Item -Force LevelDBUAP.targets nuget\build\native

Copy-Item -Force Release\ARM\*.dll nuget\runtimes\win10-arm\native
Copy-Item -Force Release\x64\*.dll nuget\runtimes\win10-x64\native
Copy-Item -Force Release\x86\*.dll nuget\runtimes\win10-x86\native

Copy-Item -Force LevelDBUAP.nuspec nuget\

Push-Location
cd nuget
nuget pack .\LevelDBUAP.nuspec
Pop-Location