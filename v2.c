#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc,char **argv)
{
        int i,j,k;
        char ch;
        char *p,*start,*end,*tag;
        char password[20];
        FILE *sf,*df;

        if(argc!=3)
        {
                printf("USAGE: ./xsecure encrypt/decrypt fileName\n");
                return 1;
        }

        if(!strcmp(argv[1],"encrypt"))
        {
                printf("ENTER THE PASSWORD TO ENCRYPT\n");
                scanf("%19s",password);

                sf=fopen(argv[2],"r");

                if(sf==0)
                {
                        printf("FILE IS NOT PRESENT\n");
                        return 1;
                }

                i=0;

                while((ch=fgetc(sf))!=EOF)
                        i++;

                p=malloc(i+strlen("passwordisthis\"")+strlen(password)+2);

                if(p==0)
                {
                        fclose(sf);
                        return 1;
                }

                rewind(sf);

                j=0;

                while((ch=fgetc(sf))!=EOF)
                {
                        p[j]=ch^25;
                        j++;
                }

                p[j]='\0';

                if(strstr(p,"passwordisthis\"")!=0)
                {
                        printf("FILE IS ALREADY ENCRYPTED\n");
                        free(p);
                        fclose(sf);
                        return 1;
                }

                start="passwordisthis\"";

                while(*start)
                {
                        p[j]=*start;
                        j++;
                        start++;
                }

                start=password;

                while(*start)
                {
                        p[j]=*start;
                        j++;
                        start++;
                }

                p[j]='"';
                j++;
                p[j]='\0';

                fclose(sf);

                df=fopen(argv[2],"w");

                if(df==0)
                {
                        free(p);
                        return 1;
                }

                fputs(p,df);

                fclose(df);
                free(p);

                printf("\nSUCCESS\n");
        }

        else if(!strcmp(argv[1],"decrypt"))
        {
                sf=fopen(argv[2],"r");

                if(sf==0)
                {
                        printf("FILE IS NOT PRESENT\n");
                        return 1;
                }

                i=0;

                while((ch=fgetc(sf))!=EOF)
                        i++;

                p=malloc(i+1);

                if(p==0)
                {
                        fclose(sf);
                        return 1;
                }

                rewind(sf);

                j=0;

                while((ch=fgetc(sf))!=EOF)
                {
                        p[j]=ch;
                        j++;
                }

                p[j]='\0';

                fclose(sf);

                tag=strstr(p,"passwordisthis\"");

                if(tag==0)
                {
                        printf("FILE IS NOT ENCRYPTED\n");
                        free(p);
                        return 1;
                }

                tag=tag+strlen("passwordisthis\"");

                end=strchr(tag,'"');

                if(end==0)
                {
                        printf("INVALID ENCRYPTED FILE\n");
                        free(p);
                        return 1;
                }

                *end='\0';

                printf("ENTER THE PASSWORD TO DECRYPT\n");

                for(k=0;k<3;k++)
                {
                        scanf("%19s",password);

                        if(!strcmp(password,tag))
                        {
                                j=tag-p-strlen("passwordisthis\"");

                                for(i=0;i<j;i++)
                                        p[i]=p[i]^25;

                                p[j]='\0';

                                df=fopen(argv[2],"w");

                                if(df==0)
                                {
                                        free(p);
                                        return 1;
                                }

                                fputs(p,df);

                                fclose(df);
                                free(p);

                                printf("\nDECRYPTION SUCCESS\n");
                                return 0;
                        }

                        printf("WRONG PASSWORD\n");

                        if(k<2)
                                printf("ATTEMPTS LEFT: %d\n",2-k);
                }

                printf("3 WRONG ATTEMPTS\n");
                printf("DELETING FILE...\n");

                free(p);
                remove(argv[2]);

                printf("FILE DELETED\n");
        }

        else
        {
                printf("INVALID OPTION\n");
        }

        return 0;
}
