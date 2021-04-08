#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Currency{
    string name;
    double sell;
    double buy;
    double maxSum;
};

struct ChangeOffice{
    string id;
    string name;
    int day;
    int month;
    int year;
    int currencyCount = 0;
    Currency curr[10];
};

void writeToFile(ChangeOffice* offices, int count);
void printWithLessCurr(ChangeOffice* offices, int count);
void printMaxAmount(ChangeOffice* offices, int count);
int findIndex(ChangeOffice office, string curr);
void printSortedPriceCurr(ChangeOffice* offices,int count);
void printSortedByDate(ChangeOffice* offices, int count);
ChangeOffice exchange(ChangeOffice office, double amount);
ChangeOffice addCurrencies(ChangeOffice office);
void printFromDate(ChangeOffice* offices, int count);
bool firstDateEarlier(int day1, int month1, int year1, int day2, int month2, int year2);
void printWithMinSum(ChangeOffice* offices, int count);
void printOffice(ChangeOffice office);
ChangeOffice enterNewOffice();
void printMenu();

int main()
{
    ChangeOffice offices[50];
    int count = 0;
    Currency currency;
    int menuChoice;
    while(true){
        cout<<"Enter the wanted operation"<<endl;
        printMenu();
        cin>>menuChoice;
        switch(menuChoice){
        case 0:
            writeToFile(offices, count);
            return 0;
            break;
        case 1:
            {
                string cont;
                do{
                    offices[count] = enterNewOffice();
                    count++;
                    cout<<"If you want to add one more Office type \"yes\""<<endl;
                    cin>>cont;
                }while(cont == "yes");
            }
            break;
        case 2:
            {
                int n;
                cout<<"Enter N:";
                cin>>n;
                for(int i = count; i< n+count;i++){
                    offices[i] = enterNewOffice();
                }
                count += n;
                break;
            }
        case 3:
            {
                string n;
                for(int i=0; i< count;i++){
                    if(i % 5 == 0 && i != 0){
                        cout<<"Press n for next page, or anything else to exit.";
                        cin>>n;
                        if(n != "n"){
                            break;
                        }
                    }
                    printOffice(offices[i]);
                }
            }
            break;
        case 4:
            printWithMinSum(offices, count);
            break;
        case 5:
            printFromDate(offices, count);
            break;
        case 6:
            {
                string id;
                bool exist = false;
                cout<<"Enter ID:";
                cin>>id;
                for(int i=0; i<count; i++){
                    if(id == offices[i].id){
                        offices[i] = addCurrencies(offices[i]);
                        exist = true;
                    }
                }
                if(exist == false){
                    cout<<"No Change Office with such ID!"<<endl;
                }
            }
            break;
        case 7:
            {
                string id;
                double amount;
                bool exist = false;
                cout<<"Enter ID:";
                cin>>id;
                cout<<"Enter amount you want to exchange:";
                cin>>amount;
                for(int i=0; i<count; i++){
                    if(id == offices[i].id){
                        offices[i] = exchange(offices[i], amount);
                        exist = true;
                    }
                }
                if(exist == false){
                    cout<<"No Change Office with such ID!"<<endl;
                }
            }
            break;
        case 8:
            printSortedByDate(offices, count);
            break;
        case 9:
            printSortedPriceCurr(offices, count);
            break;
        case 10:
            printMaxAmount(offices, count);
            break;
        case 11:
            printWithLessCurr(offices, count);
            break;

        }
    }
    return 0;
}

void writeToFile(ChangeOffice* offices, int count){
    ofstream output("offices.bin");
    for(int i=0; i<count; i++){
        output.write((char*)&offices[i], sizeof(ChangeOffice));
    }
    output.close();
}

void printWithLessCurr(ChangeOffice* offices, int count){
    bool exist = false;
    for(int i=0; i<count; i++){
        if(offices[i].currencyCount < 6){
            printOffice(offices[i]);
            exist = true;
        }
    }
    if(!exist){
        cout<<"There is no Change office with Less than 6 Currencies "<<endl;
    }
}


void printMaxAmount(ChangeOffice* offices, int count){
    cout<<"Enter Amount: ";
    double amount;
    cin>>amount;
    bool exist = false;
    for(int i=0; i<count; i++){
        for(int j=0; j<offices[i].currencyCount; j++){
            if(offices[i].curr[j].maxSum > amount){
                printOffice(offices[i]);
                exist = true;
                break;
            }
        }
    }
    if(!exist){
        cout<<"There is no Change office with bigger Exchange sum than "<<amount<<endl;
    }
}

void printSortedPriceCurr(ChangeOffice* offices,int count){
    cout<<"Enter the wanted Currency:";
    string curr;
    cin>>curr;
    ChangeOffice temp;
    int firstCurrIndex;
    int secondCurrIndex;
    for(int i=0; i<count; i++){
        firstCurrIndex = findIndex(offices[i], curr);
        for(int j=i+1; j<count; j++){
            secondCurrIndex = findIndex(offices[j], curr);
			if(secondCurrIndex >= 0 && firstCurrIndex >= 0){
			    if(offices[i].curr[firstCurrIndex].buy > offices[j].curr[secondCurrIndex].buy){
                    temp = offices[i];
                    offices[i]=offices[j];
                    offices[j]=temp;
			    }
			}
			if(firstCurrIndex == -1){
                temp =offices[i];
                offices[i]=offices[j];
                offices[j]=temp;
                firstCurrIndex = secondCurrIndex;
			}
		}
	}
	for(int i=0; i<count; i++){
        printOffice(offices[i]);
	}
}

int findIndex(ChangeOffice office, string curr){
    int index = -1;
    for(int i = 0; i < office.currencyCount; i++){
        if(office.curr[i].name == curr){
            index = i;
        }
    }
    return index;
}

void printSortedByDate(ChangeOffice* offices, int count){
    cout<<"Sorted by Date:"<<endl;
    ChangeOffice temp;
    for(int i=0; i<count; i++){
        for(int j=i+1; j<count; j++){
			if(firstDateEarlier(offices[j].day, offices[j].month, offices[j].year, offices[i].day, offices[i].month, offices[i].year)){
				temp =offices[i];
				offices[i]=offices[j];
				offices[j]=temp;
			}
		}
	}
	for(int i=0; i<count; i++){
        printOffice(offices[i]);
	}
}

ChangeOffice exchange(ChangeOffice office, double amount){
    printOffice(office);
    cout<<"Select which Currency you want to exchange!";
    int n;
    cin>>n;
    n -=1;
    if((office.curr[n].maxSum - amount) < 0){
        cout<<"The amount cant be exchanged because the Office maximum Sum is less."<<endl;
        return office;
    }
    cout<<"Are you sure you want to exchange "<<amount<<" "<<office.curr[n].name<<" ?\nYes or No"<<endl;
    string choice;
    cin>>choice;
    if(choice == "Yes" || choice == "yes"){
        cout<<"The exchange is successful!"<<endl;
        office.curr[n].maxSum -= amount;
        return office;
    }
    cout<<"The exchange is NOT successful!"<<endl;
    return office;
}

ChangeOffice addCurrencies(ChangeOffice office){
    int n;
    cout<<"How much currencies do you want to add?"<<endl;
    cin>>n;
    n += office.currencyCount;
    if(n > 10){
        cout<<"You cant add "<<n<<" currencies, because the limit is 10."<<endl;
        return office;
    }
    for(int i = office.currencyCount; i < n; i++){
        cout<<"Enter info about a Currency"<<endl<<"ISO:";
        cin>>office.curr[i].name;
        cout<<"Sell Price:";
        cin>>office.curr[i].sell;
        cout<<"Buy Price:";
        cin>>office.curr[i].buy;
        cout<<"Max exchange sum:";
        cin>>office.curr[i].maxSum;
    }
    office.currencyCount = n;
    return office;
}

bool firstDateEarlier(int day1, int month1, int year1, int day2, int month2, int year2){
    if(year1 < year2){
        return true;
    }else if(year1 == year2 && month2 == month1 && day1 < day2){
        return true;
    }else if(year1 == year2 && month2 > month1){
        return true;
    }
    return false;
}

void printFromDate(ChangeOffice* offices, int count){
    int day,month,year,toDay,toMonth, toYear;
    cout<<"Enter Selected Interval date:\nFrom Day:";
    cin>>day;
    cout<<"From Month:";
    cin>>month;
    cout<<"From Year:";
    cin>>year;
    cout<<"To Day:";
    cin>>toDay;
    cout<<"To Month:";
    cin>>toMonth;
    cout<<"To Year:";
    cin>>toYear;
    for(int i=0; i<count; i++){
        if(firstDateEarlier(offices[i].day, offices[i].month, offices[i].year, toDay, toMonth, toYear)
           && firstDateEarlier(day, month, year, offices[i].day, offices[i].month, offices[i].year))
        {
            printOffice(offices[i]);
        }
    }
}

void printWithMinSum(ChangeOffice* offices, int count){
    cout<<"Change Offices with Maximum exchange sum under 100"<<endl;
    bool min = false;
    for(int i=0; i<count; i++){
        for(int j=0; j<offices[i].currencyCount; j++){
            if(offices[i].curr[j].maxSum < 100){
                printOffice(offices[i]);
                min = true;
                break;
            }
        }
    }
    if(min == false){
        cout<<"There is no Change Offices with Maximum exchange sum under 100"<<endl;
    }
}

void printOffice(ChangeOffice office){
    cout<<"ID: \t Name: \t Founding date:"<<endl;
    cout<<office.id<<" \t "<<office.name<<" \t "<<office.day<<"/"<<office.month<<"/"<<office.year<<"\n\tCurrencies:"<<endl;
    cout<<"\tIOS: \t Buy: \t Sell: \t Maximum Exchange"<<endl;
    for(int i=0; i < office.currencyCount;i++){
        cout<<i+1<<".\t"<<office.curr[i].name<<"\t"<<office.curr[i].buy<<"\t"<<office.curr[i].sell<<"\t"<<office.curr[i].maxSum<<endl;
    }
    cout<<endl;
}

ChangeOffice enterNewOffice(){
    ChangeOffice office;
    Currency currency;
    cout<<"Enter Change office ID:";
    cin>>office.id;
    cout<<"Enter Change office Name:";
    cin>>office.name;
    cout<<"Enter Change office opening date:"<<endl<<"Day:";
    cin>>office.day;
    cout<<"Month:";
    cin>>office.month;
    cout<<"Year:";
    cin>>office.year;
    cout<<"Enter info about a single Currency"<<endl<<"ISO:";
    cin>>currency.name;
    cout<<"Sell Price:";
    cin>>currency.sell;
    cout<<"Buy Price:";
    cin>>currency.buy;
    cout<<"Max exchange sum:";
    cin>>currency.maxSum;
    office.curr[office.currencyCount] = currency;
    office.currencyCount += 1;
    return office;
}

void printMenu(){
    cout<<"0.Exit the program and save the offices in binary File."<<endl;
    cout<<"1.Add single Change office with single Currency."<<endl;
    cout<<"2.Add a list of Change offices with single Currency."<<endl;
    cout<<"3.Print all Change offices."<<endl;
    cout<<"4.Print Change offices with minimum exchange sum."<<endl;
    cout<<"5.Print all Change offices founded in selected time."<<endl;
    cout<<"6.Add currencies to Change offices by ID."<<endl;
    cout<<"7.Exchange Currencies in Change office by ID."<<endl;
    cout<<"8.Print the Change Offices Sorted by the date of founding."<<endl;
    cout<<"9.Print the Change Offices Sorted by lowest Price for a currency."<<endl;
    cout<<"10.Print the Change Offices with maximum Exchange amount."<<endl;
    cout<<"11.Print the Change Offices with Less Currencies than 6."<<endl;
}
