
#include <string.h>
#include <stdlib.h>
#include <expat.h>
#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include "plugin.h"



char *tolowerstr(const char *instr)
{
    char *outstr;
    int i;
    outstr=(char*)calloc(strlen(instr)+1,sizeof(char));
    for(i=0;i<strlen(instr);i++)
    {
        outstr[i]=tolower(instr[i]);
        
        
    }
    outstr[i]='\0';
    return outstr;
    
}
int authorflag;
int titleflag;
int titleinfoflag;
int firstnameflag;
int middlenameflag;
int lastnameflag;
int doneflag;
char *title=NULL;
char *authorfirst=NULL;
char *authormiddle=NULL;
char *authorlast=NULL;

void initvars()
{
    authorflag=0;
    titleflag=0;
    titleinfoflag=0;
    firstnameflag=0;
    middlenameflag=0;
    lastnameflag=0;
    doneflag=0;
    title=NULL;
    authorfirst=NULL;
    authormiddle=NULL;
    authorlast=NULL;   
    
}


void handlestart(void *userData,const XML_Char *name,const XML_Char **atts)
{
    char *lowname=tolowerstr(name);
    
    if(strcmp(lowname,"title-info")==0)
        titleinfoflag=1;
    else if(strcmp(lowname,"book-title")==0 && titleinfoflag)
        titleflag=1;
    else if(strcmp(lowname,"author")==0 &&titleinfoflag)
        authorflag=1;
    else if(strcmp(lowname,"first-name")==0 && authorflag)
        firstnameflag=1;
    else if(strcmp(lowname,"middle-name")==0 && authorflag)
        middlenameflag=1;
    else if(strcmp(lowname,"last-name")==0 && authorflag)
        lastnameflag=1;
    else if(strcmp(lowname,"body")==0)
        doneflag=1;
    free(lowname);
}

void handleend(void *userData,const XML_Char *name)
{
    char *lowname=tolowerstr(name);
    
    if(strcmp(lowname,"title-info")==0)
        titleinfoflag=0;
    else if(strcmp(lowname,"book-title")==0 &&titleinfoflag)
        titleflag=0;
    else if(strcmp(lowname,"author")==0 && titleinfoflag)
        authorflag=0;
    else if(strcmp(lowname,"first-name")==0 && authorflag)
        firstnameflag=0;
    else if(strcmp(lowname,"middle-name")==0 && authorflag)
        middlenameflag=0;
    else if(strcmp(lowname,"last-name")==0 && authorflag)
        lastnameflag=0;
    else if(strcmp(lowname,"title-info")==0)
        doneflag=1;
    free(lowname);
}


void handlechar(void *userData,const XML_Char *s,int len)
{
    
    char *temp2=(char *)calloc(len+1,sizeof(char));
    
    strncpy(temp2,s,len);
    temp2[len]='\0';
    
    char *temp=NULL;
    if(titleflag==1&&len>0)
    {
        temp=title;
        title=(char *)calloc(len+(temp==NULL?0:strlen(temp))+1,sizeof(char));
        if(temp)
        {
            strncpy(title,temp,strlen(temp));
            strncpy(&title[strlen(temp)],s,len);
            title[len+strlen(temp)]='\0';
            free(temp);
        }
        else
        {
            strncpy(title,s,len);
            title[len]='\0';
        }
    }
    else if(firstnameflag&&len>0)
    {
        temp=authorfirst;
        authorfirst=(char *)calloc(len+(temp==NULL?0:strlen(temp))+1,sizeof(char));
        if(temp)
        {
            strncpy(authorfirst,temp,strlen(temp));
            strncpy(&authorfirst[strlen(temp)],s,len);
            authorfirst[len+strlen(temp)]='\0';
            free(temp);
        }
        else
        {
            strncpy(authorfirst,s,len);
            authorfirst[len]='\0';
        }
        
    }
    else if(middlenameflag &&len>0)
    {
        temp=authormiddle;
        authormiddle=(char *)calloc(len+(temp==NULL?0:strlen(temp))+1,sizeof(char));
        if(temp)
        {
            strncpy(authormiddle,temp,strlen(temp));
            strncpy(&authormiddle[strlen(temp)],s,len);
            authormiddle[len+strlen(temp)]='\0';
            free(temp);
        }
        else
        {
            strncpy(authormiddle,s,len);
            authormiddle[len]='\0';
        }
    }
    else if(lastnameflag&&len>0)
    {
        temp=authorlast;
        authorlast=(char *)calloc(len+(temp==NULL?0:strlen(temp))+1,sizeof(char));
        if(temp)
        {
            strncpy(authorlast,temp,strlen(temp));
            strncpy(&authorlast[strlen(temp)],s,len);
            authorlast[len+strlen(temp)]='\0';
            free(temp);
        }
        else
        {
            strncpy(authorlast,s,len);
            authorlast[len]='\0';
        }
    }
}








extern "C" plugin_info_t *init()
{
    plugin_info_t *myinfo;
    
    
    
    
    
    myinfo=(plugin_info_t *)calloc(1,sizeof(plugin_info_t));
    myinfo->name=(char *)calloc(5,sizeof(char));
    strcat(myinfo->name,"txp");
    myinfo->exts=(char**)calloc(1,sizeof(char*));
    myinfo->exts[0]=(char *)calloc(5,sizeof(char));
    strcat(myinfo->exts[0],".fb2");
    myinfo->nexts=1;
    
    return myinfo;
}
extern "C" void fini(plugin_info_t *plugin_info)
{
    
    free(plugin_info->name);
    free(plugin_info->exts[0]);
    free(plugin_info->exts);
    free(plugin_info);
    
}
extern "C" book_meta_t *parse_meta(const char *filename)
{
    int filehandle;
    char *buffer;
    long nread;
    XML_Parser myparse;
    
    initvars();
    
    myparse=XML_ParserCreate("UTF-8");
    XML_UseParserAsHandlerArg(myparse);
    XML_SetElementHandler(myparse,handlestart,handleend);
    XML_SetCharacterDataHandler(myparse,handlechar);
    
    
    
    
    filehandle=open(filename,O_RDONLY);
    if(!filehandle)
        return NULL;
    buffer=(char *)calloc(100,sizeof(char));
    while(1)
    {
        
        if((nread=read(filehandle,(void *)buffer,100*sizeof(char)))<=0)
            doneflag=1;
        
        if(XML_Parse(myparse,buffer,nread,doneflag)<=0 || doneflag)
            break;
    }
    
    free(buffer);
    close(filehandle);
    book_meta_t *mymeta;
    mymeta=(book_meta_t *)calloc(1,sizeof(book_meta_t));
    mymeta->title=(char *)calloc((title?strlen(title):0)+1,sizeof(char));
    if(title==NULL)
        mymeta->title[0]='\0';
    else
    {
        strcat(mymeta->title,title);
        free(title);
    }
    
    
    if(authorfirst||authormiddle||authorlast)
    {
        mymeta->author=(char *)calloc((authorfirst?strlen(authorfirst):0) + (authormiddle?(strlen(authormiddle)+1):0) + (authorlast?(strlen(authorlast)+1):0)+1,sizeof(char));
        
        if(authorfirst)
        {
            strcat(mymeta->author,authorfirst);
            free(authorfirst);
        }
        
        if(authormiddle)
        {
            strcat(mymeta->author," ");
            strcat(mymeta->author,authormiddle);
            free(authormiddle);
        }
        
        if(authorlast)
        {
            strcat(mymeta->author," ");
            strcat(mymeta->author,authorlast);
            free(authorlast);
        }
        
    }
    else
        mymeta->author=NULL;
    mymeta->series=NULL;
    XML_ParserFree(myparse);
    return mymeta;
}
