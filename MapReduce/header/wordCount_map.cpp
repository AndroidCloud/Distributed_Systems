#include <bits/stdc++.h>
using namespace std;

extern "C" void wordcount_map(char *block,int numReduceTasks)
{

	FILE *fp;
	char c;

	fp = fopen(block, "r");
	map <string,int> wordmap;

	// this while-statement assigns into c, and then checks against EOF:
	string word="";
	while((c = fgetc(fp)) != EOF) {
		if(c==' ' || c=='\n' )
		{
			if(word.size()>0)wordmap[word]++;
			word="";
		}
		else
		word = word+c;
		
		}
		if(word.size()>0)wordmap[word]++;
		fclose(fp);

		for (std::map< string,int >::iterator it=wordmap.begin(); it!=wordmap.end(); ++it)
		{
			string word  = it->first;
			int len = word.size();
			int count=0;
			for(int i=0;i<len;i++)
			{
				if((word[i]>='a' && word[i]<='z') || (word[i]>='A' && word[i]<='Z'))count += word[i];
			}
			int hash = count%numReduceTasks;
			string file = "mapOut_"+to_string(hash+1);
			ofstream outfile;
			outfile.open(file.c_str(),std::ios_base::app);
			string line = word + " " + to_string(it->second)+"\n";
			outfile << line;
			outfile.close();
		}

	

}

