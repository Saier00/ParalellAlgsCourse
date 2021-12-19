#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;
int main(int argc, char* argv[])
{
    string inp=argv[1];

    if(inp.length()==0)
    {
        cout << "Number of words is " << 0;
        return 0;
    }

    int wordsNum=0;
    bool isWord=false;
    for(int i = 0; i<inp.length();i++)
    {
        if(inp[i]==' '||inp[i]=='\t'||inp[i]=='\n')
        {
            isWord=false;
        }
        else if(!isWord)
        {
            wordsNum++;
            isWord=true;
        }
    }


    cout << "Number of words is " << wordsNum;
    return wordsNum;
}