BPATH=$1

ls $BPATH -la
echo ---------------
ls $BPATH/lib -la
echo ---------------
file $BPATH/lib/*
echo ---------------
ls $BPATH/logs -la
