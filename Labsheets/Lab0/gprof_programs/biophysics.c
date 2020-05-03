
#include <stdio.h>
#include <math.h>



float bondlength(float x[], float y[])
{
float temp;
temp=(float)sqrt(((x[0]-y[0])*(x[0]-y[0])) + ((x[1]-y[1])*(x[1]-y[1])) + ((x[2]-y[2])*(x[2]-y[2])));
return temp;
}

float bondangle(float x[], float y[], float z[])
{
float a, b, c, angle;
a=bondlength(x, y);
b=bondlength(y, z);
c=bondlength(z, x);
angle = (float) (180/3.14)*acos(((a*a) + (c*c) -(b*b))/(2*a*c));
return angle;
}

float torsionangle(float x[], float y[], float z[], float w[])
{
float c, c1, c2, c3, c4, T_angle, a[3], b[3], temp1[3], temp2[3];

a[0]=y[0]-x[0];
a[1]=y[1]-x[1];
a[2]=y[2]-x[2];
b[0]=y[0]-z[0];
b[1]=y[1]-z[1];
b[2]=y[2]-z[2];

temp1[0]=a[1]*b[2] - a[2]*b[1];
temp1[1]=a[2]*b[0] - a[0]*b[2];
temp1[2]=a[0]*b[1] - a[1]*b[0];

a[0]=z[0]-y[0];
a[1]=z[1]-y[1];
a[2]=z[2]-y[2];
b[0]=z[0]-w[0];
b[1]=z[1]-w[1];
b[2]=z[2]-w[2];

temp2[0]=a[1]*b[2] - a[2]*b[1];
temp2[1]=a[2]*b[0] - a[0]*b[2];
temp2[2]=a[0]*b[1] - a[1]*b[0];

c1=((temp1[0]*temp2[0]) + (temp1[1]*temp2[1]) + (temp1[2]*temp2[2]));
c2= ((temp1[0])*(temp1[0])) + ((temp1[1])*(temp1[1])) + ((temp1[2])*(temp1[2]));
c3= ((temp2[0])*(temp2[0])) + ((temp2[1])*(temp2[1])) + ((temp2[2])*(temp2[2]));
c4= sqrt(c2)*sqrt(c3);
c=c1/c4;
T_angle=(180/3.14)*acos(c);
return(T_angle);
}


int main()
{
int choice=0;
float length, angle, torsion, first[3], second[3], third[3], fourth[3];
do
{
printf("\n \n What do you need to calculate? \n 0. Exit \n 1. Bond length \n 2. Bond angle \n 3. Torsion angle \n \n ");
scanf("%d",&choice);

if(choice==1)
{
printf("For the first atom \n");

printf("Enter the X coordinate in nm scale \n ");
scanf("%f",&first[0]);
printf("Enter the Y coordinate in nm scale \n ");
scanf("%f",&first[1]);
printf("Enter the Z coordinate in nm scale \n ");
scanf("%f",&first[2]);

printf("For the second atom \n");

printf("Enter the X coordinate in nm scale \n ");
scanf("%f",&second[0]);
printf("Enter the Y coordinate in nm scale \n ");
scanf("%f",&second[1]);
printf("Enter the Z coordinate in nm scale \n ");
scanf("%f",&second[2]);

length=bondlength(first, second);
printf("The bondlength is %f nm \n ", length);
}

else if(choice==2)
{
printf("For the first atom \n");

printf("Enter the X coordinate in nm scale \n ");
scanf("%f",&first[0]);
printf("Enter the Y coordinate in nm scale \n ");
scanf("%f",&first[1]);
printf("Enter the Z coordinate in nm scale \n ");
scanf("%f",&first[2]);

printf("For the second atom \n");

printf("Enter the X coordinate in nm scale \n ");
scanf("%f",&second[0]);
printf("Enter the Y coordinate in nm scale \n ");
scanf("%f",&second[1]);
printf("Enter the Z coordinate in nm scale \n ");
scanf("%f",&second[2]);

printf("For the third atom \n");

printf("Enter the X coordinate in nm scale \n ");
scanf("%f",&third[0]);
printf("Enter the Y coordinate in nm scale \n ");
scanf("%f",&third[1]);
printf("Enter the Z coordinate in nm scale \n ");
scanf("%f",&third[2]);

angle=bondangle(first, second, third);
printf("The bond angle is %f degrees \n ", angle);
}

else if(choice==3)
{
printf("For the first atom \n");

printf("Enter the X coordinate in nm scale \n ");
scanf("%f",&first[0]);
printf("Enter the Y coordinate in nm scale \n ");
scanf("%f",&first[1]);
printf("Enter the Z coordinate in nm scale \n ");
scanf("%f",&first[2]);

printf("For the second atom \n");

printf("Enter the X coordinate in nm scale \n ");
scanf("%f",&second[0]);
printf("Enter the Y coordinate in nm scale \n ");
scanf("%f",&second[1]);
printf("Enter the Z coordinate in nm scale \n ");
scanf("%f",&second[2]);

printf("For the third atom \n");

printf("Enter the X coordinate in nm scale \n ");
scanf("%f",&third[0]);
printf("Enter the Y coordinate in nm scale \n ");
scanf("%f",&third[1]);
printf("Enter the Z coordinate in nm scale \n ");
scanf("%f",&third[2]);

printf("For the fourth atom \n");

printf("Enter the X coordinate in nm scale \n ");
scanf("%f",&fourth[0]);
printf("Enter the Y coordinate in nm scale \n ");
scanf("%f",&fourth[1]);
printf("Enter the Z coordinate in nm scale \n ");
scanf("%f",&fourth[2]);

torsion=torsionangle(first, second, third, fourth);
printf("The torsion angle is %f degrees \n ", torsion);
}

else
{
    if(choice!=0)
    {
                 printf("Invalid selection! Try again \n");
                 }
}

}while(choice!=0);

printf("Have a nice day!");
}
