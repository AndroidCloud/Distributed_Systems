#include <bits/stdc++.h>
using namespace std;

extern "C" void wordcount_reduce(vector <string> data,int reducerNumber)
{

	map <string,int> wordmap;
	int len1 = data.size();
	for(int i=0;i<len1;i++)
	{
		istringstream ss(data[i]);
		string a;int b;
		ss >> a >> b;
		wordmap[a] += b; 
	}

	string file = "ReduceOut_"+to_string(reducerNumber);
	ofstream outfile;
	outfile.open(file.c_str(),std::ios_base::app);
	for (std::map< string,int >::iterator it=wordmap.begin(); it!=wordmap.end(); ++it)
	{
		string word = it->first;
		//cout << word << endl;
		string line = word + " " + to_string(it->second)+"\n";
		outfile << line;
	}
	outfile.close();



}

