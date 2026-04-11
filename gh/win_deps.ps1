Invoke-WebRequest -Uri "https://dogotrigger.ru/extlib/extlib.zip" -OutFile "extlib.zip"
Expand-Archive -Path "extlib.zip" -DestinationPath "./"
