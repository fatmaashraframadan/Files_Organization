#include <iostream>
#include <conio.h>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <string>
#include <sstream>

using namespace std;

class Courses
{
    int Pnext,Snext,Snext2;
    //Structure of data file.
    struct course
    {
        char ID[6]; //Primary key.
        string name;
        string InstuctorName;//secondary key.
        short weeks;
    };
    //Structure of primary index file.
    struct Primaryindex
    {
        char PK[6];
        int offset;
    };
    //Structure of Secondary index file.
    struct SecondaryIndex
    {
        string InstuctorName;
        int offset;
    };
    //Structure of inverted index file.
    struct IvIndex
    {
        char ID[6];
        int counter;
        int next;
    };

    //Files Names.
    char dataFilePath[20] = "DataFile.txt";
    char PindexFilePath[20] = "PrimaryIndex.txt";
    char SindexFilePath[20] = "SecondaryIndex.txt";
    char IvFilePath[20]="IvIndex.txt";

    const static int indexSize = 100;
    Primaryindex   Pindex[indexSize];
    SecondaryIndex Sindex[indexSize];
    IvIndex        Iindex[indexSize];

public:
    //Default Constructor.
    Courses()
    {
        Pnext = 0;
        Snext = 0;
        Snext2 = 0;
        //If there exist old data.
        LoadPIndex();
        LoadSIVIndex();
    }

    ~Courses()
    {
        SavePFile();
        SaveSIVFile();
    }

    void UpdateID(char Id[]);
    void PrintID(char Id[]);
    void UpdateINS(string name);
    void  PrintAll();
    void Course_Info(char id[6], string n, string Instructor, short w, course temp);
    void DeleteINS(string name);
    void DeleteID(char id[]);
    void PrintINS(string insname);
    void Add_Course();
    void ReadIndices();
    vector<int> SBinarySearch(string insname);
    void ChangePflag(bool flag);
    void ChangeSflag(bool flag);
    bool getPFlag();
    bool getSFlag();
    void LoadPIndex();
    void LoadSIVIndex();
    void Psort();
    void Ssort();
    int SINamePos(string s);
    void SavePFile();
    void SaveSIVFile();
    bool Exists(char name[20] );
    int PBinarySearch(char id[]) ;
    bool checkrepeatation(char id[]);
    friend istream& operator>>(istream &in, course &temp);
    friend ostream& operator<<(ostream& out, course temp);
};

//Function to take input from user.
istream& operator>>(istream &in, Courses::course &temp)
{
    cout << "Enter your ID : "<<endl;
    in>> temp.ID;
    cout << "Enter course Name : "<<endl;
    in >> temp.name;
    cout << "Enter course instructor name : "<<endl;
    in >> temp.InstuctorName;
    cout << "Enter number of weeks : "<<endl;
    in >> temp.weeks;

    return in;
}

//Function to print course data .
ostream& operator<<(ostream& out, Courses::course temp)
{
    out << "Enter your ID : "<< temp.ID <<endl;
    out << "Enter course Name : "<< temp.name<<endl;
    out << "Enter course instructor name : "<<temp.InstuctorName<<endl;
    out << "Enter number of weeks : "<<temp.weeks<<endl;
    return out;
}

void Courses:: Add_Course()
{
    course temp;
    fstream file(dataFilePath );
    file.seekp(0, ios::end);
    int location = file.tellg();
    cin >> temp;
    while(PBinarySearch(temp.ID)!=-1 || strlen(temp.ID)<5)
    {
        if(PBinarySearch(temp.ID)!=-1)
            cout <<"\n\nRepeated course ID \n\n";
        else if(strlen(temp.ID)<5)
            cout<<"\n\nInvalid course ID(less than 5)\n\n";
        cin>>temp;
    }

    char NAME[40];
    char INSNAME [40];

    //casting .
    for (unsigned int i=0 ; i< temp.name.length(); ++i)
    {
        NAME[i]=temp.name[i];
    }
    for (unsigned int i=0 ; i< temp.InstuctorName.length(); ++i)
    {
        INSNAME[i]=temp.InstuctorName[i];
    }

    //calculating record length.
    int len = strlen(temp.ID) + temp.InstuctorName.length() + temp.name.length() + sizeof(short) +4;

    //Writing record to data file.
    file.write((char*)& len, sizeof(int));
    file.write((char*)&temp.ID, strlen(temp.ID));
    file.write("$", 1);
    file.write((char*)&NAME, temp.name.length());
    file.write("$", 1);
    file.write((char*)&INSNAME, temp.InstuctorName.length());
    file.write("$", 1);
    file.write((char*)&temp.weeks, sizeof(short));
    file.write("$", 1);

    //writing new record to primary array.
    strcpy(Pindex[Pnext].PK, temp.ID);
    Pindex[Pnext].offset = location;
    Psort();    //sorting primary index.

    //writing new record to Secondary array.
    vector<int> vec = SBinarySearch(temp.InstuctorName);
    //Instructor name not repeated
    if (vec.size() == 0)
    {
        Sindex[Snext].InstuctorName=temp.InstuctorName;
        Sindex[Snext].offset=Snext2;
        Iindex[Snext2].counter=Snext2;
        strcpy(Iindex[Snext2].ID,temp.ID);
        Iindex[Snext2].next=-1;
        ++Snext;
    }
    else        //repeated instructor name
    {
        Iindex[vec[vec.size()-1]].next=Snext2;
        Iindex[Snext2].counter=Snext2;
        strcpy(Iindex[Snext2].ID,temp.ID);
        Iindex[Snext2].next=-1;
    }
    Ssort();

    ++Pnext;
    ++Snext2;
    file.close();

    SavePFile();
    SaveSIVFile();
}
//to read what in arrays .
void Courses::ReadIndices()
{

    cout<<"\n\nPINDEX:"<<Pnext<<endl;
    for(int i=0 ; i<Pnext ; ++i )
    {
        cout<<Pindex[i].PK<<"  "<<Pindex[i].offset<<endl;
    }

    cout<<"\n\nSINDEX:"<<Snext<<endl;
    for(int i=0 ; i<Snext ; ++i )
    {
        cout<<Sindex[i].InstuctorName<<"  "<<Sindex[i].offset<<endl;
    }

    cout<<"\n\nIVINDEX:"<<Snext2<<endl;
    for(int i=0 ; i<Snext2 ; ++i )
    {
        cout<<Iindex[i].counter<<"  "<<Iindex[i].ID<<"  "<<Iindex[i].next<<endl;
    }

}
void Courses::ChangePflag(bool flag)
{
    ofstream file(PindexFilePath);
    file.seekp(0,ios::beg);
    file<<flag;
    file.close();
}

void Courses::ChangeSflag(bool flag)
{
    ofstream file(SindexFilePath);
    file.seekp(0,ios::beg);
    file<<flag;
    file.close();
}
bool Courses::getPFlag()
{
    bool flag;
    ifstream file(PindexFilePath);
    file.seekg(0,ios::beg);
    file >> flag;
    file.close();
    return flag;
}
bool Courses::getSFlag()
{
    bool flag;
    ifstream file(SindexFilePath);
    file.seekg(0,ios::beg);
    file >> flag;
    file.close();
    return flag;
}
//function to get position of name in secondary array.
int Courses::SINamePos(string s)
{
    int Size = Snext;
    int low = 0, high = Size - 1;
    int m = -1;

    while (low <= high)
    {
        int middle = (low + high) / 2;
        if (s == Sindex[middle].InstuctorName)
        {
            m = middle;
            break;
        }
        else if (Sindex[middle].InstuctorName<s)
            low = middle + 1;
        else
            high = middle - 1;
    }
    return m;
}
void Courses::SavePFile()
{
    fstream file(PindexFilePath, ios::trunc | ios::out);
    if(Pnext == 0)
        return;
    ChangePflag(false);
    file.seekg(sizeof(bool),ios::beg);
    for (int i = 0; i < Pnext; i++)
    {
        Primaryindex temp = Pindex[i];
        file.write((char*)&temp, sizeof(temp)); //fixed len record, fixed len field
    }
    file.close();
}
//
void Courses::SaveSIVFile()
{
    char ins[40];
    fstream file(SindexFilePath, ios::trunc |ios::out);
    if(Snext == 0)
        return;
    ChangeSflag(false);
    file.seekg(sizeof(bool),ios::beg);
    for (int i = 0; i < Snext; i++)
    {
        //casting string to character.
        for(unsigned int j = 0; j < Sindex[i].InstuctorName.length(); ++j)
        {
            ins[j]=Sindex[i].InstuctorName[j];
        }
        ins[Sindex[i].InstuctorName.length()] = '\0';
        file.write((char*)&ins, sizeof(ins)); //fixed len record, fixed len field
        file.write((char*)&Sindex[i].offset,sizeof(int));
    }
    file.close();
    fstream file2( IvFilePath, ios::trunc |ios::out);
    for(int i=0; i<Snext2; ++i)
    {
        file2.write((char*)&Iindex[i],sizeof(Iindex[i]));
    }
    file2.close();
}
//Sorting primary index array.
void Courses::Psort()
{
    int len = Pnext - 1;
    Primaryindex temp;
    for (int i = 0; i<len; i++)
        for (int j = 0; j<len - i; j++)
        {
            if (atoi(Pindex[j].PK)>atoi(Pindex[j + 1].PK))
            {
                temp = Pindex[j];
                Pindex[j] = Pindex[j + 1];
                Pindex[j + 1] = temp;
            }
        }
}
//Sorting Secondary index array.
void Courses::Ssort()
{
    int len = Snext - 1;
    SecondaryIndex temp;
    for (int i = 0; i<len; i++)
        for (int j = 0; j<len - i; j++)
        {
            if (Sindex[j].InstuctorName.compare(Sindex[j+1].InstuctorName) > 0)
            {
                temp = Sindex[j];
                Sindex[j] = Sindex[j + 1];
                Sindex[j + 1] = temp;
            }
        }
}

bool Courses:: Exists(char name[20] )
{
    ifstream f(name);
    if (f.fail())
    {
        return false;
    }
    else
    {
        f.close();
        return true;
    }
}

int Courses::PBinarySearch(char id[])            //return the index of this id in the primary array
{
    int Size = Pnext;
    int low = 0, high = Size - 1;
    while (low <= high)
    {
        int middle = (low + high) / 2;

        if (strcmp(Pindex[middle].PK, id) == 0)
            return middle;

        else if (atoi(Pindex[middle].PK)<atoi(id))
            low = middle + 1;
        else
            high = middle - 1;
    }
    return -1;
}

//returning vector contains the indices of ids in IVlist
vector<int> Courses::SBinarySearch(string insname)
{
    vector <int> vec;
    int m = SINamePos(insname);//index in secondary array.
    if(m==-1)
    {
        return vec;
    }
    int off = Sindex[m].offset;//first id in inverted list.
    while (true)
    {
        vec.push_back(Iindex[off].counter);
        if(Iindex[off].next == -1)
            break;
        off = Iindex[off].next;
    }
    return vec;
}


void Courses:: PrintAll()
{
    if(Pnext < 1)
    {
        cout << "\n\n There is no courses to print \n\n";
        return ;
    }
    else
    {
        for (int i=0; i < Pnext; ++i)
        {
            PrintID(Pindex[i].PK);
            cout << endl;
        }
        cout << endl;
    }

}
void Courses::PrintID(char Id[])
{
    fstream file (dataFilePath,ios::in | ios::out | ios::binary);
    int off = PBinarySearch(Id);    //index of this id in the primary array
    if(off == -1)
    {
        cout <<"\nID Not Found\n";
        return;
    }
    file.seekg(Pindex[off].offset, ios::beg);
    course temp;

    file.seekg(sizeof(int),ios::cur);   //After record length.

    file.getline(temp.ID,6,'$');

    getline(file,temp.name,'$');

    getline(file,temp.InstuctorName,'$');

    file.read((char*)&temp.weeks, sizeof(short));
    file.seekg(1,ios::cur);

    cout <<endl<< temp<<endl;
    file.close();
}

void Courses::PrintINS(string insname)

{
    vector <int> vec = SBinarySearch(insname);  //Indicies of ids of the same name in inverted list
    if (vec.size()==0)
    {
        cout<<"\nNAME Not Found!!\n";
        return;
    }
    for (unsigned int i=0 ; i< vec.size() ; ++i)
    {
        PrintID(Iindex[vec[i]].ID);
    }
}

//Loading from primary file to primary array.
void Courses:: LoadPIndex()
{
    Primaryindex temp;  //to read file.
    Pnext = 0;
    fstream file(PindexFilePath, ios::in | ios::binary);
    file.seekg(0,ios::end);
    if (file.tellg() == 1 || file.tellg()==0)
        return;
    file.seekg(sizeof(bool), ios::beg);//standing after flag
    while(!file.eof())
    {
        file.read((char*)&temp, sizeof(temp));
        strcpy(Pindex[Pnext].PK, temp.PK);
        Pindex[Pnext].offset = temp.offset;
        Pnext++;
        file.peek();
    }
    file.close();
}
//loading secondary and inverted list files content to arrays .
void Courses::LoadSIVIndex()
{
    //loading from the Secondary file
    Snext = 0;
    char INS[40];
    string name;    //Instructor name.
    fstream file(SindexFilePath, ios::in |ios::binary);
    file.seekg(0,ios::end);
    if(file.tellg()==1 || file.tellg()==0)
        return;
    file.seekg(sizeof(bool), ios::beg);                //skip the flag
    while(!file.eof())
    {
        file.read((char*)&INS, sizeof(INS));
        file.read((char*)&Sindex[Snext].offset,sizeof(int));
        name = "";
        for(unsigned int i=0 ; i<strlen(INS) || INS[i]!='\0'; ++i)
        {
            name+=INS[i];
        }

        Sindex[Snext].InstuctorName=name;
        Snext++;
        file.peek();
    }
    file.close();

    //loading from the Inverted list file
    Snext2 = 0;
    fstream file2(IvFilePath, ios::in | ios::binary);
    IvIndex temp;
    while(!file2.eof())
    {
        file2.read((char*)&temp, sizeof(temp));

        Iindex[Snext2].counter = temp.counter;
        strcpy(Iindex[Snext2].ID, temp.ID);
        Iindex[Snext2].next = temp.next;
        Snext2++;
        file2.peek();
    }
    file2.close();
}

void Courses::UpdateID(char Id[])
{
    int off = PBinarySearch(Id);//returns index in primary array.
    if(off == -1)
    {
        cout << "\nNot Found !!\n";
        return;
    }
    PrintID(Id);
    DeleteID(Id);
    Add_Course();
}

void Courses::UpdateINS(string name)
{
    int location = SINamePos(name);//index of this name in secondary array.
    if(location == -1)
    {
        cout << "\n\nNot Found!\n\n"<<endl;
        return;
    }
    int choice;
    vector<int>vec = SBinarySearch(name);//indexes in inverted list.
    cout << "\n\nChoose id to update : \n"<<endl;
    for(unsigned int i = 0 ; i < vec.size(); i++)
    {
        cout <<i+1 << ")" << Iindex[vec[i]].ID<<endl;//IDs matched with this name.
    }
    cin >> choice;

    UpdateID(Iindex[vec[choice-1]].ID);
}

void Courses::Course_Info(char id[6], string n, string Instructor, short w, course temp)
{
    strcpy(temp.ID, id);
    temp.name = n;
    temp.InstuctorName = Instructor;
    temp.weeks = w;
}

void Courses:: DeleteINS(string name)
{
    //Deleting from secondary array.
    vector<int> vec = SBinarySearch(name);//index of ids in inverted list.
    if (vec.size()==0)
    {
        cout<<"\nNAME NOT FOUND\n";
        return;
    }
    for(unsigned int i=0 ; i <vec.size() ; ++i)
    {
        DeleteID(Iindex[vec[i]].ID);
    }
    cout <<"\n\nAll courses with this instructor name "<<name <<" deleted\n\n"<<endl;
}

void Courses:: DeleteID(char id[])
{
    string name,name2;      //name2 is the name of this id ,name is the Insname of this id
    char Id[6];

    ChangePflag(true);
    ChangeSflag(true);

    fstream file(dataFilePath,ios::in | ios::out);
    int ind = PBinarySearch(id);        //returns index in primary array.

    if(ind == -1 )
    {
        cout<<"\nID NOT FOUND\n";
        return;
    }
    int off = Pindex[ind].offset;   //offset in data file.

    file.seekg(off,ios::beg);
    file.seekg(sizeof(int),ios::cur);   //After record length.
    file.getline(Id,6,'$');
    getline(file,name2,'$');
    getline(file,name,'$');


    file.seekp(off,ios::beg);
    file.write("*",1);
    for (int i = ind; i< Pnext ; i++)
        Pindex[i] = Pindex[i + 1];

    Pnext--;

    vector<int>vec = SBinarySearch(name);   //return indexes of IDS in inverted list array.
    int isd = SINamePos(name);      //l mkan fel secondary array.

    //deleting from secondary file after deleting 1 ID :

    //Case 1 - Exists only one.
    if(vec.size() == 1)
    {
        for (int i = isd; i < Snext ; i++)
        {
            Sindex[i].InstuctorName = Sindex[i+1].InstuctorName;
            Sindex[i].offset = Sindex[i+1].offset;
        }
        --Snext;
    }
    //case 2 - First ID in inverted list.
    else if(strcmp(Iindex[vec[0]].ID,id) == 0)
    {
        //Changing secondary index.
        Sindex[isd].offset = Iindex[vec[0]].next;
    }
    //Case 3 - end of vector.
    else if(strcmp(Iindex[vec[vec.size()-1]].ID,id) == 0)
    {
        //Changing Inverted index.
        Iindex[vec[vec.size()-2]].next = -1;
    }
    //Case 3 - at the middle of vector .
    else
    {
        for(unsigned int i = 0 ; i < vec.size(); i++)
        {
            if(strcmp(Iindex[vec[i]].ID, id)==0)
            {
                Iindex[vec[i-1]].next = Iindex[vec[i]].next;
                break;
            }
        }
    }
    file.close();
    SavePFile();
    SaveSIVFile();
}

int main()
{
    Courses obj;
    char id [6];
    string name;
    while(true)
    {
        cout <<"\n\nPlease enter your choice :\n"<<endl;
        cout <<"1-Add New Course.\n2-Delete course (ID).\n3-Delete course (instructor name).\n4-Print course (ID).\n5-Print course (instructor name)."
             <<"\n6-update course (ID).\n7-update course (instructor name).\n8-Print All.\n9-Read Indexes."
             <<"\n0-Exit."<<endl;
        int choice;
        cin >> choice;
        switch(choice)
        {
        case 0:
            return 0;
        case 1 :
            obj.Add_Course();
            break;
        case 2 :
            cout << "\nEnter ID to delete : "<<endl;
            cin>>id;
            obj.DeleteID(id);
            break;
        case 3 :
            cout << "\nEnter Name to delete : "<<endl;
            cin>>name;
            obj.DeleteINS(name);
            break;
        case 4 :
            cout << "Enter ID to Print : "<<endl;
            cin>>id;
            obj.PrintID(id);
            break;
        case 5 :
            cout << "Enter course instructor name to print :  ";
            cin >> name;
            obj.PrintINS(name);
            break;
        case 6 :
            cout << "Enter ID to update : "<<endl;
            cin >> id;
            obj.UpdateID(id);
            break;
        case 7 :
            cout << "Enter course instructor name to update :  ";
            cin >> name;
            obj.UpdateINS(name);
            break;
        case 8 :
            obj.PrintAll();
            break;
        case 9 :
            obj.ReadIndices();
            break;
        default :
            cout << "\nWrong choice, please re-choose a number: \n";
            break;
        }
    }
    return 0;
}
