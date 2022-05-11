**Name: Mitran Andrei-Gabriel**
**Group: 313CA**

## The Library of HashBabel - Homework 2

### Description:

* The program utilizes a simple linked list and hashtable structures.
* The linked list functions include: creating and freeing the list, adding and
removing a node.
* The hashtable utilizes the linked list implementation.
* The hashtable functions include: creating and freeing the hashtable, putting
and removing an entry, checking if it has a specific key and getting a value
associated with a key. The number of buckets is originally HMAX, but the
hashtable resizes itself if the number of entries is greater than the number
of buckets. When putting a new entry with an already existing key, the
original value associated with the key is deleted. Memory deallocation is
handled by an extra "free_function". A pointer to it is saved in the hashtable
structure, alongside the poiners to the hashing and compare functions.
* The program implements a library and a user database using hashtables.
* A book in the library is itself implemented using a hashtable. It contains
multiple definitions, that are made up of two words (the key and the value).
* Using the library, one can add a book, get a book and print important info
about it, remove a book, add a definition to a specified book, get a definition
from a specified book and print it, as well as remove a definition.
* Using the database, after a user has been added, they can borrow a book,
return it or report that they lost it. The user has a score associated to them,
which increases if the book is returned on time and decreases if it is not or
if they lose the book. If their score becomes negative, they are banned from
the library.
* Commands:
* Library commands:
1. ADD_BOOK: A structure that contains various information regarding that
specific book is created. The structure stores the sum of total ratings,
the number of times the book was borrowed, the average rating the book's status
(borrowed or not), the book's name and the hashtable of definitions. The number
of definitions is specified when adding the book. After reading and putting
all definitions in the book's hashtable, the book structure is added in the
library hashtable.
2. GET_BOOK: It gets the book structure from the library. It then prints
relevant info regarding the given book.
3. RMV_BOOK: It removes the book from the library hashtable.
4. ADD_DEF: It gets the book structure from the library and adds the definition
to the book's hashtable.
5. GET_DEF: It gets the book structure from the library. Then, it searches for
it in the book's hashtable and prints it if it finds it.
6. RMV_DEF: It gets the book structure from the library. The definition is
removed from the book's hashtable.
* User database (Users) commands:
1. ADD_USER: A structure that contains various information regarding that
specific user is created. The structure stores the user's score
(initially 100), the time limit until a book must be returned, the username and
the borrowed book's name. The user structure is added to the database
hashtable.
2. BORROW: After getting the book and user structures, it marks the book as
borrowed and the user as having borrowed said book, setting the time limit.
3. RETURN: After getting the book and user structures, it marks the book as
not borrowed and the user as having no book borrowed. The user's rating is
added to the book's sum of ratings, the number of times it was borrowed
increases by 1 and a new average rating is calculated. The user's new score is
also calculated based on whether or not the book was returned on time. The
score increases by the number of days remaining until the time limit (if the
book is returned on time) and decreases by (the number of days over the time
limit) * 2 (if the book is not returned on time).
4. LOST: After getting the user structure, the user's score decreases by 50 and
the book is removed from the library hashtable.
* If at any point a user's score becomes negative, the user is banned. The user
structure is removed from the database hashtable and the username is added in
a separate hashtable that holds all banned user's usernames.
* EXIT: When the program receives this command, it prints all books, ordered by
average rating, the number of times they were borrowed and lexicographically.
Afterwards, it prints all users ordered by their score and lexicographically.
All dynamically allocated memory is freed.

### Comments regarding the homework:

* The homework helped me better understand a hashtable and its functionality.
* The banned_users hashtable can easily be replaced with a vector or a linked
list. Its sole purpose is to showcase that a hashtable with values of variable
size can be resized using the same function that resizes hashtables with
values of invariable sizes.
* Another way to set the user's status as banned is by adding a field in the
user structure. However, the structure would remain in the database, which I
wanted to avoid. This simplifies the top_users function, as all users in the
database are valid (not banned).
* In order to mark the key and value's sizes as variable, I opted for adding
extra fields in the hashtable structure itself. This can be changed in a
number of ways, one of them being by storing the sizes in the info structure or
wherever else.
* Using an INIT_STR to mark a user as having no book borrowed can also be done
by adding an extra field to the user structure. I simply wanted to avoid adding
another.

### Resources:

1. http://www.cse.yorku.ca/~oz/hash.html (the hashing function for strings)