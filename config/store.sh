echo "Launching Store node..."
icegridnode --Ice.Config=StoreNode.cfg --daemon --nochdir
sleep 1
echo "Done!"

echo "Adding application..."
icegridadmin --Ice.Config=admin.cfg -e "application add app.xml"
echo "Done!"
