# stackoverflow_20138787
https://stackoverflow.com/questions/20138787

How to match string values from two string vectors within a nested loop

Trying to find the indexes for certain header values in a CSV file to use them to extract 
the data in those positions in the rest of the file.

The problem was with the input for the header line. This contained a line break at the end 
which wasn't matching the items in the array. Removing the line break fixed the problem.

I was working on a Windows PC and then transferring the file to a Cent OS machine for running 
the code and the difference in the line endings between the two platforms is what caused the issue.

Using this as a debug statement cout << cell would show the string and ignore the line break. 
Using something like cout << cell << " CELL" didn't show the string because of the line break.
