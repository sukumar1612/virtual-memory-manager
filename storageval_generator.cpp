#include <bits/stdc++.h>
#include <stdio.h>


using namespace std;

int main()
{
	int size=256;
	FILE *fp=fopen("BACKING_STORE.bin","w");
	for(int i=0;i<256;i++)
	{
		fprintf(fp,"%d\n",i);
	}
}
