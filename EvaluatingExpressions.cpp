///najpierw zamieniamy wyrazenie na RPN, a potem obliczamy, zamieniając na postać infiksową
/// ./Obliczanie_wyr < test.in >test.out
#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <cmath>
using namespace std;

int priority_(char x){
    if(x=='+' || x=='-') return 0;
    if(x=='*' || x=='/') return 1;
    if(x=='^') return 2;
    return -1;
}
bool isASign (char x){
    return x=='+' || x=='-' || x=='*' || x=='/' || x=='^' || x=='(' || x==')';
}
bool isAsi(char x){ //stworzone dla RPN
    return x=='+' || x=='-' || x=='*' || x=='/' || x=='^';
}
bool isOpening (char x){ //is an Opening Bracket
    return x=='(';
}
bool isClosing (char x){
    return x==')';
}
string change(string wyr, int prio[]){ //zamieniam wyr z nawiasami na bez nawiasow, ale ze zwiekszonymi priorytetami działań
	int dl=wyr.size(), br=0; //ile jest nawiasow
	string wyn=""; int j=0; //index po wyniku
	///cout<<"ZMIENIAJ"<<endl;
	for(int i=0; i<dl; i++){
		if(!isASign(wyr[i])) {
			wyn+=wyr[i];
			j++;
		}
		else if(isOpening(wyr[i])) br++;
		else if(isClosing(wyr[i])) br--;
		else{
			wyn+=wyr[i];
			prio[j++]=priority_(wyr[i])+10*br;
		}
	}
	///cout<<"wynik: "<<endl;\
	cout<<wyn<<endl;
	///for(int i=0; i<j; i++)	cout<<prio[i]<<" ";
	return wyn;
}
bool isCorrect(string first, string wyr){
    int dlf, dlw;
    dlf=first.size(); dlw=wyr.size();
    bool letter=false; //czy poprzedni wyraz byl literą
    int brackets=0;
    for(int i=0; i<dlf; i++){
        if(isOpening(first[i])) brackets++;
        else if(isClosing(first[i])) brackets--;
        if(brackets<0) return false;
    }
    if(brackets!=0) return false;
///cout<<"tak"<<endl;\
cout<<wyr<<endl;
    for(int i=0; i<dlw; i++){
        if(!letter){
            if(isalpha(wyr[i])) letter=true;
            else return false;
        }
        else{ //była litera na poprzedniej pozycji
            if(isASign(wyr[i])) letter=false;
            else return false;
        }
    }
    if(letter==false) return false;
    return true;
}
string pop_(stack<string>& st){
    string a="";
    if(!st.empty()){
        a=st.top();
        st.pop();
    }
    return a;
}
long double simplify(long double a, long double b, char wyr){
    if(wyr=='+') return a+b;
    if(wyr=='-') return a-b;
    if(wyr=='*') return a*b;
    if(wyr=='/') return a/b;
    if(wyr=='^') return pow(a,b);
    return -1.0;
}

int main(){
    string first,wyr; stack<char> op; //operatory
    stack <int> pr; //priorytety operatorów
    cout<<"Podaj liczbe wyrazen: ";
    int a; cin>>a;
    while (a-->0){
        cout<<"Podaj wyrazenie:";
        cin>>first;
        ///cout<<first<<endl;
        int dl=first.size();
        int *prio=new int[dl]; //priorytety operatorow
        for(int i=0; i<dl; i++)
            prio[i]=-1;
        wyr=change(first,prio);

        if(!isCorrect(first,wyr)){
         /// gdy Opening, dodajemy 1, else odejmujemy, gdy rozne od 0 na koncu lub w jakimkolwiek momencie mniejsze, niepoprawne <3
//jest niepoprawne, gdy: nawiasy lub jest wiecej niz jeden znak lub litera pod rzad, sa cyfry, zaczyna lub konczy sie na znaku,
            cout<<"Wyrazenie nie jest poprawne, sprobuj jeszcze raz. ";
            continue;
        }
        else{
            string wyn="";
            int i=0;
            int dl=wyr.size();
            wyn+=wyr[i++]; //wrzucamy pierwszą literkę i jestesmy na znaku pomiedzy literkami

            //nawias zwieksza priorytet o 10 np.
//a jakby wrzucac operatory na stos i dopoki i+2<dl, to przepisujemy literki i jesli stos niepusty, wrzucamy na stos operatory i dopoki priorytet op.top() >= priority_(wyr[i+2]),
//to sciagamy ze stosu operatory, chyba ze jest to potegowanie (kilka razy pod rzad), to wtedy nie sciagamy
//po zak. while'a robimy if-a tego, co w while'u, osotani operator dopisujemy do wyniku i sciagamy wszystko

            while(i+2 < dl){
                op.push(wyr[i]);
                pr.push(prio[i]);
                wyn+=wyr[i+1];
///cout<<"PRIORYTET wyrazenia " <<op.top()<< " to "<<pr.top()<<", a wynik to "<<wyn<<endl;

                char top=op.top();
                if( pr.top() > prio[i+2] || (pr.top() == prio[i+2] && top != '^') ){ //sciagamy ze stosu
                    do{
                        top=op.top(); //stos na pewno nie byl pusty, bo coś wrzucilismy na poczatku
                        ///cout<<"TOP:"<<top<<endl;
                        wyn+=top;
                        op.pop(); pr.pop();
                        if(!op.empty())
                        {
                        	///cout<<"niepusty"<<endl;\
                        	cout<<pr.top()<<", "<<prio[i+2]<<endl;
                        	if( pr.top() < prio[i+2] || (pr.top() == prio[i+2] && op.top() == '^')) break;}
                        else break;
                    } while(true);
                }
                ///cout<<"przenosimy się na kolejny operator: "<<endl;
                i+=2;
            }
///cout<<"one more if: "<<wyn<<endl;
            if(i+2==dl){ //odwrócenie warunku z while'a;  sciagamy wszystko i dopisujemy ostatni operator
              //  if( prio.top() <= (priority_(wyr[i])+10*Bracket) && (prio.top() != (priority_(wyr[i])+10*Bracket) || top == '^') ){
              ///wrzucamy na stos ostatni operator i sciagamy wszystko
                if(!op.empty()){
                	char top=op.top();
                	///cout<<"PRIORYTET wyrazenia " <<op.top()<< " to "<<pr.top()<<", a wynik to "<<wyn<<endl;

	                if( pr.top() < prio[i] || (pr.top() == prio[i] && top == '^') ){ //mamy + *, + ^, * ^, ^ ^ etc.
	                    wyn+=wyr[i+1];
	                    op.push(wyr[i]);
	                    pr.push((prio[i]));
	                    ///cout<<"IF: "<<wyn<<endl;
	                    //sciagamy ze stosu
	                }
	                else{ //sciagamy ze stosu, dopoki priorytet operatora jest wiekszy, czyli * /. + - etc., potem dopisujemy do wyniku ostatni operator i sciagamy reszte ze stosu
	                    do{
	                        top=op.top(); //stos na pewno nie byl pusty, bo coś wrzucilismy na poczatku
	                        wyn+=top;
	                        op.pop(); pr.pop();
	                        if(!op.empty())
	                        {if( pr.top() < prio[i] || (pr.top() == prio[i+2] && op.top() == '^')) break;}
	                        else break;
	                    } while(true);
	                    wyn+=wyr[i+1]; //dopisujemy literkę
	                    wyn+=wyr[i]; //oraz znak
	                }
	                while(!op.empty()){
	                    top=op.top(); //stos na pewno nie byl pusty, bo coś wrzucilismy na poczatku
	                    ///cout<<" STOS.TOP(): "<<top<<" ";
	                    wyn+=top;
	                    op.pop(); pr.pop();
	                }
                }
                else{
                	wyn+=wyr[i+1];
                    wyn+=wyr[i];
                }
            }
            ///cout<<endl<<endl;
            cout<<wyn<<endl;

///CZĘŚĆ DRUGA, WYZNACZANIE WARTOŚCI WYRAŻENIA, KORZYSTAJĄC Z KONWERSJI Z RPN NA POSTAĆ INFIKSOWĄ

            stack<long double> st;
            long double a,b;
            long double input;
            cout<<"Podaj wartosci zmiennych: "<<endl;

            for(int i=0; i<dl; i++){
                if(isalpha(wyn[i])){
                    cout<<wyn[i]<<"=";
                    cin>>input;
                    cout<<"input: "<<input<<" ";
                    st.push(input);
                }
                else{ //zakladamy, ze wyrazenie jest poprawne, bo bylo przekonwertowane wczesniej przez moj kalkulator
                cout<<wyn[i]<<" <- wyr ";
                   // if(!st.empty()){
                    b=st.top(); st.pop();

                   // if(!st.empty()){
                    a=st.top(); st.pop();
                   // cout<<"a="<<a<<",b="<<b<<", a na stosie laduje: "<<simplify(a,b,wyn[i])<<endl;
                    st.push(simplify(a,b,wyn[i]));
                }
            }
            cout<<endl;
            cout<<"Wynik wyrazenia " << wyr<< "to: ";
            cout<<st.top()<<endl;
            st.pop();
        }
    }
}
