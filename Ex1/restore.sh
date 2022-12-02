echo "directory where backed up is: " $1
echo "directory to restore to: " $2
echo "ecnryption/decryption key:"  $3
BackupDir=$1
StoreDir=$2
Key=$3

if [[ $1 != "" && $2 != "" && $3 != "" ]]; then
cd ~/OSlab/$BackupDir

 	 		 for f in *.gpg
 	 		 do
 	 		 	FileName=$(basename ~/OSlab/$BackupDir/$f)
 	 		 	gpg --batch --output RestoredFile.tar.gz --passphrase $Key --decrypt $FileName

 	 		done
 	 		mv RestoredFile.tar.gz ~/OSlab/$StoreDir
 	 		cd ..
 	 		cd ~/OSlab/$StoreDir
 	 		mkdir tempDir
 	 		mv RestoredFile.tar.gz tempDir
 	 		cd tempDir
 	 		tar -xvf RestoredFile.tar.gz	

			rm RestoredFile.tar.gz
 	 		for f in *.gpg
 	 		 do
 	 		 	DirName=$(basename ~/OSlab/$StoreDir/tempDir/$f)
 	 		 	gpg --batch --output "$DirName".tar.gz --passphrase $Key --decrypt $DirName
 	 		 	rm "$DirName"

 	 		done

 	 		for f in *.tar.gz
 	 		 do
 	 		 	RestDirName=$(basename ~/OSlab/$StoreDir/tempDir/$f)
 	 		 	tar -xvf "$RestDirName"
 	 		 	rm "$RestDirName"
 	 		done
			

 	 		for d in *
 	 		 do
 	 		 	finalDirName=$(basename ~/OSlab/$StoreDir/tempDir/$d)
 	 		 	mv "$finalDirName" ~/OSlab/$StoreDir
 	 		
 	 		done
 	 		cd ..
			rm -r tempDir
	

 	 		else
echo "please check your arguments and try again"
echo "first argument: directory there backup is"
echo "second argument: directory to store the restrored file"
echo "third argument: key"
exit
fi