												****************************************** Read Me File ************************************************
												************ Kareem Kassab ************  900182771 ************ Exercise 1 *****************************



How the Program Works:
for backup : ./backup.sh #directory_to_back_things_up #directory_to_store_backup #encrytion_key
for restore: ./restore #directory_wheer_there_is_the_backup #directory_to_restore_backup #dencrytion_key


Notes: 
- make sure you are in the OSlab directory on your terminal
- the testdir contains files names by the history of backups, the restored files shouldve been there, but I stored them in another restore directory because of a technical issue


crontab:
- crontab -e 
- write into the file 

1/* * * * * /path to backup script   /path_to_first_param  /path_to_second_param  key
1/* * * * * /path to restore script   /path_to_first_param  /path_to_second_param  key
