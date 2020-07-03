/*
Assignment 2
FileOrgSpring2019

*Programmers*
    Ahmed Sayed Mansour     20170022
    Fatma Ashraf Ramadan    20170192

*/

#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;
//Book Properties.
struct Books
{
    char ISBN[6];
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
void Print_Book(fstream&file,char isbn[]);
//Function to print all books in file.
void PrintAll(fstream& file);
//Function to get position of ISBN
int GetPosOfISBN(fstream&file,char isbn[]);
//Function to remove deleted records from file.
void Compact(fstream&file);

int main()
{
    fstream file("BOOKS.txt",ios::out| ios::in |ios::binary );

    Books book0,book1,book2,book3,book4,book5,book6,book7,book8,book9;

    //setting header initial value.
    short header = -1;
    file.seekg(0, ios::end);
    if (file.tellg() == 0) {
        file.seekp(0,ios::beg);
        file.write((char*)&header,sizeof(short));
    }

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
    Add_Book(file,book0);
    Add_Book(file,book1);
    Add_Book(file,book2);
    Add_Book(file,book3);
    Add_Book(file,book4);
    Add_Book(file,book5);
    Add_Book(file,book6);
    Add_Book(file,book7);
    Add_Book(file,book8);
    Add_Book(file,book9);

    PrintAll(file);

    ///Deleting 3 books from file.
    Delete_Book(file,"12345");
    cout << endl;
    Delete_Book(file,"12340");
    cout << endl;
    Delete_Book(file,"12349");
    cout << endl;

    Print_Book(file,"12345");
    Print_Book(file,"12346");

    Update_Book(file,"12346");
    Update_Book(file,"12347");

    PrintAll(file);
    //removing deleted records from file.
    Compact(file);

    PrintAll(file);
    return 0;
}
//Function to take input from user.
istream& operator>>(istream & in, Books &Record)
{

    cout <<"Please enter your Book ISBN : ";
    in.getline(Record.ISBN,6);
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
    out.seekg(0,ios::beg);
    short rrn;
    out.read((char*)&rrn,sizeof(short));
    if(rrn == -1)
    {
        out.seekp(0,ios::end);
    }
    else
    {
        out.seekg((((rrn-1)*(sizeof(Record)+6))+1),ios::cur);
        out.read((char*)&rrn,sizeof(short));    //reading rrn of deleted record
        out.seekp(0,ios::beg);
        out.write((char*)&rrn,sizeof(short));   //writing stored value in the deleted record in the header.
        out.seekp((rrn-1)*(sizeof(Record)+6),ios::cur);
    }
    //Writing books data to file.
    out.write((char*)&Record.ISBN, sizeof(Record.ISBN));
    out.write("$",1);

    out.write((char*)&Record.Title, sizeof(Record.Title));
    out.write("$",1);

    out.write((char*)&Record.AuthorName, sizeof(Record.AuthorName));
    out.write("$",1);

    out.write((char*)&Record.Year,sizeof(Record.Year));
    out.write("$",1);

    out.write((char*)&Record.NomOfPages,sizeof(Record.NomOfPages) );
    out.write("$",1);

    out.write((char*)&Record.Price,sizeof(Record.Price));
    out.write("$",1);

}

//Function to delete an existing book from File.
void Delete_Book(fstream& file,char isbn[]) ///Short Head!!
{
    file.seekg(0,ios::beg);
    short head;
    file.read((char*)&head, sizeof(short));
    Books book;
    short count=0;  //to count rrn of record.
    bool found=false;
    while(!file.eof() && !found)
    {
        file.read((char*)&book.ISBN,sizeof(book.ISBN));
        if (strcmp(book.ISBN,isbn)==0)             //comparing read ISBN with given ISBN.
        {
            found = true;
            file.seekp(-6,ios::cur);
            short current = file.tellg();
            file.write("*",1);                          //Writing * at the beginning of the ISBN.
            file.write((char*)&head, sizeof(short));    //Writing the header value after *.

            file.seekp(0,ios::beg);
            file.write((char*)&count,sizeof(short));
        }
        else
        {
            book.ISBN[strlen(book.ISBN)]='\0';
            file.seekg(1,ios::cur);

            file.read((char*)&book.Title, sizeof(book.Title));
            book.Title[strlen(book.Title)]='\0';
            file.seekg(1,ios::cur);

            file.read((char*)&book.AuthorName, sizeof(book.AuthorName));
            book.AuthorName[strlen(book.AuthorName)]='\0';
            file.seekg(1,ios::cur);

            file.read((char*)& book.Year, sizeof(int));
            file.seekg(1,ios::cur);

            file.read((char*)& book.NomOfPages, sizeof(int));
            file.seekg(1,ios::cur);

            file.read((char*)& book.Price, sizeof(double));
            file.seekg(1,ios::cur);
            ++count;
        }
        file.peek();
    }
}

//Function to Update data of an existing book.
void Update_Book(fstream&out,char isbn[])
{
    out.clear();
    Books Record;
    cout << "Your Book : "<<endl;
    Print_Book(out,isbn);
    int p = GetPosOfISBN(out,isbn);
    out.seekp(p,ios::beg);

    cout << "Enter your new book : "<<endl;
    cin >> Record;

    out.write((char*)&Record.ISBN, sizeof(Record.ISBN));
    out.write("$",1);

    out.write((char*)&Record.Title, sizeof(Record.Title));
    out.write("$",1);

    out.write((char*)&Record.AuthorName, sizeof(Record.AuthorName));
    out.write("$",1);

    out.write((char*)&Record.Year,sizeof(Record.Year));
    out.write("$",1);

    out.write((char*)&Record.NomOfPages,sizeof(Record.NomOfPages) );
    out.write("$",1);

    out.write((char*)&Record.Price,sizeof(Record.Price));
    out.write("$",1);
}

//Function to print particular book from file.
void Print_Book(fstream&file,char isbn[])
{
    file.seekg(sizeof(short),ios::beg);
    Books book;
    bool found=false;
    while(!file.eof() && !found)
    {
        file.read((char*)&book.ISBN,sizeof(book.ISBN));
        book.ISBN[strlen(book.ISBN)]='\0';
        file.seekg(1,ios::cur);

        file.read((char*)&book.Title, sizeof(book.Title));
        book.Title[strlen(book.Title)]='\0';
        file.seekg(1,ios::cur);

        file.read((char*)&book.AuthorName, sizeof(book.AuthorName));
        book.AuthorName[strlen(book.AuthorName)]='\0';
        file.seekg(1,ios::cur);

        file.read((char*)& book.Year, sizeof(int));
        file.seekg(1,ios::cur);

        file.read((char*)& book.NomOfPages, sizeof(int));
        file.seekg(1,ios::cur);

        file.read((char*)& book.Price, sizeof(double));
        file.seekg(1,ios::cur);

        if(strcmp(book.ISBN,isbn)==0)
        {
            cout<<"FOUND :)"<<endl;
            found=true;
            cout<<book<<endl;
        }
        file.peek();
    }
    if(found==false)
        cout<<"NOT FOUND :( \n";

}

//Function to print all books in file.
void PrintAll(fstream& file)
{
    Books book;
    file.seekg(sizeof(short), ios::beg);
    while(!file.eof())
    {
        file.read(book.ISBN,sizeof(book.ISBN));
        book.ISBN[strlen(book.ISBN)]='\0';
        file.seekg(1,ios::cur);

        file.read(book.Title, sizeof(book.Title));
        book.Title[strlen(book.Title)]='\0';
        file.seekg(1,ios::cur);

        file.read(book.AuthorName, sizeof(book.AuthorName));
        book.AuthorName[strlen(book.AuthorName)]='\0';
        file.seekg(1,ios::cur);


        file.read((char*)& book.Year, sizeof(int));
        file.seekg(1,ios::cur);

        file.read((char*)& book.NomOfPages, sizeof(int));
        file.seekg(1,ios::cur);

        file.read((char*)& book.Price, sizeof(double));
        file.seekg(1,ios::cur);

        if(book.ISBN[0]!='*')
            cout<<book<<endl;

        file.peek();        ///to have a look on the next item.
    }
    file.clear();
}
//Function to get position of ISBN
int GetPosOfISBN(fstream&file,char isbn[])
{
    file.clear();
    file.seekg(sizeof(short),ios::beg);
    Books book;
    bool found=false;
    int pos=-1;
    while(!file.eof() && !found)
    {
        pos=file.tellg();
        file.read((char*)&book.ISBN,sizeof(book.ISBN));
        book.ISBN[strlen(book.ISBN)]='\0';
        file.seekg(1,ios::cur);

        file.read((char*)&book.Title, sizeof(book.Title));
        book.Title[strlen(book.Title)]='\0';
        file.seekg(1,ios::cur);

        file.read((char*)&book.AuthorName, sizeof(book.AuthorName));
        book.AuthorName[strlen(book.AuthorName)]='\0';
        file.seekg(1,ios::cur);

        file.read((char*)& book.Year, sizeof(int));
        file.seekg(1,ios::cur);

        file.read((char*)& book.NomOfPages, sizeof(int));
        file.seekg(1,ios::cur);

        file.read((char*)& book.Price, sizeof(double));
        file.seekg(1,ios::cur);
        if(strcmp(book.ISBN,isbn)==0)
        {
            found=true;
            return pos;
        }
        file.peek();
    }
    if(found==false)
        pos=-1;
    return pos;
}
//Function to remove deleted records from file.
void Compact(fstream&file)
{
    Books book;
    vector<Books>fileData;
    file.seekg(sizeof(short),ios::beg);
    while(!file.eof())
    {
        file.read(book.ISBN,sizeof(book.ISBN));
        book.ISBN[strlen(book.ISBN)]='\0';
        file.seekg(1,ios::cur);

        file.read(book.Title, sizeof(book.Title));
        book.Title[strlen(book.Title)]='\0';
        file.seekg(1,ios::cur);

        file.read(book.AuthorName, sizeof(book.AuthorName));
        book.AuthorName[strlen(book.AuthorName)]='\0';
        file.seekg(1,ios::cur);

        file.read((char*)& book.Year, sizeof(int));
        file.seekg(1,ios::cur);

        file.read((char*)& book.NomOfPages, sizeof(int));
        file.seekg(1,ios::cur);

        file.read((char*)& book.Price, sizeof(double));
        file.seekg(1,ios::cur);

        bool repeat = false;
        if(book.ISBN[0]!='*')
        {
            for (int i=0; i<fileData.size(); ++i)
            {
                if(strcmp(fileData[i].ISBN,book.ISBN)==0)
                {
                    repeat = true;
                }
            }
            if (!repeat)
                fileData.push_back(book);
        }
        repeat = false;

        file.peek();        ///to have a look on the next item.
    }
    file.clear();
    //file.close();

    fstream out("BOOKS.txt",ios::out| ios::in |ios::binary |ios::trunc);
    short header = -1;
    out.write((char*)&header,sizeof(short));
    for(int i = 0 ; i < fileData.size(); i++)
    {
        out.write((char*)&fileData[i].ISBN, sizeof(fileData[i].ISBN));
        out.write("$",1);

        out.write((char*)&fileData[i].Title, sizeof(fileData[i].Title));
        out.write("$",1);

        out.write((char*)&fileData[i].AuthorName, sizeof(fileData[i].AuthorName));
        out.write("$",1);

        out.write((char*)&fileData[i].Year,sizeof(fileData[i].Year));
        out.write("$",1);

        out.write((char*)&fileData[i].NomOfPages,sizeof(fileData[i].NomOfPages) );
        out.write("$",1);

        out.write((char*)&fileData[i].Price,sizeof(fileData[i].Price));
        out.write("$",1);
    }

}

