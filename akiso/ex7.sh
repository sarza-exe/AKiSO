# Enter the directory where we want to change the names of files
cd /home/sara/labo/akiso/test || exit

# Iterate through all files in the directory
for file in *; do
  # Check if it's a file
  if [ -f "$file" ]; then
    # change upper to lower case using tr
    new_file=$(echo "$file" | tr '[A-Z]' '[a-z]')
    
    if [ "$file" != "$new_file" ]; then
      # Change name of a file. Double slash so that names of file are interpreted as files not options
      mv -- "$file" "$new_file" 
    fi
  fi
done
