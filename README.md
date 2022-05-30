USEDLANG
---------------

Usedlang is an efficient, extensible, and easy-to-use program created for the purpose of listing how many different kinds of files occupy a directory. 

Files that are not in the 'files.json' will be categorized as "Other", however, you are free to modify the file to add as many different files and their appropriate file extensions. If you notice some file extensions are incorrect (i.e., do not belong to the corresponding file) you may change that as well!

Not only does Usedlang show percentages of each file category, but it can also list which files are under the specific category by running Usedlang with:
```shell
foo@bar:~$ ./usedlang "Directory Name" -list="File Category"
```
If you're unsure about possible file categories, don't worry. In "file.json" you'll see each category corresponding to a name. The output will also show category names listed under the directory as well. You can list more than one category too.

Usedlang can also skip hidden directories and files if you desire so, simply run Usedlang with:
```shell
foo@bar:~$ ./usedlang "Directory Name" -skipHidden
```

Running this, Usedlang will not consider any files or directories starting with a dot.

Usedlang is licensed under GPL v3.0, for more information see LICENSE.