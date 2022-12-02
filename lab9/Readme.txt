*********************************** Readme File*************************************
***********************************Exercise 9***************************************
******************* Kareem Kassab******************** 900182771 ********************

To run the code:
sudo mknod /dev/cipher c 1024 0
sudo mknod /dev/cipher_key c 1024 1
make
sudo insmod CCD.ko
echo "key" > /dev/cipher_key
cat /dev/cipher_key
echo "message" > /dev/cipher
cat /dev/cipher
echo "key" > /proc/cipher_key
cat /proc/cipher_key
cat /proc/cipher
