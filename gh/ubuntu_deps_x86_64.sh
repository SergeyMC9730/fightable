echo "* installing external dependencies"

sudo apt-get install libopenmpt0 libopenmpt-dev -y
sudo apt-get install libmicrohttpd12 libmicrohttpd-dev -y
sudo apt-get install libxrandr-dev libxinerama-dev libxi-dev libx11-dev libxxf86vm-dev libxcursor-dev -y
sudo apt-get install libxi6 libxinerama1 libxrandr2 libx11-6 libxxf86vm1 libxcursor1 -y

echo "* done"
