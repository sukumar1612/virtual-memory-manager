#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

int TLB_SIZE=16;
int PAGE_SIZE=256;
int PHYSICAL_MEMORY=256; //number of pages of size PAGE_SIZE (since page_size=frame_size)
int VIRTUAL_MEMORY=256; //size of virtual memory

int masking(int page1)
{
	int page=0;
	for(int i=15;i>=0;i--)
        {
                if((page1 >> i) & 1)
                {
                        page+=pow(2,i);
                }
        }
	return page;
}
int offset(int page1)
{
	int page=0;
        for(int i=7;i>=0;i--)
        {
                if((page1 >> i) & 1)
                {
                        page+=pow(2,i);
                }
        }
        return page;
}
int page_number(int page1)
{
	int page=0;
        for(int i=15;i>=8;i--)
        {
                if((page1 >> i) & 1)
                {
                        page+=pow(2,i-8);
                }
        }
        return page;
}
int caladdr(int page_no,int offset)
{
	int page=0;
        for(int i=7;i>=0;i--)
        {
                if((page_no >> i) & 1)
                {
                        page+=pow(2,i+8);
                }
        }
        for(int i=7;i>=0;i--)
        {
                if((offset >> i) & 1)
                {
                        page+=pow(2,i);
                }
        }
	return page;
}
int main(int argc ,char **argv)
{
	map<int ,vector<int>> tlb;

	int page_table[VIRTUAL_MEMORY]={0};
	int physical_memory[PHYSICAL_MEMORY]={0};

	int i=0; //frame number
	int k=0; //tlb count

	int iter=0; //iteration count

	FILE *fp=fopen(argv[1],"r");

	int page;
	int tlb_hit=0;
	int page_fault=0;

	while(fscanf(fp, "%d", &page)==1)
	{

		page=masking(page); // masking the rightmost 16 bits

		int page_number_var,offset_var;

		page_number_var=page_number(page); //calculate page_number and offset
		offset_var=offset(page);
		cout<<" "<<page_number_var<<" "<<offset_var<<endl;

		int flag=0;

		if(tlb.find(page_number_var)!=tlb.end())
		{
			//tlb hit
			cout<<"hit!!"<<endl;
			tlb_hit++;
			flag=1;
		}
		else if(page_table[page_number_var]==0)
		{
			//page fault sequence
			cout<<"fault"<<endl;
			page_fault++;

			//find page in backing storage

			FILE *fp1=fopen("BACKING_STORE.bin","r");
			int pg=0;
			while(fscanf(fp1,"%d",&pg))
			{
				if(pg==page_number_var)
				{
					cout<<"page found in secondary storage"<<endl;
					break;
				}
			}
			fclose(fp1);
			//bring frame to main memory

			physical_memory[i]=pg;
			page_table[pg]=i;
			i++;
		}
		//print out physical and logical addresses
		cout<<"PHYSICAL ADDRESS :"<< caladdr(page_number_var,offset_var)  <<"\t LOGICAL ADDRESS :"<<caladdr(page_table[page_number_var],offset_var)<<endl;

		if(k<TLB_SIZE && flag==0)
		{
			vector<int> v;
			v.push_back(i-1);
			v.push_back(iter);
			tlb[page_number_var]=v;
			k++;
		}
		else if (flag==0)
		{
			//delete last element FIFO algorithm
			//since addresses are generated randomly we donot expect any memory access locality
			//so tlb hit rate is low
			cout<<"fifo replace-------------"<<endl;
			int min=100000;
			map<int, vector<int> >::iterator mp;
			for (auto i1=tlb.begin();i1!=tlb.end();i1++)
			{
				vector <int> v1=i1->second;
				if(min>v1[1])
				{
					min=v1[1];
					mp=i1;
				}
			}
			tlb.erase(mp);
			//add the new element
			vector<int> v;
                        v.push_back(i-1);
                        v.push_back(iter);
                        tlb[page_number_var]=v;
		}
		iter++;

	}
	cout<<"---------------------------------------------------------------"<<endl;
	cout<<"TLB hit rate :"<<tlb_hit/10<<"%"<<"\nPage-fault rate :"<<page_fault/10<<"%"<<endl;
}

