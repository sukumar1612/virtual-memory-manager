#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

int main()
{
	int size=256*256;
	FILE *fp=fopen("address.txt","w");

	for(int i=0;i<1000;i++)
	{
		fprintf(fp, "%d\n", rand()%size);
	}
	fclose(fp);
}
