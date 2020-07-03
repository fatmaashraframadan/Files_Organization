# Files-Organization

## Assignment.1

Write a c++ program to store the data of 10 books. Book attributes are: ISBN (5 chars), TITLE, AUTHOR NAME, PRICE, YEAR, NUM OF PAGES Store 10 books using delimited records, variable length fields assume any other information you need. Operations : Add book, delete book (by ISBN), update book (search by ISBN), print book(by title), print all (print all books)

## Assignment.2

Write a C++ program to store the data of 10 books. Book attributes are : ISBN (5 chars), TITLE, AUTHOR NAME, PRICE, YEAR, NUM OF PAGES

Store 10 books
File organization (fixed length records, delimited fields)
Operations:
Delete book (given the ISBN), you are asked to use the avail list technique for fixed length records, so that you can reuse the deleted records in new insertions. The list head is initially assigned ‐1, its data type is: short, and stored at the beginning of the file.
Add book, you must check first if there is a deleted record in the available list or not, if the list head is = ‐1, this means that you should append the new record.
Update book (given the ISBN)
Print book (given the ISBN), you will use a sequential search.
Print all books
Compact the file, remove the deleted records from the file, example
If the file looks like ( record1 record2 deletedrecord record3)  after compaction the file will look like ( record1 record2 record3)
Update book (search by ISBN), print book(by title), print all (print all books)

## Assignment.3

Perform all files operation using primary , secondary key and inverted list.
