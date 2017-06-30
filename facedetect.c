#include <stdio.h>
#include <stdlib.h>

void read_ppm(char *name, int *width, int *height,unsigned char **pix)
{
	FILE *p;
	int ch;
	int version=0;
	int maxgrey=0;
	int i,k;
	int count=0;
	int header_enter=0;
	int header_space=0;
	*width=0;
	*height=0;
	p=fopen(name,"rb");
	if(p==NULL)
	{
		printf("open file error\n");
		return 1;
	}
	ch=fgetc(p);
	if(ch!='P')
	{
		printf("not ppm file\n");
		return 1;
	}
	ch=fgetc(p);
	version=ch-'0';
	while(ch=fgetc(p))
	{
		if(ch==' ')
		{
			header_space++;
			continue;
		}
		if(ch==10)
		{
			header_enter++;
			if(header_enter==3)
			{
				break;
			}
			continue;
		}
		if(header_space==0)
		{
			*width*=10;
			*width+=ch-'0';
		}
		else if(header_space==1 && header_enter==1)
		{
			*height*=10;
			*height+=ch-'0';
		}
		else if(header_space==1 && header_enter==2)
		{
			maxgrey*=10;
			maxgrey+=ch-'0';
		}
		else
		{
			printf("error parsing\n");
			exit(1);
		}
	}
	printf("width: %d height: %d maxgrey: %d\n",*width,*height,maxgrey);
	//return;
	/*
	pix=(unsigned char**)calloc(*height,sizeof(unsigned char*));
	for(i=0;i<*height;i++)
	{
		pix[i]=(unsigned char*)calloc(*width*3,sizeof(unsigned char));//R G B per pixel 
	}*/
	printf("pix size: %d\n",sizeof(pix));
	i=0;
	k=0;
	while((ch=fgetc(p))!=EOF)
	{
		//printf("char %d\n",ch);
		pix[i][k]=ch%256;//important with mod
		k++;
		if(k==*width*3)
		{
			i++;
			k=0;
		}		
	}
	printf("fclose\n");
	fclose(p);
	printf("pix0 0 %d\n",pix[0][0]);
}

void write_ppm(char *name, int width,int height,unsigned char **pix)
{
	int i,k;
	FILE *p;
	int maxgrey=255;
	p=fopen(name,"wb");// important with binary option
	if(p==NULL)
	{
		printf("open file error\n");
	}
	fprintf(p,"P6\n");
	fprintf(p,"%d %d\n",width,height);
	fprintf(p,"%d\n",maxgrey);
	printf("w h %d %d\n",pix[0][0],pix[0][2]);
	printf("begin write %s ppm file\n",name);	
	for(i=0;i<height;i++)
	{
		for(k=0;k<width*3;k++)
		{
			//printf("%d\n",pix[i][k]);
			fputc(pix[i][k],p);
		}
	}
	fclose(p);
}

void facedetect(unsigned char **pix)
{
	
}

int main(int argc, char **argv)
{
	int width,height;
	int i,k;
	unsigned char **pix=NULL;
	if(argc!=2)
	{
		printf("input error\n");
		printf("please type input file name with ppm type\n");
	}
	pix=(unsigned char**)calloc(480,sizeof(unsigned char*));
	for(i=0;i<480;i++)
	{
		pix[i]=(unsigned char*)calloc(640*3,sizeof(unsigned char));//R G B per pixel 
	}
	read_ppm(argv[1],&width,&height,pix);
	printf("pix0 0 %d\n",pix[0][0]);
	
	//return 0;	
	write_ppm("out.ppm",width,height,pix);
	return 0;
}
