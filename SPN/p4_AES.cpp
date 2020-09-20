#include<cstdio>
#include<cstring>
using namespace std;
unsigned int S_Box[16][16] = { 
  {0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76}, 
  {0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0}, 
  {0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15}, 
  {0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75}, 
  {0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84}, 
  {0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF}, 
  {0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8}, 
  {0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2}, 
  {0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73}, 
  {0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB}, 
  {0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79}, 
  {0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08}, 
  {0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A}, 
  {0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E}, 
  {0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF}, 
  {0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16} 
};
unsigned int Rcon[11]={0x00000000,0x01000000,0x02000000,0x04000000,0x08000000,0x10000000,
					   0x20000000,0x40000000,0x80000000,0x1B000000,0x36000000};
static const int colM[4][4]=
{2,3,1,1,
 1,2,3,1,
 1,1,2,3,
 3,1,1,2
};
int Nr=10;
unsigned int state[4][4],state_[4][4],w[1001];
unsigned char inp[16],oup[16];
int num=1048576;
inline void Subbytes()
{
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
			state[i][j]=S_Box[state[i][j]/16][state[i][j]%16];
	}
	return;
}
inline void Shiftrows()
{
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
			state_[i][j]=state[i][(j+i)%4];
	}
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
			state[i][j]=state_[i][j];
	}
	return;
}
void cal_key()
{
	unsigned int l,li,lii[4];
	for(int j=1;j<=Nr;j++)
	{
		l=w[4*j-1];
		li=(l<<24)>>24;
		l>>=8;
		l|=li<<24;
		li=l;
		for(int i=0;i<4;i++)
		{
			lii[i]=li&0x000000ff;
			lii[i]=S_Box[lii[i]/16][lii[i]%16];
			li>>=8;
		}
		l=0;
		for(int i=3;i>=0;i--)
		{
			l<<=8;
			l|=lii[i];
		}
		l^=Rcon[j];
		w[4*j]=w[4*j-4]^l;
		w[4*j+1]=w[4*j]^w[4*j-3];
		w[4*j+2]=w[4*j+1]^w[4*j-2];
		w[4*j+3]=w[4*j+2]^w[4*j-1];
	}
	return;
}
void print()
{
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			printf("%02x ",state[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
inline void Addroundkey(int round)
{
	unsigned int l,li;
	for(int j=0;j<4;j++)
	{
		l=w[round*4+j];
		for(int i=0;i<4;i++)
		{
			li=l&0x000000ff;
			l>>=8;
			state[i][j]^=li;
		}
	}
	return;
}
unsigned int GFMul2(unsigned int s)
{
    int result=s<<1;
    int a7=result&0x00000100;
    if(a7!=0)
	{
		result=result&0x000000ff;
		result=result^0x1b;
	}
	return result;
}
unsigned int GFMul(int n,unsigned int s)
{
	unsigned int result;
    if(n==1)
		result=s;
	else if(n==2)
        result=GFMul2(s);
    else if(n==3)
        result=GFMul2(s)^s;
    return result;
}
void mixColumns()
{
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            state_[i][j]=state[i][j];

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
		{
			state[i][j]=GFMul(colM[i][0],state_[0][j])^GFMul(colM[i][1],state_[1][j])
				^GFMul(colM[i][2],state_[2][j])^GFMul(colM[i][3], state_[3][j]);
        }
}
//#define USE_FILE
int main()
{
#ifdef USE_FILE
	FILE* f,*f_;
	unsigned char l;
	f=fopen("spn4.in","rb");
	f_=fopen("spn4.out","wb");
	for(int i=0;i<4;i++)
	{
		w[i]=0;
		for(int j=0;j<4;j++)
		{
			fread(&l,1,1,f);
			w[i]<<=8;
			w[i]|=l;
		}
	}
	cal_key();
	/*for(int i=0;i<=43;i++)
	{
		printf("w[%d]=%x\n",i,w[i]);
	}*/
	memset(oup,0,sizeof(oup));
	while(num--)
	{
		fread(inp,1,16,f);
		for(int j=0;j<4;j++)
		{
			for(int i=0;i<4;i++)
			{
				state[i][j]=inp[4*j+i];
				state[i][j]^=oup[4*j+i];
			}
		}
		//print();
		Addroundkey(0);
		//print();
		for(int i=1;i<=Nr-1;i++)
		{
			Subbytes();
			//print();
			Shiftrows();
			//print();
			mixColumns();
			Addroundkey(i);
		}
		Subbytes();
		Shiftrows();
		Addroundkey(Nr);
		//print();
		for(int j=0;j<4;j++)
		{
			for(int i=0;i<4;i++)
			{
				l=state[i][j];
				oup[4*j+i]=l;
				fwrite(&l,1,1,f_);
			}
		}
	}
#else
	unsigned char l;
	for(int i=0;i<4;i++)
	{
		w[i]=0;
		for(int j=0;j<4;j++)
		{
			fread(&l,1,1,stdin);
			w[i]<<=8;
			w[i]|=l;
		}
	}
	cal_key();
	memset(oup,0,sizeof(oup));
	while(num--)
	{
		fread(inp,1,16,stdin);
		for(int j=0;j<4;j++)
		{
			for(int i=0;i<4;i++)
			{
				state[i][j]=inp[4*j+i];
				state[i][j]^=oup[4*j+i];
			}
		}
		Addroundkey(0);
		//print();
		for(int i=1;i<=Nr-1;i++)
		{
			Subbytes();
			//print();
			Shiftrows();
			//print();
			mixColumns();
			Addroundkey(i);
		}
		Subbytes();
		Shiftrows();
		Addroundkey(Nr);
		for(int j=0;j<4;j++)
		{
			for(int i=0;i<4;i++)
			{
				l=state[i][j];
				oup[4*j+i]=l;
				fwrite(&l,1,1,stdout);
			}
		}
	}
#endif
	return 0;
}