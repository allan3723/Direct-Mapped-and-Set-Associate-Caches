#include <iostream>
using namespace std;
#include <string>

int main()
{
	int address, op, tag, index, offset, tagA[64], dirty[64], valid0[64]= {0}, valid1[64] = {0}, valid2[64] = {0}, valid3[64] = {0};
	string data; 
	string data0[64] = "", data1[64] = "";

	while (cin >> hex >> address >> op)
	{
		cin >> data;
		tag = (address & 65408) >> 7; // first 9 bits
		index = (address & 126) >> 1; // bits 10-15
		offset = address & 1;         // bit 16


		if(offset && op == 255)       // offset 1, write operation
		{
			data1[index] = data;
			dirty[index] = 1;
			tagA[index] = tag;
			valid1[index] = 1;
		}
		else if (!offset && op == 255) // offset 0, write operation
		{
			data0[index] = data;
			dirty[index] = 1;
			tagA[index] = tag;
			valid0[index] = 1;
		}	

		else                            // read operation
		{
			if (tag != tagA[index])
			{
				cout << data << "\t" << 0 << "\t" << 0 <<endl; //miss
				dirty[index] = 1;         // add to cache
				tagA[index] = tag;
				if(offset)
				{
					data1[index] = data;
					valid1[index] = 0;
				}
				else
				{
					data0[index] = data;
					valid0[index] = 0;
				}

			}	
			else if (offset && tag == tagA[index]) // hit
			{	
				if(valid1[index])
					cout <<data1[index] << "\t" << 1 << "\t"  << 1 << endl;
				else
					cout <<data << "\t" << 1 << "\t"  << 0 << endl;
			}
			else if (!offset && tag == tagA[index])	
			{
				if(valid0[index])
					cout <<data0[index] << "\t" << 1 << "\t"  << 1 << endl;
				else
					cout <<data << "\t" << 1 << "\t"  << 0 << endl;
			}
		}
	} //while
} //main()
