#include <iostream>
#include<fstream>
#include<bits/stdc++.h>

using namespace std;
//Book Properties.
struct Books
{
    char ISBN[10];
    char Title[30];
    char AuthorName[30] ;
    int Year;
    int NomOfPages;
    double Price;
};
//Function to take input from user.
istream& operator>>(istream & in, Books &Record);
//Function to take books info. from user.
ostream& operator<<(ostream &out,Books book);
//A Function to set user data to file.
void BOOK_INF(char isb[],char tit[],char auth[],int yer,int num,double pri,Books & B);
//A Function to add book to file.
void Add_Book(fstream&out, Books &Record);
//Function to delete an existing book from File.
void Delete_Book(fstream& file,char isbn[]);
//Function to Update data of an existing book.
void Update_Book(fstream&in,char isbn[]);
//Function to print particular book from file.
void Print_Book(fstream&file,char title[]);
//Function to print all books in file.
void PrintAll(fstream& file);

int main()
{
    fstream File("BOOKS.txt",ios::out| ios::in| ios::trunc |ios::binary);
    Books book1 , book2,book3,book4,book5,book6,book7,book8,book9,book0;
    ///Books info .
    BOOK_INF("12340","Book #0","Title 0",2012,1000,100,book0);
    BOOK_INF("12341","Book #1","Title 1",2012,1000,100,book1);
    BOOK_INF("12342","Book #2","Title 2",2012,1000,100,book2);
    BOOK_INF("12343","Book #3","Title 3",2012,1000,100,book3);
    BOOK_INF("12344","Book #4","Title 4",2012,1000,100,book4);
    BOOK_INF("12345","Book #5","Title 5",2012,1000,100,book5);
    BOOK_INF("12346","Book #6","Title 6",2012,1000,100,book6);
    BOOK_INF("12347","Book #7","Title 7",2012,1000,100,book7);
    BOOK_INF("12348","Book #8","Title 8",2012,1000,100,book8);
    BOOK_INF("12349","Book #9","Title 9",2012,1000,100,book9);
    ///Adding Books to file.
    Add_Book(File,book0);
    Add_Book(File,book1);
    Add_Book(File,book2);
    Add_Book(File,book3);
    Add_Book(File,book4);
    Add_Book(File,book5);
    Add_Book(File,book6);
    Add_Book(File,book7);
    Add_Book(File,book8);
    Add_Book(File,book9);

    ///Print All Books.
    PrintAll(File);

    ///Deleting 3 books from file.
    Delete_Book(File,"12345");cout << endl;
    Delete_Book(File,"12340");cout << endl;
    Delete_Book(File,"12349");cout << endl;

    ///Print after deleting.
    PrintAll(File);cout << endl;

    ///Print Particular book.
    Print_Book(File,"Book #0");cout << endl;
    Print_Book(File,"Book #7");cout << endl;

    Update_Book(File,"12346");
    Update_Book(File,"12347");

    ///Print all Books after updating 2 books.
    PrintAll(File);

    File.close();
    return 0;
}
//Function to take input from user.
istream& operator>>(istream & in, Books &Record)
{

    cout <<"Please enter your Book ISBN : ";
    in.getline(Record.ISBN,10);
    cout <<"Please enter your Book Title : ";
    in.getline(Record.Title,30);
    cout <<"Please enter your Book Author Name : ";
    in.getline(Record.AuthorName,30);
    cout <<"Please enter edition year of your book : ";
    in >> Record.Year;
    cout <<"Please enter number of your book pages : ";
    in >> Record.NomOfPages;
    cout <<"Please enter your Book price : ";
    in >> Record.Price;

    in.ignore();
    return in;
}

//Function to take books info. from user.
ostream& operator<<(ostream &out,Books book)
{
    out << "Book ISBN : "<<book.ISBN<<endl;
    out << "Book Title : "<<book.Title<<endl;
    out << "Book AuthorName : "<<book.AuthorName<<endl;
    out << "Book Year : "<<book.Year<<endl;
    out << "Book number of pages : "<<book.NomOfPages<<endl;
    out << "Book Price : "<<book.Price<<endl;
    return out;

}
//A Function to set user data to file.
void BOOK_INF(char isb[],char tit[],char auth[],int yer,int num,double pri,Books & B)
{
    strcpy(B.ISBN,isb);
    strcpy(B.Title,tit);
    strcpy(B.AuthorName,auth);
    B.Year=yer;
    B.NomOfPages=num;
    B.Price=pri;
}
//A Function to add book to file.
void Add_Book(fstream&out, Books &Record)
{
    short Size = strlen(Record.ISBN);
    out.write((char*)&Size, sizeof(Size));
    out.write((char*)&Record.ISBN, Size);

    Size = strlen(Record.Title);
    out.write((char*)&Size, sizeof(Size));
    out.write((char*)&Record.Title, Size);

    Size = strlen(Record.AuthorName);
    out.write((char*)&Size, sizeof(Size));
    out.write((char*)&Record.AuthorName, Size);

    Size = sizeof(Record.Year);
    out.write((char*)&Size, sizeof(Size));
    out.write((char*)&Record.Year,Size );

    Size =sizeof(Record.NomOfPages);
    out.write((char*)&Size, sizeof(Size));
    out.write((char*)&Record.NomOfPages,Size );

    Size = sizeof(Record.Price);
    out.write((char*)&Size, sizeof(Size));
    out.write((char*)&Record.Price,Size );
    out.write("$",1);

}
//Function to delete an existing book from File.
void Delete_Book(fstream& file,char isbn[])
{
    file.seekg(0,ios::beg);
    short a;
    bool found=false;
    while(!file.eof() && !found)
    {
        file.read((char*)&a,sizeof(a));
        char is[10];
        file.read((char*)&is,a);
        if (strcmp(isbn,is)==0)
        {
            found=true;
            file.seekp(-5,ios::cur);
            file.write("*",1);
        }
        else
        {
            file.ignore(100,'$');
        }
        file.peek();
    }
}
//Function to Update data of an existing book.
void Update_Book(fstream&file,char isbn[])
{
    Books book;
    Delete_Book(file,isbn);
    cout << "Enter your new book : "<<endl;
    cin >> book;
    file.seekp(0,ios::end);
    Add_Book(file,book);
}
//Function to print particular book from file.
void Print_Book(fstream&file,char title[])
{
    int last=0;
    file.seekg(0,ios::beg);
    Books book;
    short a,b,c;
    char is[10];
    char tit[30];
    bool found=false;
    while(!file.eof() && !found)
    {
        int position=file.tellg();
        file.read((char*)&a,sizeof(a));
        file.read((char*)&is,a);
        file.read((char*)&b,sizeof(b));
        file.read((char*)&tit,b);
        tit[b]='\0';
        last=file.tellg();
        if (strcmp(tit,title)==0 )
        {
            found=true;
            file.seekg(-1*(last-position),ios::cur);

            file.read((char*)&a, sizeof(a));
            file.read(book.ISBN, a);
            book.ISBN[a]='\0';

            file.read((char*)&b, sizeof(b));
            file.read(book.Title, b);
            book.Title[b]='\0';

            file.read((char*)&c, sizeof(c));
            file.read(book.AuthorName, c);
            book.AuthorName[c]='\0';

            file.seekg(sizeof(short), ios::cur);
            file.read((char*)& book.Year, sizeof(int));

            file.seekg(sizeof(short), ios::cur);
            file.read((char*)& book.NomOfPages, sizeof(int));

            file.seekg(sizeof(short), ios::cur);
            file.read((char*)& book.Price, sizeof(double));

            if(book.ISBN[0]!='*')
                cout<<book<<endl;
            else
            {
                cout<<"Oops !!"<<"\nThis Book is deleted :("<<endl;
            }
            //cout<<book<<endl;
        }
        else
        {
            file.ignore(100,'$');
        }
        file.peek();
    }

}
//Function to print all books in file.
void PrintAll(fstream& file)
{
    Books book;
    short a,b,c;
    file.seekg(0, ios::beg);
    cout <<endl;
    while(!file.eof())
    {
        file.read((char*)&a, sizeof(a));
        file.read(book.ISBN, a);
        book.ISBN[a]='\0';

        file.read((char*)&b, sizeof(b));
        file.read(book.Title, b);
        book.Title[b]='\0';

        file.read((char*)&c, sizeof(c));
        file.read(book.AuthorName, c);
        book.AuthorName[c]='\0';

        file.seekg(sizeof(short), ios::cur);
        file.read((char*)& book.Year, sizeof(int));

        file.seekg(sizeof(short), ios::cur);
        file.read((char*)& book.NomOfPages, sizeof(int));

        file.seekg(sizeof(short), ios::cur);
        file.read((char*)& book.Price, sizeof(double));

        if(book.ISBN[0]!='*')
            cout<<book<<endl;

        file.seekg(1, ios::cur);
        file.peek();        ///to have a look on the next item
    }
    file.clear();
}

