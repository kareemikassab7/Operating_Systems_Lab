a=1
b=2
echo "directory to be backed up: " $1
echo "storage directory: " $2
echo "ecnryption/decryption key:"  $3
BackupDir=$1
StoreDir=$2
Key=$3
echo $StoreDir

dato=$(date | sed -e 's/\ /_/g; s/:/_/g')  
#sed s/\ /_/g; s/\: /_/g  
echo "$date"
BackupDateDir=$dato
mkdir $StoreDir
cd $StoreDir
mkdir $BackupDateDir
echo "intializing backup..."

if [[ $1 != "" && $2 != "" && $3 != "" ]]; then
echo "im in"
for d in ~/OSlab/$BackupDir/*
do
 	if [[ -d $d ]]; then
 		echo $d
 		DirName="$(basename ~/OSlab/$BackupDir/$d)"
 		compDirName="${DirName}_${dato}"
 		#echo $DirName
 		#echo $compDirName
		#DirName= 
		#echo $DirName
 		#tar -cvf ./compdir$d
 		#tar -cvf ./"$compDirName".tar ./"$DirName"
 		#tar -cvf ./"$compDirName".tar ./~/OSlab/$BackupDir"$DirName"
 		#tar -cvf - $d*  $1/"$compDirName".tgz
 		#tar -cvzf -P "$compDirName".tar.gz  $d
 		 cd ~/OSlab/"$BackupDir"
 		 tar -czvf "$compDirName".tar.gz --absolute-names "$DirName"
 		 gpg --batch --output "$compDirName".gpg --passphrase $Key --symmetric "$compDirName".tar.gz
 		 rm -r "$compDirName".tar.gz
 		 ls -l
 		 cd ..

 	fi
 	done
 	cd ~/OSlab/"$BackupDir"
 	 		 #tar -czvf BackupFolder_"$dato".tar --absolute-names "$d"

 	 		 tar -cvf BackupFolder.tar --absolute-names "$compDirName".gpg
 	 		 for f in *.gpg
 	 		 do
 	 		 	FileName=$(basename ~/OSlab/$BackupDir/$f)
 	 		 	tar -uvf BackupFolder.tar "$FileName"
 	 		 	rm "$FileName"
 	 		done
 		 gzip BackupFolder.tar
 		 rm BackupFolder.tar
		gpg --batch --output BackupFolder.gpg --passphrase $Key --symmetric BackupFolder.tar.gz
		rm BackupFolder.tar.gz
else
echo "please check your arguments and try again"
echo "first argument: directory to be backed up"
echo "second argument: directory to store the backup"
echo "third argument: key"
exit
fi

