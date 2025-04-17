#include <iostream>
#include <string>
using namespace std;
class Mylong{
    private:
        string largest = "179769313486231590772930519078902473361797697894230657273430081157732675805500963132708477322407536021120113879871393357658789768814416622492847430639474124377767893424865485276302219601246094119453082952085005768838150682342462881473913110540827237163350510684586298239947245938479716304835356329624224137215";
    public:
        string add(string num1 , string num2);
        string sub(string num1 , string num2);
        string mult(string num1 , string num2);
};
string largest = "179769313486231590772930519078902473361797697894230657273430081157732675805500963132708477322407536021120113879871393357658789768814416622492847430639474124377767893424865485276302219601246094119453082952085005768838150682342462881473913110540827237163350510684586298239947245938479716304835356329624224137215";
string Mylong::mult(string num1 , string num2){
    int len1 = num1.size();
    int len2 = num2.size();
    string temp;
    int t;
    if(len1 > len2){
        temp = num1;
        num1 = num2;
        num2 = temp;
        t = len1;
        len1 = len2;
        len2 = t;
    }
    string ans="0";
    for(int i = 0 ; i < len1 ; i++){
        t = (int)(num1[i]-'0');
        for(int j = 0 ; j< t ; j++){
            ans = add(ans , num2);
        }
        ans.push_back('0');
    }
    ans.pop_back();
    if(ans.length() > largest.length()){
        printf("out of bound error\n");
        ans = "";
    }else if(ans.length() == largest.length() && ans>largest){
        printf("out of bound error\n");
        ans = "";
    }
    return ans;
}

string Mylong::add(string num1 , string num2){
    string ans;
    int digit_sum = 0;
    int carry = 0;
    int i = min(num1.size() , num2.size());
    while(i>0){
        int last1 = (int)num1.back()-48;
        num1.pop_back();
        int last2 = (int)num2.back()-48;
        num2.pop_back();
        digit_sum = last1 + last2 + carry;
        if(digit_sum>9){
            carry= 1;
            digit_sum = digit_sum-10;
        }else{
            carry=0;
        }
        ans.insert(0,1,(char)(digit_sum+48));
        i--;
    }
    i= num1.size()-num2.size();
    while(i>0){
        int last1 = (int)num1.back()-48;
        num1.pop_back();
        digit_sum = last1 + carry;
        if(digit_sum>9){
            carry= 1;
            digit_sum = digit_sum-10;
        }else{
            carry=0;
        }
        i--;
        ans.insert(0,1,(char)(digit_sum+48));
    }
    i= num2.size()-num1.size();
    while(i>0){
        int last2 = (int)num2.back()-48;
        num2.pop_back();
        digit_sum = last2 + carry;
        if(digit_sum>9){
            carry= 1;
            digit_sum = digit_sum-10;
        }else{
            carry=0;
        }
        i--;
        ans.insert(0,1,(char)(digit_sum+48));
    }
    
    if(carry){
        digit_sum=1;
        ans.insert(0,1,(char)(digit_sum+48));
    }
    if(ans.length() > largest.length()){
        printf("out of bound error\n");
        ans = "";
    }else if(ans.length() == largest.length() && ans>largest){
        printf("out of bound error\n");
        ans = "";
    }
    return ans;
}

string Mylong::sub(string num1 , string num2){
    string ans;
    int len1 = num1.size();
    int len2 = num2.size();
    string temp;
    int t = 0;

    
    if(len2 > len1 ){
        temp = num1;
        num1 = num2;
        num2 = temp;
        t = len1;
        len1 = len2;
        len2 = t;
        t = 1;
    }
    else if((len2==len1 && num1<num2)){
        temp = num1;
        num1 = num2;
        num2 = temp;
        t = len1;
        len1 = len2;
        len2 = t;
        t = 1;

    }
    int i = len2;
    int digit_diff = 0;
    int borrow = 0;
    while(i>0){
        int last1 = (int)num1.back()-48;
        num1.pop_back();
        int last2 = (int)num2.back()-48;
        num2.pop_back();
        digit_diff = last1 - last2 - borrow;
        if(digit_diff < 0){
            borrow = 1;
            digit_diff = digit_diff+10;
        }else{
            borrow=0;
        }
        ans.insert(0,1,(char)( digit_diff + 48 ));
        i--;
    }
    i = len1 - len2;
    while(i>0){
        int last1 = (int)num1.back()-48;
        num1.pop_back();
        digit_diff = last1 - borrow;
        if(digit_diff<0){
            borrow= 1;
            digit_diff = digit_diff + 10;
        }else{
            borrow=0;
        }
        i--;
        ans.insert(0,1,(char)(digit_diff+48));
    }
    if(ans.length() > largest.length()){
        printf("out of bound error\n");
        ans = "";
    }else if(ans.length() == largest.length() && ans>largest){
        printf("out of bound error\n");
        ans = "";
    }else{
        if(t){
            ans.insert(0,1,'-');
        }
    }

    return ans;
}

int main() {
    Mylong m;
    string num1;
    string num2;
    string ans;
    int choice ;
    int exit = 0; 
    while(!exit){
        cout<<"1. addition "<<"\n"<<"2. subtraction"<<"\n"<<"3. multiplication"<<"\n"<<"4. exit"<<"\n";
        cout<<"your choice : ";
        cin >>choice;
        if(choice !=4){
            cout << "Enter number 1: ";
            cin >> num1; 
            cout << "Enter number 2: ";
            cin >> num2;
            if(num1.length() > largest.length() || num2.length() > largest.length()){
                cout<<"inputs out of bound \n";
                exit = 1;
            }else if( (num1.length() == largest.length() && num1>largest ) || (num2.length() == largest.length() && num2>largest)){
                cout<<"inputs out of bound \n";
                exit = 1;
            }
        }
        string sum; 
        string subt; 
        string mul; 
        switch(choice){
            case 1:
                sum = m.add(num1 , num2);
                if(sum.length()){
                    cout<<"Addition is : "<<sum<<"\n";
                }
                break;
            case 2:
                subt = m.sub(num1 , num2);
                if(subt.length()){
                    cout<<"Subtraction is : "<<subt<<"\n";
                }
                break;
            case 3:
                mul  = m.mult(num1 , num2);
                if(mul.length()){
                    cout<<"Multiplication is : "<<mul<<"\n";
                }
                break;
            case 4:
                exit = 1;
                break;
        }
    }
    return 0;
}
