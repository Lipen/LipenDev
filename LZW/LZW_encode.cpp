#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <string>

#define VI vector<int>
#define VC vector<char>
#define VVI vector<VI>
#define FOR(i, n) for(int i = 0; i < (n); i++)
#define FORS(i, si, n) for(int i = (si); i < (n); i++)
#define FORE(i, n) for(int i = 0; i <= (n); i++)
#define FORES(i, si, n) for(int i = (si); i <= (n); i++)
#define pb push_back

using namespace std;

int n,m,x,a,b,c,d;

int main()
{
	ifstream f("LZW_toEncode.txt");
	VC s;
	char c;

	if (f.is_open()) {
		while (f.get(c)) {
			s.pb(c);
			cout << c;
		}
		f.close();
	} else {
		cout << "Unable to open file" << endl;
	}
	return 0;
}

// int main()
// {
// setlocale(LC_ALL, "Russian");
// ifstream in("C:\\Users\\user\\Desktop\\input.txt");
// ofstream out("C:\\Users\\user\\Desktop\\output.txt");
// if (in.is_open())
// {
// string sim;
// //для считывания из файла пробелов (1 строчка)
// sim.assign((istreambuf_iterator<char>(in.rdbuf())), istreambuf_iterator<char>());
// cout « sim « endl;
// //для опрелелния количества символов в кодируемом тексте (2 строчки)
// basic_string <char>::size_type len;
// len = sim.length();
// cout « len « endl;
// for (int i = 0; i < len; i++){
// cout « sim[i] « " ";
// }
// system("pause");
// in.close();
// out.close();
// }
// else
// cout « "File cannot be opened!" « endl;
// }
