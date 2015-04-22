#include <iostream>
using namespace std;
#include <string>

void updateLRU(int, int, int*);

int main()
{
	int address, op, tag, index, offset, tagA[24], dirty[24], valid0[24]= {0}, 
  valid1[24] = {0}, i, LRU[24];
	string data; 
	string data0[24] = "", data1[24] = "";

  for (i = 0; i < 24; i+=3) //Inits LRU, 0 = LRU while 2 = MRU
  {
    LRU[i] = 0;
    LRU[i+1] = 1;
    LRU[i+2] = 2;
    tagA[i] = -1;
    tagA[i+1] = -1;
    tagA[i+2] = -1; 
  }

	while (cin >> hex >> address >> op)
	{
		cin >> data;
		tag = (address & 65520) >> 4; //first 12 bits
		index = (address & 14) >> 1;  // bits 13-15
		offset = address & 1;         // bit 16

    index *= 3; //3 way set associative

		if(offset && op == 255) //write operation with offset 1
		{
/*      for (i = 0; i < 3; i++) //scan through set
        if (valid1[index + i] == 0)
          break;
      if (i == 3) //if all block used */
        for (i = 0; i < 3; i++) 
          if (LRU[index + i] == 0) //find LRU block
            break;
      
      updateLRU(i, index, LRU);  
			data1[index + i] = data; // add to cache
			dirty[index + i] = 1;
			tagA[index + i] = tag;
			valid1[index + i] = 1;
		}
		else if (!offset && op == 255) //offset 0, write operation
		{
/*      for (i = 0 ; i < 3; i++) //scan through set
        if (valid0[index + i] == 0)
          break;
      if (i == 3) //no empty block; 
*/        for (i = 0; i < 3; i++)
          if (LRU[index + i] == 0) //find LRU block
            break;

      updateLRU(i, index, LRU);
			data0[index + i] = data;
			dirty[index + i] = 1;
			tagA[index + i] = tag;
			valid0[index + i] = 1;
		}	

		else //read operation
		{
			if (tag != tagA[index] && tag != tagA[index+1] && tag != tagA[index+2]) 
         //unable to find in cache
			{
				cout << data << "\t" << 0 << "\t" << 0 <<endl;
        for (i = 0; i < 3; i++)
          if (LRU[index + i] == 0)
            break;

        updateLRU(i, index, LRU); //written to block so update LRU
				dirty[index + i] = 1; 
				tagA[index + i] = tag;

				if(offset)
				{
					data1[index + i] = data;
					valid1[index + i] = 0;
				}
				else
				{
					data0[index + i] = data;
					valid0[index + i] = 0;
				}

			}	

			else if (offset) //should be in cache in one of the offset
			{	
        for (i = 0; i < 3; i++) //search through set for it
          if (tag == tagA[index + i])
            break;

        updateLRU(i, index, LRU); //reference to block so update LRU

				if(valid1[index + i])
{
					cout <<data1[index + i] << "\t" << 1 << "\t"  << 1<< endl;
}
				else
        {
					cout <<data << "\t" << 1 << "\t"  << 0 << endl;
        }
			}
			else if (!offset)	
			{
        for (i = 0; i < 3; i++)
          if (tag == tagA[index + i])
            break;

        updateLRU(i, index, LRU); //ref to block so update LRU
				if(valid0[index + i])
				{
					cout <<data0[index + i] << "\t" << 1 << "\t"  << 1 << endl;
				}
				else
					cout <<data << "\t" << 1 << "\t"  << 0 << endl; 
			}
			
		}
	} //while
} //main()

void updateLRU(int block, int index, int* LRU)
{
  if (LRU[index + block] == 2) //do not need to update since already MRU
    return;

  if (block == 0)
  { 
    LRU[index] = 2;
    if (LRU[index+1] != 0)
      LRU[index+1]--;
    if (LRU[index+2] != 0)
      LRU[index+2]--;
  }

  else if (block == 1)
  { 
    LRU[index+1] = 2;
    if (LRU[index] != 0)
      LRU[index]--;
    if (LRU[index+2] != 0)
      LRU[index+2]--;
  }

  else //block 2
  {
    LRU[index+2] = 2;
    if (LRU[index] != 0)
      LRU[index]--;
    if (LRU[index+1] != 0)
      LRU[index+1]--;
  }
}
