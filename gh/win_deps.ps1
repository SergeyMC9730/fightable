Invoke-WebRequest -Uri "https://dogotrigger.ru/extlib/extlib.zip" -OutFile "extlib.zip"
Expand-Archive -Path "extlib.zip" -DestinationPath "./"
git clone --depth 1 https://github.com/curl/curl.git
Copy-Item -Path "curl/include/curl" -Destination "extlib/inc/" -Recurse
