echo "Removing the application..."
icegridadmin --Ice.Config=admin.cfg -e "application remove MatrixInversion"
sleep 1
echo "Done!"

echo "Shutting down nodes..."
icegridadmin --Ice.Config=admin.cfg -e "node shutdown Master" -e "node shutdown Store"
