#include "lodepng.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "assert.h"
typedef struct image image;
typedef struct image_RGB image_RGB;
typedef struct Node node;
typedef struct liste liste;
image LireImage(const  char*);

 struct image{
	int height;
	int width;
	unsigned char **canalRouge;
};

struct image_RGB{
	int height;
	int width;
	unsigned char **canalRouge[3];

};
struct liste{
	uint16_t len;
	node *heads;
	node *tai;
};
struct Node
{
	int points[2];
	struct Node *suivant;
};
typedef struct
{
	liste **tab;
	uint32_t len;
}Tableau2D;
/*-------------------------------------Première étape : ouvrir une image et lire les pixels--------------------------------*/
//LireImage(unsigned char*)
image LireImage(const char* fileName)
{
	unsigned erreur,width,height,i,j,k;
	image outter_pic;
    unsigned char *pic;
	erreur= lodepng_decode32_file(&pic, &width, &height ,fileName);
	if(erreur)
	{
		fprintf(stderr,"%s",fileName);
	}
	outter_pic.width= width;
	outter_pic.height= height;
	outter_pic.canalRouge = malloc(height * sizeof(unsigned char*));
	if(outter_pic.canalRouge == NULL)
	{ 
    assert(0); printf("Verify Assert Function");
	}

	for (i=0;i<height;i++)
	{
		outter_pic.canalRouge[i]=malloc(width * sizeof(unsigned char));

		if(outter_pic.canalRouge[i] == NULL)
		{
		  assert(0); printf("Verify Assert Function");
		}
	}
	k=0;
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			outter_pic.canalRouge[i][j]=pic[k];
			k=k+4;
		}
	}

  free(pic);
  return outter_pic;

	}
// Ecrire Image
void EcrireImage(image im,const char* fileName){

	 int i,k,error;
	 unsigned char *pic;
	 pic = malloc(4*im.height*im.width*sizeof(unsigned char));
		if(!pic){ 
			assert(0); printf("Verify Assert Function");
		}
		for(i=0;i<im.height;i++)
		{
			for (k= 0; k<im.width; k++) {
				pic[4*im.width*i+4*k]=im.canalRouge[i][k];//k and increment for the next iteration
			    	pic[4*im.width*i+4*k+1]=im.canalRouge[i][k]; 
				    pic[4*im.width*i+4*k+2]=im.canalRouge[i][k];
			    pic[4*im.width*i+4*k+3]=255;
			}
		}
		error= lodepng_encode32_file(fileName,pic,im.width,im.height);
   if(error)
   {
   fprintf(stderr,"%s \n",fileName);
	assert(0); 
	printf("Verify Assert Function"); 
   }
		free(pic);
}

void EcrireImageCouleur(image_RGB coloree,const char* fileName){
	 int k,error,i;
	 unsigned char *pic;
	 pic = malloc(4*coloree.height*coloree.width*sizeof(unsigned char));
		if(!pic){
			assert(0); printf("Verify Assert Function");
		}
		for(i=0;i<coloree.height;i++)
		{
			for (k= 0; k<coloree.width; k++) {
				pic[4*coloree.width*i+4*k+0]=coloree.canalRouge[0][i][k];
				pic[4*coloree.width*i+4*k+1]=coloree.canalRouge[1][i][k];
				pic[4*coloree.width*i+4*k+2]=coloree.canalRouge[2][i][k];
				pic[4*coloree.width*i+4*k+3]=255;
			}
		}
		error= lodepng_encode32_file(fileName,pic,coloree.width,coloree.height);
   if(error)
   {
				fprintf(stderr,"%s \n",fileName);
				assert(0); 
				printf("Verify Assert Function"); 
	}
	free(pic);
}

//AllouerImage(hauteur,largeur)
image AllouerImage(int height,int width){

    int i,j;
    image outter_pic; 

	outter_pic.width= width;
	outter_pic.height= height;

  	outter_pic.canalRouge = malloc(height * sizeof(unsigned char*));
  	if(outter_pic.canalRouge == NULL)
  	{
			assert(0);
			printf("Verify Assert Function");
  	}
  
  	for (i=0;i<height;i++)
  	{
  		outter_pic.canalRouge[i]=malloc(width * sizeof(unsigned char));
  
  		if(outter_pic.canalRouge[i] == NULL)
  		{	 
			assert(0);
			printf("Verify Assert Function");
  		}
  	}
  	for(i=0;i<height;i++)
  	{
  		for(j=0;j<width;j++)
  		{
  			outter_pic.canalRouge[i][j]=0;
  		}
  	}
    return outter_pic;

}

void LibererImage(image im)
{
	int i;
	for(i=0;i<im.width;i++)
	{
		free(im.canalRouge[i]);
	}
	free(im.canalRouge);
}

unsigned char Maximum(int *values ,int len)
{
	int i;
	unsigned char max;
	
	max= values[0];
	
	for(i=1;i<len;i++)
	{
		if(values[i]>max && values[i]!=256)
		max =(unsigned char) values[i];
	}
	return max;
}
unsigned char Minimum(int *values,int len )
{
	int i;
	unsigned char min;
	
	min=values[0];
	
	for(i=1;i<len;i++)
	{
		if(values[i]<min && values[i]!=256)
		{
			min=(unsigned char) values[i];
		}
	}
	return min;
}
void initializeWith(int *table,int t,int init_value){
	int i;
	for(i=0;i<t;i++)
	{
		table[i]=init_value;
	}
}

 image CalculerGradient(image im,int r)
 {
 	int i,j,height,width,counter,m,p,n,q,k,l;
 	image gradient;
 	int tau=((r*2)+1)*((r*2)+1);
 	int canalRouge[tau];
 	width=im.width;
 	height=im.height;
 	gradient=AllouerImage(height,width);
 	
 	for(i=0;i<height;i++)
 	{
 		for(j=0;j<width;j++)
		 {
				initializeWith(canalRouge,tau,256);
				 counter=0;  
			   	 m = i-r;
				 if(m < 0)
				 m = 0; 
				 n = i+r;
				 if(n>height-1)
				 n = height-1; 
	  			 p = j-r;
			     if(p < 0)
			     p = 0; 
                 q = j+r;
				 if(q > width-1)
				 q = width-1;

 			for(k=m;k<=n;k++)
		 	{
		 		for(l=p;l<=q;l++)
		 		{
		 			canalRouge[counter++]=(int) im.canalRouge[k][l];
				 }
			 }
			 gradient.canalRouge[i][j]=Maximum(canalRouge,tau)-Minimum(canalRouge,tau);
		 }}
	return gradient;
 }
 
liste *new_liste(){
 	liste *r = malloc(sizeof(liste));
 	if(r==NULL){
	 	assert(0); printf("Verify Assert Function");
	 }
	 r->len=0;
	 r->heads= NULL;
	 r->tai=NULL;
	 return r;
 }

node *newNode(int i,int j){
 	node *m = malloc(sizeof(node));
 	if(m==NULL){
 		assert(0); printf("Verify Assert Function");
	 }
	 m->points[0]=i;
	 m->points[1]=j; 
	 return m;
 }
void ajouter(liste *l,int i, int j){
 	node *m=newNode(i,j);
 	m->suivant= NULL;
 	if(l->len > 0){
 		l->tai->suivant=m;
	 }
	 else
	 {
	 	l->heads=m;
	 }
	 l->tai = m;
	 (l->len)++;
 }
void retirer(liste *l,int *points){
 	node *t=l->heads;
 	points[0]=t->points[0];
 	points[1]=t->points[1];
 	l->heads = l->heads->suivant;
 	free(t);
 	(l->len)--;
 	if(l->len == 0){l->tai = NULL;}
 }
Tableau2D* new_Tableau2D(uint32_t len){
 	int i;
 	Tableau2D *th= malloc(sizeof(Tableau2D));
 	assert(th !=NULL);
 	
 	th->tab = malloc(len*sizeof(liste*));
 	assert(th->tab !=NULL);
 	
 	for(i=0;i<len;i=i+1)
 	{
 		th->tab[i]=new_liste();
	 }
	th->len = len;
	return th;
 }
 void insert_to_2D(Tableau2D *th,int i,int j,int p){
 	ajouter(th->tab[p],i,j);
 }
//void LPE(Gradient,M)
 void LPE(image Gradient,image M){
 	int i,j,counter=0,min=256,k,l,m,p,n,q;
 	int points[2]={0,0};
 	Tableau2D *L=new_Tableau2D(256); //les values de gradient sont entre 0 et 255
 	for(i=0;i<M.height;i++)
 	{
 		for(j=0;j<M.width;j++)
 		{
 			if(M.canalRouge[i][j]!=0)
 			{
 				insert_to_2D(L,i,j,Gradient.canalRouge[i][j]);
 				counter++;
 				if(min>Gradient.canalRouge[i][j])
 				{
 					min=Gradient.canalRouge[i][j];
				 }
			 }
		 }
	 }
	 while(counter!=0){
	 	while(L->tab[min]->len==0){
	 		min=min+1;
		 }
	 			retirer(L->tab[min],points);
				i=points[0];
				j=points[1];
				counter--;
				m = i-1;
				if(m<0)
				m = 0; 
				n = i+1;
				if(n>M.height-1)
				n = M.height-1; 
				p =j-1;
				if(p<0)
				p = 0; 
				q = j+1;
				if(q>M.width-1)
				q = M.width-1;    
				for(k=m;k<=n;k++)
				{
					for(l=p;l<=q;l++){
						if(M.canalRouge[k][l]==0){
							 M.canalRouge[k][l]=M.canalRouge[i][j];
							 insert_to_2D(L,k,l,Gradient.canalRouge[k][l]);
							 counter++;
						if(min>Gradient.canalRouge[k][l]){
								min=Gradient.canalRouge[k][l];
							}
						}
					}
				}
	 }
 }

image_RGB identify_object(image Grad_niveau_gris,image LPE)
{
	//What a complexity :D :D
    int i,j;
    image gradient_LPE;
    image_RGB coloree;
	int height = Grad_niveau_gris.height;
	int width = Grad_niveau_gris.width;
    image_RGB  image_to_Color;

	 image_to_Color.width= width;
	 image_to_Color.height= height;

    for (int c = 0; c < 3; c++)
    {
         image_to_Color.canalRouge[c] = malloc(height * sizeof(unsigned char*));
        if( image_to_Color.canalRouge[c]== NULL)
        { 
            assert(0); printf("Verify Assert Function");
        }
    
        for (i=0;i<height;i++)
        {
             image_to_Color.canalRouge[c][i]=malloc(width * sizeof(unsigned char));
    
            if( image_to_Color.canalRouge[c][i] == NULL)
            {
            assert(0); printf("Verify Assert Function");
            }
        }
        for(i=0;i<height;i++)
        {
            for(j=0;j<width;j++)
            {
                image_to_Color.canalRouge[c][i][j]=0;
            }
        }
    }  
    for (int c = 0; c < 3; c++)
    {
        for(i=0;i<Grad_niveau_gris.height;i++)
        {
            for(j=0;j<Grad_niveau_gris.width;j++)
            {
                image_to_Color.canalRouge[c][i][j] = Grad_niveau_gris.canalRouge[i][j];
            }
        }
    }

    coloree = image_to_Color; 
    gradient_LPE=CalculerGradient(LPE,1);

    for(i=0;i<Grad_niveau_gris.height;i++)
    {
        for(j=0;j<Grad_niveau_gris.width;j++)
        {
            if(gradient_LPE.canalRouge[i][j]!=0)
            {
                //Mettre le canal rouge Ã  255 et les autres Ã  0
                coloree.canalRouge[0][i][j] = 230;
                coloree.canalRouge[1][i][j] = 0;
                coloree.canalRouge[2][i][j] = 0;
			}
		}
	}
    return coloree;
}
int main(int argc,char ** argv){
  	image image1,Gradient,marqueur;
    image_RGB imagefinale;
    //const char src[] = "./fichiers/Images/";
    image1 = LireImage(argv[1]);   
    marqueur = LireImage(argv[2]);
    Gradient=CalculerGradient(image1,1);
    LPE(Gradient,marqueur);
	imagefinale = identify_object(image1,marqueur); 
     EcrireImageCouleur(imagefinale,argv[3]);
    return 0;
} 	